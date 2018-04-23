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

#include "internal.hpp"
#include "fanuc_path.hpp"
#include <string>
#include <Fwlib32.h>
#include <logger.hpp>

using namespace std;

// C++11 helper function for obtaining a statically defined array's item count
template <typename T, std::size_t N>
constexpr std::size_t countof(T const (&)[N]) noexcept
{
	return N;
}


void FanucPath::addDatum(DeviceDatum &datum, const char *name, const char *suffix)
{
	std::string datumName(name);
	datumName.append(suffix);
	datum.setName(datumName.c_str());
	mAdapter->addDatum(datum);
}


FanucPath::FanucPath(Adapter *adapter, short pathNumber) :
	mAdapter(adapter),
	mPathNumber(pathNumber),
	mProgramNum(-1),
	mSpindleCount(0),
	mAxisCount(0),
	mToolManagementEnabled(true),
	mUseModalToolData(false),
	mXAxis(nullptr),
	mYAxis(nullptr),
	mZAxis(nullptr)
{
	auto pathNumAsString = std::to_string(pathNumber);
	if(pathNumber <= 1)
		pathNumAsString.clear();

	addDatum(mToolId, "tool_id", pathNumAsString.c_str());
	addDatum(mToolGroup, "tool_group", pathNumAsString.c_str());
	addDatum(mProgramName, "program", pathNumAsString.c_str());
	addDatum(mProgramComment, "program_comment", pathNumAsString.c_str());
	addDatum(mLine, "line", pathNumAsString.c_str());
	addDatum(mBlock, "block", pathNumAsString.c_str());
	addDatum(mPathFeedrate, "path_feedrate", pathNumAsString.c_str());
	addDatum(mPathPosition, "path_position", pathNumAsString.c_str());
	addDatum(mActiveAxes, "active_axes", pathNumAsString.c_str());
	addDatum(mMode, "mode", pathNumAsString.c_str());
	addDatum(mServo, "servo", pathNumAsString.c_str());
	addDatum(mComms, "comms", pathNumAsString.c_str());
	addDatum(mLogic, "logic", pathNumAsString.c_str());
	addDatum(mMotion, "motion", pathNumAsString.c_str());
	addDatum(mSystem, "system", pathNumAsString.c_str());
	addDatum(mExecution, "execution", pathNumAsString.c_str());
	addDatum(mCommandedFeedrate, "f_command", pathNumAsString.c_str());

	// Only track estop on the first path. Should be the same for all
	// paths, only one estop per machine.
	if (pathNumber == 1)
		addDatum(mEStop, "estop", pathNumAsString.c_str());
}


FanucPath::~FanucPath()
{
	for (auto axis : mAxes)
		delete axis;
	mAxes.clear();

	for (auto spindle : mSpindles)
		delete spindle;
	mSpindles.clear();
}


bool FanucPath::configure(unsigned short flibhndl)
{
	auto ret = cnc_setpath(flibhndl, mPathNumber);
	if (ret != EW_OK)
	{
		gLogger->error("Could not cnc_setpath: %d");
		return false;
	}

	gLogger->info("Configuration for path %d:", mPathNumber);

	// Get system info for this path:
	ODBSYS sysinfo = {0};
	ret = cnc_sysinfo(flibhndl, &sysinfo);
	if (ret == EW_OK)
	{
		gLogger->info("  Max Axis: %d", sysinfo.max_axis);
		gLogger->info("  CNC Type: %c%c", sysinfo.cnc_type[0], sysinfo.cnc_type[1]);
		gLogger->info("  MT Type: %c%c", sysinfo.mt_type[0], sysinfo.mt_type[1]);
		gLogger->info("  Series: %c%c%c%c", sysinfo.series[0], sysinfo.series[1], sysinfo.series[2], sysinfo.series[3]);
		gLogger->info("  Version: %c%c%c%c", sysinfo.version[0], sysinfo.version[1], sysinfo.version[2], sysinfo.version[3]);
		gLogger->info("  Axes: %c%c", sysinfo.axes[0], sysinfo.axes[1]);
	}


	return configureAxes(flibhndl) && configureSpindles(flibhndl);
}


bool FanucPath::configureSpindles(unsigned short flibhndl)
{
	ODBSPDLNAME spindles[MAX_SPINDLE] = {0};
	mSpindleCount = static_cast<short>(countof(spindles));
	short ret = cnc_rdspdlname(flibhndl, &mSpindleCount, spindles);
	if (ret != EW_OK)
	{
		gLogger->error("Failed to get splindle names: %d", ret);
		return false;
	}


	for (auto i = 0; i < mSpindleCount; i++)
	{
		const auto &spindle = spindles[i];
		gLogger->info("Spindle %d : %c%c%c%c", i, spindle.name, spindle.suff1, spindle.suff2, spindle.suff3);
		std::string name = {spindle.name, spindle.suff1};
		if (mPathNumber > 1)
			name.append(std::to_string(mPathNumber));

		mSpindles.push_back(new FanucSpindle(mAdapter, name.c_str(), i));
	}

	return true;
}


bool FanucPath::configureAxes(unsigned short flibhndl)
{
	const int num = 1;
	short types[num] = { 1 /* actual position */ };
	short len = MAX_AXIS;
	ODBAXDT axisData[MAX_AXIS * num] = {0};
	auto ret = cnc_rdaxisdata(
		flibhndl,
		1, // Position Value
		types,
		num,
		&len,
		axisData);

	bool hasAxisData = ret == EW_OK;
	if (!hasAxisData)
		gLogger->error("cnc_rdaxisdata returned %d for path %d", ret, mPathNumber);

	ODBAXISNAME axes[MAX_AXIS] = {0};
	mAxisCount = MAX_AXIS;
	ret = cnc_rdaxisname(flibhndl, &mAxisCount, axes);
	if (ret != EW_OK)
	{
		gLogger->error("Failed to get axis names: %d\n", ret);
		exit(999);
	}

	string activeAxes;
	for (auto i = 0; i < mAxisCount; i++)
	{
		bool add = true;
		gLogger->info("Axis %d : %c%c", i, axes[i].name, axes[i].suff);
		if (hasAxisData)
		{
			gLogger->info("Axis %s #%d - actual (unit %d flag 0x%X)",
				axisData[i].name,
				i,
				axisData[i].unit,
				axisData[i].flag);

			// Skip this axis if it isn't displayed
			if ((axisData[i].flag & 0x01) == 0)
			{
				gLogger->info("  This is an non-display axis, skipping");
				add = false;
			}

			switch (axisData[i].unit)
			{
			case 0:
				gLogger->info(" Units: mm");
				break;
			case 1:
				gLogger->info(" Units: inch");
				break;
			case 2:
				gLogger->info(" Units: degree");
				break;
			case 3:
				gLogger->info(" Units: mm/minute");
				break;
			case 4:
				gLogger->info(" Units: inch/minute");
				break;
			case 5:
				gLogger->info(" Units: rpm");
				break;
			case 6:
				gLogger->info(" Units: mm/round");
				break;
			case 7:
				gLogger->info(" Units: inch/round");
				break;
			case 8:
				gLogger->info(" Units: %%");
				break;
			case 9:
				gLogger->info(" Units: Ampere");
				break;
			}
		}

		if (!add)
			continue;

		if (mAxes.size() > 0)
			activeAxes += " ";

		std::string axisName = {axes[i].name, axes[i].suff};
		activeAxes.append(axisName.c_str());

		auto axis = new FanucAxis(mAdapter, axisName.c_str(), i);
		mAxes.push_back(axis);

		if (axes[i].name == 'X' && (!axes[i].suff || mXAxis == nullptr))
			mXAxis = axis;
		else if (axes[i].name == 'Y' && (!axes[i].suff || mYAxis == nullptr))
			mYAxis = axis;
		else if (axes[i].name == 'Z' && (!axes[i].suff || mZAxis == nullptr))
			mZAxis = axis;

	}
	mActiveAxes.setValue(activeAxes.c_str());


	short count, inprec[MAX_AXIS] = {0}, outprec[MAX_AXIS] = {0};
	ret = cnc_getfigure(flibhndl, 0, &count, inprec, outprec);
	if (ret != EW_OK)
	{
		gLogger->error("Failed to get axis scale: %d\n", ret);
		return false;
	}

	for (auto i = 0u; i < mAxes.size(); i++)
		mAxes[i]->mDivisor = pow((long double) 10.0, (long double) inprec[i]);

	return true;
}


bool FanucPath::gatherData(unsigned short flibhndl)
{
	auto ret = cnc_setpath(flibhndl, mPathNumber);
	if (ret != EW_OK)
	{
		gLogger->error("Cannot set path to: %d, %d", ret);
		return false;
	}

	return getProgramInfo(flibhndl) &&
		getStatus(flibhndl) &&
		getAxisData(flibhndl) &&
		getSpindleData(flibhndl) &&
		getToolData(flibhndl);
}


bool FanucPath::getProgramInfo(unsigned short flibhndl)
{
	char buf[1024] = {0};
	unsigned short len = sizeof(buf);
	short num(0);
	auto ret = cnc_rdexecprog(flibhndl, (unsigned short*) &len, &num, buf);
	if (ret == EW_OK)
	{
		for (auto i = 0; i < len; i++)
		{
			if (buf[i] == '\n')
			{
				buf[i] = '\0';
				break;
			}
		}
		mBlock.setValue(buf);
		return true;
	}
	else
	{
		gLogger->error("Cannot cnc_rdexecprog for path %d: %d", mPathNumber, ret);
		return false;
	}
}


bool FanucPath::getStatus(unsigned short flibhndl)
{
	ODBST status = {0};
	auto ret = cnc_statinfo(flibhndl, &status);
	if (ret != EW_OK)
	{
		gLogger->error("Cannot cnc_statinfo for path %d: %d", mPathNumber, ret);
		return false;
	}

	if (status.run == 3 || status.run == 4) // STaRT
		mExecution.setValue(Execution::eACTIVE);
	else
	{
		if (status.run == 2 || status.motion == 2 || status.mstb != 0) // HOLD or motion is Wait
			mExecution.setValue(Execution::eINTERRUPTED);
		else if (status.run == 0) // STOP
			mExecution.setValue(Execution::eSTOPPED);
		else
			mExecution.setValue(Execution::eREADY);
	}

	// This will take care of JOG
	if (status.aut == 5 || status.aut == 6)
		mMode.setValue(ControllerMode::eMANUAL);
	else if (status.aut == 0 ||status.aut == 3) // MDI and EDIT
		mMode.setValue(ControllerMode::eMANUAL_DATA_INPUT);
	else // Otherwise AUTOMATIC
		mMode.setValue(ControllerMode::eAUTOMATIC);

	if (mPathNumber == 1)
	{
		if (status.emergency == 1)
			mEStop.setValue(EmergencyStop::eTRIGGERED);
		else
			mEStop.setValue(EmergencyStop::eARMED);
	}
	return true;
	
}


bool FanucPath::getToolData(unsigned short flibhndl)
{
	if (mToolManagementEnabled)
	{
		ODBTLIFE4 toolId2 = {0};
		auto ret = cnc_toolnum(flibhndl, 0, 0, &toolId2);

		ODBTLIFE3 toolId = {0};
		ret = cnc_rdntool(flibhndl, 0, &toolId);
		if (ret == EW_OK && toolId.data != 0)
		{
			mToolId.setValue(toolId.data);
			mToolGroup.setValue(toolId.datano);
		}
		else
		{
			gLogger->warning("Cannot cnc_rdntool for path %d: %d", mPathNumber, ret);
			mToolManagementEnabled = false;
			gLogger->warning("Trying modal tool number", mPathNumber, ret);
			mUseModalToolData = true;
		}
	}

	if (mUseModalToolData)
	{
		ODBMDL command {0};
		auto ret = cnc_modal(flibhndl, 108, 1, &command);
		if (ret == EW_OK)
		{
			//gLogger->debug("cnc_modal returned: datano %d and type %d: %d %X %X",
			//  command.datano, command.type, command.modal.aux.aux_data, command.modal.aux.flag1,
			//  command.modal.aux.flag2);
			mToolId.setValue(command.modal.aux.aux_data);
		}
		else
		{
			gLogger->warning("cnc_modal failed for T on path %d: %d", mPathNumber, ret);
			mUseModalToolData = false;
		}
	}

	return true;
}


bool FanucPath::getAxisData(unsigned short flibhndl)
{
	if (mAxisCount <= 0)
		return true;

	ODBDY2 dyn = {0};
	auto ret = cnc_rddynamic2(flibhndl, ALL_AXES, sizeof(dyn), &dyn);
	if (ret != EW_OK)
	{
		gLogger->error("Cannot get the rddynamic2 data for path %d: %d", mPathNumber, ret);
		return false;
	}

	mLine.setValue(dyn.seqnum);

	ODBSVLOAD axLoad[MAX_AXIS] = {0};
	short num = MAX_AXIS;
	ret = cnc_rdsvmeter(flibhndl, &num, axLoad);
	if (ret != EW_OK)
	{
		gLogger->error("cnc_rdsvmeter failed for path %d: %d", mPathNumber, ret);
		return false;
	}

	char buf[32] = {0};
	mProgramNum = dyn.prgnum;
	sprintf(buf, "%d.%d", dyn.prgmnum, dyn.prgnum);
	mProgramName.setValue(buf);

	// Update all the axes
	for (auto axis : mAxes)
		axis->gatherData(&dyn, axLoad);

	mPathFeedrate.setValue(dyn.actf);

	// Get the modal feed for this path
	ODBMDL command = {0};
	ret = cnc_modal(flibhndl, 103, 1, &command);
	if (ret == EW_OK)
		mCommandedFeedrate.setValue(command.modal.aux.aux_data);

	double x = 0.0, y = 0.0, z = 0.0;
	if (mXAxis)
		x = dyn.pos.faxis.absolute[mXAxis->mIndex] / mXAxis->mDivisor;
	if (mYAxis)
		y = dyn.pos.faxis.absolute[mYAxis->mIndex] / mYAxis->mDivisor;
	if (mZAxis)
		z = dyn.pos.faxis.absolute[mZAxis->mIndex] / mZAxis->mDivisor;

	mPathPosition.setValue(x, y, z);

	getCondition(flibhndl, dyn.alarm);
	return true;
}


bool FanucPath::getSpindleData(unsigned short flibhndl)
{
	if (mSpindleCount <= 0)
		return true;

	// Handle spindle data...
	ODBACT2 speeds = {0};
	auto ret = cnc_acts2(flibhndl, ALL_SPINDLES, &speeds);
	if (ret != EW_OK)
	{
		gLogger->error("cnc_acts2 failed: %d for path number: %d", ret, mPathNumber);
		return false;
	}

	ODBSPLOAD spLoad[MAX_SPINDLE] = {0};
	short num = MAX_SPINDLE;
	ret = cnc_rdspmeter(flibhndl, 0, &num, spLoad);
	if (ret != EW_OK)
	{
		gLogger->error("cnc_rdspmeter failed: %d", ret);
		return false;
	}

	if (num > mSpindleCount)
	{
		gLogger->error("spindle load has more spindles than names: %d > %d\n",
						num, mSpindleCount);
		return false;
	}

	// Update all the axes
	for (auto spindle : mSpindles)
		spindle->gatherData(spLoad, &speeds);

	return true;
}


Condition *FanucPath::translateAlarmNo(long num, int axis)
{
	switch(num)
	{
	case 0: // Parameter Switch Off
		return &mLogic;

	case 2: // I/O
	case 7: // Data I/O
		return &mComms;

	case 4: // Overtravel
		if (axis > 0)
			return &(mAxes[axis - 1]->mTravel);
		else
			return &mSystem;

	case 5: // Overheat
		if (axis > 0)
			return &(mAxes[axis - 1]->mOverheat);
		else
			return &mSystem;

	case 6: // Servo
		if (axis > 0)
			return &(mAxes[axis - 1]->mServo);
		else
			return &mServo;

	case 12: // Background P/S
	case 3: // Forground P/S
	case 8: // Macro
		return &mMotion;

	case 9: // Spindle
		return &(mSpindles[0]->mServo);

	case 19: // PMC
		return &mLogic;

	default: // 10, 11, 13, 15.
		return &mSystem;
	}

	return nullptr;
}


void FanucPath::getCondition(unsigned short flibhndl, long alarm)
{
	if (!alarm)
		return;

	// Check each bit in turn, if any are set then get the associated error information
	// Only the first 20 bits are used.
	for (auto i = 0; i < 20; i++)
	{
		if (alarm & (0x1 << i))
		{
			ODBALMMSG2 alarms[MAX_AXIS] = {0};
			short num = MAX_AXIS;
			auto ret = cnc_rdalmmsg2(flibhndl, i, &num, alarms);
			if (ret != EW_OK)
				continue;

			for (auto j = 0; j < num; j++)
			{
				auto const &focasAlarm = alarms[j];
				auto condition = translateAlarmNo(i, focasAlarm.axis);
				if (!condition)
					continue;
				std::string alarmMessage(focasAlarm.alm_msg, focasAlarm.msg_len);
				condition->add(
					Condition::eFAULT,
					alarmMessage.c_str(),
					std::to_string(focasAlarm.alm_no).c_str());
			}
		}
	}

}

