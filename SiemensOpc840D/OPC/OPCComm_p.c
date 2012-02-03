
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the proxy stub code */


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

#if !defined(_M_IA64) && !defined(_M_AMD64)
#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 440
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif // __RPCPROXY_H_VERSION__


#include "OPCComm.h"

#define TYPE_FORMAT_STRING_SIZE   113                               
#define PROC_FORMAT_STRING_SIZE   307                               
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } MIDL_TYPE_FORMAT_STRING;

typedef struct _MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } MIDL_PROC_FORMAT_STRING;


static RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString;
extern const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IOPCShutdown_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IOPCShutdown_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IOPCCommon_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IOPCCommon_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IOPCServerList_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IOPCServerList_ProxyInfo;



#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT40_OR_LATER)
#error You need a Windows NT 4.0 or later to run this stub because it uses these features:
#error   -Oif or -Oicf.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will die there with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure ShutdownRequest */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x3 ),	/* 3 */
/*  8 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 10 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12 */	NdrFcShort( 0x8 ),	/* 8 */
/* 14 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter szReason */

/* 16 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 18 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 20 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 22 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 24 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 26 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetLocaleID */

/* 28 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 30 */	NdrFcLong( 0x0 ),	/* 0 */
/* 34 */	NdrFcShort( 0x3 ),	/* 3 */
/* 36 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 38 */	NdrFcShort( 0x8 ),	/* 8 */
/* 40 */	NdrFcShort( 0x8 ),	/* 8 */
/* 42 */	0x4,		/* Oi2 Flags:  has return, */
			0x2,		/* 2 */

	/* Parameter dwLcid */

/* 44 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 46 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 48 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 50 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 52 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 54 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetLocaleID */

/* 56 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 58 */	NdrFcLong( 0x0 ),	/* 0 */
/* 62 */	NdrFcShort( 0x4 ),	/* 4 */
/* 64 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 66 */	NdrFcShort( 0x0 ),	/* 0 */
/* 68 */	NdrFcShort( 0x24 ),	/* 36 */
/* 70 */	0x4,		/* Oi2 Flags:  has return, */
			0x2,		/* 2 */

	/* Parameter pdwLcid */

/* 72 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 74 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 76 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 78 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 80 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 82 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure QueryAvailableLocaleIDs */

/* 84 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 86 */	NdrFcLong( 0x0 ),	/* 0 */
/* 90 */	NdrFcShort( 0x5 ),	/* 5 */
/* 92 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 94 */	NdrFcShort( 0x0 ),	/* 0 */
/* 96 */	NdrFcShort( 0x24 ),	/* 36 */
/* 98 */	0x5,		/* Oi2 Flags:  srv must size, has return, */
			0x3,		/* 3 */

	/* Parameter pdwCount */

/* 100 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 102 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 104 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pdwLcid */

/* 106 */	NdrFcShort( 0x2013 ),	/* Flags:  must size, must free, out, srv alloc size=8 */
/* 108 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 110 */	NdrFcShort( 0xa ),	/* Type Offset=10 */

	/* Return value */

/* 112 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 114 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 116 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetErrorString */

/* 118 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 120 */	NdrFcLong( 0x0 ),	/* 0 */
/* 124 */	NdrFcShort( 0x6 ),	/* 6 */
/* 126 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 128 */	NdrFcShort( 0x8 ),	/* 8 */
/* 130 */	NdrFcShort( 0x8 ),	/* 8 */
/* 132 */	0x5,		/* Oi2 Flags:  srv must size, has return, */
			0x3,		/* 3 */

	/* Parameter dwError */

/* 134 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 136 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 138 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter ppString */

/* 140 */	NdrFcShort( 0x2013 ),	/* Flags:  must size, must free, out, srv alloc size=8 */
/* 142 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 144 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 146 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 148 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 150 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetClientName */

/* 152 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 154 */	NdrFcLong( 0x0 ),	/* 0 */
/* 158 */	NdrFcShort( 0x7 ),	/* 7 */
/* 160 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 162 */	NdrFcShort( 0x0 ),	/* 0 */
/* 164 */	NdrFcShort( 0x8 ),	/* 8 */
/* 166 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter szName */

/* 168 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 170 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 172 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 174 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 176 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 178 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure EnumClassesOfCategories */

/* 180 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 182 */	NdrFcLong( 0x0 ),	/* 0 */
/* 186 */	NdrFcShort( 0x3 ),	/* 3 */
/* 188 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 190 */	NdrFcShort( 0x10 ),	/* 16 */
/* 192 */	NdrFcShort( 0x8 ),	/* 8 */
/* 194 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x6,		/* 6 */

	/* Parameter cImplemented */

/* 196 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 198 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 200 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter rgcatidImpl */

/* 202 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 204 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 206 */	NdrFcShort( 0x36 ),	/* Type Offset=54 */

	/* Parameter cRequired */

/* 208 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 210 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 212 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter rgcatidReq */

/* 214 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 216 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 218 */	NdrFcShort( 0x44 ),	/* Type Offset=68 */

	/* Parameter ppenumClsid */

/* 220 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 222 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 224 */	NdrFcShort( 0x52 ),	/* Type Offset=82 */

	/* Return value */

/* 226 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 228 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 230 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetClassDetails */

/* 232 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 234 */	NdrFcLong( 0x0 ),	/* 0 */
/* 238 */	NdrFcShort( 0x4 ),	/* 4 */
/* 240 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 242 */	NdrFcShort( 0x44 ),	/* 68 */
/* 244 */	NdrFcShort( 0x8 ),	/* 8 */
/* 246 */	0x5,		/* Oi2 Flags:  srv must size, has return, */
			0x4,		/* 4 */

	/* Parameter clsid */

/* 248 */	NdrFcShort( 0x10a ),	/* Flags:  must free, in, simple ref, */
/* 250 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 252 */	NdrFcShort( 0x2a ),	/* Type Offset=42 */

	/* Parameter ppszProgID */

/* 254 */	NdrFcShort( 0x2013 ),	/* Flags:  must size, must free, out, srv alloc size=8 */
/* 256 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 258 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter ppszUserType */

/* 260 */	NdrFcShort( 0x2013 ),	/* Flags:  must size, must free, out, srv alloc size=8 */
/* 262 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 264 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 266 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 268 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 270 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CLSIDFromProgID */

/* 272 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 274 */	NdrFcLong( 0x0 ),	/* 0 */
/* 278 */	NdrFcShort( 0x5 ),	/* 5 */
/* 280 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 282 */	NdrFcShort( 0x0 ),	/* 0 */
/* 284 */	NdrFcShort( 0x4c ),	/* 76 */
/* 286 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter szProgId */

/* 288 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 290 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 292 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter clsid */

/* 294 */	NdrFcShort( 0x4112 ),	/* Flags:  must free, out, simple ref, srv alloc size=16 */
/* 296 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 298 */	NdrFcShort( 0x2a ),	/* Type Offset=42 */

	/* Return value */

/* 300 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 302 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 304 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/*  4 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/*  6 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/*  8 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 10 */	
			0x11, 0x14,	/* FC_RP [alloced_on_stack] [pointer_deref] */
/* 12 */	NdrFcShort( 0x2 ),	/* Offset= 2 (14) */
/* 14 */	
			0x13, 0x0,	/* FC_OP */
/* 16 */	NdrFcShort( 0x2 ),	/* Offset= 2 (18) */
/* 18 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 20 */	NdrFcShort( 0x4 ),	/* 4 */
/* 22 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x54,		/* FC_DEREFERENCE */
/* 24 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 26 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 28 */	
			0x11, 0x14,	/* FC_RP [alloced_on_stack] [pointer_deref] */
/* 30 */	NdrFcShort( 0x2 ),	/* Offset= 2 (32) */
/* 32 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 34 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 36 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 38 */	NdrFcShort( 0x8 ),	/* 8 */
/* 40 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 42 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 44 */	NdrFcShort( 0x10 ),	/* 16 */
/* 46 */	0x8,		/* FC_LONG */
			0x6,		/* FC_SHORT */
/* 48 */	0x6,		/* FC_SHORT */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 50 */	0x0,		/* 0 */
			NdrFcShort( 0xfffffff1 ),	/* Offset= -15 (36) */
			0x5b,		/* FC_END */
/* 54 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 56 */	NdrFcShort( 0x10 ),	/* 16 */
/* 58 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 60 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 62 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 64 */	NdrFcShort( 0xffffffea ),	/* Offset= -22 (42) */
/* 66 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 68 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 70 */	NdrFcShort( 0x10 ),	/* 16 */
/* 72 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 74 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 76 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 78 */	NdrFcShort( 0xffffffdc ),	/* Offset= -36 (42) */
/* 80 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 82 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 84 */	NdrFcShort( 0x2 ),	/* Offset= 2 (86) */
/* 86 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 88 */	NdrFcLong( 0x2e000 ),	/* 188416 */
/* 92 */	NdrFcShort( 0x0 ),	/* 0 */
/* 94 */	NdrFcShort( 0x0 ),	/* 0 */
/* 96 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 98 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 100 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 102 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 104 */	
			0x11, 0x0,	/* FC_RP */
/* 106 */	NdrFcShort( 0xffffffc0 ),	/* Offset= -64 (42) */
/* 108 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 110 */	NdrFcShort( 0xffffffbc ),	/* Offset= -68 (42) */

			0x0
        }
    };


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IOPCShutdown, ver. 0.0,
   GUID={0xF31DFDE1,0x07B6,0x11d2,{0xB2,0xD8,0x00,0x60,0x08,0x3B,0xA1,0xFB}} */

#pragma code_seg(".orpc")
static const unsigned short IOPCShutdown_FormatStringOffsetTable[] =
    {
    0
    };

static const MIDL_STUBLESS_PROXY_INFO IOPCShutdown_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IOPCShutdown_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IOPCShutdown_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IOPCShutdown_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(4) _IOPCShutdownProxyVtbl = 
{
    &IOPCShutdown_ProxyInfo,
    &IID_IOPCShutdown,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* IOPCShutdown::ShutdownRequest */
};

const CInterfaceStubVtbl _IOPCShutdownStubVtbl =
{
    &IID_IOPCShutdown,
    &IOPCShutdown_ServerInfo,
    4,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: IOPCCommon, ver. 0.0,
   GUID={0xF31DFDE2,0x07B6,0x11d2,{0xB2,0xD8,0x00,0x60,0x08,0x3B,0xA1,0xFB}} */

#pragma code_seg(".orpc")
static const unsigned short IOPCCommon_FormatStringOffsetTable[] =
    {
    28,
    56,
    84,
    118,
    152
    };

static const MIDL_STUBLESS_PROXY_INFO IOPCCommon_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IOPCCommon_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IOPCCommon_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IOPCCommon_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(8) _IOPCCommonProxyVtbl = 
{
    &IOPCCommon_ProxyInfo,
    &IID_IOPCCommon,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* IOPCCommon::SetLocaleID */ ,
    (void *) (INT_PTR) -1 /* IOPCCommon::GetLocaleID */ ,
    (void *) (INT_PTR) -1 /* IOPCCommon::QueryAvailableLocaleIDs */ ,
    (void *) (INT_PTR) -1 /* IOPCCommon::GetErrorString */ ,
    (void *) (INT_PTR) -1 /* IOPCCommon::SetClientName */
};

const CInterfaceStubVtbl _IOPCCommonStubVtbl =
{
    &IID_IOPCCommon,
    &IOPCCommon_ServerInfo,
    8,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: IOPCServerList, ver. 0.0,
   GUID={0x13486D50,0x4821,0x11D2,{0xA4,0x94,0x3C,0xB3,0x06,0xC1,0x00,0x00}} */

#pragma code_seg(".orpc")
static const unsigned short IOPCServerList_FormatStringOffsetTable[] =
    {
    180,
    232,
    272
    };

static const MIDL_STUBLESS_PROXY_INFO IOPCServerList_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IOPCServerList_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IOPCServerList_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IOPCServerList_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(6) _IOPCServerListProxyVtbl = 
{
    &IOPCServerList_ProxyInfo,
    &IID_IOPCServerList,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* IOPCServerList::EnumClassesOfCategories */ ,
    (void *) (INT_PTR) -1 /* IOPCServerList::GetClassDetails */ ,
    (void *) (INT_PTR) -1 /* IOPCServerList::CLSIDFromProgID */
};

const CInterfaceStubVtbl _IOPCServerListStubVtbl =
{
    &IID_IOPCServerList,
    &IOPCServerList_ServerInfo,
    6,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    __MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x20000, /* Ndr library version */
    0,
    0x600015b, /* MIDL Version 6.0.347 */
    0,
    0,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0   /* Reserved5 */
    };

const CInterfaceProxyVtbl * _OPCCOMN_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IOPCServerListProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IOPCShutdownProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IOPCCommonProxyVtbl,
    0
};

const CInterfaceStubVtbl * _OPCCOMN_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IOPCServerListStubVtbl,
    ( CInterfaceStubVtbl *) &_IOPCShutdownStubVtbl,
    ( CInterfaceStubVtbl *) &_IOPCCommonStubVtbl,
    0
};

PCInterfaceName const _OPCCOMN_InterfaceNamesList[] = 
{
    "IOPCServerList",
    "IOPCShutdown",
    "IOPCCommon",
    0
};


#define _OPCCOMN_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _OPCCOMN, pIID, n)

int __stdcall _OPCCOMN_IID_Lookup( const IID * pIID, int * pIndex )
{
    IID_BS_LOOKUP_SETUP

    IID_BS_LOOKUP_INITIAL_TEST( _OPCCOMN, 3, 2 )
    IID_BS_LOOKUP_NEXT_TEST( _OPCCOMN, 1 )
    IID_BS_LOOKUP_RETURN_RESULT( _OPCCOMN, 3, *pIndex )
    
}

const ExtendedProxyFileInfo OPCCOMN_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _OPCCOMN_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _OPCCOMN_StubVtblList,
    (const PCInterfaceName * ) & _OPCCOMN_InterfaceNamesList,
    0, // no delegation
    & _OPCCOMN_IID_Lookup, 
    3,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};


#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/

