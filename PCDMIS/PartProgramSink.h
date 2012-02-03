#pragma once
#include "oaidl.h"
#include "ocidl.h"
#include "comdef.h"

// http://support.microsoft.com/kb/309301 

//00024413-0000-0000-C000-000000000046
//const IID IID_ApplicationEvents  = 
//{0x00024413,0x0000,0x0000,{0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}};
class PcDmisAdapter;

class CPartProgramSink : public IDispatch
{
protected:
   int m_refCount;
   IConnectionPoint* m_pConnectionPoint;

public:
    DWORD m_dwConnection;
	PcDmisAdapter * _adapter;
  //Constructor.
   CPartProgramSink(PcDmisAdapter *);
   //Destructor.
   ~CPartProgramSink();
   
   /***** IUnknown Methods *****/ 
   STDMETHODIMP QueryInterface(REFIID riid, void ** ppvObj);
   STDMETHODIMP_(ULONG) AddRef();
   STDMETHODIMP_(ULONG) Release();
   
   /***** IDispatch Methods *****/ 
   STDMETHODIMP GetTypeInfoCount(UINT *iTInfo);
   STDMETHODIMP GetTypeInfo(UINT iTInfo, LCID lcid, 
      ITypeInfo **ppTInfo);
   STDMETHODIMP GetIDsOfNames(REFIID riid,  
      OLECHAR **rgszNames, 
      UINT cNames,  LCID lcid,
      DISPID *rgDispId);
   STDMETHODIMP Invoke(DISPID dispIdMember, REFIID riid, LCID lcid,
      WORD wFlags, DISPPARAMS* pDispParams,
      VARIANT* pVarResult, EXCEPINFO* pExcepInfo,
      UINT* puArgErr);

   /**** Attach/Detach from event source *****/ 
   STDMETHODIMP AttachToSource( IUnknown* pEventSource );
   STDMETHODIMP DetachFromSource();


   static const int READY=0;
   static const int RUNNING=1;
   static const int PAUSED=2;
   int _state;


   STDMETHODIMP OnStartExecution( );
   STDMETHODIMP OnEndExecution(  long nStatusCode);
   STDMETHODIMP OnOpenPartProgram( );
    STDMETHODIMP OnClosePartProgram( );
    STDMETHODIMP OnUpdateStatusMessage(_bstr_t message);


 // STDMETHODIMP OnObjectAboutToExecute(IDispatch * pDispatch);
 //  STDMETHODIMP OnObjectExecuted(IDispatch * pDispatch);
};

			

