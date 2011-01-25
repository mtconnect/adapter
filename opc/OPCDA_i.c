

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Tue Aug 03 16:03:22 2010
 */
/* Compiler settings for .\OPCDA.IDL:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


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

MIDL_DEFINE_GUID(IID, IID_IOPCServer,0x39c13a4d,0x011e,0x11d0,0x96,0x75,0x00,0x20,0xaf,0xd8,0xad,0xb3);


MIDL_DEFINE_GUID(IID, IID_IOPCServerPublicGroups,0x39c13a4e,0x011e,0x11d0,0x96,0x75,0x00,0x20,0xaf,0xd8,0xad,0xb3);


MIDL_DEFINE_GUID(IID, IID_IOPCBrowseServerAddressSpace,0x39c13a4f,0x011e,0x11d0,0x96,0x75,0x00,0x20,0xaf,0xd8,0xad,0xb3);


MIDL_DEFINE_GUID(IID, IID_IOPCGroupStateMgt,0x39c13a50,0x011e,0x11d0,0x96,0x75,0x00,0x20,0xaf,0xd8,0xad,0xb3);


MIDL_DEFINE_GUID(IID, IID_IOPCPublicGroupStateMgt,0x39c13a51,0x011e,0x11d0,0x96,0x75,0x00,0x20,0xaf,0xd8,0xad,0xb3);


MIDL_DEFINE_GUID(IID, IID_IOPCSyncIO,0x39c13a52,0x011e,0x11d0,0x96,0x75,0x00,0x20,0xaf,0xd8,0xad,0xb3);


MIDL_DEFINE_GUID(IID, IID_IOPCAsyncIO,0x39c13a53,0x011e,0x11d0,0x96,0x75,0x00,0x20,0xaf,0xd8,0xad,0xb3);


MIDL_DEFINE_GUID(IID, IID_IOPCItemMgt,0x39c13a54,0x011e,0x11d0,0x96,0x75,0x00,0x20,0xaf,0xd8,0xad,0xb3);


MIDL_DEFINE_GUID(IID, IID_IEnumOPCItemAttributes,0x39c13a55,0x011e,0x11d0,0x96,0x75,0x00,0x20,0xaf,0xd8,0xad,0xb3);


MIDL_DEFINE_GUID(IID, IID_IOPCDataCallback,0x39c13a70,0x011e,0x11d0,0x96,0x75,0x00,0x20,0xaf,0xd8,0xad,0xb3);


MIDL_DEFINE_GUID(IID, IID_IOPCAsyncIO2,0x39c13a71,0x011e,0x11d0,0x96,0x75,0x00,0x20,0xaf,0xd8,0xad,0xb3);


MIDL_DEFINE_GUID(IID, IID_IOPCItemProperties,0x39c13a72,0x011e,0x11d0,0x96,0x75,0x00,0x20,0xaf,0xd8,0xad,0xb3);


MIDL_DEFINE_GUID(IID, LIBID_OPCDA,0xB28EEDB2,0xAC6F,0x11d1,0x84,0xD5,0x00,0x60,0x8C,0xB8,0xA7,0xE9);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



