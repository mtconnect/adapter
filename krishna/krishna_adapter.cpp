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
#include "krishna_adapter.hpp"
#include "yaml.h"
#include <iostream>
#include <fstream>
#include "XBee.h"

using namespace std;
//

KrishnaAdapter::KrishnaAdapter(int aPort)
  : Adapter(aPort, 500)
{
  mConnected = 0;

  ifstream fin("krishna.yaml");
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
  if (comms.FindValue("scanDelay") != NULL)
    comms["scanDelay"] >> mScanDelay;

  mSerial = new Serial(mSerialPort.c_str(), mBaud, mParity.c_str(),
                       mDataBits, mStopBits);
  mXBee.setSerial(mSerial);

  // Get the meters to poll.
  const YAML::Node &meters = doc["meters"];
  for(unsigned i = 0; i < meters.size(); i++) {
    const YAML::Node &meter = meters[i];
    string device;
    meter["device"] >> device;
    const YAML::Node &address = meter["address"];
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
    KrishnaMeter *m = new KrishnaMeter(device, xbAddr);
    meter["prefix"] >> m->mPrefix;
    mMeters.push_back(m);

    // Get the data to pull from each node
    const YAML::Node &data = meter["data"];  
    for(unsigned i = 0; i < data.size(); i++) {
      const YAML::Node &node = data[i];
      int address;
      node["address"] >> address;
      
      KrishnaData *kdata = new KrishnaData(address);
      m->mData.push_back(kdata);
      
      const YAML::Node &items = node["items"];
      for(unsigned j = 0; j < items.size(); j++) {
        const YAML::Node &item = items[j];
        string name;
        item["name"] >> name;
        double scaler;
        uint16_t maximum = 0, minimum = 0;
        item["scaler"] >> scaler;
        int offset;
        item["offset"] >> offset;
        
        bool nonZero = false;
        if (item.FindValue("nonZero") != NULL) {
                item["nonZero"] >> nonZero;
          if (nonZero)
            minimum = 1;
        }
        if (item.FindValue("max") != NULL)
          item["max"] >> maximum;

        if (item.FindValue("min") != NULL)
          item["min"] >> minimum;

        string itemName;
        if (m->mPrefix)
          itemName = m->mName + ":" + name;
        else
          itemName = name;
        KrishnaSample *sample = new KrishnaSample(itemName.c_str(), offset, scaler, minimum, maximum);
        kdata->addSample(sample);
        addDatum(*sample);
      }

      kdata->createData();
    }
  }

  unavailable();
}

KrishnaAdapter::~KrishnaAdapter()
{
  delete mSerial;
}

void KrishnaAdapter::initialize(int aArgc, const char *aArgv[])
{
  MTConnectService::initialize(aArgc, aArgv);
  if (aArgc > 1) {
    mPort = atoi(aArgv[0]);
  }
}

void KrishnaAdapter::start()
{
  startServer();
}

void KrishnaAdapter::stop()
{
  stopServer();
}

void KrishnaAdapter::initializeMeter(KrishnaMeter *aMeter)
{
  if (!aMeter->mAvailable) {
    uint8_t command[] = { 'A', 'O' };
    uint8_t payload[] = { 0 };
    RemoteAtCommandRequest cmd(aMeter->mAddress, command, payload, sizeof(payload));
    mXBee.send(cmd);
    bool success = false;
    if (mXBee.readPacket(mTimeout)) {
      RemoteAtCommandResponse response;
      mXBee.getResponse(response);
      success = response.isOk();
    }
    if (!success)
    {
      gLogger->warning("Could not Set API output format for %s to 0", aMeter->mName.c_str());
      aMeter->mAvailable = false;
    } else {
      aMeter->mAvailable = true;
    }
  }
}

void KrishnaAdapter::requestData(KrishnaMeter *aMeter)
{
  if (!aMeter->mAvailable) return;
  
  gLogger->debug("\n-------------- %s ----------------", aMeter->mName.c_str());
  std::vector<KrishnaData*>::iterator iter;
  for (size_t i = 0; i < aMeter->mData.size(); i++) {
    bool success = true;
    KrishnaData *data = aMeter->mData[i];        
    KrishnaRequest request(aMeter->mAddress, data->getAddress(), data->getRequestCount() + 1);
    mXBee.send(request);
    KrishnaResponse response;
    bool packetArrived = mXBee.readPacket(mTimeout);
    bool nextPacket = true;
    while (success == true &&  packetArrived && nextPacket &&
           !mXBee.getResponse().isError() && 
           mXBee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
      ZBTxStatusResponse status;
      mXBee.getResponse(status);
      if (!status.isSuccess()) {
        success = false;
        aMeter->mAvailable = false;
        // data->unavailable();
        
        if (status.getDeliveryStatus() == ADDRESS_NOT_FOUND)
          gLogger->warning("Could not find address: 0x%X", status.getDeliveryStatus());
        else
          gLogger->warning("Delivery error: 0x%X", status.getDeliveryStatus());
      } else {
        gLogger->debug("Retry count: %d, discovery: %d",
          status.getTxRetryCount(), status.getDiscoveryStatus());
      }
      nextPacket = mXBee.readPacket(mTimeout);
    }
    if (!packetArrived) {
      success = false;
      aMeter->mAvailable = false;
      // unavailable();
      mSerial->disconnect();
      mConnected = false;
    } else if (success && !mXBee.getResponse().isError() &&
        mXBee.getResponse().getApiId() == ZB_RX_RESPONSE) {
      flush();
      mXBee.getResponse(response);
      success = response.getDataLength() == data->getDataLength();
      if (!success)
        gLogger->error("Length error: resp: %d != expected: %d", 
                response.getDataLength(), data->getDataLength());
    } else {
      success = false;
    }

    if (success) {
      if (data->writeData(response.getData(), response.getDataLength()))
        data->writeValues();
    }
  }
}

void KrishnaAdapter::gatherDeviceData()
{
  if (!mConnected) {
    if (mSerial->connect())
    {
      mSerial->flushInput();
      mConnected = true;

      // Make sure we are set to ATAP 2 so we escape control
      // characters
      uint8_t command[] = { 'A', 'P' };
      uint8_t value[] = { 0x2 };
      AtCommandRequest atap2(command, value, sizeof(value));
      mXBee.send(atap2);
      mXBee.readPacket(mTimeout);
      AtCommandResponse response;
      mXBee.getResponse(response);
      if (!response.isOk()) {
        mSerial->disconnect();
        gLogger->warning("Cannot change ATAP local settings");
        mConnected = false;
      }
    }
    else
    {
      Sleep(5000);
      return;
    }
  }
  
  for (size_t i = 0; i < mMeters.size(); i++) {
    KrishnaMeter *meter = mMeters[i];
    initializeMeter(meter);
    if (meter->mAvailable)
      requestData(meter);
  }
}
