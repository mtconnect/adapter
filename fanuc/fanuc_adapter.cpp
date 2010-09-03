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
#include "fanuc_adapter.hpp"
#include "minIni.h"

FanucAdapter::FanucAdapter(int aPort) : 
  Adapter(aPort), 
  mAvail("avail"), mExecution("execution"), mLine("line"),
  mPathFeedrate("path_feedrate"), 
  mProgram("program"), mBlock("block"), mProgramInfo("program_info"),
  mMode("mode"), mMessage("message"),
  mEstop("estop"), mPathPosition("path_pos"),
  mServo("servo"), mComms("comms"), mLogic("logic"),
  mMotion("motion"), mSystem("system"), mSpindle("spindle")
{
  /* Alarms */
  addDatum(mMessage);
  addDatum(mEstop);
  addDatum(mServo);
  addDatum(mComms);
  addDatum(mLogic);
  addDatum(mMotion);
  addDatum(mSystem);
  addDatum(mSpindle);

  /* Controller */
  //addDatum(mProgramInfo);
  addDatum(mProgram);
  addDatum(mAvail);
  addDatum(mExecution);
  addDatum(mLine);
  addDatum(mPathFeedrate);
  addDatum(mMode);
  addDatum(mBlock);

  addDatum(mPathPosition);

  mConfigured = mConnected = false;
  mAxisCount = mSpindleCount = mMacroSampleCount = mPMCCount =
	       mMacroPathCount = 0;
  mXPathIndex = mYPathIndex = mZPathIndex = -1;
  mAvail.unavailable();
}

FanucAdapter::~FanucAdapter()
{
  int i;
  for (i = 0; i < mAxisCount; i++)
  {
    delete mAxisAct[i];
    delete mAxisCom[i];
    delete mAxisLoad[i];
    delete mAxisTravel[i];
    delete mAxisOverheat[i];
    delete mAxisServo[i];
  }

  for (i = 0; i < mSpindleCount; i++)
  {
    delete mSpindleSpeed[i];
    delete mSpindleLoad[i];
  }
  
  disconnect();
}

void FanucAdapter::initialize(int aArgc, const char *aArgv[])
{
  MTConnectService::initialize(aArgc, aArgv);
  mDeviceIP = aArgv[0];
  mDevicePort = atoi(aArgv[1]);  

  int port = 7878;
  if (aArgc > 3)
    port = atoi(aArgv[2]);  
}

void FanucAdapter::start()
{
  startServer();
}

void FanucAdapter::stop()
{
  stopServer();
}

void FanucAdapter::gatherDeviceData()
{
  if (!mConnected)
    connect();
  else
  {
    getPositions();
    getAxisLoad();
    getSpindleLoad();
    getStatus();
    getMessages();
    getMacros();
    getPMC();
  }
}

void FanucAdapter::disconnect()
{
  if (mConnected)
  {
    printf("Machine has disconnected. Releasing Resources\n");
    cnc_freelibhndl(mFlibhndl);  
    mConnected = false;
    unavailable();
  }
}

void FanucAdapter::configSpindleNames()
{
  ODBSPDLNAME spindles[MAX_SPINDLE];
  mSpindleCount = MAX_SPINDLE;
  short ret = cnc_rdspdlname(mFlibhndl, &mSpindleCount, spindles);
  if (ret == EW_OK)
  {
    int i = 0;
    for (i = 0; i < mSpindleCount; i++)
    {
      printf("Spindle %d : %c%c%c\n", i, spindles[i].name, spindles[i].suff1, spindles[i].suff2);
      char name[12];
      int j = 0;
      name[j++] = spindles[i].name;
      if (spindles[i].suff1 > 0)
        name[j++] =  spindles[i].suff1;
      name[j] = '\0';

      char *cp = name + j;
      strcpy(cp, "speed");
      mSpindleSpeed[i] = new Sample(name);
      addDatum(*mSpindleSpeed[i]);
      strcpy(cp, "load");
      mSpindleLoad[i] = new Sample(name); 
      addDatum(*mSpindleLoad[i]);
    }
  }
  else
  {
    printf("Failed to get splindle names: %d\n", ret);
  }
}


void FanucAdapter::configAxesNames()
{
  ODBAXISNAME axes[MAX_AXIS];
  mAxisCount = MAX_AXIS;
  short ret = cnc_rdaxisname(mFlibhndl, &mAxisCount, axes);
  if (ret == EW_OK)
  {
    int i = 0;
    for (i = 0; i < mAxisCount; i++)
    {
      printf("Axis %d : %c%c\n", i, axes[i].name, axes[i].suff);
      char name[12];
      int j = 0;
      name[j++] = axes[i].name;
      if (axes[i].suff > 0)
        name[j++] =  axes[i].suff;
      name[j] = '\0';

      if (axes[i].name == 'X' && (axes[i].suff == 0 || mXPathIndex == -1))
	mXPathIndex = i;
      else if (axes[i].name == 'Y' && (axes[i].suff == 0 || mYPathIndex == -1))
	mYPathIndex = i;
      else if (axes[i].name == 'Z' && (axes[i].suff == 0 || mZPathIndex == -1))
	mZPathIndex = i;

      char *cp = name + j;
      strcpy(cp, "act");
      mAxisAct[i] = new Sample(name);
      addDatum(*mAxisAct[i]);
      strcpy(cp, "com");
      mAxisCom[i] = new Sample(name); 
      addDatum(*mAxisCom[i]);
      strcpy(cp, "load");
      mAxisLoad[i] = new Sample(name); 
      addDatum(*mAxisLoad[i]);
      strcpy(cp, "travel");
      
      mAxisTravel[i] = new Condition(name);
      addDatum(*mAxisTravel[i]);
      strcpy(cp, "overtemp");
      mAxisOverheat[i] = new Condition(name);
      addDatum(*mAxisOverheat[i]);
      strcpy(cp, "servo");
      mAxisServo[i] = new Condition(name);
      addDatum(*mAxisServo[i]);

      mAxisDivisor[i] = 1.0;

    }
  }
  else
  {
    printf("Failed to get axis names: %d\n", ret);
    exit(999);
  }

  short count, inprec[MAX_AXIS], outprec[MAX_AXIS];
  ret = cnc_getfigure(mFlibhndl, 0, &count, inprec, outprec);
  if (ret == EW_OK)
  {
    for (int i = 0; i < count; i++)
      mAxisDivisor[i] = pow((long double) 10.0, (long double) inprec[i]);
  }
  else
  {
    printf("Failed to get axis scale: %d\n", ret);
  }
}

void FanucAdapter::configMacrosAndPMC()
{
  static char *ini_file = "adapter.ini";
          
  // Read adapter.ini to get additional macro variables and
  // PMC registers
  char name[100];
  int idx;
  const static char *sDigits = "0123456789";

  mMacroMin = 99999;
  mMacroMax = 0;
          
  // First look for macro variables
  for (idx = 0;
       ini_getkey("macros", idx, name, sizeof(name), ini_file) > 0 &&
             idx < MAX_MACROS;
       idx++)
  {
    char numbers[256];
    ini_gets("macros", name, "", numbers, 256, ini_file);
    if (numbers[0] == '[')
    {
      // We have a path macro.
      int x, y, z;
      char *cp = numbers + 1, *n;
      x = strtol(cp, &n, 10);
      if (cp == n)
	continue;
      cp = n;
      y = strtol(cp, &n, 10);
      if (cp == n)
	continue;
      cp = n;
      z = strtol(cp, &n, 10);
      if (cp == n)
	continue;
      
      int i = mMacroPathCount++;
      mMacroPath[i] = new MacroPathPosition(name, x, y, z);
      addDatum(*mMacroPath[i]);

      printf("Adding path macro '%s' at location %d %d %d\n", name, x, y, z);

      if (x > mMacroMax) mMacroMax = x;
      if (x < mMacroMin) mMacroMin = x;
      if (y > mMacroMax) mMacroMax = y;
      if (y < mMacroMin) mMacroMin = y;
      if (z > mMacroMax) mMacroMax = z;
      if (z < mMacroMin) mMacroMin = z;
    }
    else
    {
      char *cp = numbers, *n;
      long v = strtol(cp, &n, 10);
      if (cp == n)
	continue;
      int i = mMacroSampleCount++;
      mMacroSample[i] = new MacroSample(name, v);
      addDatum(*mMacroSample[i]);

      printf("Adding sample macro '%s' at location %d\n", name, v);
      
      if (v > mMacroMax) mMacroMax = v;
      if (v < mMacroMin) mMacroMin = v;
    }
    
    
  }

  for (idx = 0;
       ini_getkey("pmc", idx, name, sizeof(name), ini_file) > 0 &&
             idx < MAX_PMC;
       idx++)
  {
    long v = ini_getl("pmc", name, 0, ini_file);
    mPMCVariable[idx] = new IntEvent(name);
    mPMCAddress[idx] = v;

    addDatum(*mPMCVariable[idx]);

    printf("Adding pmc '%s' at location %d\n", name, v);
  }
  mPMCCount = idx;
}

void FanucAdapter::configure()
{
  if (mConfigured || !mConnected)
    return;

  printf("Configuring...\n");
  cnc_sysinfo(mFlibhndl, &mInfo);

  configAxesNames();
  configSpindleNames();
  configMacrosAndPMC();

  mConfigured = true;
          
}

void FanucAdapter::connect()
{
  if (mConnected)
    return;
          
  printf("Connecting to Machine at %s and port %d\n", mDeviceIP, mDevicePort);
  short ret = ::cnc_allclibhndl3(mDeviceIP, mDevicePort, 10, &mFlibhndl);
  printf("Result: %d\n", ret);
  if (ret == EW_OK) 
  {
    mConnected = true;
    if (!mConfigured) configure();
    
    mAvail.available();

    // Set all conditions to normal
    mServo.setValue(Condition::eNORMAL);
    mComms.setValue(Condition::eNORMAL);
    mLogic.setValue(Condition::eNORMAL);
    mMotion.setValue(Condition::eNORMAL);
    mSystem.setValue(Condition::eNORMAL);
    mSpindle.setValue(Condition::eNORMAL);

    for (int i = 0; i < mAxisCount; i++)
    {
      mAxisTravel[i]->setValue(Condition::eNORMAL);
      mAxisOverheat[i]->setValue(Condition::eNORMAL);
      mAxisServo[i]->setValue(Condition::eNORMAL);
    }
    
  }
  else
  {
    mConnected = false;
    unavailable();
    Sleep(5000);
  }  
}

void FanucAdapter::reconnect()
{
  if (mConnected)
  {
    cnc_freelibhndl(mFlibhndl);  
    mConnected = false;

    connect();
  }
}

void FanucAdapter::getPositions()
{
  if (!mConnected)
    return;

  ODBDY2 dyn;
  short ret = cnc_rddynamic2(mFlibhndl, -1, sizeof(dyn), &dyn);
  if (ret == EW_OK)
  {
    for (int i = 0; i < mAxisCount; i++)
      mAxisAct[i]->setValue(dyn.pos.faxis.machine[i] / mAxisDivisor[i]);
    
    mPathPosition.setValue(dyn.pos.faxis.absolute[mXPathIndex] / mAxisDivisor[mXPathIndex],
			   dyn.pos.faxis.absolute[mYPathIndex] / mAxisDivisor[mYPathIndex],
			   dyn.pos.faxis.absolute[mZPathIndex] / mAxisDivisor[mZPathIndex]);
    
    char buf[32];
    mProgramNum = dyn.prgnum;
    sprintf(buf, "%d.%d", dyn.prgmnum, dyn.prgnum);
    if (mProgram.setValue(buf))
      getHeader();
            
    mPathFeedrate.setValue(dyn.actf);
    if (mSpindleCount > 0)
      mSpindleSpeed[0]->setValue(dyn.acts);
            
    /* Get the commanded positions *
       ODBMDL ModalData;
       ret = cnc_modal(mFlibhndl, -3, 1, &ModalData);
       if (ret == EW_OK)
       {
       for (int i = 0; i < mAxisCount; i++)
       mAxisCom[i]->setValue(ModalData.modal.raux2[i].aux_data / mAxisDivisor[i]);
       }
    */

    
    getCondition(dyn.alarm);
  }
  else
  {
    disconnect();
  }

  /* Only works on 15i... 
  ODB3DHDL tooltip[2] ;
  ret = cnc_rd3dtooltip(mFlibhndl, tooltip);
  if (ret == EW_OK)
  {
    // Set Path position
    printf("%d %d %d : %d %d %d\n", tooltip[0].axes[0], tooltip[0].axes[1],
	   tooltip[0].axes[2], tooltip[0].data[0], tooltip[0].data[1], tooltip[0].data[2]);
  }
  */
}

void FanucAdapter::getStatus()
{
  if (!mConnected)
    return;

  ODBST status;
  int ret = cnc_statinfo(mFlibhndl, &status);
  if (ret == EW_OK)
  {
    if (status.aut == 5 || status.aut == 6) // other than no selection
      mMode.setValue(ControllerMode::eMANUAL);
    else if (status.aut == 0) // MDI for aut
      mMode.setValue(ControllerMode::eMANUAL_DATA_INPUT);
    else // Other than MDI and Manual
      mMode.setValue(ControllerMode::eAUTOMATIC);
              
    if (status.run == 3) // STaRT
      mExecution.setValue(Execution::eACTIVE);
    else 
    {
      if (status.run == 2 || status.run == 4 || status.motion == 2 || status.mstb != 0) // HOLD or motion is Wait
        mExecution.setValue(Execution::eINTERRUPTED);
      else if (status.run == 0) // STOP
        mExecution.setValue(Execution::eSTOPPED);
      else
        mExecution.setValue(Execution::eREADY);
    }
    if (status.emergency == 1)
	  mEstop.setValue(EmergencyStop::eTRIGGERED);
    else
	  mEstop.setValue(EmergencyStop::eARMED);

    char buf[1024];
    unsigned short len = sizeof(buf);
    short num;
    ret = cnc_rdexecprog(mFlibhndl, (unsigned short*) &len, &num, buf);
    if (ret == EW_OK)
    {
      buf[len] = '\0';
      for (int i = 0; i < len; i++)
      {
        if (buf[i] == '\n')
        {
          buf[i] = '\0';
          break;
        }
      }
              
      mBlock.setValue(buf);
      mLine.setValue(num);
    }
  }
  else
  {
    disconnect();
  }
}

void FanucAdapter::getMacros()
{
  if (mMacroSampleCount == 0 && mMacroPathCount == 0)
    return;
  
  // For now we assume they are close in range. If this proves to not
  // be true, we will have to get more creative.
  IODBMR *macros = new IODBMR[mMacroMax - mMacroMin];
  short ret = cnc_rdmacror(mFlibhndl, mMacroMin, mMacroMax, 
                           sizeof(IODBMR) * (mMacroMax - mMacroMin + 1),
                           macros);
  
  if (ret == EW_OK) {
    for (int i = 0; i < mMacroSampleCount; i++)
    {
      int off = mMacroSample[i]->getNumber() - mMacroMin;
      if (macros->data[off].mcr_val != 0 || macros->data[off].dec_val != -1)
      {
        mMacroSample[i]->setValue(((double) macros->data[off].mcr_val) /
                            pow(10.0, macros->data[off].dec_val));
      }
    }
    for (int i = 0; i < mMacroPathCount; i++)
    {
      int x = mMacroPath[i]->getX() - mMacroMin;
      int y = mMacroPath[i]->getY() - mMacroMin;
      int z = mMacroPath[i]->getZ() - mMacroMin;
      
      if ((macros->data[x].mcr_val != 0 || macros->data[x].dec_val != -1) &&
	  (macros->data[y].mcr_val != 0 || macros->data[y].dec_val != -1) &&
	  (macros->data[z].mcr_val != 0 || macros->data[z].dec_val != -1))
      {
	mMacroPath[i]->setValue(((double) macros->data[x].mcr_val) /
				  pow(10.0, macros->data[x].dec_val),
				((double) macros->data[y].mcr_val) /
				  pow(10.0, macros->data[y].dec_val),
				((double) macros->data[z].mcr_val) /
				  pow(10.0, macros->data[z].dec_val));
      }
    }
  }
  else
  {
    printf("Could not read macro variables: %d\n", ret);
  }

  delete[] macros;
}

void FanucAdapter::getPMC()
{
  for (int i = 0; i < mPMCCount; i++)
  {
    IODBPMC buf;
    short ret = pmc_rdpmcrng(mFlibhndl, 0 /* G */, 0 /* byte */,
                             mPMCAddress[i], mPMCAddress[i], 8 + 1,
                             &buf);
    if (ret == EW_OK)
    {
      if (buf.u.cdata[0] < 0)
        mPMCVariable[i]->setValue(-buf.u.cdata[0] - 1);
      else
        mPMCVariable[i]->setValue(buf.u.cdata[0]);
    }
    else
    {
      printf("Could not retrieve PMC data at %d for %s: %d\n",
             mPMCAddress[i], mPMCVariable[i]->getName(), ret);
    }
  }
}

void FanucAdapter::getMessages()
{
  if (!mConnected)
    return;
          
  OPMSG messages[6];
  int ret = cnc_rdopmsg(mFlibhndl, 0, 6 + 256, messages);
  if (ret == EW_OK && messages->datano != -1)
  {
    char buf[32];
    sprintf(buf, "%04", messages->datano);
    mMessage.setValue(messages->data, buf);
  }
}

Condition *FanucAdapter::translateAlarmNo(long aNum, int aAxis)
{
  switch(aNum) 
  {
  case 0: // Parameter Switch Off
    return &mLogic;

  case 2: // I/O
  case 7: // Data I/O
    return &mComms;

  case 4: // Overtravel
    if (aAxis > 0)
      return mAxisTravel[aAxis - 1];
    else
      return &mSystem;

  case 5: // Overheat
    if (aAxis > 0)
      return mAxisOverheat[aAxis - 1];
    else
      return &mSystem;

  case 6: // Servo
    if (aAxis > 0)
      return mAxisServo[aAxis - 1];
    else
      return &mServo;

  case 12: // Background P/S
  case 3: // Forground P/S
  case 8: // Macro
    return &mMotion;

  case 9: // Spindle
    return &mSpindle;
    
  case 19: // PMC
    return &mLogic;
    
  default: // 10, 11, 13, 15.
    return &mSystem;
  }

  return NULL;
}

void FanucAdapter::getCondition(long aAlarm)
{
  if (aAlarm != 0) 
  {
    for (int i = 0; i < 31; i++) 
    {
      if (aAlarm & (0x1 << i))
      {
	ODBALMMSG2 alarms[MAX_AXIS];
	short num = MAX_AXIS;
	
	short ret = cnc_rdalmmsg2(mFlibhndl, i, &num, alarms);
	if (ret != EW_OK)
	  continue;

	for (int j = 0; j < num; j++) 
	{
	  ODBALMMSG2 &alarm = alarms[j];
	  char num[16];
	  
	  Condition *cond = translateAlarmNo(i, alarm.axis);
	  if (cond == NULL)
	    continue;

	  sprintf(num, "%d", alarm.alm_no);
	  if (cond->setValue(Condition::eFAULT, alarm.alm_msg, num))
		mActiveConditions.add(cond);
	}
      }
    }       
  } else {
    ConditionList::Iterator it = mActiveConditions.begin();
    while(!it.end())
    {
      Condition *cond = it;
      cond->setValue(Condition::eNORMAL);
      it.remove();
    }
  }
}

void FanucAdapter::getAxisLoad()
{
  if (!mConnected)
    return;

  ODBSVLOAD load[MAX_AXIS];
  short num = MAX_AXIS;
  short ret = cnc_rdsvmeter(mFlibhndl, &num, load);
  if (ret == EW_OK) {
    if (num > mAxisCount)
    {
      printf("Error: Axis load has more axes than names: %d > %d\n",
             num, mSpindleCount);
      return;
    }
            
    for (int i = 0; i < mAxisCount; i++)
      mAxisLoad[i]->setValue(load[i].svload.data /pow((long double) 10.0,
                                                      (long double) load[0].svload.dec));
  }
}

void FanucAdapter::getSpindleLoad()
{
  if (!mConnected)
    return;

  ODBSPLOAD load[4];
  short num = MAX_AXIS;
  short ret = cnc_rdspmeter(mFlibhndl, 0, &num, load);
  if (ret == EW_OK) {
    if (num > mSpindleCount)
    {
      printf("Error: spindle load has more spindles than names: %d > %d\n",
             num, mSpindleCount);
      return;
    }
          
    for (int i = 0; i < num; i ++)
      mSpindleLoad[i]->setValue(load[i].spload.data / pow( (long double) 10.0,
                                                           ( long double) load[i].spload.dec));
  }
}

void FanucAdapter::getHeader()
{
  /* This is not needed since we're getting the codes from
     macros now. */
  return;
  
  char program[2048];
  short ret = cnc_upstart(mFlibhndl, mProgramNum);
  if (ret == EW_OK)
  {
    long len = sizeof(program);
    do 
    {
      ret = cnc_upload3(mFlibhndl, &len, program);
      if (ret == EW_OK)
      {
        program[len] = '\0';
        mProgramInfo.setValue(program);
      }
    } while (ret == EW_BUFFER);
  }
  cnc_upend3(mFlibhndl);
}
