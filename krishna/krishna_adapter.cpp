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
  : Adapter(aPort, 1000)
{
  mConnected = 0;
  mXBee.setCanEscape(false);

  ifstream fin("krishna.yaml");
  YAML::Parser parser(fin);
  YAML::Node doc;
  parser.GetNextDocument(doc);
  const YAML::Node &comms = doc["communications"];
  comms["port"] >> mSerialPort;
  comms["baud"] >> mBaud;
  comms["dataBits"] >> mDataBits;
  comms["stopBits"] >> mStopBits;
  comms["parity"] >> mParity;

  mSerial = new Serial(mSerialPort.c_str(), mBaud, mParity.c_str(),
                       mDataBits, mStopBits, true);
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
	item["scaler"] >> scaler;
	int offset;
	item["offset"] >> offset;
        

	string itemName;
	if (m->mPrefix)
	  itemName = m->mName + ":" + name;
	else
	  itemName = name;
	KrishnaSample *sample = new KrishnaSample(itemName.c_str(), offset, scaler);
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
    uint8_t command[] = { 'A', 'P' };
    uint8_t value[] = { 0x1 };
    RemoteAtCommandRequest atap2(aMeter->mAddress, command, value, sizeof(value));
    mXBee.send(atap2);
    mXBee.readPacket();
    RemoteAtCommandResponse response;
    mXBee.getResponse(response);
    if (!response.isOk()) {
      cerr << "Could not set remote ATAP 2 for " << aMeter->mName << endl;
      aMeter->mAvailable = false;
      return;
    } else {
      aMeter->mAvailable = true;
    }
    
    command[1] = 'C';
    RemoteAtCommandRequest atac(aMeter->mAddress, command);
    mXBee.send(atac);
    mXBee.readPacket();
    mXBee.getResponse(response);
    if (!response.isOk()) {
      cerr << "Could not set remote ATAC for " << aMeter->mName << endl;
      aMeter->mAvailable = false;
      return;
    } else {
      aMeter->mAvailable = true;
    }

  }
}

void KrishnaAdapter::requestData(KrishnaMeter *aMeter)
{
  if (!aMeter->mAvailable) return;
  
  std::vector<KrishnaData*>::iterator iter;
  for (size_t i = 0; i < aMeter->mData.size(); i++) {
    bool success = false;
    KrishnaData *data = aMeter->mData[i];
    
    uint8_t payload[] = { 0x01, 0x03, 0x00, 0x00, 0x00, 0x00 };
    payload[2] = (data->getAddress() >> 8) & 0xFF;
    payload[3] = data->getAddress() & 0xFF;
    payload[4] = (data->getRequestCount() >> 8) & 0xFF;
    payload[5] = data->getRequestCount() & 0xFF;
        
    ZBTxRequest request(aMeter->mAddress, payload, sizeof(payload));
    mXBee.send(request);

    KrishnaResponse response;
    mXBee.readPacket();
    if (mXBee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
      ZBTxStatusResponse status;
      mXBee.getResponse(status);
      if (status.isSuccess()) {
        mXBee.readPacket();
        if (mXBee.getResponse().getApiId() == ZB_RX_RESPONSE) {
          flush();
          mXBee.getResponse(response);
          success = response.getDataLength() == data->getDataLength();
        }
      } else {
        aMeter->mAvailable = false;
        if (status.getDeliveryStatus() == ADDRESS_NOT_FOUND)
          printf("Could not find address: 0x%X\n", status.getDeliveryStatus());
        else
          printf("Delivery error: 0x%X\n", status.getDeliveryStatus());
      }
    }

    if (success) {
      memcpy(data->getData(), response.getData(), response.getDataLength());
      data->writeValues();
    } else {
      data->unavailable();
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
      uint8_t value[] = { 0x1 };
      AtCommandRequest atap2(command, value, sizeof(value));
      mXBee.send(atap2);
      mXBee.readPacket();
      AtCommandResponse response;
      mXBee.getResponse(response);
      if (!response.isOk()) {
        mSerial->disconnect();
        fprintf(stderr, "Cannot change ATAP local settings\n");
        mConnected = false;
      } else {
	command[1] = 'C';
	AtCommandRequest atac(command);
	mXBee.send(atac);
	mXBee.readPacket();
	AtCommandResponse response;
	mXBee.getResponse(response);
	if (!response.isOk()) {
	  mSerial->disconnect();
	  fprintf(stderr, "Cannot change ATAC local settings\n");
	  mConnected = false;
	}
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
    requestData(meter);
  }
}
