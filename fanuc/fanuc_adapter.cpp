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

FanucAdapter::FanucAdapter(int aPort, const char *aDeviceIP, int aDevicePort) : 
  Adapter(aPort), 
  mPower("power"), mExecution("execution"), mLine("line"),
  mPathFeedrate("path_feedrate"), 
  mProgram("program"), mBlock("block"), mProgramInfo("program_info"),
  mMode("mode"), mMessage("alarm"),
  mEstop("alarm", Alarm::eESTOP, "EMerGency", Alarm::eCRITICAL, "EMerGency Status Set"),
  mOvertravel("alarm", Alarm::eOVERLOAD, "Overtravel", Alarm::eCRITICAL, "Overtravel alarm"),
  mOverheat("alarm", Alarm::eOVERLOAD, "Overheat", Alarm::eCRITICAL, "Overheat alarm"),
  mServo("alarm", Alarm::eFAULT, "Servo", Alarm::eCRITICAL, "Servo alarm"),
  mSpindleAlarm("alarm", Alarm::eFAULT, "Spindle", Alarm::eCRITICAL, "Spindle alarm"),
  mMacroAlarm("alarm", Alarm::eOTHER, "Macro", Alarm::eINFO, "Macro alarm"),
  mDataIo("alarm", Alarm::eOTHER, "DataIO", Alarm::eERROR, "DataIO error"), mAlarm("alarm")
{
  /* Alarms */
  addDatum(mMessage);
  addDatum(mEstop);
  addDatum(mOvertravel);
  addDatum(mOverheat);
  addDatum(mServo);
  addDatum(mSpindleAlarm);
  addDatum(mMacroAlarm);
  addDatum(mDataIo);
  addDatum(mAlarm);

  /* Controller */
  addDatum(mProgramInfo);
  addDatum(mProgram);
  addDatum(mPower);
  addDatum(mExecution);
  addDatum(mLine);
  addDatum(mPathFeedrate);
  addDatum(mMode);
  addDatum(mBlock);

  mDevicePort = aDevicePort;
  mDeviceIP = aDeviceIP;
  mConfigured = mConnected = false;
  mAxisCount = mSpindleCount = mMacroCount = mPMCCount = 0;
}

FanucAdapter::~FanucAdapter()
{
  int i;
  for (i = 0; i < mAxisCount; i++)
  {
    delete mAxisAct[i];
    delete mAxisCom[i];
    delete mAxisLoad[i];
  }

  for (i = 0; i < mSpindleCount; i++)
  {
    delete mSpindleSpeed[i];
    delete mSpindleLoad[i];
  }
          
  disconnect();
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
    getAlarms();
    getMacros();
    getPMC();
  }
}

void FanucAdapter::disconnect()
{
  if (mConnected)
  {
    printf("Machine has disconnected. Releasing Resources\n");
    mPower.setValue(Power::eOFF);
    cnc_freelibhndl(mFlibhndl);  
    mConnected = false;
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
    }
  }
  else
  {
    printf("Failed to get axis names: %d\n", ret);
    exit(999);
  }

  short count, inprec[MAX_AXIS], outprec[MAX_AXIS];;
  ret = cnc_getfigure(mFlibhndl, 0, &count, inprec, outprec);
  if (ret == EW_OK)
  {
    for (int i = 0; i < mAxisCount; i++)
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

  mMacroMin = 99999;
  mMacroMax = 0;
          
  // First look for macro variables
  for (idx = 0;
       ini_getkey("macros", idx, name, sizeof(name), ini_file) > 0 &&
             idx < MAX_MACROS;
       idx++)
  {
    long v = ini_getl("macros", name, 0, ini_file);
    mMacro[idx] = new IntEvent(name);
    mMacroNumber[idx] = v;
    addDatum(*mMacro[idx]);

    printf("Adding macro '%s' at location %d\n", name, v);
    
    if (v > mMacroMax) mMacroMax = v;
    if (v < mMacroMin) mMacroMin = v;
    
  }
  mMacroCount = idx;

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
    mPower.setValue(Power::eON);

    if (!mConfigured) configure();
  }
  else
  {
    mPower.setValue(Power::eOFF);
    mConnected = false;
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

  ODBDY dyn;
  short ret = cnc_rddynamic(mFlibhndl, -1, 24 + 16 * mInfo.max_axis, &dyn);
  if (ret == EW_OK)
  {
    for (int i = 0; i < mAxisCount; i++)
      mAxisAct[i]->setValue(dyn.pos.faxis.absolute[i] / mAxisDivisor[i]);
            
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
  }
  else
  {
    disconnect();
  }
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
      mEstop.active();
    else
      mEstop.cleared();

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
  if (mMacroCount == 0)
    return;
  
  // For now we assume they are close in range. If this proves to not
  // be true, we will have to get more creative.
  IODBMR macros;
  short ret = cnc_rdmacror(mFlibhndl, mMacroMin, mMacroMax, 
                           8 + 8 * (mMacroMax - mMacroMin + 1),
                           &macros);
  
  if (ret == EW_OK) {
    for (int i = 0; i < mMacroCount; i++)
    {
      int off = mMacroNumber[i] - mMacroMin;
      if (macros.data[off].mcr_val != 0 || macros.data[off].dec_val != -1)
      {
        mMacro[i]->setValue((int) ((double) macros.data[off].mcr_val) /
                            pow(10.0, macros.data[off].dec_val));
      }
    }
  }
  else
  {
    printf("Could not read macro variables: %d\n", ret);
  }
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
    mMessage.setValue(Alarm::eMESSAGE, buf, Alarm::eINFO, Alarm::eINSTANT, messages->data);
  }
}

void FanucAdapter::setAlarm(StatefullAlarm &aAlarm, int data, int bit_mask)
{
  if (data & bit_mask)
    aAlarm.active();
  else
    aAlarm.cleared();
}

void FanucAdapter::getAlarms()
{
  ODBALM buf ;
  cnc_alarm(mFlibhndl, &buf ) ;

  int data = buf.data;
  setAlarm(mDataIo, data, (0x1 << 2) | (0x1 << 7));
  setAlarm(mServo, data, 0x1 << 6);
  setAlarm(mMacroAlarm, data, 0x1 << 8);
  setAlarm(mOverheat, data, 0x1 << 5);
  setAlarm(mOvertravel, data, 0x1 << 4);
  setAlarm(mSpindleAlarm, data, 0x1 << 9); 
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
