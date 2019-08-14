//
// Copyright (c) 2008, AMT - The Association For Manufacturing Technology ("AMT")
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the AMT nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// DISCLAIMER OF WARRANTY. ALL MTCONNECT MATERIALS AND SPECIFICATIONS PROVIDED
// BY AMT, MTCONNECT OR ANY PARTICIPANT TO YOU OR ANY PARTY ARE PROVIDED "AS IS"
// AND WITHOUT ANY WARRANTY OF ANY KIND. AMT, MTCONNECT, AND EACH OF THEIR
// RESPECTIVE MEMBERS, OFFICERS, DIRECTORS, AFFILIATES, SPONSORS, AND AGENTS
// (COLLECTIVELY, THE "AMT PARTIES") AND PARTICIPANTS MAKE NO REPRESENTATION OR
// WARRANTY OF ANY KIND WHATSOEVER RELATING TO THESE MATERIALS, INCLUDING, WITHOUT
// LIMITATION, ANY EXPRESS OR IMPLIED WARRANTY OF NONINFRINGEMENT,
// MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
//
// LIMITATION OF LIABILITY. IN NO EVENT SHALL AMT, MTCONNECT, ANY OTHER AMT
// PARTY, OR ANY PARTICIPANT BE LIABLE FOR THE COST OF PROCURING SUBSTITUTE GOODS
// OR SERVICES, LOST PROFITS, LOSS OF USE, LOSS OF DATA OR ANY INCIDENTAL,
// CONSEQUENTIAL, INDIRECT, SPECIAL OR PUNITIVE DAMAGES OR OTHER DIRECT DAMAGES,
// WHETHER UNDER CONTRACT, TORT, WARRANTY OR OTHERWISE, ARISING IN ANY WAY OUT OF
// THIS AGREEMENT, USE OR INABILITY TO USE MTCONNECT MATERIALS, WHETHER OR NOT
// SUCH PARTY HAD ADVANCE NOTICE OF THE POSSIBILITY OF SUCH DAMAGES.
//
#include "internal.hpp"
#include "server.hpp"
#include "client.hpp"
#include "logger.hpp"
#include <chrono>

const int READ_BUFFER_LEN = 8092;

// Create the server and bind to the port
Server::Server(int port, std::chrono::milliseconds heartbeatFreq)
{
	mNumClients = 0;
	mPort = port;
	mTimeout = heartbeatFreq * 2;

	SOCKADDR_IN t;

#ifndef WIN32
	signal(SIGPIPE, SIG_IGN);
#else
	WSADATA w;
	int iResult = WSAStartup(MAKEWORD(2, 2), &w);

	if (iResult != NO_ERROR)
	{
		gLogger->error("Error at WSAStartup()\n");
		exit(1);
	}

#endif

	mSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (mSocket == INVALID_SOCKET)
	{
		gLogger->error("Error at socket().", stderr);
		delete this;
		exit(1);
	}

	t.sin_family = AF_INET;
	t.sin_port = htons(port);
	t.sin_addr.s_addr = htonl(INADDR_ANY);

	if (::bind(mSocket, (SOCKADDR *)&t, sizeof(t)) == SOCKET_ERROR)
	{
		gLogger->error("Failed to bind on port %d",  port);
		delete this;
		exit(1);
	}

	if (listen(mSocket, 4) == SOCKET_ERROR)
	{
		gLogger->error("Error listening.");
		delete this;
		exit(1);
	}

	// Default to a 10 second heartbeat
	sprintf(mPong, "* PONG %I64d\n", heartbeatFreq.count());

	gLogger->info("Server started, waiting on port %d", port);
}


Server::~Server()
{
	for (int i = 0; i < mNumClients; i++)
	{
		Client *client = mClients[i];
		delete client;
	}

	::shutdown(mSocket, SHUT_RDWR);

#ifdef _WINDOWS
	WSACleanup();
#endif
}


void Server::readFromClients()
{
	std::lock_guard<std::mutex> lock(mListLock);

	fd_set rset;
	FD_ZERO(&rset);
	int nfds = 0;

	for (int i = 0; i < mNumClients; i++)
	{
		Client *client = mClients[i];
		FD_SET(client->socket(), &rset);
#ifndef WIN32

		if (client->socket() > nfds)
			nfds = client->socket();

#endif
	}

#ifdef WIN32
	nfds = mNumClients;
#else
	nfds++;
#endif

	struct timeval timeout;
	::memset(&timeout, 0, sizeof(timeout));

	if (::select(nfds, &rset, 0, 0, &timeout) > 0)
	{
		char buffer[READ_BUFFER_LEN];
		int len;

		// Since clients can be removed, we need to iterate backwards
		for (int i = mNumClients - 1; i >= 0; i--)
		{
			Client *client = mClients[i];

			if (FD_ISSET(client->socket(), &rset))
			{
				len = client->read(buffer, READ_BUFFER_LEN);

				if (len > 0)
				{
					// Check for heartbeat
					if (strncmp(buffer, "* PING", 6) == 0)
					{
						if (!client->mHeartbeats)
							client->mHeartbeats = true;

						client->mLastHeartbeat = std::chrono::system_clock::now();
						client->write(mPong);
					}
					else
						printf("Received: %s", buffer);
				}
				else
					removeClientInternal(client);
			}
		}

	}

	// Check heartbeats
	for (int i = mNumClients - 1; i >= 0; i--)
	{
		auto client = mClients[i];
		auto now = std::chrono::system_clock::now();

		if (client->mHeartbeats)
		{
			if(now - client->mLastHeartbeat >mTimeout)
			{
				gLogger->warning("Client has not sent heartbeat in over %d ms, disconnecting",
						 mTimeout);
				removeClientInternal(client);
			}
		}
	}
}


void Server::sendToClient(Client *client, const char *string)
{
	if (client->write(string) < 0)
		removeClient(client);
}


void Server::sendToClients(const char *string)
{
	std::lock_guard<std::mutex> lock(mListLock);

	for (int i = mNumClients - 1; i >= 0; i--)
	{
		if (mClients[i]->write(string) < 0)
			removeClientInternal(mClients[i]);
	}
}


Client *Server::connectToClients()
{
	fd_set rset;
	FD_ZERO(&rset);
	FD_SET(mSocket, &rset);
#ifdef WIN32
	int nfds = 1;
#else
	int nfds = mSocket + 1;
#endif

	struct timeval timeout;
	::memset(&timeout, 0, sizeof(timeout));

	Client *client = nullptr;
	bool added = false;

	if (::select(nfds, &rset, 0, 0, &timeout) > 0)
	{
		SOCKADDR_IN addr;
		socklen_t len = sizeof(addr);
		memset(&addr, 0, sizeof(addr));

		SOCKET socket = ::accept(mSocket, (SOCKADDR *) &addr, &len);

		if (socket == INVALID_SOCKET)
		{
			gLogger->error("Error at accept().");
			return 0;
		}

		char targetNameBuffer[INET_ADDRSTRLEN] = {0};
		auto targetName = inet_ntop(AF_INET, &addr.sin_addr, targetNameBuffer, sizeof(targetNameBuffer));
		gLogger->info("Connected to: %s on port %d", targetName, ntohs(addr.sin_port));

		int flag = 1;
		::setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, (const char *) &flag, sizeof(int));

		client = addClient(new Client(socket));
		added = true;
	}

	return client;
}


void Server::removeClientInternal(Client *client)
{
	int pos = 0;

	for (pos = 0; pos < mNumClients; pos++)
	{
		if (mClients[pos] == client)
			break;
	}

	if (pos < mNumClients)
	{
		mNumClients--;

		if (pos < mNumClients)
		{
			// Shift the array left to remove the item
			memmove(mClients + pos,
				mClients + (pos + 1),
				(mNumClients - pos) * sizeof(Client *));
		}

		delete client; client = nullptr;
		mClients[mNumClients + 1] = nullptr;
	}
}


// Removes a client from the client list.
// Because the client can be removed during list iteration, lists
// should always be iterated from last to first.
//
void Server::removeClient(Client *client)
{
	std::lock_guard<std::mutex> lock(mListLock);
	removeClientInternal(client);
}


Client *Server::addClient(Client *client)
{
	std::lock_guard<std::mutex> lock(mListLock);

	if (mNumClients < MAX_CLIENTS)
	{
		mClients[mNumClients] = client;
		mNumClients++;
	}
	else
	{
		delete client;
		client = nullptr;
	}

	return client;
}
