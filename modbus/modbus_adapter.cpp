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
#include "modbus_adapter.hpp"
#include "yaml.h"
#include <iostream>
#include <fstream>

ModbusAdapter::ModbusAdapter(int aPort)
  : Adapter(aPort, 1000)
{
  mConnected = 0;

  std::ifstream fin("modbus.yaml");
  YAML::Parser parser(fin);
  YAML::Node doc;
  parser.GetNextDocument(doc);
  const YAML::Node &comms = doc["communications"];
  comms["port"] >> mSerialPort;
  comms["baud"] >> mBaud;
  comms["dataBits"] >> mDataBits;
  comms["stopBits"] >> mStopBits;
  comms["parity"] >> mParity;
  
  const YAML::Node &data = doc["data"];  
  for(unsigned i = 0; i < data.size(); i++) {
    const YAML::Node &node = data[i];
    std::string type;
    ModbusData *data = NULL;
    int address;
    
    node["type"] >> type;
    node["address"] >> address;
    if (type == "coil") {
      data = new ModbusCoil(address);
    } else if (type == "register") {
      data = new ModbusRegister(address);
    } else if (type == "double") {
      int scaling;
      const YAML::Node *scaleNode = node.FindValue("scalingAddress");
      if (scaleNode != NULL) {
        *scaleNode >> scaling;
        data = new ModbusDouble(address, scaling, 0);
      } else if ((scaleNode = node.FindValue("scaler")) != NULL) {
        *scaleNode >> scaling;
        data = new ModbusDouble(address, 0, scaling);
      } else {
        data = new ModbusDouble(address, 0, 1);
      }
    }

    std::vector<std::string> nameList;
    const YAML::Node &names = node["names"];
    for(unsigned j = 0; j < names.size(); j++) {
      nameList.push_back(names[j]);
    }
    data->createDataItems(nameList);
    
    std::vector<DeviceDatum*>::iterator iter;
    for (iter = data->mDataItems.begin(); iter != data->mDataItems.end(); iter++) {
      addDatum(**iter);
    }

    mData.push_back(data);
  }

  unavailable();
}

ModbusAdapter::~ModbusAdapter()
{
  modbus_close(&mb_param);
}

void ModbusAdapter::initialize(int aArgc, const char *aArgv[])
{
  
  MTConnectService::initialize(aArgc, aArgv);
  if (aArgc > 1) {
    mPort = atoi(aArgv[0]);
  }
}

void ModbusAdapter::start()
{
  startServer();
}

void ModbusAdapter::stop()
{
  stopServer();
}

void ModbusAdapter::gatherDeviceData()
{
  if (!mConnected) {
    modbus_init_rtu(&mb_param, mSerialPort.c_str(), mBaud, mParity.c_str(), mDataBits, mStopBits);
    modbus_set_debug(&mb_param, TRUE);
    if (modbus_connect(&mb_param) == -1) {
      unavailable();
      printf("ERROR Connection failed\n");
      sleep(5);
      return;
    } else {
      mConnected = true;
    }
  }

  std::vector<ModbusData*>::iterator iter;
  for (iter = mData.begin(); iter != mData.end(); iter++) {
    bool success = false;
    if ((*iter)->type() == ModbusData::eCOIL) {
      success = read_input_status(&mb_param, 1, (*iter)->address(), (*iter)->count(),
                                (static_cast<ModbusCoil*>(*iter))->data()) > 0;
    } else { 
      success = read_holding_registers(&mb_param, 1, (*iter)->address(), (*iter)->count(),
                                       (static_cast<ModbusRegister*>(*iter))->data()) > 0;
      if (success && (*iter)->type() == ModbusData::eDOUBLE) {
        ModbusDouble *d = static_cast<ModbusDouble*>(*iter);
        if (d->scalerAddress() != 0) {
          uint16_t scaler[1];
          success = read_holding_registers(&mb_param, 1, d->scalerAddress(), 1, scaler) > 0;
          if (success) d->scaler = scaler[0];
        }
      }
    }

    if (success) {
      (*iter)->writeValues();
    } else {
      (*iter)->unavailable();
    }
  }
}

