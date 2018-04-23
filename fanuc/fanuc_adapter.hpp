//
// Copyright Copyright 2012, System Insights, Inc.
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.
//
#pragma once
#include <vector>
#include <memory>
#include <Fwlib32.h>
#include <adapter.hpp>
#include <condition.hpp>
#include <device_datum.hpp>
#include <service.hpp>
#include "fanuc_path.hpp"


constexpr int MAX_MACROS = 256; // Max number of entries in the ini file
constexpr int MAX_PMC = 32;


class MacroSample : public Sample
{
protected:
	int mNumber;

public:
	MacroSample(const char *aName, int aNum) :
		Sample(aName),
		mNumber(aNum)
	{
	}

	int getNumber() {
		return mNumber; }
};


class MacroPathPosition : public PathPosition
{
protected:
	int m_X;
	int m_Y;
	int m_Z;

public:
	 MacroPathPosition(const char *name, int X, int Y, int Z) :
		PathPosition(name),
		 m_X(X),
		 m_Y(Y),
		 m_Z(Z)
	{
	}

	int getX() {
		return m_X; }
	int getY() {
		return m_Y; }
	int getZ() {
		return m_Z; }
};


//
// Provides a connection to the data available from the FANUC Focus library.
//
class FanucAdapter : public Adapter, public MTConnectService
{
protected:
	// Define all the data values here
	std::vector<std::unique_ptr<FanucPath>> mPaths;


	// Conditions

	// Events
	Message mMessage;

	Availability mAvail;
	IntEvent mPartCount;

	// Macro variables
	std::vector<MacroSample*> mMacroSamples;
	std::vector<MacroPathPosition*> mMacroPathPositions;
	int mMacroMin;
	int mMacroMax;

	// Macro variables
	IntEvent *mPMCVariable[MAX_PMC];
	int mPMCAddress[MAX_PMC];
	int mPMCCount;

	unsigned short mFlibhndl;
	bool mConnected, mConfigured;
	int mDevicePort;
	char mDeviceIP[64];

protected:
	void connect();
	void configure();
	void configMacrosAndPMC(const char *iniFile);

	void reconnect();
	void disconnect();
	void getMessages();

	void getPathData();

	void getMacros();
	void getPMC();

	void getCounts();

	void innerGatherDeviceData();

public:
	FanucAdapter(int serverPort);
	~FanucAdapter();

	// For Service
	void initialize(int argc, const char *argv[]) final;
	void start() final;
	void stop() final;

	void gatherDeviceData() final;
};
