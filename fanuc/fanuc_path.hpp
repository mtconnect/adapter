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

#include "device_datum.hpp"
#include "condition.hpp"
#include "adapter.hpp"
#include "fanuc_axis.hpp"
#include <vector>


class StaticEvent : public Event
{
public:
	StaticEvent(const char *name = "") :
		Event(name)
	{
	}

	bool unavailable() override {
		return false; }
};


class FanucPath
{
public:
	FanucPath(Adapter *adapter, short pathNumber);
	~FanucPath();

	bool configure(unsigned short flibhndl);
	bool gatherData(unsigned short flibhndl);

protected:
	bool configureAxes(unsigned short flibhndl);
	bool configureSpindles(unsigned short flibhndl);

	bool getToolData(unsigned short flibhndl);
	bool getProgramInfo(unsigned short flibhndl);
	bool getStatus(unsigned short flibhndl);
	bool getAxisData(unsigned short flibhndl);
	bool getSpindleData(unsigned short flibhndl);

	void getCondition(unsigned short flibhndl, long alarm);
	Condition *translateAlarmNo(long num, int axis);

	void addDatum(DeviceDatum &datum, const char *name, const char *suffix);

protected:
	Adapter *mAdapter;

	short mPathNumber;

	// Path specific data items
	Execution mExecution;
	IntEvent mToolId;
	IntEvent mToolGroup;
	Event mProgramName;
	Event mProgramComment;
	IntEvent mLine;
	Event mBlock;
	StaticEvent mActiveAxes;
	ControllerMode mMode;
	EmergencyStop mEStop;

	Sample mPathFeedrate;
	PathPosition mPathPosition;
	Sample mCommandedFeedrate;

	int mProgramNum;
	short mSpindleCount;
	short mAxisCount;

	bool mToolManagementEnabled;
	bool mUseModalToolData;

	// Path related conditions
	Condition mServo;
	Condition mComms;
	Condition mLogic;
	Condition mMotion;
	Condition mSystem;

	FanucAxis *mXAxis, *mYAxis, *mZAxis;

	std::vector<FanucAxis*> mAxes;
	std::vector<FanucSpindle*> mSpindles;
};
