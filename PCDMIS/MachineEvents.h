//
// MachineEvents.h
//


#pragma once
#include "oaidl.h"
#include "ocidl.h"
#include "comdef.h"

// http://support.microsoft.com/kb/309301 

//00024413-0000-0000-C000-000000000046
//const IID IID_ApplicationEvents  = 
//{0x00024413,0x0000,0x0000,{0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}};
class PcDmisAdapter;
class CMachineEvents : public IDispatch
{
protected:
	int m_refCount;
	IConnectionPoint* m_pConnectionPoint;

public:
	DWORD m_dwConnection;
	PcDmisAdapter * _adapter;
	//////////////////////////////////////////////////////////////////////
	//Constructor.
	CMachineEvents(PcDmisAdapter * adapter);
	//Destructor.
	~CMachineEvents();

	/***** IUnknown Methods *****/ 
	STDMETHODIMP QueryInterface(REFIID riid, void ** ppvObj);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();

	/***** IDispatch Methods *****/ 
	STDMETHODIMP GetTypeInfoCount(UINT *iTInfo){ *iTInfo = 0;return S_OK; }
	STDMETHODIMP GetTypeInfo(UINT iTInfo, LCID lcid, 
		ITypeInfo **ppTInfo){ return E_NOTIMPL; }
	STDMETHODIMP GetIDsOfNames(REFIID riid,  
		OLECHAR **rgszNames, 
		UINT cNames,  LCID lcid,
		DISPID *rgDispId) { return E_NOTIMPL; }
	STDMETHODIMP Invoke(DISPID dispIdMember, REFIID riid, LCID lcid,
		WORD wFlags, DISPPARAMS* pDispParams,
		VARIANT* pVarResult, EXCEPINFO* pExcepInfo,
		UINT* puArgErr);

	/**** Attach/Detach from event source *****/ 
	STDMETHODIMP AttachToSource( IUnknown* pEventSource );
	STDMETHODIMP DetachFromSource();

	// Methods:
	STDMETHODIMP LearnHit (
		double X,
		double Y,
		double Z,
		double I,
		double J,
		double K );
	STDMETHODIMP ExecuteHit (
		double X,
		double Y,
		double Z,
		double I,
		double J,
		double K );
	STDMETHODIMP ErrorMsg (
		_bstr_t Msg,
		long ErrorType );
	STDMETHODIMP Command (
		long Code );
	STDMETHODIMP Readout (
		double X,
		double Y,
		double Z,
		double I,
		double J,
		double K );
	STDMETHODIMP ExecuteManualScanHit (
		double X,
		double Y,
		double Z,
		double I,
		double J,
		double K );
	STDMETHODIMP FeatureID (
		_bstr_t fid );
	STDMETHODIMP FeatureType (
		_bstr_t ftype );
	STDMETHODIMP RMS (
		double RMS );
	STDMETHODIMP DistanceToTarget (
		double dX,
		double dY,
		double dZ,
		double T,
		double tol );
	STDMETHODIMP DistanceToClosestCAD (
		double dX,
		double dY,
		double dZ,
		double T,
		double tol );
	STDMETHODIMP ReadoutExt (
		double X,
		double Y,
		double Z );

};



