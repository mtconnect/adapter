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
	
// see http://www.w3.org/TR/NOTE-datetime Note = no milliseconds in timestamp
static std::string GetTimestamp()
{
	char run_date[128];
	time_t tod;
	time( &tod );
	strftime( run_date, sizeof(run_date), "%Y-%m-%dT%H:%M:%S-%z", gmtime( &tod ) );
	return std::string( run_date );
};

////////////////////////////////////////////////////////////////

COpcAdapter::COpcAdapter(MtcOpcAdapter * mtcadapter, 
								 std::string machine, 
								 std::string device, 
								 std::string inisection) : 
_mtcadapter(mtcadapter), 
_device(device)
{
	static int _nMutexes=0;
	_bLogging =false;
	_bTracing=false;
	_nDelayTime=0;
	_opcservermachine=machine;
	_eOPCStatus=_eOPCLastStatus=WAITING;
	_sTagSectionName=inisection;

	//_bRunning=false;
	std::string mutexname = StdStringFormat("COpcAdapter Mutex %d", _nMutexes++);
	_hRunMutex.Attach( CreateMutex( NULL, TRUE, mutexname.c_str() ));      // Set 

	_szConfigFile= ::ExeDirectory() + "adapter.ini";
}
COpcAdapter::~COpcAdapter()
{
}

//void COpcAdapter::SetMTCTagValue(tstring device, tstring tag, tstring value)
//{
//	//Agent * agent = _config->getAgent();
//	//Device *dev = agent->getDeviceByName(device);
//	//DataItem *di = dev->getDeviceDataItem(tag);
//	//if (di != NULL)
//	//{
//	//	std::string time = getCurrentTime(GMT_UV_SEC);
//	//	agent->addToBuffer(di, value, time);
//	//}
//	//else
//	//	sLogger << LINFO  <<  "(" << device << ") Could not find data item: " << tag << "\n";
//
//
//
//}


std::string COpcAdapter::GetSymbolValue(tstring section, tstring tag, const char * defaultval) 
{
	return _config.GetSymbolValue(section+"."+tag, defaultval).str();
}
std::vector<std::string> COpcAdapter::GetSectionKeys(tstring section)
{
	return _config.getkeys(section);
}

void COpcAdapter::OnApplicationInit() 
{ 
	COPCBaseClass::OnApplicationInit() ; 

	try {

	}
	catch(tstring errmsg)
	{
		//sLogger << LFATAL  << errmsg << "\n";
	}
}

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

static void trans_func( unsigned int u, EXCEPTION_POINTERS* pExp )
{

	OutputDebugString( StdStringFormat("COpcAdapter In trans_func - Code = 0x%x\n",  pExp->ExceptionRecord->ExceptionCode).c_str() );
	

	throw std::exception();
}



void COpcAdapter::OnApplicationAction()
{ 
	USES_CONVERSION;
	try 
	{
		COPCBaseClass::OnApplicationAction() ; 
	}
	catch(...)
	{
		//sLogger << LWARN << "Catastrophic errror OnApplicationAction MT Connect store";
	}
}
void COpcAdapter::OnApplicationClose()
{ 
	COPCBaseClass::OnApplicationClose() ; 
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
		//if (_symbols->GetIniSection(_sTagSectionName).size()<1)
		//	throw std::exception(StdStringFormat("Error: No [%s] Section\n", _sTagSectionName.c_str()).c_str());

		std::vector<tstring> tags = GetSectionKeys(_sTagSectionName);
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
					_mtcadapter->AddSample(item->_alias);
				}
				else if(item->_alias.find(_T("Event."))==0)
				{
					item->_alias=ReplaceOnce(item->_alias,_T("Event."), _T(""));
					item->_type=_T("Event");
					_mtcadapter->AddEvent(item->_alias);
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
					_mtcadapter->AddCondition(item->_alias);
				}
				tstring tmp= GetSymbolValue(_sTagSectionName, tags[i],"");
				item->_tagname=tmp;
				opcitems.push_back(item);
			}

		}
#ifdef ALARMS
		//Alarm Management
		_alarmconfig.load(ExeDirectory() + "English\\Alarms.cfg");
#endif
		//// Error checking
		//if(_config->getAgent()==NULL)
		//	throw std::exception("Machine Adapter - null Agent\n");
		//if(_config->getAgent()->getDeviceByName(_device)==NULL)
		//	throw std::exception("Machine Adapter - null Device\n");

	}
	catch(tstring errmsg)
	{
		_gszStartupErrors+=errmsg;
		ErrorMessage( _gszStartupErrors + "\n");
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

	// Add logger - can't do two event handlers ???? Why?



	//ValuesChanged += abc::event_handler(this, &COpcAdapter::OnValueChanged); // non-static member
	//ValuesChanged += abc::event_handler(this, &COpcAdapter::DetectInspection); // non-static member

	return S_OK;
}
void COpcAdapter::Stop()
{
	//ReleaseMutex( ) releases the mutex and allows another thread to acquire it. Here, hMutex is the handle to the mutex. The function returns nonzero if successful and zero on failure.
	// The thread that owns a mutex can specify the same mutex in repeated wait function calls without blocking its execution.
	ReleaseMutex( _hRunMutex );
	//_bRunning=false;
}
bool COpcAdapter::Join(int nWait)
{
	if( WaitForSingleObject( _hJoinMutex, nWait) == WAIT_OBJECT_0)
		return 1;
	return 0;
}

void COpcAdapter::Start() 
{ 
	// NOTE: can really only run once 
	if(_hThread == NULL)
		_hThread.Attach((HANDLE) _beginthreadex( NULL, 0, &COpcAdapter::entry, this, 0, &_threadID ));

}
UINT __stdcall  COpcAdapter::entry(void *oObject)
{
	BOOL bHandled;
	if (!oObject) return NULL;
	COpcAdapter *object = static_cast <COpcAdapter*> (oObject);
	object->OnTimer(NULL, NULL, NULL, bHandled);
	return NULL;
}


LRESULT COpcAdapter::OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	COleDateTime now ;
	try{

		HRESULT hr;
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
		
#if 0
		ComInitializeSecuity dcom;
		tstring s_authLevelComSecurityEnum=GetSymbolValue("OPCSERVER", "AuthLevelComSecurity", "RPC_C_AUTHN_LEVEL_NONE") ;
		tstring s_authLevelComSecurityEnum=GetSymbolValue("OPCSERVER", "ImpLevelComSecurity", "RPC_C_IMP_LEVEL_IDENTIFY") ;
		int n_authLevelComSecurityEnum  = DCOMEnums.Find(_bstr_t(s_authLevelComSecurityEnum));
		int n_impLevelComSecurityEnum = DCOMEnums.Find(_bstr_t(s_impLevelComSecurityEnum));
		dcom.dwAuthnLevel=RPC_C_AUTHN_LEVEL_NONE; // n_authLevelComSecurityEnum;
		dcom.dwImpLevel=RPC_C_IMP_LEVEL_IDENTIFY; // n_impLevelComSecurityEnum;
		hr = dcom.InitializeSecurity();

#endif

		_set_se_translator( trans_func );  // doesn't seem required.

		// Setup when we are done with this thread
		static int _nMutexes=0;
		std::string mutexname = StdStringFormat("COpcAdapter JoinMutex %d", _nMutexes++);
		_hJoinMutex.Attach( CreateMutex( NULL, TRUE, mutexname.c_str() ));  

		// WAIT_TIMEOUT=258L  
		//WAIT_OBJECT_0
		while(WaitForSingleObject( _hRunMutex, this->_nOPCServerRate))
		{
			SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS);

			DelayTime()=DelayTime() - _nOPCServerRate;
			if(DelayTime()<0)
				DelayTime()=0;
			if(DelayTime()>0)
				continue;

			// Check if connected - this could take too long if 
			if(!IsConnected() && _bAutoConnect)
			{
				now = COleDateTime::GetCurrentTime();
#ifdef DEBUG
				OutputDebugString(StdStringFormat("%s Attempt Connect =%s\n", this->_opcservermachine.c_str(), now.Format()).c_str());
#endif
				_mtcadapter->SetEvent("power", "OFF");
				if(FAILED(Connect()))
				{
					_eOPCStatus = TRYING;
					Disconnect();
					::Sleep(this->_nQueryServerPeriod);
					continue ;
				}
			}

			SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);

			if(IsConnected())
			{
				USES_CONVERSION;
				_eOPCStatus = CONNECTED;

				if(FAILED(GetStatusString(_status)))
				{
					Disconnect();
				}
				if(IsConnected() && _bSynchronousUpdate)
				{
					_mtcadapter->SetEvent("power", "ON");
					//SetMTCTagValue(_device, "power", "ON");
					UpdateGroup();
				}
			}
			// Add test for midnite
			_eOPCLastStatus=_eOPCStatus;
		}
	}
	catch(std::exception e)
	{
		OutputDebugString(StdStringFormat("Unhandled exception in %s - COpcAdapter::OnTimer()%s\n" , this->_device.c_str(), e.what()).c_str());
	}
	catch(...)
	{
		//DebugBreak();
		OutputDebugString("Unhandled exception  - COpcAdapter::OnTimer()\n");
	}
	try { Disconnect();} catch(...) {}
	CoUninitialize();
	OutputDebugString(StdStringFormat("COpcAdapter(%s) Exit:%s\n" , this->_device.c_str(), now.Format()).c_str());
	ReleaseMutex( _hJoinMutex );
	return 0;
}

void COpcAdapter::OnValueChanged(ItemList & _opcitems)
{
	//Agent * agent = _config->getAgent();
	//if(agent==NULL)
	//	return;

	//std::string time = getCurrentTime(GMT_UV_SEC);
	//Device *dev = agent->getDeviceByName(_device);
	//if (dev == NULL) 
	//{
	//	sLogger << LINFO << ((std::string) "Cannot find device: ") + _device;
	//	return;
	//}

	for (int i =0; i<_opcitems.size(); i++) 
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
#ifdef ALARMS
		/*
		1.	Level – This must be normal, warning, fault, or unavailable (case is not important). If this value is not one of the previous values, unavailable will be assumed.
		2.	Native Code – This is the number or short name of the alarm or warning passed through from the device.
		3.	Native Severity – The severity of the alarm as passed through from the device.
		4.	Qualifier  – This is a modifier which indicates which end of the range the condition applies or qualifies the condition. 
		Examples are: HIGH, LOW, etc…
		5.	Text – The text describing the condition. This text is usually taken from the data source and passed through.
		*/

		if(opcitems[i]->_type=="Condition" && _sTagSectionName ==_T("SIEMENS") )	 
		{
			// Check for alarm support if none, continue
			long lNativeCode = opcitems.GetValue<long>(_T("alarm"),-1); 
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
			std::string alarmLevel= (lNativeCode <3)? sLevel[lNativeCode] : _T("UNAVAILABLE");
			std::string alarmSeverity= (lNativeCode <3)? sSeverities[lNativeCode] : _T("CRITICAL");	

			std::string sAlarmMessage;
			std::string sNativeCode = opcitems.GetValue(_T("alarm") , ""); 

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
			value=alarmLevel + "|" + sNativeCode + "|" + alarmSeverity + "|"+sAlarmMessage ;
			SetMTCTagValue(_device, _opcitems[i]->_alias, value);

		}
#endif
		// Alarm data = CODE|NATIVECODE|SEVERITY|STATE
		// Conditon data: SEVERITY|NATIVE_CODE|[SUB_TYPE]
		// Asset changed: type|id
		// Event Sample changed: value
 
		if(opcitems[i]->_type==_T("Event"))
		{

			_mtcadapter->SetEvent(_opcitems[i]->_alias, value);

		}
		if( opcitems[i]->_type==_T("Sample"))
		{
			_mtcadapter->SetSample(_opcitems[i]->_alias, value);
		}
		else if(opcitems[i]->_type==_T("Condition")) 
		{
		}
	}
#ifdef QMR
	// Now detect if QMR results are appropriate
	DetectInspection(opcitems);
#endif
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

