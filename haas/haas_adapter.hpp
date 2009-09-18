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

#ifndef HAAS_ADAPTER_HPP
#define HAAS_ADAPTER_HPP

#include "adapter.hpp"
#include "device_datum.hpp"
#include "string_array.hpp"
#include "haas_adapter.hpp"
#include "haas_serial.hpp"

#define LINELEN 1024

class HaasSerial;

class HaasAdapter : public Adapter 
{
protected:
  /* Define all the data values here */
  
  /* Define all the data values here */
  
  /* Events */
  Alarm mAlarm;
  Alarm mZeroRet;
  Alarm mEstop;
  Alarm mMessage;
  Power mPower;
  Execution mExecution;
  IntEvent mLine; 
  IntEvent mLineMax;
  IntEvent mPartCount;
  Event mProgram;
  ControllerMode mMode;
  Event mBlock;
  
  /* Samples */
  /* Linear Axis */
  Sample mXact;
  Sample mYact;
  Sample mZact;
  
  Sample mXcom;
  Sample mYcom;
  Sample mZcom;
  
  /* Spindle */
  Sample mSpindleSpeed;  
  Sample mSpindleSpeedOverride;  
  
  /* Path Feedrate */
  Sample mPathFeedrate;
  Sample mPathFeedrateOverride;
  
protected:
  char mErrorString[LINELEN];
  StringArray mBlocks;
  HaasSerial *mSerial;
  
protected:
  bool connect();
  void disconnect();
  void actual();
  void commanded();
  void spindle();
  void feedrate();
  void execution();
  
public:
  HaasAdapter(int aPort, HaasSerial *aSerial);
  ~HaasAdapter();
  
  virtual void gatherDeviceData();
};

#endif

