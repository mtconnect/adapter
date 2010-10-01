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

#ifndef FAKE_ADAPTER_HPP
#define FAKE_ADAPTER_HPP

#include "adapter.hpp"
#include "service.hpp"
#include "device_datum.hpp"
#include "modbus.h"
#include <string>
#include <vector>

class ModbusData {
public:
  typedef enum {
    eCOIL,
    eREGISTER,
    eDOUBLE
  } tType;
    
public:
  ModbusData(int a) : mAddress(a) { mCount = 0; }
  
  int address() { return mAddress; }
  int count() { 
    if (mCount == 0) {
      for (int i = 0; i < mSize.size(); i++) mCount += mSize[i];
    }
    return mCount; 
  }
  tType type() { return mType; }

  std::vector<DeviceDatum*> mDataItems;
  std::vector<int> mSize;
  
  virtual void createDataItems(std::vector<std::string> &aNames, std::vector<int> &aSize) = 0;
  virtual void writeValues() = 0;

  void unavailable() {
    for (int i = 0; i < mDataItems.size(); i++) {
      mDataItems[i]->unavailable();
    }
  }
  
protected:
  int mAddress;
  tType mType;
  int mCount;
};

class ModbusCoil : public ModbusData {
public:
  ModbusCoil(int a) : ModbusData(a) { mType = eCOIL; }
  
  ~ModbusCoil() {
    delete[] mData;
  }
  
  uint8_t operator[](int n) { return mData[n]; }
  uint8_t *data() { return mData; }

  virtual void createDataItems(std::vector<std::string> &aNames, std::vector<int> &aSize) {
    std::vector<std::string>::iterator iter;
    for (iter = aNames.begin(); iter != aNames.end(); iter++) {
      mDataItems.push_back(new IntEvent(iter->c_str()));
      mSize.push_back(1);
    }
    mData = new uint8_t[count()];
  }

  virtual void writeValues() {
    for (int i = 0; i < mDataItems.size(); i++) {
      (static_cast<IntEvent*>(mDataItems[i]))->setValue(mData[i]);
    }
  }
  
protected:
  uint8_t *mData;
};

struct ModbusRegister : public ModbusData {
public:
  ModbusRegister(int a) : ModbusData(a) {
    mType = eREGISTER;
  }
  
  ~ModbusRegister() {
    delete[] mData;
  }
  
  uint16_t operator[](int n) { return mData[n]; }
  uint16_t *data() { return mData; }
  
  virtual void createDataItems(std::vector<std::string> &aNames, std::vector<int> &aSize) {
    std::vector<std::string>::iterator iter;
    for (iter = aNames.begin(); iter != aNames.end(); iter++) {
      mDataItems.push_back(new IntEvent(iter->c_str()));
    }
    mSize = aSize;
    mData = new uint16_t[count()];
  }
  
  virtual void writeValues() {
    int offset = 0;
    for (int i = 0; i < mDataItems.size(); i++) {
      int value;
      if (mSize[i] == 1)
        value = mData[offset];
      else
        value = (static_cast<int>(mData[offset]) << 16) + mData[offset + 1];
      (static_cast<IntEvent*>(mDataItems[i]))->setValue(value);;
      offset += mSize[i];
    }
  }
  
protected:
  uint16_t *mData;
};

struct ModbusDouble : public ModbusRegister {
public:
  ModbusDouble(int a, int s, int c) : ModbusRegister(a) {
    mScalerAddress = s;
    if (s == 0) scaler = c;
    mType = eDOUBLE;
  }
  
  double operator[](int n) { return ((double) mData[n]) / scaler; }
  int scalerAddress() { return mScalerAddress; }
  double scaler;

  virtual void createDataItems(std::vector<std::string> &aNames, std::vector<int> &aSize) {
    std::vector<std::string>::iterator iter;
    for (iter = aNames.begin(); iter != aNames.end(); iter++) {
      mDataItems.push_back(new Sample(iter->c_str()));
    }
    mSize = aSize;
    mData = new uint16_t[count()];
  }

  virtual void writeValues() {
    int offset = 0;
    for (int i = 0; i < mDataItems.size(); i++) {
      int value;
      if (mSize[i] == 1)
        value = mData[offset];
      else
        value = (static_cast<int>(mData[offset]) << 16) + mData[offset + 1];
      (static_cast<Sample*>(mDataItems[i]))->setValue(static_cast<double>(value) / scaler);
      offset += mSize[i];
    }
  }

protected:
  int mScalerAddress;
};

class ModbusAdapter : public Adapter, public MTConnectService
{
protected:
  /* Events */
  std::vector<ModbusData*> mData; 

  modbus_param_t mb_param;

  int mConnected;
  std::string mSerialPort;
  std::string mParity;
  int mBaud, mDataBits, mStopBits;
  
public:
  ModbusAdapter(int aPort);
  ~ModbusAdapter();
  
  virtual void initialize(int aArgc, const char *aArgv[]);
  virtual void start();
  virtual void stop();
  
  virtual void gatherDeviceData();
};

#endif

