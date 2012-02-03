// **************************************************************************
// OPCBackEnd.h: interface for the COPCBackEnd class.
//
// Description:
//
// DISCLAIMER:
// This software was developed by U.S. Government employees as part of
// their official duties and is not subject to copyright. No warranty implied 
// or intended.
// **************************************************************************
#pragma once

#include "atlapp.h"
#include "atlstr.h"
#include <Ws2tcpip.h>
#include "DCOMUtils.h"

#include "opcda.h"

#include "OPCCallback.h"
#include "OPCItem.h"
#include "StdStringEx.h"


#define RELEASE(X) {X=NULL;}

template <class TOPCItem>
class COPCBackEnd  : 
	public COPCShutdown
{
public:
	typedef  TOPCItem  OpcItem;
	typedef  COPCItems<TOPCItem>  ItemList;

	COPCBackEnd() 
	{
		Init();
	}
	virtual void Init()
	{
		_pIOPCServer			= NULL;
		_pIOPCItemMgt			= NULL;
		_pIOPCSyncIO			= NULL;


		_hServerHandleGroup	= 0;
		_dwAdviseDataCallbackHandle=NULL;
		_dwAdviseShutdownHandle=NULL;
		_bSynchronousUpdate=true;
	}

	~COPCBackEnd()
	{
		// ????!!!!!!!! 
		//for(UINT i=0; i<opcitems.size(); i++) 
		//	delete opcitems[i];

	}
	bool IsConnected()
	{
		return _pIOPCServer!=NULL;
	}
	// OPC Interfaces specific stuff
	virtual HRESULT AddOPCItem(COPCItem * item) ;
	virtual HRESULT _stdcall AdviseDataCallbackConnection(bool bOn=true);
	virtual HRESULT WriteItemValue(std::string tag,_variant_t val);
	virtual HRESULT _stdcall OnDataChange( 
		/* [in] */ DWORD dwTransid,
		/* [in] */ OPCHANDLE hGroup,
		/* [in] */ HRESULT hrMasterquality,
		/* [in] */ HRESULT hrMastererror,
		/* [in] */ DWORD dwCount,
		/* [size_is][in] */ OPCHANDLE  *phClientItems,
		/* [size_is][in] */ VARIANT  *pvValues,
		/* [size_is][in] */ WORD  *pwQualities,
		/* [size_is][in] */ FILETIME  *pftTimeStamp,
		/* [size_is][in] */ HRESULT  *pErrors) ;
	virtual  HRESULT Connect() ;
	virtual void Disconnect() ;
	virtual HRESULT WriteItem(COPCItem * item, _variant_t value) ;
	HRESULT GetStatusString(std::string &tmp);
	HRESULT _stdcall ShutdownRequest(/* [string][in] */ LPCWSTR szReason);
	HRESULT _stdcall AdviseShutdownConnection(bool bOn);

	// Application specific stuff
	virtual HRESULT UpdateGroup();
	virtual int ReadOPCSettings(tstring filename);  // done 1st
	virtual void OnApplicationInit(); // done 2nd
	virtual void OnApplicationAction(){}  // done for each new data 
	virtual void OnApplicationClose(){}   // at end
	virtual void OnValueChanged(ItemList & _opcitems){}
	virtual void ErrorMessage(std::string errmsg) { OutputDebugString(errmsg.c_str()); }
	virtual void DebugMessage(std::string errmsg) { OutputDebugString(errmsg.c_str()); }
	tstring  & TagSectionName() { return _sTagSectionName; }
	CComAutoCriticalSection _CriticalSection; 

	// Threading
	virtual void Stop(){}
	virtual void Start(){}
	
	// Configuration management
	virtual std::string GetSymbolValue(tstring section, tstring tag, const char * defaultval) =0;
	virtual std::vector<std::string>  GetSectionKeys(tstring section)=0;

	template <typename T>
	T GetSymbolValue(tstring section, tstring tag, T defaultval)
	{ 
		return ConvertFromString<T>(GetSymbolValue(section, 
			tag,
			(const char *) ConvertToString<T>(defaultval).c_str()), defaultval);
	}

	// Items / Tags
	COPCItems<TOPCItem> opcitems;

	// opc handles
	OPCHANDLE _hServerHandleGroup;					// server handle of our group
	OPCHANDLE _dwAdviseDataCallbackHandle;

	// needed interfaces
	CComPtr<IOPCServer>		_pIOPCServer;
	CComPtr<IOPCItemMgt>	_pIOPCItemMgt;
	CComPtr<IOPCSyncIO>		_pIOPCSyncIO;

protected:
	DWORD _dwAdviseShutdownHandle;

	// Configuration 
	bool _bAutoConnect;
	bool _bSynchronousUpdate;
	long _nOPCServerRate;
	long _nQueryServerPeriod;
	long _nMaxAutoReconnects;
	long _nReconnects;
	long _nProcessPriority;
	CLSID _gOpcServerClsid;


	//tstring _gInifilename;
	//tstring _contents;
	tstring _sTagSectionName;

	tstring _gszStartupErrors;
	//tstring _szLoggingFilename;

	// OPC Server parameters
	tstring _gszCncControllerProcessName;
	tstring _opcservermachine;
	tstring _gszOPCServerName;
	tstring _sClsid;



};

template <class TOPCItem>
HRESULT _stdcall COPCBackEnd<TOPCItem>::ShutdownRequest(/* [string][in] */ LPCWSTR szReason)
{
	static char name[] = "COPCBackEnd::ShutdownRequest";
	DebugMessage(StdStringFormat(_T("OPC Server %s Shutdown %S\n"),  _gszOPCServerName.c_str(),  szReason));
	Stop();
	Disconnect();

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////////////
// handle function, if user pressed on "Disconnect" button (OPC!!!)
template <class TOPCItem>
void COPCBackEnd<TOPCItem>::Disconnect() 
{
	static char name[] = "COPCBackEnd::Disconnect";
	if ( _pIOPCServer==NULL )
		return;
	AdviseShutdownConnection(false);
	AdviseDataCallbackConnection(false);
	////////////////////////////////////////////////////////////////////////////////
	/////// 1. release IOPCSyncIO interface
	if ( _pIOPCSyncIO )
	{
		_pIOPCSyncIO = NULL;
	}

	////////////////////////////////////////////////////////////////////////////////
	////////// 2. remove item from group aund release IOPCItemMgt interface
	OPCHANDLE		phServer[1];
	HRESULT*		pErrors;

	if ( _pIOPCItemMgt && opcitems._nValidItems>0)
	{
		phServer[0] = opcitems[0]->_hServerHandleItem;
		_pIOPCItemMgt->RemoveItems( 1, phServer, &pErrors );
		::CoTaskMemFree( pErrors );
		_pIOPCItemMgt 		= NULL;
	}

	///////////////////////////////////////////////////////////////////////////////
	////// 3. remove group from server and release IOPCServer interface
	if ( _pIOPCServer )
	{
		_pIOPCServer->RemoveGroup( _hServerHandleGroup, FALSE );
		_hServerHandleGroup	= 0;
		_pIOPCServer = NULL;
	}

}


//////////////////////////////////////////////////////////////////////////////////
// handle function, if user pressed on "Connect" button (OPC!!!)
template <class TOPCItem>
HRESULT COPCBackEnd<TOPCItem>::Connect() 
{
	static char name[] = "COPCBackEnd::Connect";
	USES_CONVERSION;

	HRESULT	hResult=S_OK;	// function results
	std::string computername;
	std::string sClsid;
	int i,j;

	// release old connections
	Disconnect();

	/////////////////////////////////////////////////////////////////////////////
	// create OPC server 
	/////////////////////////////////////////////////////////////////////////////
	_pIOPCServer=NULL;
	_pIOPCItemMgt=NULL;
	_pIOPCSyncIO=NULL;

	try 
	{
		// CLSID of remote OPC server may not be present on local computer, only remote computer.
		//hResult = CLSIDFromProgID( "OPC.SINUMERIK.Machineswitch", &clsid );

		if(!TestPing(_opcservermachine))
			throw StdStringFormat( _T("WARNING: Couldn't ping machine=%s"), _opcservermachine.c_str());


		MakeLower(_opcservermachine);
		if(_opcservermachine.size() < 1)
		{
			// create instance of the OPC server 
			hResult = CoCreateInstance(	_gOpcServerClsid,
				NULL,
				CLSCTX_SERVER,
				IID_IOPCServer,
				(void**)&_pIOPCServer 
				);
			if (FAILED (hResult))
			{
				_pIOPCServer=NULL;
				throw StdStringFormat( _T("WARNING: Couldn't create %s-Server\n CLSID=%s"), _gszOPCServerName.c_str(), sClsid.c_str());
			}
		}
		else {


			COAUTHIDENTITY* pAuthIdentityData=NULL; 

#if 1
			std::vector<tstring> id=Tokenize(GetSymbolValue("OPCSERVER", "User", ""),",") ;

			if(id.size()<1)
			{
				pAuthIdentityData=NULL; 
			}
			else
			{
				tstring _domain = (id.size() > 1)? id[0] : ".";
				tstring _user = (id.size() > 2)? id[1] : "auduser";
				tstring _password = (id.size() > 3)? id[2] : "SUNRISE";
				Auth_Identity idn(A2W( _domain.c_str()),A2W(_user.c_str()),A2W(_password.c_str()));
				pAuthIdentityData=idn.GetNullAuth_Identity();
			}

			tstring s_dwAuthzSvc=GetSymbolValue("OPCSERVER", "AuthzSvc", "RPC_C_AUTHZ_NAME") ;
			tstring s_dwAuthnLevel=GetSymbolValue("OPCSERVER", "AuthnLevel", "RPC_C_AUTHN_LEVEL_NONE") ;
			DWORD dwAuthzSvc  = DCOMEnums.Find(_bstr_t(s_dwAuthzSvc.c_str()));
			DWORD dwAuthnLevel  = DCOMEnums.Find(_bstr_t(s_dwAuthnLevel.c_str()));

#else

			DWORD dwAuthzSvc  =RPC_C_AUTHZ_NAME;
			DWORD dwAuthnLevel  =RPC_C_AUTHN_LEVEL_NONE;
#endif			
			COAUTHINFO athn;
			athn.dwAuthnSvc = RPC_C_AUTHN_WINNT; // NEVER CHANGES
			athn.dwAuthzSvc = dwAuthzSvc; // RPC_C_AUTHZ_NONE; // dwAuthzSvc; 
			athn.pwszServerPrincName = NULL;
			athn.dwAuthnLevel =  dwAuthnLevel; // RPC_C_AUTHN_LEVEL_NONE; // dwAuthnLevel;
			athn.dwImpersonationLevel = RPC_C_IMP_LEVEL_IDENTIFY; // n_impersonationlevelEnum;
			athn.dwCapabilities = EOAC_NONE;
			athn.pAuthIdentityData=pAuthIdentityData;
			//athn.pAuthIdentityData = (COAUTHIDENTITY*)idn;

			//COSERVERINFO is the class that stores the name of the server
			COSERVERINFO srvinfo; // = {0, server, NULL, 0}; // Create the object and query for two interfaces
			srvinfo.dwReserved1=0;
			srvinfo.dwReserved2=0;
			srvinfo.pwszName=A2W(_opcservermachine.c_str());
			srvinfo.pAuthInfo=&athn;

			//CoInitializeEx(NULL, COINIT_MULTITHREADED  );
			MULTI_QI mqi[]=
			{ 
				{&IID_IOPCServer, NULL , 0} 
			};

			hResult=CoCreateInstanceEx(
				_gOpcServerClsid, // Request an instance of class CLSID_MyBackupService
				NULL, // No aggregation
				CLSCTX_SERVER, // CLSCTX_SERVER, // Any server is fine
				&srvinfo, // Contains remote server name
				sizeof(mqi)/sizeof(mqi[0]), // number of interfaces we are requesting (2)
				(MULTI_QI        *) &mqi); // structure indicating IIDs and interface pointers

			::Sleep(1000);

			if (FAILED(hResult))
				throw StdStringFormat (_T("FAIL: Connect() Remote CoCreateInstanceEx.\n"));

			if (FAILED(mqi[0].hr)) 
				throw StdStringFormat(_T("FAIL: Connect() Remote mqi[0].hr.\n"));

			_pIOPCServer = (IOPCServer *) (mqi[0].pItf); // Retrieve first interface pointer hr=pBackupAdmin->StartBackup(); // use it…
		}


		if(_pIOPCServer==NULL)
			return E_FAIL;

		// This is how we will detect if the server is connected to the CNC
		OPCSERVERSTATUS * pServerStatus;
		if(FAILED(_pIOPCServer->GetStatus(&pServerStatus)))
		{
			_pIOPCServer=NULL;
			return E_FAIL;
		}

		/////////////////////////////////////////////////////////////////////////////
		//Add group to server
		/////////////////////////////////////////////////////////////////////////////

		// variables for AddGroup method
		LONG	lTimeBias = 1;				// we have greenwich meantime + 1 (Germany)
		FLOAT	fDeadband = 0;				// no deadband
		DWORD	dwRevisedUpdateRate = 0;	// revised update rate from server


		// add our group to the OPC server
		if(FAILED(hResult = _pIOPCServer->AddGroup(	
			L"TrackingGroup",		// name of the new group
			TRUE,					// group is active (sends callbacks)
			_nOPCServerRate,		// update rate of 100 ms
			23111980,				// our client handle of this group
			&lTimeBias,				// time bias
			&fDeadband,				// deadband until server sends change notifications
			LOCALE_USER_DEFAULT,	// LCID of the user
			&_hServerHandleGroup,	// server handle of the group
			&dwRevisedUpdateRate,	// revised update rate
			IID_IOPCItemMgt,		// desired interface of the group
			(LPUNKNOWN*)&_pIOPCItemMgt
			)))
		{
			_pIOPCItemMgt=NULL;
			throw std::string (_T("FAIL: Connect() couldn't add group to server!\n"));
		}


		// Get interface for read operations
		if ( FAILED(hResult =  _pIOPCItemMgt->QueryInterface(IID_IOPCSyncIO,(void**)&_pIOPCSyncIO )))
		{
			_pIOPCSyncIO=NULL;
			throw std::string(_T("FAIL: Connect() cculdn't query IOPCSyncIO interface.\n"));
		}

		/////////////////////////////////////////////////////////////////////////////
		// Add items to group
		/////////////////////////////////////////////////////////////////////////////
		std::string errmsg(_T(""));
		opcitems._nValidItems=0;
		for(j=0,i=0; i< opcitems.size(); i++)
		{
			opcitems[i]->_bEnabled=false;
			if(opcitems[i]->vt== VT_USERDEFINED)
				continue;
			if(FAILED(hResult = AddOPCItem(opcitems[i])))
			{
				throw StdStringFormat(_T("FAIL: Connect() couldn't add item %s to OPC Group.\n"), opcitems[i]->_alias.c_str());
				// If all was well, we could erase bad tags, unfortunately, if SERVER is quasi state, results can be bad
				// This all or none approach is questionable.
#if 0
				if(hResult==E_ABORT)
					throw StdStringFormat(_T("FAIL: Connect() couldn't add item %s to OPC Group.\n"), opcitems[i]->_alias.c_str());

				std::string err=StdStringFormat(_T("FAIL: Connect() couldn't add item %s to OPC Group.\n"), opcitems[i]->_alias.c_str());
				opcitems.erase(opcitems.begin() + i);
				i--; // move back. 
				ErrorMessage( errmsg);
				errmsg+= err;
#endif
			}
			else 
			{
				j++;
				opcitems._nValidItems++;
				opcitems[i]->_bEnabled=true;
			}
		}
		// If no items were added, we have attached to placeholder OPC server not attached to dormant CNC.
		if((j==0) && (opcitems.size()>0))
		{
			std::string err=StdStringFormat(_T("FAIL: Connect() couldn't add any Group items to OPC Server %s.\n"), _opcservermachine.c_str());
		}

	}
	catch(std::string errmsg)
	{
		std::string hrerr=StdStringFormat(_T("FAIL: Connect() OPC Server %s Connect failed %s COM Error=%x %s\n"),  _opcservermachine.c_str(), errmsg.c_str(), hResult,  ErrorFormatMessage(hResult).c_str());
		ErrorMessage( hrerr);

		// This will release the objects
		_pIOPCItemMgt=NULL;
		_pIOPCSyncIO=NULL;
		_pIOPCServer=NULL;
		return E_FAIL;

	}
	catch(...)
	{
		std::string hrerr=_T("OPC Server Connect failed\n");
		ErrorMessage( hrerr);
		// This will release the objects
		_pIOPCItemMgt=NULL;
		_pIOPCSyncIO=NULL;
		_pIOPCServer=NULL;
		return E_FAIL;
	}
	return S_OK;
}

inline WCHAR * WSTRClone(_bstr_t oldstr)
{
	WCHAR *newstr;
	//if(oldstr==NULL)
	//	oldstr=_bstr_t("");

	newstr = (WCHAR*)CoTaskMemAlloc(sizeof(WCHAR) * (wcslen(oldstr) + 1));

	if(newstr) wcscpy_s(newstr, wcslen(oldstr) + 1, oldstr.GetBSTR());
	return newstr;
}
template <class TOPCItem>
HRESULT COPCBackEnd<TOPCItem>::AddOPCItem(COPCItem * item) 
{
	static char name[] = "COPCBackEnd::AddOPCItem";
	// variables for AddItem method
	HRESULT hr=S_OK;
	OPCITEMDEF		opcItemDef[1];			// item definition structure
	OPCITEMRESULT*	popcItemResult =NULL;	// array of item result structures
	HRESULT*		phResultArray = NULL;	// array of result
	try 
	{
		// fill item definition structure with data of our item (/bag/state/opmode)
		opcItemDef[0].szAccessPath=L"";					// no access path description
		opcItemDef[0].szItemID=WSTRClone(item->_tagname.c_str());			// ItemID of the variable (BTSS name)
		opcItemDef[0].bActive=TRUE;						// set item active to get notifications, if value changes
		opcItemDef[0].hClient=(OPCHANDLE)item;		    // client handle of the item
		opcItemDef[0].dwBlobSize=0;						// no blob
		opcItemDef[0].pBlob=NULL;						//  -"-
		opcItemDef[0].vtRequestedDataType=VT_BSTR;		// value of the item should be delivered as BSTR

		// add our item to the group
		if(FAILED(hr=_pIOPCItemMgt->AddItems(	
			1,									// add 1 item...
			opcItemDef,							// with this definition
			(OPCITEMRESULT**)&popcItemResult,	// server data of this item
			(HRESULT**)&phResultArray			// was addition successfull?
			))) 
			return hr;

		if(FAILED(hr))
		{
			//GLogger << LINFO << (StdStringFormat(_T("COPCBackEnd::AddOPCItem  %s FAILED\n"), item->_tagname.c_str()));
		}
		else if(hr==S_FALSE)
		{
			///GLogger << LINFO << (StdStringFormat(_T("COPCBackEnd::AddOPCItem  %s not Successful\n"), item->_tagname.c_str()));
		}

		if(phResultArray[0]!=S_OK)
		{
			//GLogger << LINFO << (StdStringFormat(_T("COPCBackEnd::AddOPCItem  %s not Successful\n"), item->_tagname.c_str()));
			hr=E_FAIL;
		}
		item->_hServerHandleItem=popcItemResult[0].hServer;
	}
	catch(...)
	{

	}

	if(opcItemDef[0].szItemID!=NULL)
		::CoTaskMemFree( opcItemDef[0].szItemID );

	if(popcItemResult!=NULL)
		::CoTaskMemFree( popcItemResult );

	if(phResultArray!=NULL)
		::CoTaskMemFree( phResultArray );
	return hr;
}

template <class TOPCItem>
HRESULT _stdcall COPCBackEnd<TOPCItem>::OnDataChange( 
	/* [in] */ DWORD dwTransid,
	/* [in] */ OPCHANDLE hGroup,
	/* [in] */ HRESULT hrMasterquality,
	/* [in] */ HRESULT hrMastererror,
	/* [in] */ DWORD dwCount,
	/* [size_is][in] */ OPCHANDLE  *phClientItems,
	/* [size_is][in] */ VARIANT  *pvValues,
	/* [size_is][in] */ WORD  *pwQualities,
	/* [size_is][in] */ FILETIME  *pftTimeStamps,
	/* [size_is][in] */ HRESULT  *pErrors) 
{ 

	static char name[] = "COPCBackEnd::OnDataChange";
	if(dwCount<1)
		return E_FAIL;
	if(_hServerHandleGroup == hGroup)
		return E_FAIL;
	// read and log each item

	opcitems._ftTimeStamp=COleDateTime::GetCurrentTime ();

	for(long i=0; i< dwCount; i++)
	{

		if(dwCount>opcitems.size())
			break;
		if(!opcitems.ValidHandle(phClientItems[i]))
			continue; // this is an error of some sort

		COPCItem * item = (COPCItem *) phClientItems[i];

		item->_lastvalue=item->_value;
		item->_value=pvValues[i];
		::GetSystemTimeAsFileTime(&item->_time);
	}

	//OnApplicationAction();
	return S_OK; 
}

template <class TOPCItem>
HRESULT COPCBackEnd<TOPCItem>::UpdateGroup() 
{	
	static char name[] = "COPCBackEnd::UpdateGroup";

	HRESULT hr=S_OK;
	int i;

	// variables for Read method
	//std::vector<OPCHANDLE> phServers;
	OPCHANDLE * phServers=NULL;	
	OPCHANDLE phServer;
	OPCITEMSTATE*	pItemValues=NULL;	// item value
	HRESULT*		pErrors=NULL;		// read errors
	long n;                             // number of actual items
	std::string tmp;

	try
	{
		if (_pIOPCSyncIO==NULL)
			throw std::string(_T("COPCClientDlg::ReadGroup() Error: Bad IOpSyncIO \n"));

		if(opcitems.size()==0)
			throw std::string(_T("COPCClientDlg::ReadGroup() Error: _hServerHandleItems.size()==0 \n"));

		// Allocate storage for server item handles:
		phServers = (OPCHANDLE *) CoTaskMemAlloc (opcitems.size() * sizeof (OPCHANDLE));

		opcitems._ftTimeStamp=COleDateTime::GetCurrentTime ();
		for(n=0, i=0; i<	opcitems.size(); i++)
		{
			if(opcitems[i]->vt== VT_USERDEFINED || !opcitems[i]->_bEnabled)
				continue;
			n++;
			phServer = opcitems[i]->_hServerHandleItem; 
			phServers[i]=(phServer);

		}

		// read our group
		if(FAILED(hr = _pIOPCSyncIO->Read(
			OPC_DS_DEVICE,			// read from device (not from cache)...
			n,						// # items...
			&phServers[0],			// with this serverhandle and store...
			&pItemValues,			// value in this array
			&pErrors				// was reading successfull?
			)))
		{
			if(hr==E_ABORT)
			{
				Disconnect();
			}
			throw StdStringFormat(_T("UpdateGroup Error=%x\n"), hr);
		}

		// save values of each item
		for(int j=0, i=0; i< opcitems.size(); i++)
		{
			if(opcitems[i]->vt== VT_USERDEFINED || !opcitems[i]->_bEnabled)
				continue;
#if 1
			// check for bad data
			if (pErrors[j]!=S_OK)
			{
				// bad data
				//GLogger << LINFO << StdStringFormat(_T("Error for OPC data item %s \n"),opcitems[i]->_tagname.c_str() );
			}
			else if(pItemValues[j].wQuality==OPC_QUALITY_BAD)
			{
				// bad data
				//sLogger << LINFO << StdStringFormat(_T("Bad Quality data for OPC data item %s \n"),opcitems[i]->_tagname.c_str() );
			}
			else
#endif
			{
				_CriticalSection.Lock();
				opcitems[i]->_lastvalue=opcitems[i]->_value;
				opcitems[i]->_value.Attach(pItemValues[j].vDataValue);
				opcitems[i]->_time=pItemValues[j].ftTimeStamp;
				_CriticalSection.Unlock();
			}
			VariantClear(&pItemValues[j].vDataValue); // memory leak otherwise ...
			j++;

		}
		// These were event handlers to clean and filter data and notify listeners.
		//FilterValues(opcitems);
		//ValuesChanged(opcitems);

		// Switched back to virtual override for simplicity.
		OnValueChanged(opcitems);
		// Virtual method override Callback
		OnApplicationAction();

	}
	catch(std::string errmsg)
	{
		//GetErrorString(   dwError, LCID dwLocale, LPWSTR __RPC_FAR *ppString);
		//sLogger << LFATAL << (errmsg);
		hr= E_FAIL;
	}
	catch(...)
	{
		//sLogger << LFATAL  <<(_T("OPCBackend::UpdateGroup Unspecified Error\n"));
		// something catastrophic
		hr=E_FAIL;
	}

	// free server allocated memory
	if(phServers)
		::CoTaskMemFree (phServers);
	if(pItemValues)
		::CoTaskMemFree( pItemValues );
	if(pErrors)
		::CoTaskMemFree( pErrors );

	return hr;

}

template <class TOPCItem>
HRESULT COPCBackEnd<TOPCItem>::WriteItemValue(std::string tag,_variant_t val)
{
	static char name[] = "COPCBackEnd::WriteItemValue";

	TOPCItem * item = opcitems.FindItem(tag);
	if(item==NULL)
		return E_FAIL;
	item->_lastvalue=item->_value;
	item->_value=val;
	WriteItem(item,val);
	return S_OK;
}

template <class TOPCItem>
HRESULT COPCBackEnd<TOPCItem>::WriteItem(COPCItem * item, _variant_t value) 
{	
	static char name[] = "COPCBackEnd::WriteItem";

	/////////////////////////////////////////////////////////////////////////////
	////////// Write Item

	HRESULT hr;
	// variables for Read method
	OPCHANDLE		phServer[1];	// server handle of our item
	VARIANT  	    pItemValues[1];	// item value
	HRESULT*		pErrors;		// read errors

	//	phServer[0] = _hServerHandleItem;
	phServer[0] = item->_hServerHandleItem; 
	pItemValues[0]=value;

	// write our item
	if(FAILED(hr = _pIOPCSyncIO->Write(
		1,				// 1 item...
		phServer,		// with this serverhandle and store...
		(VARIANT *) &pItemValues,   // value in this array
		&pErrors		// was reading successfull?
		)))
		return hr;
	hr=pErrors[0];
	::CoTaskMemFree( pErrors );

	return hr;
}

template <class TOPCItem>
HRESULT _stdcall COPCBackEnd<TOPCItem>::AdviseShutdownConnection(bool bOn)
{
	static char name[] = "COPCBackEnd::AdviseShutdownConnection";
	HRESULT hr=S_OK;
	if(!IsConnected())
		return E_FAIL;
	if(bOn)
	{
		//hr=AtlAdvise(_pIOPCServer, pOPCShutdown, IID_IOPCShutdown, &dwAdviseShutdownHandle); 
		hr=AtlAdvise(_pIOPCServer, (IOPCShutdown*) this, IID_IOPCShutdown, &_dwAdviseShutdownHandle); 
	}
	else
	{
		if(_dwAdviseShutdownHandle!=NULL)
			hr=AtlUnadvise(_pIOPCServer, IID_IOPCShutdown, _dwAdviseShutdownHandle);
		_dwAdviseShutdownHandle=NULL;
	}

	return hr;

}
template <class TOPCItem>
HRESULT _stdcall COPCBackEnd<TOPCItem>::AdviseDataCallbackConnection(bool bOn)
{
	static char name[] = "COPCBackEnd::AdviseDataCallbackConnection";
	HRESULT hr=S_OK;

	// Error checks
	if(_pIOPCItemMgt==NULL)
		return E_FAIL;
	if(bOn && _dwAdviseDataCallbackHandle!=NULL)
		return E_FAIL;
	if(!bOn && _dwAdviseDataCallbackHandle==NULL)
		return E_FAIL;

	if(bOn)
	{
		//sLogger << LINFO  << "OPC Server" << _gszOPCServerName  << "Advice Datacallback\n";
		hr=AtlAdvise(_pIOPCItemMgt, (IOPCDataCallback *) this, IID_IOPCDataCallback, &_dwAdviseDataCallbackHandle); 
	}
	else
	{
		if(dwAdviseDataCallbackHandle!=NULL)
			hr=AtlUnadvise(_pIOPCItemMgt, IID_IOPCDataCallback, _dwAdviseDataCallbackHandle); 
		_dwAdviseDataCallbackHandle=NULL;

	}
	return hr;
}

static TCHAR * GetServerState( OPCSERVERSTATE dwServerState)
{
	static TCHAR * sServerState[]=
	{ _T(""),
	_T("OPC_STATUS_RUNNING"),
	_T("OPC_STATUS_FAILED"),
	_T("OPC_STATUS_NOCONFIG"),
	_T("OPC_STATUS_SUSPENDED"),
	_T("OPC_STATUS_TEST")
	};
	if(dwServerState> 0 && dwServerState< 5)
		return sServerState[dwServerState];
	else return _T("");
}

/* 
GetStatusString handles a break on the COM connection.
*/
template <class TOPCItem>
HRESULT COPCBackEnd<TOPCItem>::GetStatusString(std::string &tmp)
{
	static char name[] = "COPCBackEnd::GetStatusString";

	HRESULT hr=S_OK;
	USES_CONVERSION;
	OPCSERVERSTATUS * pServerStatus=NULL;
	tmp.clear();
	try {

		if(_pIOPCServer==NULL) 
			throw StdStringFormat(_T("OPC Server GetStatusString Failed OPC Server Not Connected"));

		if(_pIOPCServer!=NULL)
			hr = _pIOPCServer->GetStatus(&pServerStatus);

		//OPC Server Status Failed Reason=0x800706be  "The RPC server is unavailable"
		if(FAILED(hr) )
		{
			ErrorMessage(  StdStringFormat(_T("OPC Server GetStatusString Failed Reason=0x%x\n"), hr));
			_nReconnects++;

			//	if(hr == 0x800706be)
			{
				// Reset pointers in case connection to COM components is broken.
				_pIOPCServer=NULL;
				throw StdStringFormat(_T("OPC Server Not Connected"));
			}
		}

		COleDateTime ftStartTime=pServerStatus->ftStartTime;
		COleDateTime ftCurrentTime=pServerStatus->ftCurrentTime;
		COleDateTime ftLastUpdateTime=pServerStatus->ftLastUpdateTime;

		std::stringstream buf;
		buf << "Vendor = " << W2A(pServerStatus->szVendorInfo) << std::endl;
		buf << "Status = " << GetServerState(pServerStatus->dwServerState) << std::endl;
		buf << "Start Time = " <<  ftStartTime.Format(_T("%B %d %Y %I:%M:%S %p")) << std::endl;
		buf << "Current Time = " << ftCurrentTime.Format(_T("%B %d %Y %I:%M:%S %p")) << std::endl;
		//tmp.Append("Last Update = %s\n", (BSTR)( ftLastUpdateTime.DateFormat("ddd',' MMM dd yy ") + ftLastUpdateTime.TimeFormat("hh':'mm':'ss tt")));
		buf << "GroupCount = " <<  pServerStatus->dwGroupCount << std::endl;
		buf << "Bandwidth = " << pServerStatus->dwBandWidth << std::endl;
		buf << "MajorVersion = " <<  pServerStatus->wMajorVersion << std::endl;
		buf << "MinorVersion = " <<  pServerStatus->wMinorVersion << std::endl;
		buf << "BuildNumber = " <<  pServerStatus->wBuildNumber << std::endl;
		tmp=buf.str();
	}
	catch(std::string errmsg)
	{
		tmp=errmsg;
		DebugMessage( errmsg );

		hr=E_FAIL;
	}
	catch(...)
	{
		DebugMessage( "OPC Server Status Failed" );
		hr=E_FAIL;
	}
	if(pServerStatus!=NULL)
	{
		::CoTaskMemFree(  pServerStatus->szVendorInfo );
		::CoTaskMemFree(  pServerStatus );
	}

	return hr;
}

// Done 1st - errors are handled by throwing std::exception
template <class TOPCItem>
int COPCBackEnd<TOPCItem>::ReadOPCSettings(tstring filename)
{
	static char name[] = "COPCBackEnd::ReadOPCSettings";

	/** Inifile [OPCSERVER]  Items*/
	_bAutoConnect=  GetSymbolValue("OPCSERVER", "AutoConnect", (bool) true);
	_nMaxAutoReconnects=  GetSymbolValue("OPCSERVER", "AutoReconnectMaxAttempts", (long) 0);
	_nQueryServerPeriod=  GetSymbolValue("OPCSERVER", "ServerRate", (long) 5000);
	_nProcessPriority=  GetSymbolValue("OPCSERVER", "ProcessPriority", (long) ABOVE_NORMAL_PRIORITY_CLASS);
	_bSynchronousUpdate=  GetSymbolValue("OPCSERVER", "SynchronousUpdate",(bool) true);
	_nOPCServerRate=  GetSymbolValue("OPCSERVER", "ServerRate",(long) 1000);

	// Check [OPCSERVER] data
	if(_bSynchronousUpdate && _nOPCServerRate<0)
		throw std::exception(_T("Error: Negative Synchronous OPC Server Sampling Rate\n"));
	if(_sTagSectionName.size()<1)
		throw std::exception(_T("Error: [OPCSERVER] Parameter TAGS Undefined\n"));
	if(_opcservermachine.empty())
		throw std::exception(_T("Error: [OPCSERVER] Opc Server Machine (IP) Undefined\n"));
	if(_sTagSectionName.empty())
		throw std::exception(_T("Error: [OPCSERVER] Tag names Undefined\n"));
 

	_sClsid= GetSymbolValue(_sTagSectionName, "CLSID", "");
	_sClsid = "{" +_sClsid + "}";
	if(FAILED(CLSIDFromString(_bstr_t(_sClsid.c_str()) , &_gOpcServerClsid)))
		throw std::exception("Misformed or mistyped OPC Server Class ID\n");
	_gszOPCServerName= GetSymbolValue(_sTagSectionName, "SERVERNAME", "");
	_gszCncControllerProcessName= GetSymbolValue(_sTagSectionName, "CNCProcessName", "");

	return 0;

}

// NORMAL_PRIORITY_CLASS             0x00000020
// HIGH_PRIORITY_CLASS               0x00000080
// REALTIME_PRIORITY_CLASS           0x00000100
// BELOW_NORMAL_PRIORITY_CLASS       0x00004000
// ABOVE_NORMAL_PRIORITY_CLASS       0x00008000
// IDLE_PRIORITY_CLASS               0x00000040

inline void SetBackgroundPriority(long nProcessPriority)
{
	DWORD dwError, dwPriClass;

	//Short circuit for now
	nProcessPriority=0x00008000;

	if(!SetPriorityClass(GetCurrentProcess(), nProcessPriority))
	{
		dwError = GetLastError();
		//OutputDebugString(StrFormat(_T("Failed to enter background mode (%d)\n"), dwError).c_str());
		//sLogger << LWARN  << "Failed to enter background mode " ;// << (long) dwError  << std::endl;
		goto Cleanup;
	} 

	// Display priority class

	dwPriClass = GetPriorityClass(GetCurrentProcess());


Cleanup:
	// TODO: Clean up
	;
}
// Follows readopcsettings...
template <class TOPCItem>
void COPCBackEnd<TOPCItem>::OnApplicationInit()
{
	// BELOW_NORMAL_PRIORITY_CLASS, PROCESS_MODE_BACKGROUND_BEGIN
	SetBackgroundPriority(_nProcessPriority);
}
