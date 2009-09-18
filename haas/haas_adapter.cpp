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
#include "haas_adapter.hpp"

using namespace std;

HaasAdapter::HaasAdapter(int aPort, HaasSerial *aSerial)
  : Adapter(aPort, 1000), 
    mAlarm("alarm"), mZeroRet("alarm"), mMessage("alarm"), mPower("power"), mExecution("execution"),
    mLine("line"), mXact("Xact"), mYact("Yact"), mZact("Zact"), 
    mXcom("Xcom"), mYcom("Ycom"), mZcom("Zcom"),
    mSpindleSpeed("spindle_speed"), mPathFeedrate("path_feedrate"),
    mProgram("program"), mMode("mode"), mBlock("block"),
    mPathFeedrateOverride("feed_ovr"), mSpindleSpeedOverride("SspeedOvr"),
    mLineMax("line_max"), mPartCount("PartCount"), mEstop("alarm")
{
  addDatum(mAlarm);
  addDatum(mMessage);
  addDatum(mZeroRet);
  addDatum(mPower);
  addDatum(mExecution);
  addDatum(mLine);
  addDatum(mXact);
  addDatum(mYact);
  addDatum(mZact);
  addDatum(mXcom);
  addDatum(mYcom);
  addDatum(mZcom);
  addDatum(mSpindleSpeed);
  addDatum(mPathFeedrate);
  addDatum(mProgram);
  addDatum(mMode);
  addDatum(mBlock);
  addDatum(mPathFeedrateOverride);
  addDatum(mSpindleSpeedOverride);
  addDatum(mLineMax);
  addDatum(mPartCount);
  addDatum(mEstop);
    
  
  mErrorString[0] = 0;

  mSerial = aSerial;
}

bool HaasAdapter::connect()
{
  bool ret = mSerial->connect();
  if (ret)
  {
    mSerial->flushInput();
  }
  
  return ret;    
}

void HaasAdapter::disconnect()
{
  mSerial->disconnect();
  mPower.setValue(Power::eOFF);
}

void HaasAdapter::actual()
{
  double result;
  if (mSerial->getVariable(5041, result))
  {
    mBuffer.timestamp();
    mXact.setValue(result);
    flush();
  }
  
  if (mSerial->getVariable(5042, result))
  {
    mBuffer.timestamp();
    mYact.setValue(result);
    flush();
  }

  if (mSerial->getVariable(5043, result))
  {
    mBuffer.timestamp();
    mZact.setValue(result);
    flush();
  }
}

void HaasAdapter::commanded()
{
}

void HaasAdapter::spindle()
{
  double result;
  if (mSerial->getVariable(3027, result))
  {
    mBuffer.timestamp();
    mSpindleSpeed.setValue(result);
    flush();
  }
}

void HaasAdapter::feedrate()
{
  
}

void HaasAdapter::execution()
{
  vector<string> *res = mSerial->sendCommand("Q104");
  if (res && res->size() > 1)
  {
    string &mode = res->at(1);
    if (mode == "(MDI)")
      mMode.setValue(ControllerMode::eMANUAL_DATA_INPUT);
    else if (mode == "(JOG)" || mode == "(ZERO RET)")
      mMode.setValue(ControllerMode::eMANUAL);
    else 
      mMode.setValue(ControllerMode::eAUTOMATIC);

    if (mode == "(ZERO RET)")
      mZeroRet.setValue(Alarm::eOTHER, "NO ZERO X", Alarm::eCRITICAL, Alarm::eACTIVE, "NO ZERO X");
    else
      mZeroRet.setValue(Alarm::eOTHER, "NO ZERO X", Alarm::eCRITICAL, Alarm::eCLEARED, "NO ZERO X");
  }
  if (res)
    delete res;
  
  res = mSerial->sendCommand("Q500");
  if (res && res->size() > 1)
  {
    string first = res->at(0);
    string second = res->at(1);

    if (first == "PROGRAM")
    {
      if (second != "MDI")
	mProgram.setValue(second.c_str());
      
      string third = res->at(2);
      if (third == "IDLE")
	mExecution.setValue(Execution::eREADY);
      else if (third == "FEED HOLD")
	mExecution.setValue(Execution::eINTERRUPTED);
      
      if (third == "ALARM ON")
      {
	mExecution.setValue(Execution::eSTOPPED);
	double result;
	if (mSerial->getVariable(1007, result))
	{
	  if (result > 0.0)
	    mEstop.setValue(Alarm::eESTOP, "ESTOP", Alarm::eCRITICAL, Alarm::eACTIVE, "ESTOP Active");
	  else
	    mEstop.setValue(Alarm::eESTOP, "ESTOP", Alarm::eCRITICAL, Alarm::eCLEARED, "ESTOP Cleared");
	}
	
	mAlarm.setValue(Alarm::eOTHER, "ALARM ON", Alarm::eCRITICAL, Alarm::eACTIVE, "Alarm on indicator");
      }
      else
      {
	mEstop.setValue(Alarm::eESTOP, "ESTOP", Alarm::eCRITICAL, Alarm::eCLEARED, "ESTOP Cleared");
	mAlarm.setValue(Alarm::eOTHER, "ALARM ON", Alarm::eCRITICAL, Alarm::eCLEARED, "Alarm on indicator");
      }
      
      if (res->at(3) == "PARTS")
	mPartCount.setValue(atoi(res->at(4).c_str()));
    }
    
    if (first == "STATUS" && second == "BUSY")
      mExecution.setValue(Execution::eACTIVE);
  }
  
  if (res)
    delete res;

  flush();
}

HaasAdapter::~HaasAdapter()
{
  disconnect();
}

void HaasAdapter::gatherDeviceData()
{
  try
  {
    if (!mSerial->connected())
    {
      if (!connect())
		sleep(5);
    }
    else
    {
      mSerial->flushInput();

      // Test connection and make sure we're synched before we start
      vector<string> *ret = mSerial->sendCommand("Q100");
      if (ret && ret->size() == 2 && ret->front() == "S/N")
      {
	mBuffer.timestamp();
	mPower.setValue(Power::eON);
	actual();
	commanded();
	spindle();
	execution();
      }
      else
      {
	disconnect();
      }
    }
  }
  
  catch (Serial::SerialError &e)
  {
    printf("SerialError: %s\n", e.message());
    disconnect();
  }
}

