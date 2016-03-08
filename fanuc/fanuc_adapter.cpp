/*
 * Copyright Copyright 2012, System Insights, Inc.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */
 
#include "internal.hpp"
#include "fanuc_adapter.hpp"
#include "minIni.h"
#include <excpt.h>

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

  printf("Arguments: %d\n", aArgc);
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
    printf("Ini File: %s\n", iniFile);
  }
  
  FILE *fp = fopen(iniFile, "r");
  printf("FP = %d, %x\n", errno, fp);
  if (fp != 0) fclose(fp);

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

void FanucAdapter::innerGatherDeviceData()
{
  try
  {
    if (!mConnected)
      connect();
    else
    {
      getPathData();
      getMessages();
      getMacros();
      getPMC();
	  getParameters();
	  getDgn();
      getCounts();
    }
  }
  catch (...)
  {
    gLogger->error("Unhandled exception occurred during gathering device data, disconnecting.");
    disconnect();
  }
}

void FanucAdapter::gatherDeviceData()
{
  __try {
    innerGatherDeviceData();
  }

  __except(EXCEPTION_EXECUTE_HANDLER) {
      gLogger->error("Unhandled structured exception occurred during gathering device data, disconnecting.");
      disconnect();
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
	mPort = ini_getl("adapter", "port", mPort, aIniFile);
	ini_gets("adapter", "service", "MTConnect Fanuc Adapter", mName,
		SERVICE_NAME_LEN, aIniFile);
	mScanDelay = ini_getl("adapter", "scanDelay", mScanDelay, aIniFile);

	ini_gets("focus", "host", mDeviceIP, mDeviceIP, MAX_HOST_LEN, aIniFile);
	mDevicePort = ini_getl("focus", "port", mDevicePort, aIniFile);

	char dnc[8];
	ini_gets("focus", "dnc", "yes", dnc, 8, aIniFile);
	mAllowDNC = _strnicmp(dnc, "no", 2) != 0;

	if (!mAllowDNC)
		printf("Disabling retrieval of program header\n");

	//********** Added for DisplayHidden Setting *******
	char dhdn[8];
	ini_gets("settings", "displayhidden", "no", dhdn, 8, aIniFile);
	mDisplayHidden = _strnicmp(dhdn, "no", 2) != 0;

	if (!mDisplayHidden)
		printf("Not showing hidden axis.\n");
	if (mDisplayHidden)
		printf("Showing hidden axis.\n");
	//***************************************************


	// Read adapter.ini to get additional macro variables and
	// PMC registers
	char name[100];
	char paraName[100];
	char dgnName[100];

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
		long v = ini_getl("pmc", name, 0, aIniFile); // was long
		mPMCVariable[idx] = new IntEvent(name);
		mPMCAddress[idx] = v;

		addDatum(*mPMCVariable[idx]);

		printf("Adding pmc '%s' at location %d\n", name, v);
	}
	mPMCCount = idx;

	//Read Parameters from ini file
	for (idx = 0;
		ini_getkey("parameters", idx, paraName, sizeof(paraName), aIniFile) > 0 &&
		idx < MAX_PMC;
	idx++)
	{
		long v = ini_getl("parameters", paraName, 0, aIniFile); // was long
		mParameterVariable[idx] = new IntEvent(paraName);
		mParameterAddress[idx] = v;

		addDatum(*mParameterVariable[idx]);

		printf("Adding parameter '%s' at location %d\n", paraName, v);
	}
	mParameterCount = idx;


	//Read Diagnostics from ini file
	for (idx = 0;
		ini_getkey("diagnostics", idx, dgnName, sizeof(paraName), aIniFile) > 0 &&
		idx < MAX_PMC;
	idx++)
	{
		long v = ini_getl("diagnostics", dgnName, 0, aIniFile); // was long
		mDgnVariable[idx] = new IntEvent(dgnName);
		mDgnAddress[idx] = v;

		addDatum(*mDgnVariable[idx]);

		printf("Adding diagnostic '%s' at location %d\n", dgnName, v);
	}
	mDgnCount = idx;
}

void FanucAdapter::configure()
{
  if (mConfigured || !mConnected)
    return;

  int ret;
  gLogger->info("Configuring...\n");

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

	  path->allowDNC(mAllowDNC);
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

	// TODO really needs cleanup accross control models, dgn types, etc


	if (!mConnected)
		return;
	for (int i = 0; i < mMacroSampleCount; i++)
	{
		ODBM macro;
		short ret = cnc_rdmacro(mFlibhndl, mMacroSample[i]->getNumber(), sizeof(ODBM), &macro);
		if (ret == EW_OK)
		{
			double rational = macro.mcr_val;
			double decimal = macro.dec_val;
			double exp = pow(10, decimal);
			double resultant = rational / exp;
			mMacroSample[i]->setValue(resultant);
		}
		else
		{
			printf("Could not retrieve Macro data at %d for %s: %d\n", mMacroSample[i], mMacroSample[i]->getNumber(), ret);
		}
	}
}

void FanucAdapter::getParameters()
{

	// TODO really needs cleanup accross control models, dgn types, etc


	if (!mConnected)
		return;

	for (int i = 0; i < mParameterCount; i++)
	{
		ODBPARAIF odbparaif;
		// Find out the parameter type (really ineffecient, should be read and stored)
		short ret = cnc_rdparainfo(mFlibhndl, mParameterAddress[i], 1, &odbparaif);
		if (ret == EW_OK){
			// Should just be a parameter read
			IODBPSD buf;
			ret = cnc_rdparam(mFlibhndl, mParameterAddress[i], 0, 8, &buf);
			if (ret == EW_OK)
			{
				if ((odbparaif.info[0].prm_type & 0x03) == 1)
				{
					mParameterVariable[i]->setValue(buf.u.cdata);
				}
				else if ((odbparaif.info[0].prm_type & 0x03) == 2)
				{
					mParameterVariable[i]->setValue(buf.u.ldata);
				}	
				else if ((odbparaif.info[0].prm_type & 0x03) == 3)
				{
					mParameterVariable[i]->setValue(buf.u.ldata);
				}
			}
		}

		else {
			printf("Could not retrieve Parameter data at %d for %s: %d\n",
				mParameterAddress[i], mParameterVariable[i]->getName(), ret);
		}
	}

}

void FanucAdapter::getPMC()
{

	// TODO really needs cleanup accross control models, dgn types, etc

	if (!mConnected)
		return;

	for (int i = 0; i < mPMCCount; i++)
	{
		IODBPMC buf;

		// Identify Handling Type

		bool isNegative = false;
		int pmcAddress = mPMCAddress[i];
		if (pmcAddress < 0) {
			isNegative = true;
			pmcAddress = 0 - pmcAddress;
		}
		// Seperate the data type
		int pmcType = pmcAddress / 10000;
		pmcAddress = pmcAddress % 10000;


		short ret = pmc_rdpmcrng(mFlibhndl, pmcType, 0,
			pmcAddress, pmcAddress, 8 + 1,
			&buf);
		if (ret == EW_OK)
		{
			if (isNegative){
				if (buf.u.cdata[0] < 0)
					mPMCVariable[i]->setValue(-buf.u.cdata[0] - 1);
				else
					mPMCVariable[i]->setValue(buf.u.cdata[0]);
			}
			else {
				if (buf.u.cdata[0] < 0)
					mPMCVariable[i]->setValue(256 + buf.u.cdata[0]);
				else
					mPMCVariable[i]->setValue(buf.u.cdata[0]);
			}
		}
		else {
			printf("Could not retrieve PMC data at %d for %s: %d\n",
				mPMCAddress[i], mPMCVariable[i]->getName(), ret);
		}
	}
}

void FanucAdapter::getDgn()
{

	// TODO really needs cleanup accross control models, dgn types, etc

	if (!mConnected)
		return;

	for (int i = 0; i < mDgnCount; i++)
	{

		// Seperate the data type
		int dgnAddress = mDgnAddress[i];
		int dgnAxis = dgnAddress / 10000;
		dgnAddress = dgnAddress % 10000;


		ODBDGN buf;
		ODBDIAGIF odbdiagif;

		short ret = cnc_rddiaginfo(mFlibhndl, dgnAddress, 1, &odbdiagif);
		if(ret == EW_OK)
		{

			int length;
			if ((odbdiagif.info[0].diag_type & 0x03) == 0)
			{
				length = 8;
			}
			else if ((odbdiagif.info[0].diag_type & 0x03) == 1)
			{
				length = 8;
			}
			else if ((odbdiagif.info[0].diag_type & 0x03) == 2)
			{
				length = 12;
			}
			else if ((odbdiagif.info[0].diag_type & 0x03) == 3)
			{
				length = 16;
			}

			(odbdiagif.info[0].diag_type && 0x03) * 8;

			short ret = cnc_diagnoss(mFlibhndl, dgnAddress, dgnAxis, length, &buf);
			if (ret == EW_OK)
			{
				if (length == 16)
				{ 
					mDgnVariable[i]->setValue(buf.u.ldata);
				}
				else if (length == 12)
				{
					mDgnVariable[i]->setValue(buf.u.cdata);
				}
					
			}
			else {
				printf("Could not retrieve Diagnostic data at %d for %s: %d\n",
					mDgnAddress[i], mDgnVariable[i]->getName(), ret);
			}
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
  if (!mConnected)
    return;
  
  // Should just be a parameter read
  IODBPSD buf;
  short ret = cnc_rdparam(mFlibhndl, 6711, 0, 8, &buf);
  if (ret == EW_OK)
  {
    mPartCount.setValue(buf.u.ldata);
  }
}

void FanucAdapter::getPathData()
{
  if (!mConnected)
    return;
  
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

