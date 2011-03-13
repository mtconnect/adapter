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
#include "connectone_adapter.hpp"
#include "yaml.h"
#include <iostream>
#include <fstream>
#include "XBee.h"
#include <time.h>

using namespace std;
//

#ifdef WIN32
static CRITICAL_SECTION sWriteLock;
#else
#include <pthread.h>
static pthread_mutex_t sWriteLock;
#endif
static bool sWriteLockInitialized = false;

ConnectOneAdapter::ConnectOneAdapter(int aPort)
  : Adapter(aPort, 500)
{
  if (!sWriteLockInitialized) {
#ifdef WIN32
    InitializeCriticalSection(&sWriteLock);
#else
    pthread_mutex_init(&sWriteLock, NULL);
#endif
    sWriteLockInitialized = true;
  }

  mConnected = 0;
  mSilenceTimeout = 20;

  ifstream fin("connectone.yaml");
  YAML::Parser parser(fin);
  YAML::Node doc;
  parser.GetNextDocument(doc);
  const YAML::Node &comms = doc["communications"];
  comms["port"] >> mSerialPort;
  comms["baud"] >> mBaud;
  if (comms.FindValue("dataBits") != NULL)
    comms["dataBits"] >> mDataBits;
  else
    mDataBits = 8;
  comms["stopBits"] >> mStopBits;
  comms["parity"] >> mParity;
  comms["timeout"] >> mTimeout;
  if (comms.FindValue("silenceTimeout") != NULL)
	comms["silenceTimeout"] >> mSilenceTimeout;

  mSerial = new Serial(mSerialPort.c_str(), mBaud, mParity.c_str(),
                       mDataBits, mStopBits);
  mXBee.setSerial(mSerial);

  // Get the devices to poll.
  if (doc.FindValue("devices") != NULL) {
    const YAML::Node &devices = doc["devices"];
    for(unsigned i = 0; i < devices.size(); i++) {
      const YAML::Node &device = devices[i];
      string deviceName;
      device["device"] >> deviceName;
      const YAML::Node &address = device["address"];
      uint32_t msb = 0, lsb = 0;
      for (int j = 0; j < 8; j++) {
        uint32_t v;
        address[j] >> v;
        if (j > 3)
          lsb |= v << (7 - j) * 8;
        else
          msb |= v << (3 - j) * 8;
      }
      XBeeAddress64 xbAddr(msb, lsb);
      ConnectOneDevice *d = new ConnectOneDevice(deviceName, xbAddr);
      device["prefix"] >> d->mPrefix;
      mDevices.push_back(d);
    }
  }
}

ConnectOneAdapter::~ConnectOneAdapter()
{
  delete mSerial;
}

void ConnectOneAdapter::initialize(int aArgc, const char *aArgv[])
{
  FILE *log = fopen("connectone.log", "a");
  gLogger = new Logger(log);
  MTConnectService::initialize(aArgc, aArgv);
  if (aArgc > 1) {
    mPort = atoi(aArgv[0]);
  }
  if (mDebug)
    gLogger->setLogLevel(Logger::eDEBUG);
}

void ConnectOneAdapter::start()
{
  startServerThread();

  while (mRunning)
  {
    if (mSerial->connected())
    {
      mXBee.readPacket();    
      XBeeResponse &response = mXBee.getResponse();
      if (response.isAvailable())
      {
        switch(response.getApiId()) 
        {
        case 0x90:
          handleMTConnectData(response);
          break;
        
        case 0x88:
          handleAtCommand(response);
          break;
        }

        sendPing();
      }
      else if (response.isError())
      {
        gLogger->error("Repsonse error: %d", response.getErrorCode());
      }
    }
    else
    {
      // Make everything unavailable...
      unavailable();
      
      if (!mSerial->connect())
        sleep(10);
      else
      {
        uint8_t command[] = { 'N', 'P' };
        uint8_t value[] = { 0x0 };
        
        AtCommandRequest atap2(command, value, 0);
        mXBee.send(atap2);
        gLogger->info("Connected to serial port");
      }
    }
  }
}

void ConnectOneAdapter::handleMTConnectData(XBeeResponse &aResponse)
{
  mBuffer.timestamp();
  ZBRxResponse rx;
  aResponse.getZBRxResponse(rx);
          
  string data((const char*) rx.getData(), rx.getDataLength());
  XBeeAddress64& addr = rx.getRemoteAddress64();
          
  // Prefix data items...
  ConnectOneDevice *dev = NULL;
  for (size_t i = 0; dev == NULL && i < mDevices.size(); i++) 
  {
    if (mDevices[i]->mAddress == addr) 
      dev = mDevices[i];
  }
          
  if (dev == NULL)
  {
    dev = new ConnectOneDevice("discovered", addr);
    dev->mAvailable = true;
    mDevices.push_back(dev);
  }
          
  if (dev != NULL)
  {
    size_t pos = data.find_first_of('|');
    while (pos != string::npos)
    {
      pos += 1;
      size_t start = pos;
      pos = data.find_first_of('|', pos);
              
      // Find the end of the data item name string...
      if (pos != string::npos)
      {
        // See if we need to add a new data item...
        string key = data.substr(start, pos - start);
        Sample *sample;
        if (dev->mDataItems.count(key) == 0) 
        {
          if (dev->mPrefix) {
            key.insert((size_t) 0, ":");
            key.insert((size_t) 0, dev->mName);
          }
                  
          sample = new Sample(key.c_str());
          dev->mDataItems.insert(pair<string, Sample*>(key, sample));
          addDatum(*sample);
        }
        else
        {
          sample = dev->mDataItems[key];
        }
                
        // Get the data...
        start = pos + 1;
        pos = data.find_first_of('|', pos + 1);
        string value = data.substr(start, pos - start);
        sample->setValue(atof(value.c_str()));
      }
    }
  }
          
  dev->mLastMsgTime = time(NULL);
  sendChangedData();
  mBuffer.reset();
  cleanup();
}

void ConnectOneAdapter::handleAtCommand(XBeeResponse &aResponse)
{
  AtCommandResponse cmd;
  aResponse.getAtCommandResponse(cmd);
  if (cmd.isOk())
  {
    uint8_t *cp = cmd.getCommand();
    if (cp[0] == 'N' && cp[1] == 'P')
    {
      uint16_t v = ntohs(*((uint16_t*) cmd.getValue()));
      mXBee.setMaxSize(v);
    }
  }
}

void ConnectOneAdapter::stop()
{
  stopServer();
}

void ConnectOneAdapter::gatherDeviceData()
{
}

void ConnectOneAdapter::sendPing()
{
  for (size_t i = 0; i < mDevices.size(); i++) 
  {
    ConnectOneDevice *dev = mDevices[i];
    ZBTxRequest request(dev->mAddress, (uint8_t*) "* PING", (uint8_t) 6);
    mXBee.send(request);
  }
}

void ConnectOneAdapter::unavailable()
{
#ifdef WIN32
  EnterCriticalSection(&sWriteLock);
#else
  pthread_mutex_lock(&sWriteLock);
#endif
  
  mBuffer.timestamp();

  for (size_t i = 0; i < mDevices.size(); i++) 
  {
    ConnectOneDevice *dev = mDevices[i];
    dev->mAvailable = false;
  }
  Adapter::unavailable();

#ifdef WIN32
  LeaveCriticalSection(&sWriteLock);
#else
  pthread_mutex_unlock(&sWriteLock);
#endif
}

void ConnectOneAdapter::unavailable(ConnectOneDevice *aDevice)
{
#ifdef WIN32
  EnterCriticalSection(&sWriteLock);
#else
  pthread_mutex_lock(&sWriteLock);
#endif
  
  if (aDevice->mAvailable) 
  {
    mBuffer.timestamp();
    map<string, Sample*>::iterator iter;
    for (iter = aDevice->mDataItems.begin();
         iter != aDevice->mDataItems.end();
         ++iter)
    {
      iter->second->unavailable();
    }
    
    aDevice->mAvailable = false;
    flush();
  }

#ifdef WIN32
  LeaveCriticalSection(&sWriteLock);
#else
  pthread_mutex_unlock(&sWriteLock);
#endif
}

void ConnectOneAdapter::periodicWork()
{
  time_t now = time(NULL);
  for (size_t i = 0; i < mDevices.size(); i++) 
  {
    ConnectOneDevice *dev = mDevices[i];
    if (dev->mAvailable && (now - dev->mLastMsgTime) > mSilenceTimeout)
      unavailable(dev);
  }
}
