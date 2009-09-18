/*
* Copyright (c) 2008, AMT – The Association For Manufacturing Technology (“AMT”)
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of the AMT nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* DISCLAIMER OF WARRANTY. ALL MTCONNECT MATERIALS AND SPECIFICATIONS PROVIDED
* BY AMT, MTCONNECT OR ANY PARTICIPANT TO YOU OR ANY PARTY ARE PROVIDED "AS IS"
* AND WITHOUT ANY WARRANTY OF ANY KIND. AMT, MTCONNECT, AND EACH OF THEIR
* RESPECTIVE MEMBERS, OFFICERS, DIRECTORS, AFFILIATES, SPONSORS, AND AGENTS
* (COLLECTIVELY, THE "AMT PARTIES") AND PARTICIPANTS MAKE NO REPRESENTATION OR
* WARRANTY OF ANY KIND WHATSOEVER RELATING TO THESE MATERIALS, INCLUDING, WITHOUT
* LIMITATION, ANY EXPRESS OR IMPLIED WARRANTY OF NONINFRINGEMENT,
* MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. 

* LIMITATION OF LIABILITY. IN NO EVENT SHALL AMT, MTCONNECT, ANY OTHER AMT
* PARTY, OR ANY PARTICIPANT BE LIABLE FOR THE COST OF PROCURING SUBSTITUTE GOODS
* OR SERVICES, LOST PROFITS, LOSS OF USE, LOSS OF DATA OR ANY INCIDENTAL,
* CONSEQUENTIAL, INDIRECT, SPECIAL OR PUNITIVE DAMAGES OR OTHER DIRECT DAMAGES,
* WHETHER UNDER CONTRACT, TORT, WARRANTY OR OTHERWISE, ARISING IN ANY WAY OUT OF
* THIS AGREEMENT, USE OR INABILITY TO USE MTCONNECT MATERIALS, WHETHER OR NOT
* SUCH PARTY HAD ADVANCE NOTICE OF THE POSSIBILITY OF SUCH DAMAGES.
*/

#include "internal.hpp"
#include "server.hpp"
#include "client.hpp"

/* Constants */
const int READ_BUFFER_LEN = 8092;

/* Create the server and bind to the port */
Server::Server(int aPort)
{

  mNumClients = 0;
  mPort = aPort;
  
  SOCKADDR_IN t;
  
#ifndef WIN32
  signal(SIGPIPE, SIG_IGN);
#else
  WSADATA w;
  int iResult = WSAStartup(MAKEWORD(2, 2), &w);
  
  if (iResult != NO_ERROR) {
    fputs("Error at WSAStartup()\n", stderr);
    exit(1);
  }
#endif
  
  mSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  
  if (mSocket == INVALID_SOCKET) {
    fputs("Error at socket().\n", stderr);
    delete this;
    exit(1);
  }
  
  t.sin_family = AF_INET;
  t.sin_port = htons(aPort);
  t.sin_addr.s_addr = htonl(INADDR_ANY);
  
  if (::bind(mSocket, (SOCKADDR *)&t, sizeof(t)) == SOCKET_ERROR) {
    fprintf(stderr, "Failed to bind on port %d\n",  aPort);
    delete this;
    exit(1);
  }
  
  if (listen(mSocket, 4) == SOCKET_ERROR) {
    fputs("Error listening.\n", stderr);
    delete this;
    exit(1);
  }
  
  printf("Server started, waiting on port %d\n", aPort);
}

Server::~Server()
{
  for (int i = 0; i < mNumClients; i++)
  {
    Client *client = mClients[i];
    delete client;
  }

  ::shutdown(mSocket, SHUT_RDWR);
  
#ifdef WINDOWS
  WSACleanup();
#endif
}

void Server::readFromClients()
{
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
    
    /* Since clients can be removed, we need to iterate backwards */
    for (int i = mNumClients - 1; i >= 0; i--)
    {
      Client *client = mClients[i];
      if (FD_ISSET(client->socket(), &rset))
      {
        len = client->read(buffer, READ_BUFFER_LEN);
        if (len > 0)
          printf("Received: %s\n", buffer);
        else 
          removeClient(client);
      }
    }
  }
}

void Server::sendToClient(Client *aClient, const char *aString)
{
  if (aClient->write(aString) < 0)
    removeClient(aClient);
}

void Server::sendToClients(const char *aString)
{
  for (int i = mNumClients - 1; i >= 0; i--)
    sendToClient(mClients[i], aString);
}

Client **Server::connectToClients()
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
    
    Client **clients = mClients + mNumClients;
    clients[0] = 0;
    bool added = false;

    if (::select(nfds, &rset, 0, 0, &timeout) > 0)
    {
      SOCKADDR_IN addr;
      socklen_t len = sizeof(addr);
      memset(&addr, 0, sizeof(addr));
      
      SOCKET socket = ::accept(mSocket, (SOCKADDR*) &addr, &len);
      if (socket == INVALID_SOCKET) {
  	    fputs("Error at accept().\n", stderr);
        return 0;
      }
  	  printf("Connected to: %s on port %d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
      
      Client *client = new Client(socket);
      addClient(client);
      added = true;
    }

    if (added)
      mClients[mNumClients] = 0;
    else
      clients = 0;
    
    return clients;
}


/* Removes a client from the client list.
 * Because the client can be removed during list iteration, lists
 * should always be iterated from last to first. 
 */
void Server::removeClient(Client *aClient)
{
  int pos = 0;
  
  for (pos = 0; pos < mNumClients; pos++)
  {
    if (mClients[pos] == aClient)
      break;
  }
    
  if (pos < mNumClients)
  {
    mNumClients--;
    if (pos < mNumClients)
    {
      /* Shift the array left to remove the item */
      memmove(mClients + pos,
              mClients + (pos + 1),
              (mNumClients - pos) * sizeof(Client*));
    }
    delete aClient;
    mClients[mNumClients + 1] = 0;
  }
}

void Server::addClient(Client *aClient)
{
  if (mNumClients < MAX_CLIENTS)
  {
    mClients[mNumClients] = aClient;
    mNumClients++;
  }
  else
  {
    delete aClient;
  }
}

