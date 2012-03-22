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

#ifndef FANUC_ADAPTER_HPP
#define FANUC_ADAPTER_HPP

#include "adapter.hpp"
#include "condition.hpp"
#include "device_datum.hpp"
#include "service.hpp"
#include "fanuc_path.hpp"
#include "Fwlib32.h"
#include <vector>

#define MAX_MACROS 32
#define MAX_PMC 32
const int MAX_HOST_LEN = 64;

class MacroSample : public Sample 
{
protected:
  int mNumber;

public:
  MacroSample(const char *aName, int aNum) :
    Sample(aName), mNumber(aNum) {}
  int getNumber() { return mNumber; }
};

class MacroPathPosition : public PathPosition 
{
protected:
  int mX;
  int mY;
  int mZ;

public:
  MacroPathPosition(const char *aName, int aX, int aY, int aZ) :
    PathPosition(aName), mX(aX), mY(aY), mZ(aZ) {}
  int getX() { return mX; }
  int getY() { return mY; }
  int getZ() { return mZ; }
};
  
/* 
 * Provides a connection to the data available from the FANUC Focus library.
 */
class FanucAdapter : public Adapter, public MTConnectService 
{
protected:
  /* Define all the data values here */
  short mMaxPath;
  std::vector<FanucPath*> mPaths;


  /* Conditions */
  
  /* Events */
  Message mMessage;
  
  Availability mAvail;
  IntEvent mPartCount;

  /* Macro variables */
  MacroSample         *mMacroSample[MAX_MACROS];
  MacroPathPosition   *mMacroPath[MAX_MACROS];
  int       mMacroMin;
  int       mMacroMax;
  int       mMacroSampleCount;
  int       mMacroPathCount;
    
  /* Macro variables */
  IntEvent *mPMCVariable[MAX_PMC];
  int       mPMCAddress[MAX_PMC];
  int       mPMCCount;

  unsigned short mFlibhndl;
  bool mConnected, mConfigured;
  int mDevicePort;
  char mDeviceIP[MAX_HOST_LEN];

protected:
  void connect();
  void configure();
  void configMacrosAndPMC(const char *aIniFile);
  
  void reconnect();
  void disconnect();
  void getMessages();
  
  void getPathData();

  void getMacros();
  void getPMC();

  void getCounts();

  void innerGatherDeviceData();

public:
  FanucAdapter(int aServerPort);
  ~FanucAdapter();
  
  // For Service
  virtual void initialize(int aArgc, const char *aArgv[]);
  virtual void start();
  virtual void stop();
  
  virtual void gatherDeviceData();
};

#endif
