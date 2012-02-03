
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0347 */
/* at Thu Jul 01 09:42:19 2004
 */
/* Compiler settings for OPCCOMN.IDL:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __OPCComm_h__
#define __OPCComm_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IOPCShutdown_FWD_DEFINED__
#define __IOPCShutdown_FWD_DEFINED__
typedef interface IOPCShutdown IOPCShutdown;
#endif 	/* __IOPCShutdown_FWD_DEFINED__ */


#ifndef __IOPCCommon_FWD_DEFINED__
#define __IOPCCommon_FWD_DEFINED__
typedef interface IOPCCommon IOPCCommon;
#endif 	/* __IOPCCommon_FWD_DEFINED__ */


#ifndef __IOPCServerList_FWD_DEFINED__
#define __IOPCServerList_FWD_DEFINED__
typedef interface IOPCServerList IOPCServerList;
#endif 	/* __IOPCServerList_FWD_DEFINED__ */


#ifndef __IOPCCommon_FWD_DEFINED__
#define __IOPCCommon_FWD_DEFINED__
typedef interface IOPCCommon IOPCCommon;
#endif 	/* __IOPCCommon_FWD_DEFINED__ */


#ifndef __IOPCShutdown_FWD_DEFINED__
#define __IOPCShutdown_FWD_DEFINED__
typedef interface IOPCShutdown IOPCShutdown;
#endif 	/* __IOPCShutdown_FWD_DEFINED__ */


#ifndef __IOPCServerList_FWD_DEFINED__
#define __IOPCServerList_FWD_DEFINED__
typedef interface IOPCServerList IOPCServerList;
#endif 	/* __IOPCServerList_FWD_DEFINED__ */


/* header files for imported files */
#include "unknwn.h"
#include "comcat.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __IOPCShutdown_INTERFACE_DEFINED__
#define __IOPCShutdown_INTERFACE_DEFINED__

/* interface IOPCShutdown */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IOPCShutdown;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F31DFDE1-07B6-11d2-B2D8-0060083BA1FB")
    IOPCShutdown : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE ShutdownRequest( 
            /* [string][in] */ LPCWSTR szReason) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOPCShutdownVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IOPCShutdown * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IOPCShutdown * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IOPCShutdown * This);
        
        HRESULT ( STDMETHODCALLTYPE *ShutdownRequest )( 
            IOPCShutdown * This,
            /* [string][in] */ LPCWSTR szReason);
        
        END_INTERFACE
    } IOPCShutdownVtbl;

    interface IOPCShutdown
    {
        CONST_VTBL struct IOPCShutdownVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOPCShutdown_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IOPCShutdown_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IOPCShutdown_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IOPCShutdown_ShutdownRequest(This,szReason)	\
    (This)->lpVtbl -> ShutdownRequest(This,szReason)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IOPCShutdown_ShutdownRequest_Proxy( 
    IOPCShutdown * This,
    /* [string][in] */ LPCWSTR szReason);


void __RPC_STUB IOPCShutdown_ShutdownRequest_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IOPCShutdown_INTERFACE_DEFINED__ */


#ifndef __IOPCCommon_INTERFACE_DEFINED__
#define __IOPCCommon_INTERFACE_DEFINED__

/* interface IOPCCommon */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IOPCCommon;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F31DFDE2-07B6-11d2-B2D8-0060083BA1FB")
    IOPCCommon : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetLocaleID( 
            /* [in] */ LCID dwLcid) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetLocaleID( 
            /* [out] */ LCID *pdwLcid) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QueryAvailableLocaleIDs( 
            /* [out] */ DWORD *pdwCount,
            /* [size_is][size_is][out] */ LCID **pdwLcid) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetErrorString( 
            /* [in] */ HRESULT dwError,
            /* [string][out] */ LPWSTR *ppString) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetClientName( 
            /* [string][in] */ LPCWSTR szName) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOPCCommonVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IOPCCommon * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IOPCCommon * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IOPCCommon * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetLocaleID )( 
            IOPCCommon * This,
            /* [in] */ LCID dwLcid);
        
        HRESULT ( STDMETHODCALLTYPE *GetLocaleID )( 
            IOPCCommon * This,
            /* [out] */ LCID *pdwLcid);
        
        HRESULT ( STDMETHODCALLTYPE *QueryAvailableLocaleIDs )( 
            IOPCCommon * This,
            /* [out] */ DWORD *pdwCount,
            /* [size_is][size_is][out] */ LCID **pdwLcid);
        
        HRESULT ( STDMETHODCALLTYPE *GetErrorString )( 
            IOPCCommon * This,
            /* [in] */ HRESULT dwError,
            /* [string][out] */ LPWSTR *ppString);
        
        HRESULT ( STDMETHODCALLTYPE *SetClientName )( 
            IOPCCommon * This,
            /* [string][in] */ LPCWSTR szName);
        
        END_INTERFACE
    } IOPCCommonVtbl;

    interface IOPCCommon
    {
        CONST_VTBL struct IOPCCommonVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOPCCommon_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IOPCCommon_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IOPCCommon_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IOPCCommon_SetLocaleID(This,dwLcid)	\
    (This)->lpVtbl -> SetLocaleID(This,dwLcid)

#define IOPCCommon_GetLocaleID(This,pdwLcid)	\
    (This)->lpVtbl -> GetLocaleID(This,pdwLcid)

#define IOPCCommon_QueryAvailableLocaleIDs(This,pdwCount,pdwLcid)	\
    (This)->lpVtbl -> QueryAvailableLocaleIDs(This,pdwCount,pdwLcid)

#define IOPCCommon_GetErrorString(This,dwError,ppString)	\
    (This)->lpVtbl -> GetErrorString(This,dwError,ppString)

#define IOPCCommon_SetClientName(This,szName)	\
    (This)->lpVtbl -> SetClientName(This,szName)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IOPCCommon_SetLocaleID_Proxy( 
    IOPCCommon * This,
    /* [in] */ LCID dwLcid);


void __RPC_STUB IOPCCommon_SetLocaleID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCCommon_GetLocaleID_Proxy( 
    IOPCCommon * This,
    /* [out] */ LCID *pdwLcid);


void __RPC_STUB IOPCCommon_GetLocaleID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCCommon_QueryAvailableLocaleIDs_Proxy( 
    IOPCCommon * This,
    /* [out] */ DWORD *pdwCount,
    /* [size_is][size_is][out] */ LCID **pdwLcid);


void __RPC_STUB IOPCCommon_QueryAvailableLocaleIDs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCCommon_GetErrorString_Proxy( 
    IOPCCommon * This,
    /* [in] */ HRESULT dwError,
    /* [string][out] */ LPWSTR *ppString);


void __RPC_STUB IOPCCommon_GetErrorString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCCommon_SetClientName_Proxy( 
    IOPCCommon * This,
    /* [string][in] */ LPCWSTR szName);


void __RPC_STUB IOPCCommon_SetClientName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IOPCCommon_INTERFACE_DEFINED__ */


#ifndef __IOPCServerList_INTERFACE_DEFINED__
#define __IOPCServerList_INTERFACE_DEFINED__

/* interface IOPCServerList */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IOPCServerList;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("13486D50-4821-11D2-A494-3CB306C10000")
    IOPCServerList : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE EnumClassesOfCategories( 
            /* [in] */ ULONG cImplemented,
            /* [size_is][in] */ CATID rgcatidImpl[  ],
            /* [in] */ ULONG cRequired,
            /* [size_is][in] */ CATID rgcatidReq[  ],
            /* [out] */ IEnumGUID **ppenumClsid) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetClassDetails( 
            /* [in] */ REFCLSID clsid,
            /* [out] */ LPOLESTR *ppszProgID,
            /* [out] */ LPOLESTR *ppszUserType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CLSIDFromProgID( 
            /* [in] */ LPCOLESTR szProgId,
            /* [out] */ LPCLSID clsid) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOPCServerListVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IOPCServerList * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IOPCServerList * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IOPCServerList * This);
        
        HRESULT ( STDMETHODCALLTYPE *EnumClassesOfCategories )( 
            IOPCServerList * This,
            /* [in] */ ULONG cImplemented,
            /* [size_is][in] */ CATID rgcatidImpl[  ],
            /* [in] */ ULONG cRequired,
            /* [size_is][in] */ CATID rgcatidReq[  ],
            /* [out] */ IEnumGUID **ppenumClsid);
        
        HRESULT ( STDMETHODCALLTYPE *GetClassDetails )( 
            IOPCServerList * This,
            /* [in] */ REFCLSID clsid,
            /* [out] */ LPOLESTR *ppszProgID,
            /* [out] */ LPOLESTR *ppszUserType);
        
        HRESULT ( STDMETHODCALLTYPE *CLSIDFromProgID )( 
            IOPCServerList * This,
            /* [in] */ LPCOLESTR szProgId,
            /* [out] */ LPCLSID clsid);
        
        END_INTERFACE
    } IOPCServerListVtbl;

    interface IOPCServerList
    {
        CONST_VTBL struct IOPCServerListVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOPCServerList_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IOPCServerList_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IOPCServerList_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IOPCServerList_EnumClassesOfCategories(This,cImplemented,rgcatidImpl,cRequired,rgcatidReq,ppenumClsid)	\
    (This)->lpVtbl -> EnumClassesOfCategories(This,cImplemented,rgcatidImpl,cRequired,rgcatidReq,ppenumClsid)

#define IOPCServerList_GetClassDetails(This,clsid,ppszProgID,ppszUserType)	\
    (This)->lpVtbl -> GetClassDetails(This,clsid,ppszProgID,ppszUserType)

#define IOPCServerList_CLSIDFromProgID(This,szProgId,clsid)	\
    (This)->lpVtbl -> CLSIDFromProgID(This,szProgId,clsid)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IOPCServerList_EnumClassesOfCategories_Proxy( 
    IOPCServerList * This,
    /* [in] */ ULONG cImplemented,
    /* [size_is][in] */ CATID rgcatidImpl[  ],
    /* [in] */ ULONG cRequired,
    /* [size_is][in] */ CATID rgcatidReq[  ],
    /* [out] */ IEnumGUID **ppenumClsid);


void __RPC_STUB IOPCServerList_EnumClassesOfCategories_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCServerList_GetClassDetails_Proxy( 
    IOPCServerList * This,
    /* [in] */ REFCLSID clsid,
    /* [out] */ LPOLESTR *ppszProgID,
    /* [out] */ LPOLESTR *ppszUserType);


void __RPC_STUB IOPCServerList_GetClassDetails_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCServerList_CLSIDFromProgID_Proxy( 
    IOPCServerList * This,
    /* [in] */ LPCOLESTR szProgId,
    /* [out] */ LPCLSID clsid);


void __RPC_STUB IOPCServerList_CLSIDFromProgID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IOPCServerList_INTERFACE_DEFINED__ */



#ifndef __OPCCOMN_LIBRARY_DEFINED__
#define __OPCCOMN_LIBRARY_DEFINED__

/* library OPCCOMN */
/* [helpstring][version][uuid] */ 





EXTERN_C const IID LIBID_OPCCOMN;
#endif /* __OPCCOMN_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


