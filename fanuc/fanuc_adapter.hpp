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

#ifndef FANUC_ADAPTER_HPP
#define FANUC_ADAPTER_HPP

#include "adapter.hpp"
#include "condition.hpp"
#include "device_datum.hpp"
#include "service.hpp"
#include "fanuc_path.hpp"
#include "fwlib32.h"
#include <vector>
#include <math.h>

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

	/* PMC variables */
	IntEvent *mPMCVariable[MAX_PMC];
	int       mPMCAddress[MAX_PMC];
	int       mPMCCount;

	/* Parameters */
	IntEvent *mParameterVariable[MAX_PMC];
	int       mParameterAddress[MAX_PMC];
	int       mParameterCount;

	/* Diagnostics */
	IntEvent *mDgnVariable[MAX_PMC];
	int       mDgnAddress[MAX_PMC];
	int       mDgnCount;

	/* Alarms */
	Event *mAlmVariable[MAX_PMC];
	int       mAlmAddress[MAX_PMC];
	int       mAlmCount;

	/* Critical */
	Event *mCriticalVariable[MAX_PMC];
	int       mCriticalAddress[MAX_PMC];
	int       mCriticalCount;

	unsigned short mFlibhndl;
	bool mConnected, mConfigured;
	bool mAllowDNC;
	bool mDisplayHidden;
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
	void getParameters();
	void getDgn();
	void getAlarm();
	void getCritical();

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
