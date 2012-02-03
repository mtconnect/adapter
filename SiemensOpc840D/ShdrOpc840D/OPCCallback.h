// **************************************************************************
// OPCCallback.h: interface for the COPCCallback class.
//
// Description:
//
// DISCLAIMER:
// This software was developed by U.S. Government employees as part of
// their official duties and is not subject to copyright. No warranty implied 
// or intended.
// **************************************************************************


#pragma once
#include "opcda.h"
#include "opccomm.h"


class COPCShutdown : 
public CComObjectRootEx<CComMultiThreadModel>,
public CComCoClass<COPCShutdown, &CLSID_NULL>,
public IOPCShutdown,
public IOPCDataCallback
{
public:
	typedef CComCoClass<COPCShutdown, &CLSID_NULL> COPCBaseClass;
	virtual ULONG STDMETHODCALLTYPE AddRef( void) 
	{
		return S_OK;
		
	}
	
	virtual ULONG STDMETHODCALLTYPE Release( void) 
	{
		return S_OK;
		
	}
	STDMETHODIMP QueryInterface( REFIID iid, LPVOID* ppInterface)
	{
		// check valid pointer
		//
		if ( ppInterface == NULL)
			return E_INVALIDARG;

		// default return is NULL in case of error
		//
		*ppInterface = NULL;	

		if ( iid == IID_IUnknown )
		{
			*ppInterface = (IUnknown*)  (IOPCDataCallback*)  this;
		}

		else if ( iid == IID_IOPCDataCallback)
		{
			*ppInterface = (IOPCDataCallback*) this;
		}

		else if ( iid == IID_IOPCShutdown)
		{
			*ppInterface = (IOPCShutdown*) this;
		}

		else return E_FAIL;

		return S_OK;

	}
	virtual HRESULT STDMETHODCALLTYPE ShutdownRequest( 
		/* [string][in] */ LPCWSTR szReason)
	{
		return S_OK;
	}


		DWORD dwAdviseDataCallbackHandle;

	virtual HRESULT STDMETHODCALLTYPE OnDataChange( 
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
		return E_NOTIMPL; 
	}

	virtual HRESULT STDMETHODCALLTYPE OnReadComplete( 
		/* [in] */ DWORD dwTransid,
		/* [in] */ OPCHANDLE hGroup,
		/* [in] */ HRESULT hrMasterquality,
		/* [in] */ HRESULT hrMastererror,
		/* [in] */ DWORD dwCount,
		/* [size_is][in] */ OPCHANDLE  *phClientItems,
		/* [size_is][in] */ VARIANT  *pvValues,
		/* [size_is][in] */ WORD  *pwQualities,
		/* [size_is][in] */ FILETIME  *pftTimeStamps,
		/* [size_is][in] */ HRESULT  *pErrors) { return E_NOTIMPL; }

		virtual HRESULT STDMETHODCALLTYPE OnWriteComplete( 
		/* [in] */ DWORD dwTransid,
		/* [in] */ OPCHANDLE hGroup,
		/* [in] */ HRESULT hrMastererr,
		/* [in] */ DWORD dwCount,
		/* [size_is][in] */ OPCHANDLE  *pClienthandles,
		/* [size_is][in] */ HRESULT  *pErrors) { return E_NOTIMPL; };

		virtual HRESULT STDMETHODCALLTYPE OnCancelComplete( 
		/* [in] */ DWORD dwTransid,
		/* [in] */ OPCHANDLE hGroup) { return E_NOTIMPL; }
		// Dummy out AddRef and Release, as we don't want the item to be deleted...
	
};
typedef CComObject<COPCShutdown> CComOPCShutdown;

class ATL_NO_VTABLE COPCDataCallback : 
//	public CComObjectRootEx<CComSingleThreadModel>,
//	public CComCoClass<COPCDataCallback, &CLSID_NULL>,
	public IOPCDataCallback
{

public:
		DWORD dwAdviseDataCallbackHandle;

	static HRESULT WINAPI Error(LPCOLESTR lpszDesc,
		const IID& iid = GUID_NULL, HRESULT hRes = 0)
	{
		//return AtlReportError(CLSID_NULL, lpszDesc, iid, hRes);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE OnDataChange( 
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
		return E_NOTIMPL; 
	}

	virtual HRESULT STDMETHODCALLTYPE OnReadComplete( 
		/* [in] */ DWORD dwTransid,
		/* [in] */ OPCHANDLE hGroup,
		/* [in] */ HRESULT hrMasterquality,
		/* [in] */ HRESULT hrMastererror,
		/* [in] */ DWORD dwCount,
		/* [size_is][in] */ OPCHANDLE  *phClientItems,
		/* [size_is][in] */ VARIANT  *pvValues,
		/* [size_is][in] */ WORD  *pwQualities,
		/* [size_is][in] */ FILETIME  *pftTimeStamps,
		/* [size_is][in] */ HRESULT  *pErrors) { return E_NOTIMPL; }

		virtual HRESULT STDMETHODCALLTYPE OnWriteComplete( 
		/* [in] */ DWORD dwTransid,
		/* [in] */ OPCHANDLE hGroup,
		/* [in] */ HRESULT hrMastererr,
		/* [in] */ DWORD dwCount,
		/* [size_is][in] */ OPCHANDLE  *pClienthandles,
		/* [size_is][in] */ HRESULT  *pErrors) { return E_NOTIMPL; };

		virtual HRESULT STDMETHODCALLTYPE OnCancelComplete( 
		/* [in] */ DWORD dwTransid,
		/* [in] */ OPCHANDLE hGroup) { return E_NOTIMPL; }
		// Dummy out AddRef and Release, as we don't want the item to be deleted...
		virtual ULONG STDMETHODCALLTYPE AddRef( void) 
	{
		return S_OK;

	}

	virtual ULONG STDMETHODCALLTYPE Release( void) 
	{
		return S_OK;

	}
	STDMETHODIMP QueryInterface( REFIID iid, LPVOID* ppInterface)
	{
		// check valid pointer
		//
		if ( ppInterface == NULL)
			return E_INVALIDARG;

		// default return is NULL in case of error
		//
		*ppInterface = NULL;	

		if ( iid == IID_IUnknown )
		{
			*ppInterface = (IUnknown*) this;
		}

		else if ( iid == IID_IOPCDataCallback)
		{
			*ppInterface = (IOPCDataCallback*) this;
		}
		else return E_FAIL;

		return S_OK;

	}
};

