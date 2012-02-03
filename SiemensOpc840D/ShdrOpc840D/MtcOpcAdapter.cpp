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

#include "stdafx.h"
#include "internal.hpp"
#include "MtcOpcAdapter.hpp"
#include <string>
#include <comdef.h>
#include <atlpath.h>
#include <exception>
#include "StdStringEx.h"

#define MULTISHDR

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

static std::map<std::string, Condition::ELevels> create_map() 
{   
	std::map<std::string,Condition::ELevels> m;   
	m["UNAVAILABLE"] =  Condition::eUNAVAILABLE;
	m["NORMAL"] =  Condition::eNORMAL;
	m["WARNING"] =  Condition::eWARNING;
	m["FAULT"] =  Condition::eFAULT;
	return m; 
}  
std::map<std::string,Condition::ELevels> LevelMap = create_map(); 

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

/*
To support this configuration, we added an optional : in the key. 
If the key contains a : it will be split into <device>:<key> and treated 
as it was previously in the key value pair matching referenced above. 
This is currently experimental. If the device portion is not specified, the 
default device for this adapter will be used and the key resolved in the normal manor.
*/

// Treat samples as events - simpler. Just store text.
// Dont need to worry about double, integer, etc.
// Ignores deadband issues for now.
void MtcOpcAdapter::AddSample(std::string device, std::string tagname)
{
 ///	static char name[] = "MtcOpcAdapter::AddSample";

#ifdef MULTISHDR
	tagname=device+":"+tagname;
#endif
	_events[tagname]=new Event(tagname.c_str());
	addDatum(*_events[tagname]);
}
void MtcOpcAdapter::AddEvent(std::string device, std::string tagname)
{
 //	static char name[] = "MtcOpcAdapter::AddEvent";

#ifdef MULTISHDR
	tagname=device+":"+tagname;
#endif
	_events[tagname]=new Event(tagname.c_str());
	addDatum(*_events[tagname]);
}
void MtcOpcAdapter::AddCondition(std::string device, std::string tagname)
{
 //	static char name[] = "MtcOpcAdapter::AddCondition";
#ifdef MULTISHDR
	tagname=device+":"+tagname;
#endif
	_conditions[tagname]=new Condition(tagname.c_str());
	addDatum(*_conditions[tagname]);
}
void MtcOpcAdapter::SetSample(std::string device, std::string tagname, std::string value)
{
 //	static char name[] = "MtcOpcAdapter::SetSample";

#ifdef MULTISHDR
	tagname=device+":"+tagname;
#endif
	if(_events.find(tagname) != _events.end())
		_events[tagname]->setValue(value.c_str());
	// FIXME:else  throw error
}
void MtcOpcAdapter::SetEvent(std::string device, std::string tagname, std::string value)
{
 //	static char name[] = "MtcOpcAdapter::SetEvent";
#ifdef MULTISHDR
	tagname=device+":"+tagname;
#endif
	if(_events.find(tagname) != _events.end())
		_events[tagname]->setValue(value.c_str());
	// FIXME: else throw error
}
// Level|NativeCode|NativeSeverity|Qualifier|Description
void MtcOpcAdapter::SetCondition(std::string device, 
								 std::string tagname, 
								 std::string level,
								 std::string NativeCode,
								 std::string NativeSeverity,
								 std::string Qualifier,
								 std::string Description
								 )
{
// 	static char name[] = "MtcOpcAdapter::SetCondition";
#ifdef MULTISHDR
	tagname=device+":"+tagname;
#endif
	//eUNAVAILABLE,    eNORMAL,    eWARNING,    eFAULT
	int n = (int) LevelMap[level];
	if(_conditions.find(tagname) != _conditions.end())
		_conditions[tagname]->add(LevelMap[level], Description.c_str(), NativeCode.c_str(),
		Qualifier.c_str(), NativeSeverity.c_str());

}
void MtcOpcAdapter::initialize(int aArgc, const char *aArgv[])
{
 	static char name[] = "MtcOpcAdapter::initialize";
	try{
		GLogger << LDEBUG << "MTConnect Adapter Service Configure" << std::endl;

		_inifile					= ExeDirectory() + "\\adapter.ini";
		if(!ATL::ATLPath::FileExists(_inifile.c_str()))
			throw std::exception(StdStringFormat("Config file missing %s\n", _inifile.c_str()).c_str());

		_iniconfig.load(_inifile);

		MTConnectService::setName(_iniconfig.GetSymbolValue("Adapter.ServiceName", "MTCAdapter").c_str());
		Adapter::mScanDelay			= ConvertFromString<int>(_iniconfig.GetSymbolValue("Adapter.ScanDelay", "1000").str(), 1000);
		this->Adapter::mPort		= ConvertFromString<int>(_iniconfig.GetSymbolValue("Adapter.Port", "7878").str(), 7878);


		std::string sLogLevel		= _iniconfig.GetSymbolValue("logger_config.logging_level", "debug");
		std::string nLogName		= _iniconfig.GetSymbolValue("logger_config.output", "OuputDebugString");

		//fatal, error, warn, info, debug
		GLogger.DebugLevel()= LDEBUG;
		if(MakeLower(sLogLevel) == "none")
			GLogger.DebugLevel()= FATAL;
		else if(MakeLower(sLogLevel) == "error")
			GLogger.DebugLevel()= ERROR;
		else if(MakeLower(sLogLevel) == "warn")
			GLogger.DebugLevel()= WARNING;
		else if(MakeLower(sLogLevel) == "info")
			GLogger.DebugLevel()= INFO;

		GLogger.Timestamping()= true;
		if(nLogName=="OuputDebugString")
		{
			GLogger.DebugString() = true;
		}
		else
		{
			GLogger.DebugString() = false;
			GLogger.Open(::ExeDirectory() + nLogName);
		}


		MTConnectService::initialize(aArgc, aArgv);
		if (aArgc > 1) {
			mPort			= atoi(aArgv[1]);
		}

		_bResetAtMidnight	= ConvertFromString<bool>(_iniconfig.GetSymbolValue("OPCSERVER.ResetAtMidnight", "false").str(), false);
		_bOPCEnabled		= ConvertFromString<bool>(_iniconfig.GetSymbolValue("OPCSERVER.OPCEnabled", "false").str(), false);;
		_servers			= Tokenize(_iniconfig.GetSymbolValue("OPCSERVER.ServerMachineName", "").str(), ",");
		_devices			= Tokenize(_iniconfig.GetSymbolValue("OPCSERVER.MTConnectDevice", "").str(), ",");
		_tags				= Tokenize(_iniconfig.GetSymbolValue("OPCSERVER.OpcTags", "").str(), ",");
		
		if(_servers.size() != _devices.size() && _servers.size() != _tags.size())
			throw std::exception("OPC Number of Server IPs does not match number of MTConnect devices or Section Tags" );
	}
	catch(std::exception e)
	{
		GLogger << FATAL << "Adapter::initialize() failed: " << e.what()  << std::endl;
		throw e;
	}
	catch(...) {}
}
void MtcOpcAdapter::gatherDeviceData()
{
 	static char name[] = "MtcOpcAdapter::gatherDeviceData";
	_set_se_translator( trans_func );  // correct thread?

	for(int i=0; i< _machines.size(); i++)
	{
		if(_machines[i]->IsConnected() )
		{
			try
			{
				_machines[i]->_CriticalSection.Lock();
				_machines[i]->ExportValues();
				_machines[i]->_CriticalSection.Unlock();

			}
			catch(...)
			{
				GLogger << INFO << "MtcOpcAdapter::gatherDeviceData() exception "  << std::endl;
			}
		}
	}
}

// NOTE: Windows SCM more tolerant of slow starting processes than terminating processes.
void MtcOpcAdapter::start()
{
 	static char name[] = "MtcOpcAdapter::start";
	_bRunning=true; 
	if(_bResetAtMidnight)
	{
		COleDateTime now = COleDateTime::GetCurrentTime();
		COleDateTime date2 =  COleDateTime(now.GetYear(), now.GetMonth(), now.GetDay(), 0, 0, 0) +  COleDateTimeSpan(1, 0, 0, 1);
		//COleDateTime date2 =  now +  COleDateTimeSpan(0, 0, 2, 0); // testing reset time - 2 minutes

		COleDateTimeSpan tilmidnight = date2-now;
		_resetthread.Initialize();
		_resetthread.AddTimer(
			(long) tilmidnight.GetTotalSeconds() * 1000,
			&_ResetThread,
			(DWORD_PTR) this,
			&_ResetThread._hTimer  // stored newly created timer handle
			) ;

		GLogger << INFO << "Adapter will Reset At Midnight " << date2.Format() << std::endl;
	}

	if(_bOPCEnabled)
	{
		_workerthread.Initialize();
		::SetEvent (_StartThread._hEvent); // start OPC thread
		_workerthread.AddHandle(_StartThread._hEvent, &_StartThread,(DWORD_PTR) this);
	}

	// This goes last... never returns
	startServer();
}

HRESULT MtcOpcAdapter::CStartThread::Execute(DWORD_PTR dwParam, HANDLE hObject)
{ 
 	static char name[] = "CStartThread::CStartThread";
	
	MtcOpcAdapter * adapter = (MtcOpcAdapter*) dwParam;
	_set_se_translator( trans_func );  // doesn't seem required.
	GLogger << ENTRY << "MTConnect Adapter Service Start OPC "  << std::endl;
	try {
		::ResetEvent(hObject);
		if(!adapter->_bOPCEnabled)
			return S_OK;

		adapter->_machines.clear();

		for(int i=0; i< adapter->_servers.size(); i++)
		{
			COpcAdapter * opc= new COpcAdapter((MtcOpcAdapter *) adapter, Trim(adapter->_servers[i]), Trim(adapter->_devices[i]), Trim(adapter->_tags[i])); 
			adapter->_machines.push_back(opc);
			if(FAILED(opc->ReadOPCSettings(adapter->_inifile)))
				throw std::exception("Error in reading OPCSettings - see log for details.\n" );
			opc->Start() ;
		}
	}
	catch(std::exception e)
	{
		GLogger << FATAL << "MtcOpcAdapter::StartOPCMachines() failed " << e.what() << std::endl;
		return E_FAIL;
	}
	catch(...) 
	{
		GLogger << FATAL << "MtcOpcAdapter::StartOPCMachines() " << std::endl;
		return E_FAIL;
	}
	return S_OK; 
}

void MtcOpcAdapter::stop()
{
 	static char name[] = "MtcOpcAdapter::stop";

	gSvcStatus.dwCurrentState = SERVICE_STOP_PENDING;
	ReportSvcStatus(SERVICE_STOP_PENDING, 0, 10000);
	_enderthread.Initialize();
	::SetEvent (_EndThread._hEvent);
	_enderthread.AddHandle(_EndThread._hEvent, &_EndThread,(DWORD_PTR) this);
}

HRESULT MtcOpcAdapter::CEndThread::Execute(DWORD_PTR dwParam, HANDLE hObject)
{
 	static char name[] = "CEndThread::Execute";

	MtcOpcAdapter * adapter = (MtcOpcAdapter*) dwParam;
	_set_se_translator( trans_func );  // doesn't seem required.
	HRESULT hr = S_OK;
	try
	{
		::ResetEvent(hObject);
		// Check if invoked from SCM as opposed to midnite reset.
		if( adapter->_hThread!=NULL && adapter->_bRunning)
		{
			BOOL  bTerminate= TerminateThread(adapter->_hThread, 0);
		}
		GLogger << ENTRY << "MTConnect Adapter Service Stopping " << std::endl;

		if(adapter->_bOPCEnabled)
		{
			for(int i=0; i< adapter->_machines.size(); i++)
			{
				adapter->_machines[i]->Stop() ;
				if(! adapter->_machines[i]->Join(2000))
				GLogger << FATAL << "MTConnect Adapter Failed " << adapter->_machines[i]->Device() << " to stop " << std::endl;
				//if(machines[i]!=NULL)
				//	delete machines[i] ;
			}
			adapter->_machines.clear();
		}
		adapter->stopServer();
	}
	catch(std::exception e)
	{
		GLogger << FATAL << "MtcOpcAdapter::StartOPCMachines() failed " << e.what() <<  std::endl;
		hr = E_FAIL;
	}
	catch(...) 
	{
		GLogger << FATAL << "MtcOpcAdapter::StartOPCMachines() failed " << std::endl;
		hr = E_FAIL;
	}
	GLogger << ENTRY << "MTConnect Adapter Service Stopped " << std::endl;
	ReportSvcStatus(SERVICE_STOP, hr, 5000);
	return hr;
}

HRESULT MtcOpcAdapter::CResetThread::Execute(DWORD_PTR dwParam, HANDLE hObject)
{
 	static char name[] = "CResetThread::Execute";

	MtcOpcAdapter * adapter = (MtcOpcAdapter*) dwParam;
	CancelWaitableTimer(hObject);

	try {
		GLogger << ENTRY << "MTConnect adapter Service Start Resetting " << std::endl;

		PROCESS_INFORMATION pi;
		ZeroMemory( &pi, sizeof(pi) );

		STARTUPINFO si;
		ZeroMemory( &si, sizeof(si) );
		si.cb = sizeof(si);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;	 // set the window display to HIDE	
		
		// SCM reset command of this service 
		std::string cmd = StdStringFormat("cmd /c net stop \"%s\" & net start \"%s\"", adapter->name(), adapter->name());        // Command line

		::CreateProcess( NULL,   // No module name (use command line)
			const_cast<char *>(cmd.c_str()),
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			0,              // No creation flags
			NULL,           // Use parent's environment block
			NULL,           // Use parent's starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi );           // Pointer to PROCESS_INFORMATION structure

		::Sleep(5000); // make sure process has spawned before killing thread
	}
	catch(...)
	{
		GLogger << FATAL << "Exception  - ResetAtMidnightThread(void *oObject)" << std::endl;
	}
	return S_OK;
}