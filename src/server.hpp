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

#ifndef SERVER_HPP
#define SERVER_HPP

#include "threading.hpp"

class Client;

/* Some constants */
const int MAX_CLIENTS = 64;

/* A socket server abstraction */
class Server
{
protected:
  SOCKET mSocket;
  Client *mClients[MAX_CLIENTS + 1];
  int mNumClients;
  int mPort;
  char mPong[32];
  int mTimeout;
  
  MTCMutex mListLock;
  
protected:
  // Assumes the mutex is already locked.
  void removeClientInternal(Client *aClient);
  
  // Locks the mutex.
  void removeClient(Client *aClient);
  
  Client *addClient(Client *aClient);
  unsigned int getTimestamp();
  unsigned int deltaTimestamp(unsigned int, unsigned int);
  
public:
  Server(int aPort, int aHeartbeatFreq);
  ~Server();

  // Returns the new client.
  Client *connectToClients(); /* Client factory */

  /* I/O methods */
  void readFromClients();         /* discard data on read side of
					                              sockets */
  void sendToClients(const char *aString);
  void sendToClient(Client *aClient, const char *aString);
  
  /* Getters */
  int numClients() { return mNumClients; }
  bool hasClients() { return mNumClients > 0; }
  
};

#endif
