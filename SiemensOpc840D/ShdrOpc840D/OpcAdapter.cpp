// **************************************************************************
// OpcAdapter.cpp
//
// Description:
//
// DISCLAIMER:
// This software was developed by U.S. Government employees as part of
// their official duties and is not subject to copyright. No warranty implied 
// or intended.
// **************************************************************************



#include "stdafx.h"
#include <time.h>
#include <fstream>
#include "OpcAdapter.h"
#include "atlcomtime.h"
#include <atlpath.h>
#include "DCOMUtils.h"

#include "Logger.h"
#include <exception>

#include "MtcOpcAdapter.hpp"
#include "VarTypes.h"

#pragma comment(lib, "comsuppw.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Iphlpapi.lib")
	
////////////////////////////////////////////////////////////////

COpcAdapter::COpcAdapter(MtcOpcAdapter * mtcadapter, 
								 std::string machine, 
								 std::string device, 
								 std::string inisection) : 
_mtcadapter(mtcadapter), 
_device(device)
{
	_opcservermachine=machine;
	_sTagSectionName=inisection;
	_szConfigFile= ::ExeDirectory() + "adapter.ini";
	_workerthread.Initialize();

}
COpcAdapter::~COpcAdapter()
{
}
void COpcAdapter::ErrorMessage(std::string errmsg) 
{
	std::string msg = (errmsg.rfind("\n") != std::string::npos) ? errmsg.substr(0,errmsg.size()-1) : errmsg;
	GLogger << ENTRY << msg << std::endl;
}
void COpcAdapter::DebugMessage(std::string errmsg) 
{
	std::string msg = (errmsg.rfind("\n") != std::string::npos) ? errmsg.substr(0,errmsg.size()-1) : errmsg;
	GLogger << LDEBUG << msg << std::endl;
}
std::string COpcAdapter::GetSymbolValue(tstring section, tstring tag, const char * defaultval) 
{
	return _config.GetSymbolValue(section+"."+tag, defaultval).str();
}
std::vector<std::string> COpcAdapter::GetSectionKeys(tstring section)
{
	return _config.getkeys(section);
}

static void trans_func( unsigned int u, EXCEPTION_POINTERS* pExp )
{
	std::string errmsg =  StdStringFormat("COpcAdapter In trans_func - Code = 0x%x\n",  pExp->ExceptionRecord->ExceptionCode);
	OutputDebugString(errmsg.c_str() );
	throw std::exception(errmsg.c_str() , pExp->ExceptionRecord->ExceptionCode);
} 
void COpcAdapter::Off() 
{ 			 
	_mtcadapter->mAvailability.unavailable();
	_mtcadapter->SetEvent(this->_device, "power", "OFF");

}
void COpcAdapter::On() 
{ 			 
	_mtcadapter->mAvailability.available();
	_mtcadapter->SetEvent(this->_device, "power", "ON");
}

int COpcAdapter::ReadOPCSettings(tstring filename)
{
	try 
	{
		//std::string szConfigFile =	_config->configfile().c_str();
		if(!ATL::ATLPath::FileExists(_szConfigFile.c_str()))
		{
			throw std::exception(StdStringFormat("Config file missing %s\n", _szConfigFile.c_str()).c_str());
		}
		_config.load(_szConfigFile);

		// Read OPC Server details
		COPCBaseClass::ReadOPCSettings(filename);

		// Now parse the tag information into MTConnect information
		std::vector<tstring> tags = GetSectionKeys(_sTagSectionName);
		if(tags.size() < 1)
			throw std::exception(StdStringFormat("Error: No [%s] Section\n", _sTagSectionName.c_str()).c_str());
		for(int i=0; i<tags.size(); i++)
		{
			VarTypes vt;

			// make sure its is an OPC Tag
			if(tags[i].find(_T("Tag.")) == 0)
			{
				OpcItem * item = new OpcItem();
				item->_alias=tags[i];
				item->_alias=ReplaceOnce(item->_alias, _T("Tag."), _T(""));
				item->vt=VT_BSTR;
				if(item->_alias.find(_T("Enum."))==0)
				{
					item->_alias=ReplaceOnce(item->_alias, _T("Enum."), _T(""));
					item->_subtype="enumeration";
				}
				else if(item->_alias.find(_T("Const."))==0)
				{
					item->_alias=ReplaceOnce(item->_alias,_T("Const."), _T(""));
					item->vt= VT_USERDEFINED;
				}
				else if(item->_alias.find(_T("Prog."))==0)
				{
					item->_alias=ReplaceOnce(item->_alias,_T("Prog."), _T(""));
					item->vt= VT_USERDEFINED;
					item->_subtype="program";
				}

				if(item->_alias.find(_T("Sample."))==0)
				{
					item->_alias=ReplaceOnce(item->_alias,_T("Sample."), _T(""));
					item->_type=_T("Sample");
					_mtcadapter->AddSample(this->_device, item->_alias);
				}
				else if(item->_alias.find(_T("Event."))==0)
				{
					item->_alias=ReplaceOnce(item->_alias,_T("Event."), _T(""));
					item->_type=_T("Event");
					_mtcadapter->AddEvent(this->_device, item->_alias);
				}
				else if(item->_alias.find(_T("OPC."))==0)  // reads opc values does not send to MT Connect
				{
					item->_alias=ReplaceOnce(item->_alias, _T("OPC."), _T(""));  // OPC value for scripting
					item->_type=_T("OPC");
				}
				else if(item->_alias.find(_T("Condition."))==0)
				{
					item->_alias=ReplaceOnce(item->_alias,_T("Condition."), _T(""));
					item->_type="Condition";
					_mtcadapter->AddCondition(this->_device, item->_alias);
				}
				tstring tmp= GetSymbolValue(_sTagSectionName, tags[i],"");
				item->_tagname=tmp;
				opcitems.push_back(item);
			}

		}
		// Test to see if we have a power, if not add one.
		if(opcitems.FindItem("power")==NULL)
			_mtcadapter->AddEvent(this->_device, "power");

#ifdef ALARMS
		//Alarm Management
		_alarmconfig.load(ExeDirectory() + "English\\Alarms.cfg");
#endif
	}
	catch(tstring errmsg)
	{
		_gszStartupErrors+=errmsg;
		ErrorMessage( errmsg + "\n");
		return E_FAIL;

	}
	catch(std::exception e)
	{
		ErrorMessage( e.what());
		return E_FAIL;
	}
	catch(...)
	{
		ErrorMessage( "ReadOPCSettings() OPC Items Ini error \n");
		return E_FAIL;

	}
	//Logger.DebugLevel() =	GetSymbolValue("OPCSERVER", "Debug", (long) 0);
	//_device = GetSymbolValue("OPCSERVER", "MTConnectDevice", "");

	return S_OK;
}
void COpcAdapter::Stop()
{
	_workerthread.Shutdown(2000);
	ReleaseMutex( _hJoinMutex );
}
bool COpcAdapter::Join(int nWait)
{
	if( WaitForSingleObject( _hJoinMutex, nWait) == WAIT_OBJECT_0)
		return 1;
	return 0;
}

void COpcAdapter::Start() 
{ 
	_bInitialized=false;

	_workerthread.AddTimer(
		_nQueryServerPeriod,
		this,
		NULL,
		&_hTimer  // stored newly created timer handle
		) ;
}

HRESULT COpcAdapter::Initialize()
{
	try{
		HRESULT hr;
		_bInitialized=true;
		CoInitialize(NULL);

		hr = ::CoInitializeSecurity( NULL, //Points to security descriptor 
			-1, //Count of entries in asAuthSvc 
			NULL, //Array of names to register 
			NULL, //Reserved for future use 
			RPC_C_AUTHN_LEVEL_NONE, //The default authentication //level for proxies 
			RPC_C_IMP_LEVEL_IDENTIFY, //The default impersonation //level for proxies 
			NULL, //Reserved; must be set to  NULL
			EOAC_NONE, //Additional client or //server-side capabilities 
			NULL //Reserved for future use 
			);  
		_set_se_translator( trans_func );  // doesn't seem required.
		SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);

		// Setup when we are done with this thread
		static int _nMutexes=0;
		std::string mutexname = StdStringFormat("COpcAdapter JoinMutex %d", _nMutexes++);
		_hJoinMutex.Attach( CreateMutex( NULL, TRUE, mutexname.c_str() ));  
	}
	catch(...)
	{
		GLogger<< FATAL << "Unhandled exception  - COpcAdapter::Initialize()" << std::endl;
		return E_FAIL;
	}
	return S_OK;
}
HRESULT COpcAdapter::CloseHandle(HANDLE h)
{
	try { Disconnect();} catch(...) {}
	CoUninitialize();
	GLogger<< ENTRY << "COpcAdapter("<<_device.c_str() << ") Exit:" << COleDateTime::GetCurrentTime().Format() << "\n";
	::CloseHandle(h);  // timer Handle, not CHandle
	return S_OK;
}
HRESULT COpcAdapter::Execute(DWORD_PTR dwParam, HANDLE hObject)
{
	USES_CONVERSION;
	HRESULT hr=S_OK;
	ULONG dwInterval = _nOPCServerRate;
	try 
	{
		CancelWaitableTimer(_hTimer);
		DWORD starttick = GetTickCount();

		if(!_bInitialized)
			Initialize();

		// Check if connected - this could take too long if 
			if(!IsConnected() && _bAutoConnect)
			{
				GLogger << INFO << _opcservermachine <<  " Attempt Connect at " << COleDateTime::GetCurrentTime().Format() << std::endl;
				Off();
				if(FAILED(Connect()))
					throw std::exception("Attempt Connect FAILED\n");
			}

			if(IsConnected())
			{
				if(FAILED(GetStatusString(_status)))
					throw std::exception("GetStatusString failed \n");

				// OPC Server can be working, but adding items can fail
				if(FAILED(UpdateGroup()))
					throw std::exception("UpdateGroup failed \n");
				
				On();
				
				// Update sleep interval to be more exacting...
#define TICKS_DIFF(cur, prev) ((cur) >= (prev)) ? ((cur)-(prev)) : ((0xFFFFFFFF-(prev))+(cur))
				dwInterval-= TICKS_DIFF(GetTickCount() , starttick);
			}
	}
	catch(std::exception e)
	{
		GLogger<< LDEBUG << "Exception in " << _device << " - COpcAdapter::Execute() " <<  e.what() << std::endl;;
		Off();
		dwInterval=_nQueryServerPeriod;
		Disconnect();
		hr= E_FAIL;
	}
	catch(...)
	{
		GLogger<< LDEBUG << "Exception in " << _device << " - COpcAdapter::Execute() " << std::endl;;
		Off();
		dwInterval=_nQueryServerPeriod;
		Disconnect();
		hr= E_FAIL;
	}

	LARGE_INTEGER liDueTime;
	liDueTime.QuadPart = -10000 * (__int64) ((dwInterval>10000) ? 1000 : dwInterval) ; 
	BOOL bRet = SetWaitableTimer(_hTimer, &liDueTime, 0,  NULL, NULL, FALSE);

	return S_OK;
}

void COpcAdapter::OnValueChanged(ItemList & _opcitems)
{
}
void COpcAdapter::ExportValues()
{
	for (int i =0; i<opcitems.size(); i++) 
	{
		tstring alias =  opcitems[i]->_alias;
		tstring value =  opcitems[i]->GetValue<tstring>();

		if(opcitems[i]->_subtype=="enumeration")
		{
			std::string szEnumTag="Enum." + alias + "." + value;
			//szEnumTag.Format(_T("Enum.%s.%s"), alias, value);
			value = GetSymbolValue(_sTagSectionName, szEnumTag, (LPCSTR) "UNAVAILABLE");

			// Use default if available
			if(value == "UNAVAILABLE")
			{
				szEnumTag="Enum." + alias + ".-1";
				value = GetSymbolValue(_sTagSectionName, szEnumTag, (LPCSTR) "UNAVAILABLE");
			}
		}
		// Condition Doesn't work as advertised.

/*
HOW TO MAP ALARM TO CONDITION
1.	Level – This must be normal, warning, fault, or unavailable (case is not important). If this value is not one of the previous values, unavailable will be assumed.
2.	Native Code – This is the number or short name of the alarm or warning passed through from the device.
3.	Native Severity – The severity of the alarm as passed through from the device.
4.	Qualifier  – This is a modifier which indicates which end of the range the condition applies or qualifies the condition. 
Examples are: HIGH, LOW, etc…
5.	Text – The text describing the condition. This text is usually taken from the data source and passed through.

2010-09-29T23:59:33.460470Z|htemp|WARNING|HTEMP|1|HIGH|Oil Temperature High

<DataItem category="CONDITION" id="htemp" type="TEMPERATURE"/>


<Warning dataItemId="htemp" timestamp="2010-09-29T23:59:33.460470Z" sequence="399237840" type="TEMPERATURE" nativeSeverity="1" nativeCode="HTEMP" qualifier="HIGH" >Oil Temperature High</Warning>

If there is no message, as is usually the case when the operation is Normal, leave all the fields blank, but still provide the pipe delimiter for proper parsing as follows:
2010-09-29T23:59:33.460470Z|htemp|NORMAL||||

Expected data to parse in SDHR format:
 *   Time|Condition|Level|NativeCode|Native Severity|Qualifier|Description
 *   Time|Item|Value
 *   Time|Item1|Value1|Item2|Value2...

*/
		/*
		1.	Level – This must be normal, warning, fault, or unavailable (case is not important). If this value is not one of the previous values, unavailable will be assumed.
		2.	Native Code – This is the number or short name of the alarm or warning passed through from the device.
		3.	Native Severity – The severity of the alarm as passed through from the device.
		4.	Qualifier  – This is a modifier which indicates which end of the range the condition applies or qualifies the condition. 
		Examples are: HIGH, LOW, etc…
		5.	Text – The text describing the condition. This text is usually taken from the data source and passed through.
		*/
#ifdef ALARMS
		std::string alarmLevel;
		std::string alarmSeverity;
		std::string sAlarmMessage;
		std::string sNativeCode;
		std::string sQualifier;
		long lNativeCode;
		if(opcitems[i]->_type=="Condition" && _sTagSectionName ==_T("SIEMENS") )	 
		{
			// Check for alarm support if none, continue
			lNativeCode = opcitems.GetValue<long>(_T("alarm"),-1); 
			if(lNativeCode<0) 
				continue;

			//  840D Code whether alarm pending.
			//  0 = no alarm in this channel
			//	1 = alarm without stop
			//	2 = alarm with stop
		

			//  severity - CRITICAL, ERROR, WARNING, or INFORMATION.
			// state Either INSTANT, ACTIVE or CLEARED.
			static char * sSeverities[3] = {_T("NONE"), _T("WARNING"), _T("CRITICAL")};
			static char * sLevel[3] = {_T("NORMAL"), _T("WARNING") , _T("FAULT")};
			alarmLevel= (lNativeCode <3)? sLevel[lNativeCode] : _T("UNAVAILABLE");
			alarmSeverity= (lNativeCode <3)? sSeverities[lNativeCode] : _T("CRITICAL");	

			sNativeCode = opcitems.GetValue(_T("alarm") , ""); 

			//if (reader.is_block_defined("Alarms") && reader.block("Alarms").is_key_defined(sNativeCode))
			//	sAlarmMessage= reader.block("Alarms")[sNativeCode];
			sAlarmMessage = "None";
			if(lNativeCode>0)
				sAlarmMessage = _alarmconfig.GetSymbolValue("Alarms."+sNativeCode).str();

			// Get opc items
			//static const string sConditionUnavailable("UNAVAILABLE|||");

			std::string fillText;
			for(int j=0; lNativeCode>0 && j<4; j++)
			{
				fillText=opcitems.GetValue(StdStringFormat("fillText%d", j+1), "");
				if(fillText.empty())
					continue;
				if(fillText[0]==_T('S') || fillText[0]==_T('K'))
					fillText.substr(1);
				ReplaceOnce(sAlarmMessage,StdStringFormat("%d%%",j+1), fillText);
			}
		}
#endif
		_CriticalSection.Lock();

		if(opcitems[i]->_type==_T("Event"))
		{
			_mtcadapter->SetEvent(this->_device, opcitems[i]->_alias, value);
		}
		else if( opcitems[i]->_type==_T("Sample"))
		{
			_mtcadapter->SetSample(this->_device, opcitems[i]->_alias, value);
		}
		else if( opcitems[i]->_type==_T("Condition"))
		{
			if(lNativeCode>0)
				_mtcadapter->SetCondition(this->_device, opcitems[i]->_alias, 
				alarmLevel,
				sNativeCode,
				alarmSeverity,
				sQualifier,
				sAlarmMessage
				);
		}
		_CriticalSection.Unlock();

	}
}

#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IOPCShutdown,0xF31DFDE1,0x07B6,0x11d2,0xB2,0xD8,0x00,0x60,0x08,0x3B,0xA1,0xFB);


MIDL_DEFINE_GUID(IID, IID_IOPCCommon,0xF31DFDE2,0x07B6,0x11d2,0xB2,0xD8,0x00,0x60,0x08,0x3B,0xA1,0xFB);


MIDL_DEFINE_GUID(IID, IID_IOPCServerList,0x13486D50,0x4821,0x11D2,0xA4,0x94,0x3C,0xB3,0x06,0xC1,0x00,0x00);


MIDL_DEFINE_GUID(IID, LIBID_OPCCOMN,0xB28EEDB1,0xAC6F,0x11d1,0x84,0xD5,0x00,0x60,0x8C,0xB8,0xA7,0xE9);


const IID IID_IOPCServer = {0x39c13a4d,0x011e,0x11d0,{0x96,0x75,0x00,0x20,0xaf,0xd8,0xad,0xb3}};


const IID IID_IOPCServerPublicGroups = {0x39c13a4e,0x011e,0x11d0,{0x96,0x75,0x00,0x20,0xaf,0xd8,0xad,0xb3}};


const IID IID_IOPCBrowseServerAddressSpace = {0x39c13a4f,0x011e,0x11d0,{0x96,0x75,0x00,0x20,0xaf,0xd8,0xad,0xb3}};


const IID IID_IOPCGroupStateMgt = {0x39c13a50,0x011e,0x11d0,{0x96,0x75,0x00,0x20,0xaf,0xd8,0xad,0xb3}};


const IID IID_IOPCPublicGroupStateMgt = {0x39c13a51,0x011e,0x11d0,{0x96,0x75,0x00,0x20,0xaf,0xd8,0xad,0xb3}};


const IID IID_IOPCSyncIO = {0x39c13a52,0x011e,0x11d0,{0x96,0x75,0x00,0x20,0xaf,0xd8,0xad,0xb3}};


const IID IID_IOPCAsyncIO = {0x39c13a53,0x011e,0x11d0,{0x96,0x75,0x00,0x20,0xaf,0xd8,0xad,0xb3}};


const IID IID_IOPCItemMgt = {0x39c13a54,0x011e,0x11d0,{0x96,0x75,0x00,0x20,0xaf,0xd8,0xad,0xb3}};


const IID IID_IEnumOPCItemAttributes = {0x39c13a55,0x011e,0x11d0,{0x96,0x75,0x00,0x20,0xaf,0xd8,0xad,0xb3}};


const IID IID_IOPCDataCallback = {0x39c13a70,0x011e,0x11d0,{0x96,0x75,0x00,0x20,0xaf,0xd8,0xad,0xb3}};


const IID IID_IOPCAsyncIO2 = {0x39c13a71,0x011e,0x11d0,{0x96,0x75,0x00,0x20,0xaf,0xd8,0xad,0xb3}};


const IID IID_IOPCItemProperties = {0x39c13a72,0x011e,0x11d0,{0x96,0x75,0x00,0x20,0xaf,0xd8,0xad,0xb3}};


const IID LIBID_OPCDA = {0xB28EEDB2,0xAC6F,0x11d1,{0x84,0xD5,0x00,0x60,0x8C,0xB8,0xA7,0xE9}};


#undef MIDL_DEFINE_GUID

