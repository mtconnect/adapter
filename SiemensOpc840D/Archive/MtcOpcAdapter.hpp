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
//
// MtcOpcAdapter.hpp
//
#pragma once

#include "adapter.hpp"
#include "device_datum.hpp"
#include "service.hpp"
#include "condition.hpp"
#include "atlcom.h"
#include "Config.h"
#include "OpcAdapter.h"
#include "atlutil.h"

class COpcAdapter;


class MtcOpcAdapter : public Adapter, public MTConnectService
{
protected:
	/* Define all the data values here */

	/* Events */
	Availability mAvailability; 
	std::map<std::string, Event *> _events;
	std::map<std::string, Condition *> _conditions;

public:
	MtcOpcAdapter(int aPort);
	~MtcOpcAdapter();

	virtual void AddSample(std::string name);
	virtual void AddEvent(std::string name);
	virtual void AddCondition(std::string name);

	virtual void SetSample(std::string name, std::string value);
	virtual void SetEvent(std::string name, std::string value);
	virtual void SetCondition(std::string name, std::string value);

	virtual void initialize( // overides Adapter::initialize
		int aArgc, 
		const char *aArgv[]);
	virtual void start();	// overrides Adapter::start
	virtual void stop();	// overrides Adapter::stop

	/* Instantiate Pure virtual method to get the data from the device. */

	virtual void gatherDeviceData();

	COpcAdapter * _opcadapter;

	// Resisted temptation to include boost...
	// OPC Specific threading to allow bad opc servers to reset
	static UINT __stdcall  ResetAtMidnightThread(void *oObject);

	// OPC Specific threading to allow service to behave properly
	//static DWORD WINAPI StartOpcThread(void * pObj);
	//virtual void  StartOpc();
	//void  StartOPCMachines();
	void StopAll();
	static DWORD WINAPI StopThread(void * pObj);
	void StopOPCMachines();


	Utilities::Config	_iniconfig;
	std::string _inifile;

	bool _bOPCEnabled;
	bool _bResetAtMidnight;
	CHandle _hThread;
	unsigned int _threadID;
	bool _bRunning;

	std::vector<COpcAdapter *> _machines;
	std::vector<std::string> _servers ;
	std::vector<std::string> _devices ;
	std::vector<std::string> _tags;

	CWorkerThread<> _workerthread;
	struct CStartThread : public IWorkerThreadClient
	{
		CStartThread()
		{ 
			_hEvent.Attach(CreateEvent(NULL, TRUE, FALSE, NULL)); 
		}
		HRESULT Execute(DWORD_PTR dwParam, HANDLE hObject);
		HRESULT CloseHandle(HANDLE){ return S_OK; }
		CHandle _hEvent;
	} _StartThread;

	struct CEndThread : public IWorkerThreadClient
	{
		CEndThread()
		{ 
			_hEvent.Attach(CreateEvent(NULL, TRUE, FALSE, NULL)); 
		}
		HRESULT Execute(DWORD_PTR dwParam, HANDLE hObject){ return S_OK; }
		HRESULT CloseHandle(HANDLE){ return S_OK; }
		CHandle _hEvent;
	} _EndThread;
	
	struct CResetThread : public IWorkerThreadClient
	{
		CResetThread()
		{ 
			_hEvent.Attach(CreateEvent(NULL, TRUE, FALSE, NULL)); 
		}
		HRESULT Execute(DWORD_PTR dwParam, HANDLE hObject){ return S_OK; }
		HRESULT CloseHandle(HANDLE){ return S_OK; }
		CHandle _hEvent;
	} _ResetThread;

};


