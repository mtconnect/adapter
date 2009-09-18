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
#include "adapter.hpp"
#include "device_datum.hpp"

Adapter::Adapter(int aPort, int aScanDelay)
 : mNumDeviceData(0)
{
  mScanDelay = aScanDelay;
  mServer = 0;
  mPort = aPort;
}

Adapter::~Adapter()
{
  if (mServer)
    delete mServer;
}

/* Add a data value to the list of data values */
void Adapter::addDatum(DeviceDatum &aValue)
{
  if (mNumDeviceData < MAX_DEVICE_DATA)
  {
    mDeviceData[mNumDeviceData++] = &aValue;
    mDeviceData[mNumDeviceData] = 0;
  }
}

void Adapter::sleepMs(int aMs)
{
#ifndef WIN32
    usleep(aMs * 1000);
#else
    Sleep(aMs);
#endif
  
}

/*  
 * This is the main loop of the application. Once the server is started and 
 * the application runs forever until it is killed. The process follows this flow:
 *    See if any new clients have arrived.
 *      If new clients are present, sent the initial values to them.
 *    Read from the clients any data that is currently in the socket to keep anyone from
 *      blocking.
 *    If we have at least one client:
 *      Timestamp the buffer
 *      gather data from the API
 *      send the data values that have changed to the clients
 *    Sleep for a few millies and repeat.
 */
void Adapter::startServer()
{  
  mServer = new Server(mPort);
  bool hasClients = false;
  
  /* Process forever... */
  while (1) 
  {
    /* Check if we have any new clients */
    Client **clients = mServer->connectToClients();
    if (clients != 0)
    {
      hasClients = true;
      for (int i = 0; clients[i] != 0; i++)
      {
        /* If there are any new clients, send them the initial values for all the 
         * data values */
        sendInitialData(clients[i]);
      }
    }
    
    /* Read and discard all data from the clients */
    mServer->readFromClients();
    
    /* Don't bother getting data if we don't have anyone to read it */
    if (mServer->numClients() > 0)
    {      
      mBuffer.timestamp();
      gatherDeviceData();
      sendChangedData();
      mBuffer.reset();
    }
    else if (hasClients)
    {
      hasClients = false;
      clientsDisconnected();
    }
    
    sleepMs(mScanDelay);
  }
}

/* Send a single value to the buffer. */
void Adapter::sendDatum(DeviceDatum *aValue)
{
  if (aValue->requiresFlush())
    sendBuffer();
  aValue->append(mBuffer);
  if (aValue->requiresFlush())
    sendBuffer();
}

/* Send the buffer to the clients. Only sends if there is something in the buffer. */
void Adapter::sendBuffer()
{
  if (mBuffer.length() > 0)
  {
    mBuffer.append("\n");
    mServer->sendToClients(mBuffer);
    mBuffer.reset();  
  }
}

/* Send the initial values to a client */
void Adapter::sendInitialData(Client *aClient)
{
  mDisableFlush = true;
  mBuffer.timestamp();
  gatherDeviceData();
  
  for (int i = 0; i < mNumDeviceData; i++)
  {
    DeviceDatum *value = mDeviceData[i];
    if (value->hasInitialValue())
      sendDatum(value);
  }
  sendBuffer();
  mDisableFlush = false;
}

/* Send the values that have changed to the clients */
void Adapter::sendChangedData()
{
  for (int i = 0; i < mNumDeviceData; i++)
  {
    DeviceDatum *value = mDeviceData[i];
    if (value->changed())
      sendDatum(value);
  }  
  sendBuffer();
}

void Adapter::flush()
{
  if (!mDisableFlush)
  {
    sendChangedData();
    mBuffer.reset();
    mBuffer.timestamp();
  }
}

void Adapter::clientsDisconnected()
{
  /* Do nothing for now ... */
  printf("All clients have disconnected\n");
}

