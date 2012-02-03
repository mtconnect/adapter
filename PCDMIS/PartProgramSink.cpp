//
// PartProgramSink.cpp
//

#include "stdafx.h"
#include "PartProgramSink.h"
#import "pcdlrn.tlb" 
#include "StdStringFcn.h"
#include "PcDmis_Adapter.hpp"

//Constructor.
CPartProgramSink::CPartProgramSink(PcDmisAdapter * adapter) :
m_pConnectionPoint(NULL),
m_dwConnection(0),
_adapter(adapter)
{
	m_refCount = 0;
}

//Destructor.
CPartProgramSink::~CPartProgramSink()
{}

/******************************************************************************
*   IUnknown Interfaces -- All COM objects must implement, either 
*  directly or indirectly, the IUnknown interface.
******************************************************************************/ 

/******************************************************************************
*  QueryInterface -- Determines if this component supports the 
*  requested interface, places a pointer to that interface in ppvObj if it is 
*  available, and returns S_OK.  If not, sets ppvObj to NULL and returns 
*  E_NOINTERFACE.
******************************************************************************/ 
STDMETHODIMP CPartProgramSink::QueryInterface(REFIID riid, void ** ppvObj)
{
	if (riid == IID_IUnknown){
		*ppvObj = static_cast<IUnknown*>(this);
	}

	else if (riid == IID_IDispatch){
		*ppvObj = static_cast<IDispatch*>(this);
	}

	else if (riid == __uuidof(PCDLRN::IApplicationEvents)){
		*ppvObj = static_cast<IDispatch*>(this);
	}

	else{
		*ppvObj = NULL;
		return E_NOINTERFACE;
	}

	static_cast<IUnknown*>(*ppvObj)->AddRef();
	return S_OK;
}

/******************************************************************************
*  AddRef() -- In order to allow an object to delete itself when 
*  it is no longer needed, it is necessary to maintain a count of all 
*  references to this object. When a new reference is created, this function 
*  increments the count.
******************************************************************************/ 
STDMETHODIMP_(ULONG) CPartProgramSink::AddRef()
{
	return ++m_refCount;
}

/******************************************************************************
*  Release() -- When a reference to this object is removed, this 
*  function decrements the reference count. If the reference count is 0, then 
*  this function deletes this object and returns 0.
******************************************************************************/ 
STDMETHODIMP_(ULONG) CPartProgramSink::Release()
{
	m_refCount--;

	if (m_refCount == 0)
	{
		delete this;
		return 0;
	}
	return m_refCount;
}

/******************************************************************************
*   IDispatch Interface -- This interface allows this class to be used as an
*   automation server, allowing its functions to be called by other COM
*   objects.
******************************************************************************/ 

/******************************************************************************
*   GetTypeInfoCount -- This function determines if the class supports type 
*   information interfaces or not. It places 1 in iTInfo if the class supports
*   type information and 0 if it does not.
******************************************************************************/ 
STDMETHODIMP CPartProgramSink::GetTypeInfoCount(UINT *iTInfo)
{
	*iTInfo = 0;
	return S_OK;
}

/******************************************************************************
*   GetTypeInfo -- Returns the type information for the class. For classes 
*   that do not support type information, this function returns E_NOTIMPL;
******************************************************************************/ 
STDMETHODIMP CPartProgramSink::GetTypeInfo(UINT iTInfo, LCID lcid, 
										   ITypeInfo **ppTInfo)
{
	return E_NOTIMPL;
}

/******************************************************************************
*   GetIDsOfNames -- Takes an array of strings and returns an array of DISPIDs
*   that correspond to the methods or properties indicated. If the name is not 
*   recognized, returns DISP_E_UNKNOWNNAME.
******************************************************************************/ 
STDMETHODIMP CPartProgramSink::GetIDsOfNames(REFIID riid,  
											 OLECHAR **rgszNames, 
											 UINT cNames,  LCID lcid,
											 DISPID *rgDispId)
{
	return E_NOTIMPL;
}

/******************************************************************************
*   Invoke -- Takes a dispid and uses it to call another of this class's 
*   methods. Returns S_OK if the call was successful.
******************************************************************************/ 
STDMETHODIMP CPartProgramSink::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid,
									  WORD wFlags, DISPPARAMS* pDispParams,
									  VARIANT* pVarResult, EXCEPINFO* pExcepInfo,
									  UINT* puArgErr)
{
	try
	{
		switch(dispIdMember)
		{
		case 0x003:
			OnStartExecution();
			break;
		case 0x004:
			{
				OnEndExecution( pDispParams->rgvarg[1].lVal);
				//if(pDispParams->rgvarg[1].vt & VT_BYREF)
				//{
				//   HandleSheetChange( // Call the function.
				//      *(pDispParams->rgvarg[1].ppdispVal),
				//      *(pDispParams->rgvarg[0].ppdispVal));
				//}
				//else
				//{
				//   HandleSheetChange(  // Call the function.
				//      pDispParams->rgvarg[1].pdispVal,
				//      pDispParams->rgvarg[0].pdispVal);
				//}
			}
			break;
		case 0x006:
			{
				this->OnOpenPartProgram( );
			}
			break;
		case 0x007:
			{
				this->OnClosePartProgram();
			}
			break;
		case 0x009:
			{
				_bstr_t status(pDispParams->rgvarg[0].bstrVal);
				OnUpdateStatusMessage(status);
			}
			break;
		default:
			OutputDebugString(StdStringFormat("Default On status: \n").c_str());
			//_bstr_t status(pDispParams->rgvarg[0].bstrVal);
			//OnUpdateStatusMessage(status);
		}

	}
	catch(...)
	{
		OutputDebugString("CPartProgramSink::Invoke Exception\n");
	}
	return S_OK;
}
/******************************************************************************
*  AttachToSource -- This method attaches to an event source.
******************************************************************************/ 
STDMETHODIMP CPartProgramSink::AttachToSource( IUnknown* pEventSource )
{
	HRESULT hr = S_OK;

	IConnectionPointContainer* pCPC = NULL;
	hr = pEventSource->QueryInterface( IID_IConnectionPointContainer, 
		(void**)&pCPC );
	if (SUCCEEDED(hr)){

		hr = pCPC->FindConnectionPoint( __uuidof(PCDLRN::IApplicationEvents), 
			&m_pConnectionPoint );
		if (SUCCEEDED(hr)){

			hr = m_pConnectionPoint->Advise( this, &m_dwConnection );
		}
		pCPC->Release();
	}

	return hr;
}
/******************************************************************************
*  DetachFromSource -- This method detaches from an event source.
******************************************************************************/ 
STDMETHODIMP CPartProgramSink::DetachFromSource()
{
	HRESULT hr = S_OK;
	if (m_pConnectionPoint != NULL){
		m_pConnectionPoint->Unadvise( m_dwConnection );
		m_pConnectionPoint = NULL;
		m_dwConnection=NULL;
	}
	return hr;
}
/******************************************************************************
*  HandleBeforeWorkbookClose -- This method processes the BeforeWorkbookClose
*  event for the application attached to this event handler.
******************************************************************************/ 
STDMETHODIMP CPartProgramSink::OnStartExecution( )
{
	OutputDebugString("OnStartExecution\n");
	_state= CPartProgramSink::RUNNING;
	HRESULT hr = S_OK;
	return hr;
}
STDMETHODIMP CPartProgramSink::OnEndExecution( long statuscode )
{
	OutputDebugString("OnEndExecution\n");
	_adapter->UpdateProbingOperations();
	_state= CPartProgramSink::READY;
	HRESULT hr = S_OK;
	return hr;
}
STDMETHODIMP CPartProgramSink::OnOpenPartProgram( )
{
	OutputDebugString("OnOpenPartProgram\n");
	// Testing purposes to see what existing program has to say
	//_adapter->CountOutToleranceFeatures();
	HRESULT hr = S_OK;
	return hr;
}
STDMETHODIMP CPartProgramSink::OnClosePartProgram(  )
{
	OutputDebugString("OnClosePartProgram\n");
	//_adapter->UpdateProbingOperations();
	HRESULT hr = S_OK;
	return hr;
} 
STDMETHODIMP CPartProgramSink::OnUpdateStatusMessage(_bstr_t message)
{
	message="OnUpdateStatusMessage\n"+message;
	OutputDebugString(message);
	HRESULT hr = S_OK;
	return hr;
} 