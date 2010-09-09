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

HaasAdapter::HaasAdapter(int aPort)
  : Adapter(aPort, 1000), 
    mZeroRet("servo"), mMessage("message"), mExecution("execution"),
    mXact("Xact"), mYact("Yact"), mZact("Zact"), 
    mSpindleSpeed("spindle_speed"), 
    mProgram("program"), mMode("mode"), 
    mPartCount("PartCount"), mEstop("estop"), mSystem("system"),
    mAvail("avail"), mPositions(false)
{
  addDatum(mMessage);
  addDatum(mSystem);
  addDatum(mZeroRet);
  addDatum(mSystem);
  addDatum(mExecution);
  addDatum(mXact);
  addDatum(mYact);
  addDatum(mZact);
  addDatum(mSpindleSpeed);
  addDatum(mProgram);
  addDatum(mMode);
  addDatum(mPartCount);
  addDatum(mEstop);
  addDatum(mAvail);
    
  
  mErrorString[0] = 0;
}

void HaasAdapter::initialize(int aArgc, const char *aArgv[])
{
  MTConnectService::initialize(aArgc, aArgv);
  
  int port = 7878;
  int i = 0;

  while (aArgv[i][0] == '-' && aArgc > 0)
  {
    if (aArgv[i][1] == 'p')
      mPositions = true;
    else
    {
      gLogger->error("Invalid option: %s\nUsage: adapter [-p] <Serial_COM> [port]", aArgv[i]);
      exit(1);
    }

    i++;
    aArgc--;
  }

  if (aArgc < 1)
  {
    gLogger->error("Usage: %s <Serial_COM>", aArgv[0]);
    exit(1);
  }

  if (aArgc > 2)
    mPort = atoi(aArgv[i + 1]);
    
  /* Construct the adapter and start the server */
  mSerial = new HaasSerial(aArgv[i], 19200, "none", 7, 1);
}

void HaasAdapter::start()
{
  startServer();
}

void HaasAdapter::stop()
{
  stopServer();
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
  unavailable();
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
      mZeroRet.setValue(Condition::eFAULT, "NO ZERO X");
    else
      mZeroRet.setValue(Condition::eNORMAL);
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
            mEstop.setValue(EmergencyStop::eTRIGGERED);
          else
            mEstop.setValue(EmergencyStop::eARMED);
        }
        
        mSystem.setValue(Condition::eFAULT, "Alarm on indicator");
      }
      else
      {
        mEstop.setValue(EmergencyStop::eARMED);
        mSystem.setValue(Condition::eNORMAL);
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
        flush();
        mAvail.available();
        if (mPositions)
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
    gLogger->error("SerialError: %s\n", e.message());
    disconnect();
  }
}

