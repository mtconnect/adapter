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
#include "device_datum.hpp"
#include "Fwlib32.h"

#define MAX_MACROS 32
#define MAX_PMC 32

/* 
 * Provides a connection to the data available from the FANUC Focus library.
 */
class FanucAdapter : public Adapter 
{
protected:
  /* Define all the data values here */
  
  /* Events */
  StatefullAlarm mOvertravel;
  StatefullAlarm mOverheat;
  StatefullAlarm mServo;
  StatefullAlarm mDataIo;
  StatefullAlarm mMacroAlarm;
  StatefullAlarm mSpindleAlarm;
  StatefullAlarm mEstop;
  Alarm mMessage;
  Alarm mAlarm;
  
  Power mPower;
  Execution mExecution;
  IntEvent mLine; 
  ControllerMode mMode;
  Event mProgram;
  Event mProgramInfo;
  Event mBlock;

  /* Macro variables */
  IntEvent *mMacro[MAX_MACROS];
  int       mMacroNumber[MAX_MACROS];
  int       mMacroMin;
  int       mMacroMax;
  int       mMacroCount;
  
  /* Macro variables */
  IntEvent *mPMCVariable[MAX_PMC];
  int       mPMCAddress[MAX_PMC];
  int       mPMCCount;
  
  /* Samples */
  /* Linear Axis */
  Sample *mAxisAct[MAX_AXIS];
  Sample *mAxisCom[MAX_AXIS];
  Sample *mAxisLoad[MAX_AXIS];

  /* Spindle */
  Sample *mSpindleSpeed[MAX_SPINDLE];
  Sample *mSpindleLoad[MAX_SPINDLE];
  
  /* Path Feedrate */
  Sample mPathFeedrate;
  
  unsigned short mFlibhndl;
  bool mConnected, mConfigured;
  int mDevicePort;
  const char *mDeviceIP;
  int mProgramNum;
  ODBSYS mInfo;
  double mAxisDivisor[MAX_AXIS];

  short mAxisCount, mSpindleCount;

protected:
  void connect();
  void configure();
  void configAxesNames();
  void configSpindleNames();
  void configMacrosAndPMC();
  
  void reconnect();
  void disconnect();
  void getPositions();
  void getLine();
  void getStatus();
  void getMessages();
  void getAlarms();
  void setAlarm(StatefullAlarm &aAlarm, int data, int bit_mask);
  void getAxisLoad();
  void getSpindleLoad();
  void getHeader();

  void getMacros();
  void getPMC();

public:
  FanucAdapter(int aServerPort, const char *aDeviceIP, int aDevicePort);
  ~FanucAdapter();
  
  virtual void gatherDeviceData();
};

#endif
