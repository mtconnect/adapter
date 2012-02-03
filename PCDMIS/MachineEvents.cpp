//
// MachineEvents.cpp
//

#include "stdafx.h"
#include "PcDmis_Adapter.hpp"
#include "MachineEvents.h"
#import "pcdlrn.tlb" 
#include "StdStringFcn.h"

//Constructor.
CMachineEvents::CMachineEvents(PcDmisAdapter * adapter) :
m_pConnectionPoint(NULL),
m_dwConnection(0),
_adapter(adapter)
{
	m_refCount = 0;
}

//Destructor.
CMachineEvents::~CMachineEvents()
{}

STDMETHODIMP CMachineEvents::QueryInterface(REFIID riid, void ** ppvObj)
{
	if (riid == IID_IUnknown){
		*ppvObj = static_cast<IUnknown*>(this);
	}

	else if (riid == IID_IDispatch){
		*ppvObj = static_cast<IDispatch*>(this);
	}

	else if (riid == __uuidof(PCDLRN::IMachineEvents)){
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
STDMETHODIMP_(ULONG) CMachineEvents::AddRef()
{
	return ++m_refCount;
}

/******************************************************************************
*  Release() -- When a reference to this object is removed, this 
*  function decrements the reference count. If the reference count is 0, then 
*  this function deletes this object and returns 0.
******************************************************************************/ 
STDMETHODIMP_(ULONG) CMachineEvents::Release()
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
*  AttachToSource -- This method attaches to an event source.
******************************************************************************/ 
STDMETHODIMP CMachineEvents::AttachToSource( IUnknown* pEventSource )
{
	HRESULT hr = S_OK;

	IConnectionPointContainer* pCPC = NULL;
	hr = pEventSource->QueryInterface( IID_IConnectionPointContainer, 
		(void**)&pCPC );
	if (SUCCEEDED(hr)){

		hr = pCPC->FindConnectionPoint( __uuidof(PCDLRN::IMachineEvents), 
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
STDMETHODIMP CMachineEvents::DetachFromSource()
{
	HRESULT hr = S_OK;
	if (m_pConnectionPoint != NULL){
		m_pConnectionPoint->Unadvise( m_dwConnection );
		m_pConnectionPoint = NULL;
		m_dwConnection=NULL;
	}
	return hr;
}



STDMETHODIMP CMachineEvents::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid,
									  WORD wFlags, DISPPARAMS* pDispParams,
									  VARIANT* pVarResult, EXCEPINFO* pExcepInfo,
									  UINT* puArgErr)
{
	try {
		switch(dispIdMember)
		{
		case 0x001:
			LearnHit(pDispParams->rgvarg[0].dblVal,pDispParams->rgvarg[1].dblVal,pDispParams->rgvarg[2].dblVal,pDispParams->rgvarg[3].dblVal,pDispParams->rgvarg[4].dblVal,pDispParams->rgvarg[5].dblVal);
			break;
		case 0x002:
			ExecuteHit(pDispParams->rgvarg[0].dblVal,pDispParams->rgvarg[1].dblVal,pDispParams->rgvarg[2].dblVal,pDispParams->rgvarg[3].dblVal,pDispParams->rgvarg[4].dblVal,pDispParams->rgvarg[5].dblVal);
			break;
		case 0x003:
			ErrorMsg(pDispParams->rgvarg[0].bstrVal,pDispParams->rgvarg[1].lVal);
			break;
		case 0x004:
			Command(pDispParams->rgvarg[0].lVal);
			break;
		case 0x005:
			Readout(pDispParams->rgvarg[0].dblVal,pDispParams->rgvarg[1].dblVal,pDispParams->rgvarg[2].dblVal,pDispParams->rgvarg[3].dblVal,pDispParams->rgvarg[4].dblVal,pDispParams->rgvarg[5].dblVal);
			break;
		case 0x006:
			ExecuteManualScanHit(pDispParams->rgvarg[0].dblVal,pDispParams->rgvarg[1].dblVal,pDispParams->rgvarg[2].dblVal,pDispParams->rgvarg[3].dblVal,pDispParams->rgvarg[4].dblVal,pDispParams->rgvarg[5].dblVal);
			break;
		case 0x007:
			FeatureID(pDispParams->rgvarg[0].bstrVal);
			break;
		case 0x008:
			FeatureType(pDispParams->rgvarg[0].bstrVal);
			break;
		case 0x009:
			RMS(pDispParams->rgvarg[0].dblVal);
			break;
		case 0x0a:
			DistanceToTarget(pDispParams->rgvarg[0].dblVal,pDispParams->rgvarg[1].dblVal,pDispParams->rgvarg[2].dblVal,pDispParams->rgvarg[3].dblVal,pDispParams->rgvarg[4].dblVal);
			break;
		case 0x0b:
			DistanceToClosestCAD(pDispParams->rgvarg[0].dblVal,pDispParams->rgvarg[1].dblVal,pDispParams->rgvarg[2].dblVal,pDispParams->rgvarg[3].dblVal,pDispParams->rgvarg[4].dblVal);
			break;
		case 0x0c:
			ReadoutExt(pDispParams->rgvarg[0].dblVal,pDispParams->rgvarg[1].dblVal,pDispParams->rgvarg[2].dblVal);
			break;
		default:
			OutputDebugString("CPartProgramSink::Invoke Unhandled invoke\n");
			break;

		}
	}
	catch(...)
	{
		OutputDebugString("CPartProgramSink::Invoke exception\n");

	}
	return S_OK;
}

// Methods:
STDMETHODIMP CMachineEvents::LearnHit (
									   double X,
									   double Y,
									   double Z,
									   double I,
									   double J,
									   double K )
{
	OutputDebugString(StdStringFormat("CMachineEvents::LearnHit (%f, %f, %f) (%f, %f, %f) \n", X,Y,Z, I,J,K).c_str());
	return S_OK;
}
STDMETHODIMP CMachineEvents::ExecuteHit (
	double X,
	double Y,
	double Z,
	double I,
	double J,
	double K )
{
	OutputDebugString(StdStringFormat("CMachineEvents::ExecuteHit (%f, %f, %f) (%f, %f, %f) \n", X,Y,Z, I,J,K).c_str());
	return S_OK;
}
STDMETHODIMP CMachineEvents::ErrorMsg (
									   _bstr_t Msg,
									   long ErrorType )
{
	OutputDebugString(StdStringFormat("CMachineEvents::ErrorMsg %s | %d\n", (LPCSTR) Msg, ErrorType).c_str());
	return S_OK;
}
STDMETHODIMP CMachineEvents::Command (
									  long Code )
{
	OutputDebugString(StdStringFormat("CMachineEvents::Command %d \n", Code).c_str());
	return S_OK;
}
STDMETHODIMP CMachineEvents::Readout (
									  double X,
									  double Y,
									  double Z,
									  double I,
									  double J,
									  double K )
{
	OutputDebugString(StdStringFormat("CMachineEvents::Readout (%f, %f, %f) (%f, %f, %f) \n", X,Y,Z, I,J,K).c_str());
	return S_OK;
}
STDMETHODIMP CMachineEvents::ExecuteManualScanHit (
	double X,
	double Y,
	double Z,
	double I,
	double J,
	double K )
{
	OutputDebugString(StdStringFormat("CMachineEvents::ExecuteManualScanHit (%f, %f, %f) (%f, %f, %f) \n", X,Y,Z, I,J,K).c_str());
	return S_OK;
}
STDMETHODIMP CMachineEvents::FeatureID (
										_bstr_t fid )
{
	OutputDebugString(StdStringFormat("CMachineEvents::FeatureID %s\n", (LPCSTR) fid).c_str());
	return S_OK;
}
STDMETHODIMP CMachineEvents::FeatureType (
	_bstr_t ftype )
{
	OutputDebugString(StdStringFormat("CMachineEvents::RMS %s\n", (LPCSTR) ftype).c_str());
	return S_OK;
}
STDMETHODIMP CMachineEvents::RMS (double RMS )
{
	OutputDebugString(StdStringFormat("CMachineEvents::RMS %f\n", RMS).c_str());
	return S_OK;
}
STDMETHODIMP CMachineEvents::DistanceToTarget (
	double dX,
	double dY,
	double dZ,
	double T,
	double tol )
{
	OutputDebugString(StdStringFormat("CMachineEvents::DistanceToTarget %f, %f, %f\n", dX,dY,dZ).c_str());
	return S_OK;
}
STDMETHODIMP CMachineEvents::DistanceToClosestCAD (
	double dX,
	double dY,
	double dZ,
	double T,
	double tol )
{
	OutputDebugString(StdStringFormat("CMachineEvents::DistanceToClosestCAD %f, %f, %f\n", dX,dY,dZ).c_str());
	return S_OK;
}
STDMETHODIMP CMachineEvents::ReadoutExt (
	double X,
	double Y,
	double Z )
{
	OutputDebugString(StdStringFormat("CMachineEvents::ReadoutExt (%f, %f, %f)\n", X,Y,Z).c_str());
	return S_OK;
}