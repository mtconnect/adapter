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

FanucAdapter::FanucAdapter(int aPort)
  : Adapter(aPort), mAvail("avail"), mMessage("message"), mPartCount("part_count"),
    mMacroSampleCount(0), mMacroPathCount(0), mPMCCount(0)
{
  /* Alarms */
  addDatum(mMessage);

  /* Controller */
  addDatum(mAvail);
  addDatum(mPartCount);

  mConfigured = mConnected = false;
  mAvail.unavailable();
}

FanucAdapter::~FanucAdapter()
{
  size_t i;

  for (i = 0; i < mPaths.size(); i++)
    delete mPaths[i];
  mPaths.clear();
  
  disconnect();
}

void FanucAdapter::initialize(int aArgc, const char *aArgv[])
{
  MTConnectService::initialize(aArgc, aArgv);

  const char *iniFile = "adapter.ini";

  if (aArgc > 1) {
    strncpy(mDeviceIP, aArgv[0], MAX_HOST_LEN - 1);
    mDeviceIP[MAX_HOST_LEN - 1] = '\0';
    mDevicePort = atoi(aArgv[1]);
    
    mPort = 7878;
    if (aArgc > 2)
      mPort = atoi(aArgv[2]);
  }
  else
  {
    mDevicePort = 8193;
    strcpy(mDeviceIP, "localhost");
    mPort = 7878;
    if (aArgc > 0)
      iniFile = aArgv[0];
  }
  
  configMacrosAndPMC(iniFile);
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
    getPathData();
    getMessages();
    getMacros();
    getPMC();
    getCounts();
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


void FanucAdapter::configMacrosAndPMC(const char *aIniFile)
{
  // Read adapter configuration
  mPort = ini_getl("adapter", "port",  mPort, aIniFile);
  ini_gets("adapter", "service", "MTConnect Fanuc Adapter", mName,
           SERVICE_NAME_LEN, aIniFile);

  ini_gets("focus", "host", mDeviceIP, mDeviceIP, MAX_HOST_LEN, aIniFile);
  mDevicePort = ini_getl("focus", "port", mDevicePort, aIniFile);

  // Read adapter.ini to get additional macro variables and
  // PMC registers
  char name[100];
  int idx;
  const static char *sDigits = "0123456789";

  mMacroMin = 99999;
  mMacroMax = 0;
          
  // First look for macro variables
  for (idx = 0;
       ini_getkey("macros", idx, name, sizeof(name), aIniFile) > 0 &&
             idx < MAX_MACROS;
       idx++)
  {
    char numbers[256];
    ini_gets("macros", name, "", numbers, 256, aIniFile);
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
       ini_getkey("pmc", idx, name, sizeof(name), aIniFile) > 0 &&
             idx < MAX_PMC;
       idx++)
  {
    long v = ini_getl("pmc", name, 0, aIniFile);
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

  gLogger->info("Configuring...\n");
  cnc_sysinfo(mFlibhndl, &mInfo);

  int ret;
  short path;
  ret = cnc_getpath(mFlibhndl, &path, &mMaxPath);
  if (ret != EW_OK)
  {
    gLogger->error("Cannot find number of paths: %d", ret);
    mMaxPath = 1;
    path = 1;
  }

  for (int i = 1; i <= mMaxPath; i++) 
  {
    FanucPath *path = new FanucPath(this, i);
    if (!path->configure(mFlibhndl))
    {
      gLogger->error("Could not configure path: %d", i);
      exit(1);
    }
    
    mPaths.push_back(path);
  }

  gLogger->info("Current path: %d, maximum paths: %d", path, mMaxPath);

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
    mAvail.available();
    mConnected = true;
    if (!mConfigured) configure();

    // Resets conditions to normal.
    initializeDeviceDatum();    
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

void FanucAdapter::getCounts()
{
  // Should just be a parameter read
  IODBPSD buf;
  short ret = cnc_rdparam(mFlibhndl, 6711, 0, 8, &buf);
  if (ret == EW_OK)
  {
    mPartCount.setValue(buf.u.idata);
  }
}

void FanucAdapter::getPathData()
{
  int i;
  for (i = mMaxPath - 1; i >= 0; i--)
  {
    if (!mPaths[i]->gatherData(mFlibhndl))
    {
      disconnect();
      break;
    }
  }
  if (mConnected && i > 0)
    cnc_setpath(mFlibhndl, 0);
}

