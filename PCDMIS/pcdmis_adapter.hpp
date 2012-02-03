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

#ifndef PCDMIS_ADAPTER_HPP
#define PCDMIS_ADAPTER_HPP

#include "adapter.hpp"
#include "device_datum.hpp"
#include "service.hpp"
#include "condition.hpp"
#include "atlcom.h"
#include "Config.h"
#import "pcdlrn.tlb"
#include "PartProgramSink.h"
#include "MachineEvents.h"
#include <vector>

class PcDmisAdapter : public Adapter, public MTConnectService
{
protected:
	/* Define all the data values here */

	/* Events */
	Availability mAvailability; 
	Condition mSystem; 
	Sample mFeedrate;
	Execution mExecution;
	Event mProgram;
	Event mPower;
	ControllerMode mMode;
	IntEvent mPartCount;
	IntEvent mBadPartCount;
	IntEvent mProbed;

	//PathPosition mSetPoint;
	//PathPosition mActualPoint;
	Sample mSetPoint;
	Sample mActualPoint;
	Sample mTolerance;
	Sample mPlusTolerance;
	Sample mMinusTolerance;
	Sample mMinDeviation;
	Sample mMaxDeviation;
	Sample mDeviation;
	Sample mOuttol;
	Event mProbeName;
	Event mCharacteristicType;
	Event mFeatureType;
	IntEvent mProbedId;

public:
	PcDmisAdapter(int aPort);
	~PcDmisAdapter();

	virtual void initialize(int aArgc, const char *aArgv[]);
	virtual void start();
	virtual void stop();
	virtual void connect();
	virtual void disconnect();
	virtual bool IsConnnected();
	/* Instantiate Pure virtual method to get the data from the device. */

	virtual void gatherDeviceData();
	bool ConnectMachine();


	virtual void clear();
	void UpdatePartCount(int partcount);
	void ClearPartCounts(){_nPartCount=_nGoodPartCount=0;}
	int UpdateProbingOperations();

	static const int OFF=1;
	static const int RUNNING=2;
	static const int DISCONNECTING=2;
	int _state;

	///////////////////////////////////////////////////////////////////
	CComPtr<IDispatch> _appdispatch;
	CComPtr<IDispatch> _progdispatch;
	CComPtr<PCDLRN::IApplication> pPCDmisApp;
	CComPtr<PCDLRN::IMachines> pMachines;
	CComPtr<PCDLRN::IMachine> pMachine;
	CComPtr<PCDLRN::IExecutedCommands> pExecutedCommands;

	PCDLRN::IApplicationObjectEventsPtr pAppObjEvents;
	CPartProgramSink * m_pAppEventListener;
	CMachineEvents  * m_pMachineListener;

	Utilities::Config	_iniconfig;
	int _nSleep;
	int _nCycleTime;  // configuration for cycle time
	bstr_t _bstrPartName;
	bstr_t _bstrRevisionNumber;

	long _nSpeed;
	long _nError;
	std::string _sProgid ;
	std::string _sPcdmisId;
	CLSID _clsdid;
	long _nPartCount;
	long _nGoodPartCount;
	long _manauto;
	bstr_t _bstrProbeName;


	long	_probed ;
	double _SetPoint;
	double _ActualPoint;
	double _Tolerance;
	double _PlusTolerance;
	double _MinusTolerance;
	double _MinDeviation;
	double _MaxDeviation;
	double _Deviation;
	double _Outtol;
	std::string _ProbeName;
	std::string _CharacteristicType;
	std::string _FeatureType;
	std::string _ProbedId;

};

#endif

