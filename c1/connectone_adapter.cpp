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
static CRITICAL_SECTION sSendLock;
#define LOCK(s) EnterCriticalSection(&s)
#define UNLOCK(s) LeaveCriticalSection(&s)
#define INIT(s) InitializeCriticalSection(&s)
#else // !WIN32
#include <pthread.h>
static pthread_mutex_t sWriteLock;
static pthread_mutex_t sSendLock;
#define LOCK(s) pthread_mutex_lock(&s);
#define UNLOCK(s) pthread_mutex_unlock(&s);
#define INIT(s) pthread_mutex_init(&s, NULL)
#endif // WIN32

static bool sWriteLockInitialized = false;

#define SET_WITH_DEFAULT(node, key, value, def) \
  if (node.FindValue(key) != NULL)              \
    node[key] >> value;                        \
  else                                          \
    value = def

ConnectOneAdapter::ConnectOneAdapter(int aPort)
  : Adapter(aPort, 500)
{
  if (!sWriteLockInitialized) {
    INIT(sWriteLock);
    INIT(sSendLock);
    sWriteLockInitialized = true;
  }

  mConnected = 0;

  ifstream fin("connectone.yaml");
  YAML::Parser parser(fin);
  YAML::Node doc;
  parser.GetNextDocument(doc);
  const YAML::Node &comms = doc["communications"];
  comms["port"] >> mSerialPort;
  comms["baud"] >> mBaud;
  SET_WITH_DEFAULT(comms, "dataBits", mDataBits, 8);
  comms["stopBits"] >> mStopBits;
  comms["parity"] >> mParity;

  SET_WITH_DEFAULT(comms, "silenceTimeout", mSilenceTimeout, 20);
  SET_WITH_DEFAULT(comms, "honorTimestamp", mHonorTimestamp, false);
  SET_WITH_DEFAULT(comms, "heartBeat", mHeartBeat, 2);

  mSerial = new Serial(mSerialPort.c_str(), mBaud, mParity.c_str(),
                       mDataBits, mStopBits);
  mXBee.setSerial(mSerial);

  // Get the devices to poll.
  if (doc.FindValue("devices") != NULL) {
    const YAML::Node &devices = doc["devices"];
    for(unsigned i = 0; i < devices.size(); i++) {
      const YAML::Node &device = devices[i];
      if (device.FindValue("name") == NULL ||
          device.FindValue("address") == NULL ||
          device.FindValue("prefix") == NULL)
      {
        gLogger->error("Device must have 'name', 'address', and 'prefix'");
        continue;
      }

      string deviceName;
      device["name"] >> deviceName;
      const YAML::Node &address = device["address"];
	  if (address.GetType() != YAML::CT_SEQUENCE || address.size() < 8)
	  {
		gLogger->error("Address must be 8 characters");
		continue;
	  }
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
      SET_WITH_DEFAULT(device, "silenceTimeout", d->mSilenceTimeout, mSilenceTimeout);
      SET_WITH_DEFAULT(device, "honorTimestamp", d->mHonorTimestamp, mHonorTimestamp);
      SET_WITH_DEFAULT(device, "heartBeat", d->mHeartBeat, mHeartBeat);

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
      LOCK(sSendLock);
      mXBee.readPacket();
      UNLOCK(sSendLock);
      XBeeResponse &response = mXBee.getResponse();
      if (response.isAvailable())
      {
        switch(response.getApiId()) 
        {
        case 0x90:
          handleRxResponse(response);
          break;
        
        case 0x95:
          handleXBeeDiscovery(response);
          break;
        
        case 0x88:
          handleAtCommand(response);
          break;

        case 0x8B:
          handleXBeeTxStatus(response);
          break;
        }
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

      LOCK(sSendLock);
      bool connected = mSerial->connect();
      UNLOCK(sSendLock);

      if (!connected)
        sleep(10);
      else
        initializeXBee();
    }
  }
}

void ConnectOneAdapter::initializeXBee()
{
  LOCK(sSendLock);
  
  // Set the PAN ID
  {
    uint8_t command[] = { 'A', 'P' };
    uint8_t value[] = { 0x2 };
    
    AtCommandRequest request(command, value, sizeof(value));
    mXBee.send(request);
  }
  {
    uint8_t command[] = { 'C', 'H' };
    uint8_t value[] = { 0x0 };
    
    AtCommandRequest request(command, value, 0);
    mXBee.send(request);
  }
  {
    uint8_t command[] = { 'N', 'P' };
    uint8_t value[] = { 0 };
    
    AtCommandRequest request(command, value, 0);
    mXBee.send(request);
  }
  
  gLogger->info("Connected to serial port");
  
  UNLOCK(sSendLock);
}

void ConnectOneAdapter::handleXBeeTxStatus(XBeeResponse &aResponse)
{
  ZBTxStatusResponse status;
  aResponse.getZBTxStatusResponse(status);
  if (!status.isSuccess())
  {
    gLogger->info("Could not transmit frame: %d", status.getFrameId());

    for (size_t i = 0; i < mDevices.size(); i++) 
    {
      ConnectOneDevice *dev = mDevices[i];
      if (dev->mAvailable && dev->hasFrame(status.getFrameId()))
      {
        unavailable(dev);
        break;
      }
    }
  }
}

void ConnectOneAdapter::handleRxResponse(XBeeResponse &aResponse)
{
  ZBRxResponse rx;
  aResponse.getZBRxResponse(rx);
          
  string data((const char*) rx.getData(), rx.getDataLength());
  XBeeAddress64& addr = rx.getRemoteAddress64();
          
  // Prefix data items...
  ConnectOneDevice *dev = getOrCreateDevice(addr);

  if (data[0] == '*')
    handleMTConnectCommand(dev, data);
  else
    handleMTConnectSamples(dev, data);
}

void ConnectOneAdapter::handleMTConnectSamples(ConnectOneDevice *dev, std::string &data)
{
  LOCK(sWriteLock);

  size_t pos = data.find_first_of('|');
  if (pos != string::npos && dev->mHonorTimestamp) {
    mBuffer.setTimestamp(data.substr(0, pos).c_str());
  } else {
    mBuffer.timestamp();
  }
  
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
        string name(key);
        if (dev->mPrefix) {
          name.insert((size_t) 0, ":");
          name.insert((size_t) 0, dev->mName);
        }
        
        sample = new Sample(name.c_str());
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
          
  sendChangedData();
  mBuffer.reset();
  cleanup();
  
  UNLOCK(sWriteLock);
}

void ConnectOneAdapter::handleMTConnectCommand(ConnectOneDevice *dev, std::string &data)
{
  size_t pos = data.find("* PONG");
  if (pos != string::npos)
  {
    int hb = atoi(data.substr(pos + 7).c_str());
    if (hb > 0) dev->mHeartBeat = hb / 1000;
    gLogger->debug("Got pong: heartbeat = %d", dev->mHeartBeat);
  }
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

      gLogger->debug("Maximum payload = %d", v);

      // Set the PAN ID
      {
        uint8_t command[] = { 'I', 'D' };
        uint8_t value[] = { 0x11, 0x11 };
        
        AtCommandRequest request(command, value, sizeof(value));
        mXBee.send(request);
      }
    }
    else if (cp[0] == 'I' && cp[1] == 'D')
    {
      // Send a discovery request
      uint8_t command[] = { 'N', 'D' };
      uint8_t value[] = { 0 };
      
      AtCommandRequest request(command, value, 0);
      mXBee.send(request);
    }        
    else if (cp[0] == 'N' && cp[1] == 'D')
    {
      uint8_t *response = cmd.getValue();
      XBeeAddress64 addr(ntohl(*((uint32_t*) (response + 2))),
                         ntohl(*((uint32_t*) (response + 6))));
      
      getOrCreateDevice(addr);
    }
  }
}

void ConnectOneAdapter::handleXBeeDiscovery(XBeeResponse &aResponse)
{
  uint8_t *frame = aResponse.getFrameData();
  XBeeAddress64 addr(ntohl(*((uint32_t*) (frame + 0))),
                     ntohl(*((uint32_t*) (frame + 4))));
  
  getOrCreateDevice(addr);
}

ConnectOneDevice *ConnectOneAdapter::getOrCreateDevice(XBeeAddress64 &aAddr)
{
  ConnectOneDevice *dev = NULL;
  for (size_t i = 0; dev == NULL && i < mDevices.size(); i++) 
  {
    if (mDevices[i]->mAddress == aAddr) 
      dev = mDevices[i];
  }
  
  if (dev == NULL)
  {
    gLogger->info("Device Discovered: 0x%08X%08X", aAddr.getMsb(), aAddr.getLsb());
    
    dev = new ConnectOneDevice("discovered", aAddr);
    
    // set defaults for discovered connections.
    dev->mSilenceTimeout = mSilenceTimeout;
    dev->mHonorTimestamp = mHonorTimestamp;
    dev->mHeartBeat = mHeartBeat;
    
    mDevices.push_back(dev);
  }

  if (!dev->mAvailable) 
  {
    gLogger->info("Device Available: 0x%08X%08X", aAddr.getMsb(), aAddr.getLsb());
    dev->mAvailable = true;
  }
  dev->mLastMsgTime = time(NULL);

  return dev;
}

void ConnectOneAdapter::stop()
{
  stopServer();
}

void ConnectOneAdapter::gatherDeviceData()
{
}

void ConnectOneAdapter::unavailable()
{
  LOCK(sWriteLock);
  
  mBuffer.timestamp();

  for (size_t i = 0; i < mDevices.size(); i++) 
  {
    ConnectOneDevice *dev = mDevices[i];
    dev->mAvailable = false;
  }
  Adapter::unavailable();
  
  UNLOCK(sWriteLock);
}

void ConnectOneAdapter::unavailable(ConnectOneDevice *aDevice)
{
  LOCK(sWriteLock);
  
  if (aDevice->mAvailable) 
  {
    gLogger->info("Device unvaailable: 0x%08X%08X",
                 aDevice->mAddress.getMsb(),
                 aDevice->mAddress.getLsb());
    
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

  UNLOCK(sWriteLock);
}

void ConnectOneAdapter::periodicWork()
{
  LOCK(sSendLock);
  
  time_t now = time(NULL);
  for (size_t i = 0; i < mDevices.size(); i++) 
  {
    ConnectOneDevice *dev = mDevices[i];
    if (dev->mAvailable)
    {
      if ((now - dev->mLastMsgTime) > mSilenceTimeout)
        unavailable(dev);
      else if (mSerial->connected() && (now - dev->mLastHeartbeat) >= mHeartBeat)
      {
        char *payload = "* PING\n";
        ZBTxRequest request(dev->mAddress, (uint8_t*) payload,
                            (uint8_t) strlen(payload));
        uint8_t frame = mXBee.getNextFrameId();
        dev->addFrame(frame);
        request.setFrameId(frame);
        mXBee.send(request);
        dev->mLastHeartbeat = now;
      }
    }
  }
  
  UNLOCK(sSendLock);
}
