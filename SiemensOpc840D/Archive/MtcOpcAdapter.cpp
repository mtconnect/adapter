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
// opc840d_adapter.cpp
#include "stdafx.h"
#include "internal.hpp"
#include "MtcOpcAdapter.hpp"
#include <string>
#include <comdef.h>
#include <atlpath.h>
#include <exception>
#include "StdStringEx.h"

//////////////////////////////////////////////////////////////////////////////////////////
extern SERVICE_STATUS          gSvcStatus; 
extern VOID ReportSvcStatus( DWORD dwCurrentState, //    The current state (see SERVICE_STATUS)
							DWORD dwWin32ExitCode, //   The system error code
							DWORD dwWaitHint); //    Estimated time for pending operation, 


// Func for  Microsoft structured exception handler

static void trans_func( unsigned int u, EXCEPTION_POINTERS* pExp )
{
	OutputDebugString( StdStringFormat("MtcOpcAdapter trans_func - Code = 0x%x\n",  
		pExp->ExceptionRecord->ExceptionCode).c_str() );
	throw std::exception();
}

//////////////////////////////////////////////////////////////////////////////////////////

MtcOpcAdapter::MtcOpcAdapter(int aPort)
: Adapter(aPort, 1000),
mAvailability("avail")
{
	addDatum(mAvailability);
	_bResetAtMidnight=false;	  
	_bRunning=false;
	_bOPCEnabled=false; 
}

MtcOpcAdapter::~MtcOpcAdapter()
{
}

// Treat samples as events - simpler. Just store text.
// Dont need to worry about double, integer(no deadband), etc.
// Ignores floating deadband issues for now.
void MtcOpcAdapter::AddSample(std::string name)
{
	_events[name]=new Event(name.c_str());
	addDatum(*_events[name]);
}
void MtcOpcAdapter::AddEvent(std::string name)
{
	_events[name]=new Event(name.c_str());
	addDatum(*_events[name]);

}
void MtcOpcAdapter::AddCondition(std::string name)
{
	_conditions[name]=new Condition(name.c_str());
	addDatum(*_conditions[name]);

}
void MtcOpcAdapter::SetSample(std::string name, std::string value)
{
	if(_events.find(name) != _events.end())
		_events[name]->setValue(value.c_str());
	// FIXME:else  throw error
}
void MtcOpcAdapter::SetEvent(std::string name, std::string value)
{
	if(_events.find(name) != _events.end())
		_events[name]->setValue(value.c_str());
	// FIXME: else throw error

}
void MtcOpcAdapter::SetCondition(std::string name, std::string value)
{


}


void MtcOpcAdapter::initialize(int aArgc, const char *aArgv[])
{
	USES_CONVERSION;

	_inifile = ExeDirectory() + "\\adapter.ini";

	MTConnectService::initialize(aArgc, aArgv);
	if (aArgc > 1) {
		mPort = atoi(aArgv[1]);
	}
	OutputDebugString(StdStringFormat( "MTConnect Adapter Service Start %s\n" , COleDateTime::GetCurrentTime().Format() ).c_str());
	try{
		if(!ATL::ATLPath::FileExists(_inifile.c_str()))
			throw std::exception(StdStringFormat("Config file missing %s\n", _inifile.c_str()).c_str());

		// In case we eventually have version problems
		_iniconfig.load(_inifile);

		_bResetAtMidnight = ConvertFromString<bool>(_iniconfig.GetSymbolValue("OPCSERVER.ResetAtMidnight", "false").str(), false);
		_bOPCEnabled = ConvertFromString<bool>(_iniconfig.GetSymbolValue("OPCSERVER.OPCEnabled", "false").str(), false);;
		_servers = Tokenize(_iniconfig.GetSymbolValue("OPCSERVER.ServerMachineName", "").str(), ",");
		_devices = Tokenize(_iniconfig.GetSymbolValue("OPCSERVER.MTConnectDevice", "").str(), ",");
		_tags = Tokenize(_iniconfig.GetSymbolValue("OPCSERVER.OpcTags", "").str(), ",");
		if(_servers.size() != _devices.size() && _servers.size() != _tags.size())
			throw std::exception("OPC Number of Server IPs does not match number of MTConnect devices or Section Tags" );
	}
	catch(std::exception e)
	{
		OutputDebugString(StdStringFormat("Adapter::initialize() failed - %s\n", e.what()).c_str());
		throw e;
	}
	catch(...) {}
}

// Windows SCM more tolerant of slow starting processes than terminating processes.
void MtcOpcAdapter::start()
{
	_bRunning=true; 
	if(_bResetAtMidnight)
	{
		OutputDebugString("Adapter will Reset At Midnight\n");
		_hThread.Attach((HANDLE) _beginthreadex( NULL, 0, &MtcOpcAdapter::ResetAtMidnightThread, this, 0, &_threadID ));
	}

	if(_bOPCEnabled)
	{
		_workerthread.Initialize();
		::SetEvent (_StartThread._hEvent);
		_workerthread.AddHandle(_StartThread._hEvent, &_StartThread,(DWORD_PTR) this);
		//StartOPCMachines();
	}

	// This goes last... never returns
	startServer();
}

HRESULT MtcOpcAdapter::CStartThread::Execute(DWORD_PTR dwParam, HANDLE hObject)
{ 
	MtcOpcAdapter * adapter = (MtcOpcAdapter*) dwParam;
	OutputDebugString(StdStringFormat( "MTConnect Adapter Service Start OPC %s\n" , COleDateTime::GetCurrentTime().Format() ).c_str());
	try {
		::ResetEvent(hObject);
		if(!adapter->_bOPCEnabled)
			return S_OK;
	
		adapter->_machines.clear();

		/////////////////////////////////////
		/// BEGIN OPC Related Code
		for(int i=0; i< adapter->_servers.size(); i++)
		{
			COpcAdapter * opc= new COpcAdapter((MtcOpcAdapter *) adapter, Trim(adapter->_servers[i]), Trim(adapter->_devices[i]), Trim(adapter->_tags[i])); 
			adapter->_machines.push_back(opc);
			if(FAILED(opc->ReadOPCSettings(adapter->_inifile)))
				throw std::exception("Error in reading OPCSettings - see log for details.\n" );
			opc->Start() ;
		}
		/// END OPC Related Startup  Code
		/////////////////////////////////////

	}
	catch(std::exception e)
	{
		OutputDebugString(StdStringFormat("MtcOpcAdapter::StartOPCMachines() failed - %s\n", e.what()).c_str());
		return E_FAIL;
	}
	catch(...) 
	{
		OutputDebugString("MtcOpcAdapter::StartOPCMachines() failed \n");	
		return E_FAIL;
	}
	return S_OK; 
}
void MtcOpcAdapter::gatherDeviceData()
{
	_set_se_translator( trans_func );  // correct thread?
	mAvailability.available();
}

//DWORD WINAPI MtcOpcAdapter::StartOpcThread(void * pObj){ ((MtcOpcAdapter *) pObj)->StartOPCMachines(); return 0; }
//void  MtcOpcAdapter::StartOpc()
//{
//	OutputDebugString(StdStringFormat( "MTConnect Adapter Service Start OPC %s\n" , COleDateTime::GetCurrentTime().Format() ).c_str());
//	CHandle thread(::CreateThread(0, // default security
//		0, // default stack size
//		StartOpcThread,
//		this, // context
//		0, // no flags
//		0)); // ignore thread id	  
//}
//
//void  MtcOpcAdapter::StartOPCMachines()
//{	  
//	try {
//		_machines.clear();
//		if(_bOPCEnabled)
//		{
//			/////////////////////////////////////
//			/// BEGIN OPC Related Code
//			for(int i=0; i< _servers.size(); i++)
//			{
//				COpcAdapter * opc= new COpcAdapter((MtcOpcAdapter *) this, Trim(_servers[i]), Trim(_devices[i]), Trim(_tags[i])); // for now load in device name
//				_machines.push_back(opc);
//				if(FAILED(opc->ReadOPCSettings(_inifile)))
//					throw std::exception("Error in reading OPCSettings - see log for details.\n" );
//				opc->Start() ;
//			}
//			/// END OPC Related Startup  Code
//			/////////////////////////////////////
//		}
//	}
//	catch(std::exception e)
//	{
//		OutputDebugString(StdStringFormat("MtcOpcAdapter::StartOPCMachines() failed - %s\n", e.what()).c_str());
//		throw e;
//	}
//	catch(...) {}
//}
void MtcOpcAdapter::StopOPCMachines()
{
	if(_bOPCEnabled)
	{
		for(int i=0; i< _machines.size(); i++)
		{
			_machines[i]->Stop() ;
			if(! _machines[i]->Join(2000))
				OutputDebugString(StdStringFormat( "Machine Adapter Failed %s to stop \n", _machines[i]->_device.c_str() ).c_str());
			//if(machines[i]!=NULL)
			//	delete machines[i] ;
		}
		_machines.clear();
	}
}
void MtcOpcAdapter::stop()
{
	CHandle thread(::CreateThread(0, // default security
		0, // default stack size
		StopThread,
		this, // context
		0, // no flags
		0)); // ignore thread id
}
DWORD WINAPI MtcOpcAdapter::StopThread(void * pObj)
{
	gSvcStatus.dwCurrentState = SERVICE_STOP_PENDING;
	ReportSvcStatus(SERVICE_STOP_PENDING, 0, 10000);
	((MtcOpcAdapter *) pObj)->StopAll();
	ReportSvcStatus(SERVICE_STOP, NO_ERROR, 5000);
	return 0;
}
void MtcOpcAdapter::StopAll()
{
	// Check if invoked from SCM as opposed to midnite reset.
	if( _hThread!=NULL && _bRunning)
	{
		BOOL  bTerminate= TerminateThread(_hThread, 0);
	}
	OutputDebugString(StdStringFormat( "MTConnect Adapter Service Stopping %s\n", COleDateTime::GetCurrentTime().Format() ).c_str());

	StopOPCMachines();

	stopServer();
	OutputDebugString(StdStringFormat( "MTConnect Adapter Service Stopped %s\n", COleDateTime::GetCurrentTime().Format() ).c_str());

}

UINT __stdcall  MtcOpcAdapter::ResetAtMidnightThread(void *oObject)
{
	try {
		if (!oObject) 
			throw;

		MtcOpcAdapter *adapter = static_cast <MtcOpcAdapter*> (oObject);
		_set_se_translator( trans_func );  // doesn't seem required.

		COleDateTime now = COleDateTime::GetCurrentTime();
		COleDateTime date2 =  COleDateTime(now.GetYear(), now.GetMonth(), now.GetDay(), 0, 0, 0) +  COleDateTimeSpan(1, 0, 0, 0);
		//COleDateTime date2 =  now +  COleDateTimeSpan(0, 0, 1, 0);
		OutputDebugString(StdStringFormat( "MTConnect adapter Service Will Reset at %s\n", date2.Format() ).c_str());
		while(adapter->_bRunning)
		{
			// Detect midnite and reset OPC connection
			now = COleDateTime::GetCurrentTime();
			if (now > date2)
			{
				COleDateTime date2 =  COleDateTime(now.GetYear(), now.GetMonth(), now.GetDay(), 0, 0, 0) +  COleDateTimeSpan(1, 0, 0, 0);
				OutputDebugString(StdStringFormat("OPC Resetting to clear COM at  Date2=%s\n", date2.Format()).c_str());
				adapter->_bRunning=false;
			}
			::Sleep(5000);

		}
		OutputDebugString(StdStringFormat( "MTConnect adapter Service Start Resetting %s\n", COleDateTime::GetCurrentTime().Format() ).c_str());
		// Reset the service
		ReportSvcStatus(SERVICE_STOP, 0, 5000); // Dont report SERVICE_STOP or no recovery
		adapter->StopAll();
		OutputDebugString(StdStringFormat( "MTConnect adapter Service Reset %s\n", COleDateTime::GetCurrentTime().Format() ).c_str());

		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		ZeroMemory( &si, sizeof(si) );
		si.cb = sizeof(si);
		ZeroMemory( &pi, sizeof(pi) );
		std::string cmd = StdStringFormat("cmd /c net stop \"%s\" & net start \"%s\"", adapter->name(), adapter->name());        // Command line

		CreateProcess( NULL,   // No module name (use command line)
			const_cast<char *>(cmd.c_str()),
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			0,              // No creation flags
			NULL,           // Use parent's environment block
			NULL,           // Use parent's starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi );           // Pointer to PROCESS_INFORMATION structure

		::Sleep(5000);
	}
	catch(...)
	{
		OutputDebugString("Exception  - ResetAtMidnightThread(void *oObject)\n");
	}
	return 0;
}