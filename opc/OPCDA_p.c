

/* this ALWAYS GENERATED file contains the proxy stub code */


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

#if !defined(_M_IA64) && !defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */
#pragma warning( disable: 4211 )  /* redefine extent to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma optimize("", off ) 

#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif // __RPCPROXY_H_VERSION__


#include "OPCDA_h.h"

#define TYPE_FORMAT_STRING_SIZE   1945                              
#define PROC_FORMAT_STRING_SIZE   2491                              
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   1            

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


extern const MIDL_SERVER_INFO IOPCServer_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IOPCServer_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IOPCServerPublicGroups_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IOPCServerPublicGroups_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IOPCBrowseServerAddressSpace_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IOPCBrowseServerAddressSpace_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IOPCGroupStateMgt_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IOPCGroupStateMgt_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IOPCPublicGroupStateMgt_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IOPCPublicGroupStateMgt_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IOPCSyncIO_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IOPCSyncIO_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IOPCAsyncIO_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IOPCAsyncIO_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IOPCItemMgt_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IOPCItemMgt_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IEnumOPCItemAttributes_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IEnumOPCItemAttributes_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IOPCDataCallback_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IOPCDataCallback_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IOPCAsyncIO2_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IOPCAsyncIO2_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IOPCItemProperties_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IOPCItemProperties_ProxyInfo;


extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ];

#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
#error You need a Windows 2000 or later to run this stub because it uses these features:
#error   /robust command line switch.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will die there with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure AddGroup */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x3 ),	/* 3 */
/*  8 */	NdrFcShort( 0x34 ),	/* x86 Stack size/offset = 52 */
/* 10 */	NdrFcShort( 0x9c ),	/* 156 */
/* 12 */	NdrFcShort( 0x40 ),	/* 64 */
/* 14 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0xc,		/* 12 */
/* 16 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 18 */	NdrFcShort( 0x1 ),	/* 1 */
/* 20 */	NdrFcShort( 0x0 ),	/* 0 */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter szName */

/* 24 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 26 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 28 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter bActive */

/* 30 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 32 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 34 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter dwRequestedUpdateRate */

/* 36 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 38 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 40 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter hClientGroup */

/* 42 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 44 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 46 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pTimeBias */

/* 48 */	NdrFcShort( 0xa ),	/* Flags:  must free, in, */
/* 50 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 52 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Parameter pPercentDeadband */

/* 54 */	NdrFcShort( 0xa ),	/* Flags:  must free, in, */
/* 56 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 58 */	NdrFcShort( 0xa ),	/* Type Offset=10 */

	/* Parameter dwLCID */

/* 60 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 62 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 64 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter phServerGroup */

/* 66 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 68 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 70 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pRevisedUpdateRate */

/* 72 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 74 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 76 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter riid */

/* 78 */	NdrFcShort( 0x10a ),	/* Flags:  must free, in, simple ref, */
/* 80 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
/* 82 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter ppUnk */

/* 84 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 86 */	NdrFcShort( 0x2c ),	/* x86 Stack size/offset = 44 */
/* 88 */	NdrFcShort( 0x28 ),	/* Type Offset=40 */

	/* Return value */

/* 90 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 92 */	NdrFcShort( 0x30 ),	/* x86 Stack size/offset = 48 */
/* 94 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetErrorString */

/* 96 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 98 */	NdrFcLong( 0x0 ),	/* 0 */
/* 102 */	NdrFcShort( 0x4 ),	/* 4 */
/* 104 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 106 */	NdrFcShort( 0x10 ),	/* 16 */
/* 108 */	NdrFcShort( 0x8 ),	/* 8 */
/* 110 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x4,		/* 4 */
/* 112 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 114 */	NdrFcShort( 0x0 ),	/* 0 */
/* 116 */	NdrFcShort( 0x0 ),	/* 0 */
/* 118 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dwError */

/* 120 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 122 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 124 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter dwLocale */

/* 126 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 128 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 130 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter ppString */

/* 132 */	NdrFcShort( 0x2013 ),	/* Flags:  must size, must free, out, srv alloc size=8 */
/* 134 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 136 */	NdrFcShort( 0x34 ),	/* Type Offset=52 */

	/* Return value */

/* 138 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 140 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 142 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetGroupByName */

/* 144 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 146 */	NdrFcLong( 0x0 ),	/* 0 */
/* 150 */	NdrFcShort( 0x5 ),	/* 5 */
/* 152 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 154 */	NdrFcShort( 0x44 ),	/* 68 */
/* 156 */	NdrFcShort( 0x8 ),	/* 8 */
/* 158 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 160 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 162 */	NdrFcShort( 0x1 ),	/* 1 */
/* 164 */	NdrFcShort( 0x0 ),	/* 0 */
/* 166 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter szName */

/* 168 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 170 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 172 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter riid */

/* 174 */	NdrFcShort( 0x10a ),	/* Flags:  must free, in, simple ref, */
/* 176 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 178 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter ppUnk */

/* 180 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 182 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 184 */	NdrFcShort( 0x3c ),	/* Type Offset=60 */

	/* Return value */

/* 186 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 188 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 190 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetStatus */

/* 192 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 194 */	NdrFcLong( 0x0 ),	/* 0 */
/* 198 */	NdrFcShort( 0x6 ),	/* 6 */
/* 200 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 202 */	NdrFcShort( 0x0 ),	/* 0 */
/* 204 */	NdrFcShort( 0x8 ),	/* 8 */
/* 206 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 208 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 210 */	NdrFcShort( 0x0 ),	/* 0 */
/* 212 */	NdrFcShort( 0x0 ),	/* 0 */
/* 214 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ppServerStatus */

/* 216 */	NdrFcShort( 0x2013 ),	/* Flags:  must size, must free, out, srv alloc size=8 */
/* 218 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 220 */	NdrFcShort( 0x48 ),	/* Type Offset=72 */

	/* Return value */

/* 222 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 224 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 226 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RemoveGroup */

/* 228 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 230 */	NdrFcLong( 0x0 ),	/* 0 */
/* 234 */	NdrFcShort( 0x7 ),	/* 7 */
/* 236 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 238 */	NdrFcShort( 0x10 ),	/* 16 */
/* 240 */	NdrFcShort( 0x8 ),	/* 8 */
/* 242 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 244 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 246 */	NdrFcShort( 0x0 ),	/* 0 */
/* 248 */	NdrFcShort( 0x0 ),	/* 0 */
/* 250 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter hServerGroup */

/* 252 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 254 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 256 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter bForce */

/* 258 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 260 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 262 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 264 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 266 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 268 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateGroupEnumerator */

/* 270 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 272 */	NdrFcLong( 0x0 ),	/* 0 */
/* 276 */	NdrFcShort( 0x8 ),	/* 8 */
/* 278 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 280 */	NdrFcShort( 0x4a ),	/* 74 */
/* 282 */	NdrFcShort( 0x8 ),	/* 8 */
/* 284 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x4,		/* 4 */
/* 286 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 288 */	NdrFcShort( 0x1 ),	/* 1 */
/* 290 */	NdrFcShort( 0x0 ),	/* 0 */
/* 292 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dwScope */

/* 294 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 296 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 298 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Parameter riid */

/* 300 */	NdrFcShort( 0x10a ),	/* Flags:  must free, in, simple ref, */
/* 302 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 304 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter ppUnk */

/* 306 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 308 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 310 */	NdrFcShort( 0x7a ),	/* Type Offset=122 */

	/* Return value */

/* 312 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 314 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 316 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetPublicGroupByName */

/* 318 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 320 */	NdrFcLong( 0x0 ),	/* 0 */
/* 324 */	NdrFcShort( 0x3 ),	/* 3 */
/* 326 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 328 */	NdrFcShort( 0x44 ),	/* 68 */
/* 330 */	NdrFcShort( 0x8 ),	/* 8 */
/* 332 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 334 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 336 */	NdrFcShort( 0x1 ),	/* 1 */
/* 338 */	NdrFcShort( 0x0 ),	/* 0 */
/* 340 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter szName */

/* 342 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 344 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 346 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter riid */

/* 348 */	NdrFcShort( 0x10a ),	/* Flags:  must free, in, simple ref, */
/* 350 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 352 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter ppUnk */

/* 354 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 356 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 358 */	NdrFcShort( 0x86 ),	/* Type Offset=134 */

	/* Return value */

/* 360 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 362 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 364 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RemovePublicGroup */

/* 366 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 368 */	NdrFcLong( 0x0 ),	/* 0 */
/* 372 */	NdrFcShort( 0x4 ),	/* 4 */
/* 374 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 376 */	NdrFcShort( 0x10 ),	/* 16 */
/* 378 */	NdrFcShort( 0x8 ),	/* 8 */
/* 380 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 382 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 384 */	NdrFcShort( 0x0 ),	/* 0 */
/* 386 */	NdrFcShort( 0x0 ),	/* 0 */
/* 388 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter hServerGroup */

/* 390 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 392 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 394 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter bForce */

/* 396 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 398 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 400 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 402 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 404 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 406 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure QueryOrganization */

/* 408 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 410 */	NdrFcLong( 0x0 ),	/* 0 */
/* 414 */	NdrFcShort( 0x3 ),	/* 3 */
/* 416 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 418 */	NdrFcShort( 0x0 ),	/* 0 */
/* 420 */	NdrFcShort( 0x22 ),	/* 34 */
/* 422 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 424 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 426 */	NdrFcShort( 0x0 ),	/* 0 */
/* 428 */	NdrFcShort( 0x0 ),	/* 0 */
/* 430 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pNameSpaceType */

/* 432 */	NdrFcShort( 0x2010 ),	/* Flags:  out, srv alloc size=8 */
/* 434 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 436 */	NdrFcShort( 0x92 ),	/* Type Offset=146 */

	/* Return value */

/* 438 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 440 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 442 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ChangeBrowsePosition */

/* 444 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 446 */	NdrFcLong( 0x0 ),	/* 0 */
/* 450 */	NdrFcShort( 0x4 ),	/* 4 */
/* 452 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 454 */	NdrFcShort( 0x6 ),	/* 6 */
/* 456 */	NdrFcShort( 0x8 ),	/* 8 */
/* 458 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 460 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 462 */	NdrFcShort( 0x0 ),	/* 0 */
/* 464 */	NdrFcShort( 0x0 ),	/* 0 */
/* 466 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dwBrowseDirection */

/* 468 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 470 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 472 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Parameter szString */

/* 474 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 476 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 478 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 480 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 482 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 484 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure BrowseOPCItemIDs */

/* 486 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 488 */	NdrFcLong( 0x0 ),	/* 0 */
/* 492 */	NdrFcShort( 0x5 ),	/* 5 */
/* 494 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 496 */	NdrFcShort( 0x14 ),	/* 20 */
/* 498 */	NdrFcShort( 0x8 ),	/* 8 */
/* 500 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 502 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 504 */	NdrFcShort( 0x0 ),	/* 0 */
/* 506 */	NdrFcShort( 0x0 ),	/* 0 */
/* 508 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dwBrowseFilterType */

/* 510 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 512 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 514 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Parameter szFilterCriteria */

/* 516 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 518 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 520 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter vtDataTypeFilter */

/* 522 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 524 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 526 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter dwAccessRightsFilter */

/* 528 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 530 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 532 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter ppIEnumString */

/* 534 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 536 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 538 */	NdrFcShort( 0x96 ),	/* Type Offset=150 */

	/* Return value */

/* 540 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 542 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 544 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetItemID */

/* 546 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 548 */	NdrFcLong( 0x0 ),	/* 0 */
/* 552 */	NdrFcShort( 0x6 ),	/* 6 */
/* 554 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 556 */	NdrFcShort( 0x0 ),	/* 0 */
/* 558 */	NdrFcShort( 0x8 ),	/* 8 */
/* 560 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 562 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 564 */	NdrFcShort( 0x0 ),	/* 0 */
/* 566 */	NdrFcShort( 0x0 ),	/* 0 */
/* 568 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter szItemDataID */

/* 570 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 572 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 574 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter szItemID */

/* 576 */	NdrFcShort( 0x2013 ),	/* Flags:  must size, must free, out, srv alloc size=8 */
/* 578 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 580 */	NdrFcShort( 0x34 ),	/* Type Offset=52 */

	/* Return value */

/* 582 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 584 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 586 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure BrowseAccessPaths */

/* 588 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 590 */	NdrFcLong( 0x0 ),	/* 0 */
/* 594 */	NdrFcShort( 0x7 ),	/* 7 */
/* 596 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 598 */	NdrFcShort( 0x0 ),	/* 0 */
/* 600 */	NdrFcShort( 0x8 ),	/* 8 */
/* 602 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 604 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 606 */	NdrFcShort( 0x0 ),	/* 0 */
/* 608 */	NdrFcShort( 0x0 ),	/* 0 */
/* 610 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter szItemID */

/* 612 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 614 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 616 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter ppIEnumString */

/* 618 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 620 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 622 */	NdrFcShort( 0x96 ),	/* Type Offset=150 */

	/* Return value */

/* 624 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 626 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 628 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetState */

/* 630 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 632 */	NdrFcLong( 0x0 ),	/* 0 */
/* 636 */	NdrFcShort( 0x3 ),	/* 3 */
/* 638 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
/* 640 */	NdrFcShort( 0x0 ),	/* 0 */
/* 642 */	NdrFcShort( 0xcc ),	/* 204 */
/* 644 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x9,		/* 9 */
/* 646 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 648 */	NdrFcShort( 0x0 ),	/* 0 */
/* 650 */	NdrFcShort( 0x0 ),	/* 0 */
/* 652 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pUpdateRate */

/* 654 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 656 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 658 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pActive */

/* 660 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 662 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 664 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter ppName */

/* 666 */	NdrFcShort( 0x2013 ),	/* Flags:  must size, must free, out, srv alloc size=8 */
/* 668 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 670 */	NdrFcShort( 0x34 ),	/* Type Offset=52 */

	/* Parameter pTimeBias */

/* 672 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 674 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 676 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pPercentDeadband */

/* 678 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 680 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 682 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter pLCID */

/* 684 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 686 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 688 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter phClientGroup */

/* 690 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 692 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 694 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter phServerGroup */

/* 696 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 698 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 700 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 702 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 704 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 706 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetState */

/* 708 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 710 */	NdrFcLong( 0x0 ),	/* 0 */
/* 714 */	NdrFcShort( 0x4 ),	/* 4 */
/* 716 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 718 */	NdrFcShort( 0xa8 ),	/* 168 */
/* 720 */	NdrFcShort( 0x24 ),	/* 36 */
/* 722 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x8,		/* 8 */
/* 724 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 726 */	NdrFcShort( 0x0 ),	/* 0 */
/* 728 */	NdrFcShort( 0x0 ),	/* 0 */
/* 730 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pRequestedUpdateRate */

/* 732 */	NdrFcShort( 0xa ),	/* Flags:  must free, in, */
/* 734 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 736 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Parameter pRevisedUpdateRate */

/* 738 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 740 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 742 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pActive */

/* 744 */	NdrFcShort( 0xa ),	/* Flags:  must free, in, */
/* 746 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 748 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Parameter pTimeBias */

/* 750 */	NdrFcShort( 0xa ),	/* Flags:  must free, in, */
/* 752 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 754 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Parameter pPercentDeadband */

/* 756 */	NdrFcShort( 0xa ),	/* Flags:  must free, in, */
/* 758 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 760 */	NdrFcShort( 0xa ),	/* Type Offset=10 */

	/* Parameter pLCID */

/* 762 */	NdrFcShort( 0xa ),	/* Flags:  must free, in, */
/* 764 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 766 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Parameter phClientGroup */

/* 768 */	NdrFcShort( 0xa ),	/* Flags:  must free, in, */
/* 770 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 772 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Return value */

/* 774 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 776 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 778 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetName */

/* 780 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 782 */	NdrFcLong( 0x0 ),	/* 0 */
/* 786 */	NdrFcShort( 0x5 ),	/* 5 */
/* 788 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 790 */	NdrFcShort( 0x0 ),	/* 0 */
/* 792 */	NdrFcShort( 0x8 ),	/* 8 */
/* 794 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 796 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 798 */	NdrFcShort( 0x0 ),	/* 0 */
/* 800 */	NdrFcShort( 0x0 ),	/* 0 */
/* 802 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter szName */

/* 804 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 806 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 808 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 810 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 812 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 814 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CloneGroup */

/* 816 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 818 */	NdrFcLong( 0x0 ),	/* 0 */
/* 822 */	NdrFcShort( 0x6 ),	/* 6 */
/* 824 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 826 */	NdrFcShort( 0x44 ),	/* 68 */
/* 828 */	NdrFcShort( 0x8 ),	/* 8 */
/* 830 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 832 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 834 */	NdrFcShort( 0x1 ),	/* 1 */
/* 836 */	NdrFcShort( 0x0 ),	/* 0 */
/* 838 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter szName */

/* 840 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 842 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 844 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter riid */

/* 846 */	NdrFcShort( 0x10a ),	/* Flags:  must free, in, simple ref, */
/* 848 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 850 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter ppUnk */

/* 852 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 854 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 856 */	NdrFcShort( 0xb0 ),	/* Type Offset=176 */

	/* Return value */

/* 858 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 860 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 862 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetState */

/* 864 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 866 */	NdrFcLong( 0x0 ),	/* 0 */
/* 870 */	NdrFcShort( 0x3 ),	/* 3 */
/* 872 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 874 */	NdrFcShort( 0x0 ),	/* 0 */
/* 876 */	NdrFcShort( 0x24 ),	/* 36 */
/* 878 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 880 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 882 */	NdrFcShort( 0x0 ),	/* 0 */
/* 884 */	NdrFcShort( 0x0 ),	/* 0 */
/* 886 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pPublic */

/* 888 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 890 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 892 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 894 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 896 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 898 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure MoveToPublic */

/* 900 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 902 */	NdrFcLong( 0x0 ),	/* 0 */
/* 906 */	NdrFcShort( 0x4 ),	/* 4 */
/* 908 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 910 */	NdrFcShort( 0x0 ),	/* 0 */
/* 912 */	NdrFcShort( 0x8 ),	/* 8 */
/* 914 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 916 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 918 */	NdrFcShort( 0x0 ),	/* 0 */
/* 920 */	NdrFcShort( 0x0 ),	/* 0 */
/* 922 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 924 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 926 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 928 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Read */

/* 930 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 932 */	NdrFcLong( 0x0 ),	/* 0 */
/* 936 */	NdrFcShort( 0x3 ),	/* 3 */
/* 938 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 940 */	NdrFcShort( 0xe ),	/* 14 */
/* 942 */	NdrFcShort( 0x8 ),	/* 8 */
/* 944 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 946 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 948 */	NdrFcShort( 0x22 ),	/* 34 */
/* 950 */	NdrFcShort( 0x1 ),	/* 1 */
/* 952 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dwSource */

/* 954 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 956 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 958 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Parameter dwCount */

/* 960 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 962 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 964 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter phServer */

/* 966 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 968 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 970 */	NdrFcShort( 0xc0 ),	/* Type Offset=192 */

	/* Parameter ppItemValues */

/* 972 */	NdrFcShort( 0x2013 ),	/* Flags:  must size, must free, out, srv alloc size=8 */
/* 974 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 976 */	NdrFcShort( 0xcc ),	/* Type Offset=204 */

	/* Parameter ppErrors */

/* 978 */	NdrFcShort( 0x2013 ),	/* Flags:  must size, must free, out, srv alloc size=8 */
/* 980 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 982 */	NdrFcShort( 0x4ec ),	/* Type Offset=1260 */

	/* Return value */

/* 984 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 986 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 988 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Write */

/* 990 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 992 */	NdrFcLong( 0x0 ),	/* 0 */
/* 996 */	NdrFcShort( 0x4 ),	/* 4 */
/* 998 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1000 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1002 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1004 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 1006 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 1008 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1010 */	NdrFcShort( 0x22 ),	/* 34 */
/* 1012 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dwCount */

/* 1014 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1016 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1018 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter phServer */

/* 1020 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1022 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1024 */	NdrFcShort( 0x4f8 ),	/* Type Offset=1272 */

	/* Parameter pItemValues */

/* 1026 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1028 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1030 */	NdrFcShort( 0x516 ),	/* Type Offset=1302 */

	/* Parameter ppErrors */

/* 1032 */	NdrFcShort( 0x2013 ),	/* Flags:  must size, must free, out, srv alloc size=8 */
/* 1034 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1036 */	NdrFcShort( 0x52c ),	/* Type Offset=1324 */

	/* Return value */

/* 1038 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1040 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1042 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Read */

/* 1044 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1046 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1050 */	NdrFcShort( 0x3 ),	/* 3 */
/* 1052 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 1054 */	NdrFcShort( 0x16 ),	/* 22 */
/* 1056 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1058 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x7,		/* 7 */
/* 1060 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 1062 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1064 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1066 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dwConnection */

/* 1068 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1070 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1072 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter dwSource */

/* 1074 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1076 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1078 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Parameter dwCount */

/* 1080 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1082 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1084 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter phServer */

/* 1086 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1088 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1090 */	NdrFcShort( 0x538 ),	/* Type Offset=1336 */

	/* Parameter pTransactionID */

/* 1092 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1094 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1096 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter ppErrors */

/* 1098 */	NdrFcShort( 0x2013 ),	/* Flags:  must size, must free, out, srv alloc size=8 */
/* 1100 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1102 */	NdrFcShort( 0x544 ),	/* Type Offset=1348 */

	/* Return value */

/* 1104 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1106 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 1108 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Write */

/* 1110 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1112 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1116 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1118 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 1120 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1122 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1124 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x7,		/* 7 */
/* 1126 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 1128 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1130 */	NdrFcShort( 0x22 ),	/* 34 */
/* 1132 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dwConnection */

/* 1134 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1136 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1138 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter dwCount */

/* 1140 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1142 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1144 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter phServer */

/* 1146 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1148 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1150 */	NdrFcShort( 0xc0 ),	/* Type Offset=192 */

	/* Parameter pItemValues */

/* 1152 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1154 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1156 */	NdrFcShort( 0x550 ),	/* Type Offset=1360 */

	/* Parameter pTransactionID */

/* 1158 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1160 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1162 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter ppErrors */

/* 1164 */	NdrFcShort( 0x2013 ),	/* Flags:  must size, must free, out, srv alloc size=8 */
/* 1166 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1168 */	NdrFcShort( 0x4ec ),	/* Type Offset=1260 */

	/* Return value */

/* 1170 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1172 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 1174 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Refresh */

/* 1176 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1178 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1182 */	NdrFcShort( 0x5 ),	/* 5 */
/* 1184 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1186 */	NdrFcShort( 0xe ),	/* 14 */
/* 1188 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1190 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x4,		/* 4 */
/* 1192 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1194 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1196 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1198 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dwConnection */

/* 1200 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1202 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1204 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter dwSource */

/* 1206 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1208 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1210 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Parameter pTransactionID */

/* 1212 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1214 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1216 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1218 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1220 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1222 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Cancel2 */


	/* Procedure Cancel */

/* 1224 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1226 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1230 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1232 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1234 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1236 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1238 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1240 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1242 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1244 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1246 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dwCancelID */


	/* Parameter dwTransactionID */

/* 1248 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1250 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1252 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 1254 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1256 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1258 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AddItems */

/* 1260 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1262 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1266 */	NdrFcShort( 0x3 ),	/* 3 */
/* 1268 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1270 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1272 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1274 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 1276 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 1278 */	NdrFcShort( 0x3 ),	/* 3 */
/* 1280 */	NdrFcShort( 0x2 ),	/* 2 */
/* 1282 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dwCount */

/* 1284 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1286 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1288 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pItemArray */

/* 1290 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1292 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1294 */	NdrFcShort( 0x5a4 ),	/* Type Offset=1444 */

	/* Parameter ppAddResults */

/* 1296 */	NdrFcShort( 0x2013 ),	/* Flags:  must size, must free, out, srv alloc size=8 */
/* 1298 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1300 */	NdrFcShort( 0x5d6 ),	/* Type Offset=1494 */

	/* Parameter ppErrors */

/* 1302 */	NdrFcShort( 0x2013 ),	/* Flags:  must size, must free, out, srv alloc size=8 */
/* 1304 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1306 */	NdrFcShort( 0x52c ),	/* Type Offset=1324 */

	/* Return value */

/* 1308 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1310 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1312 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ValidateItems */

/* 1314 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1316 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1320 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1322 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 1324 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1326 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1328 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 1330 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 1332 */	NdrFcShort( 0x3 ),	/* 3 */
/* 1334 */	NdrFcShort( 0x2 ),	/* 2 */
/* 1336 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dwCount */

/* 1338 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1340 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1342 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pItemArray */

/* 1344 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1346 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1348 */	NdrFcShort( 0x5a4 ),	/* Type Offset=1444 */

	/* Parameter bBlobUpdate */

/* 1350 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1352 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1354 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter ppValidationResults */

/* 1356 */	NdrFcShort( 0x2013 ),	/* Flags:  must size, must free, out, srv alloc size=8 */
/* 1358 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1360 */	NdrFcShort( 0x5d6 ),	/* Type Offset=1494 */

	/* Parameter ppErrors */

/* 1362 */	NdrFcShort( 0x2013 ),	/* Flags:  must size, must free, out, srv alloc size=8 */
/* 1364 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1366 */	NdrFcShort( 0x52c ),	/* Type Offset=1324 */

	/* Return value */

/* 1368 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1370 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1372 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RemoveItems */

/* 1374 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1376 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1380 */	NdrFcShort( 0x5 ),	/* 5 */
/* 1382 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1384 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1386 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1388 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 1390 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 1392 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1394 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1396 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dwCount */

/* 1398 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1400 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1402 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter phServer */

/* 1404 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1406 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1408 */	NdrFcShort( 0x4f8 ),	/* Type Offset=1272 */

	/* Parameter ppErrors */

/* 1410 */	NdrFcShort( 0x2013 ),	/* Flags:  must size, must free, out, srv alloc size=8 */
/* 1412 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1414 */	NdrFcShort( 0x52c ),	/* Type Offset=1324 */

	/* Return value */

/* 1416 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1418 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1420 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetActiveState */

/* 1422 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1424 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1428 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1430 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1432 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1434 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1436 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 1438 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 1440 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1442 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1444 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dwCount */

/* 1446 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1448 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1450 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter phServer */

/* 1452 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1454 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1456 */	NdrFcShort( 0x4f8 ),	/* Type Offset=1272 */

	/* Parameter bActive */

/* 1458 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1460 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1462 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter ppErrors */

/* 1464 */	NdrFcShort( 0x2013 ),	/* Flags:  must size, must free, out, srv alloc size=8 */
/* 1466 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1468 */	NdrFcShort( 0x52c ),	/* Type Offset=1324 */

	/* Return value */

/* 1470 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1472 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1474 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetClientHandles */

/* 1476 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1478 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1482 */	NdrFcShort( 0x7 ),	/* 7 */
/* 1484 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1486 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1488 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1490 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 1492 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 1494 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1496 */	NdrFcShort( 0x2 ),	/* 2 */
/* 1498 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dwCount */

/* 1500 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1502 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1504 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter phServer */

/* 1506 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1508 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1510 */	NdrFcShort( 0x4f8 ),	/* Type Offset=1272 */

	/* Parameter phClient */

/* 1512 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1514 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1516 */	NdrFcShort( 0x4f8 ),	/* Type Offset=1272 */

	/* Parameter ppErrors */

/* 1518 */	NdrFcShort( 0x2013 ),	/* Flags:  must size, must free, out, srv alloc size=8 */
/* 1520 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1522 */	NdrFcShort( 0x52c ),	/* Type Offset=1324 */

	/* Return value */

/* 1524 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1526 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1528 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetDatatypes */

/* 1530 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1532 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1536 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1538 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1540 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1542 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1544 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 1546 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 1548 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1550 */	NdrFcShort( 0x2 ),	/* 2 */
/* 1552 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dwCount */

/* 1554 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1556 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1558 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter phServer */

/* 1560 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1562 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1564 */	NdrFcShort( 0x4f8 ),	/* Type Offset=1272 */

	/* Parameter pRequestedDatatypes */

/* 1566 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1568 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1570 */	NdrFcShort( 0x628 ),	/* Type Offset=1576 */

	/* Parameter ppErrors */

/* 1572 */	NdrFcShort( 0x2013 ),	/* Flags:  must size, must free, out, srv alloc size=8 */
/* 1574 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1576 */	NdrFcShort( 0x52c ),	/* Type Offset=1324 */

	/* Return value */

/* 1578 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1580 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1582 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateEnumerator */

/* 1584 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1586 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1590 */	NdrFcShort( 0x9 ),	/* 9 */
/* 1592 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1594 */	NdrFcShort( 0x44 ),	/* 68 */
/* 1596 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1598 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 1600 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 1602 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1604 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1606 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter riid */

/* 1608 */	NdrFcShort( 0x10a ),	/* Flags:  must free, in, simple ref, */
/* 1610 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1612 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter ppUnk */

/* 1614 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 1616 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1618 */	NdrFcShort( 0x634 ),	/* Type Offset=1588 */

	/* Return value */

/* 1620 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1622 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1624 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Next */

/* 1626 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1628 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1632 */	NdrFcShort( 0x3 ),	/* 3 */
/* 1634 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1636 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1638 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1640 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x4,		/* 4 */
/* 1642 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 1644 */	NdrFcShort( 0x22 ),	/* 34 */
/* 1646 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1648 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter celt */

/* 1650 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1652 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1654 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter ppItemArray */

/* 1656 */	NdrFcShort( 0x2013 ),	/* Flags:  must size, must free, out, srv alloc size=8 */
/* 1658 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1660 */	NdrFcShort( 0x640 ),	/* Type Offset=1600 */

	/* Parameter pceltFetched */

/* 1662 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1664 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1666 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1668 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1670 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1672 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Skip */

/* 1674 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1676 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1680 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1682 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1684 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1686 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1688 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1690 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1692 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1694 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1696 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter celt */

/* 1698 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1700 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1702 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1704 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1706 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1708 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Reset */

/* 1710 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1712 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1716 */	NdrFcShort( 0x5 ),	/* 5 */
/* 1718 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1720 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1722 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1724 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 1726 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1728 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1730 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1732 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 1734 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1736 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1738 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Clone */

/* 1740 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1742 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1746 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1748 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1750 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1752 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1754 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1756 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1758 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1760 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1762 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ppEnumItemAttributes */

/* 1764 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 1766 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1768 */	NdrFcShort( 0x68e ),	/* Type Offset=1678 */

	/* Return value */

/* 1770 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1772 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1774 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnDataChange */

/* 1776 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1778 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1782 */	NdrFcShort( 0x3 ),	/* 3 */
/* 1784 */	NdrFcShort( 0x30 ),	/* x86 Stack size/offset = 48 */
/* 1786 */	NdrFcShort( 0x28 ),	/* 40 */
/* 1788 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1790 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0xb,		/* 11 */
/* 1792 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1794 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1796 */	NdrFcShort( 0x25 ),	/* 37 */
/* 1798 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dwTransid */

/* 1800 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1802 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1804 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter hGroup */

/* 1806 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1808 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1810 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter hrMasterquality */

/* 1812 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1814 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1816 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter hrMastererror */

/* 1818 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1820 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1822 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter dwCount */

/* 1824 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1826 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1828 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter phClientItems */

/* 1830 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1832 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1834 */	NdrFcShort( 0x6a8 ),	/* Type Offset=1704 */

	/* Parameter pvValues */

/* 1836 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1838 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 1840 */	NdrFcShort( 0x6b8 ),	/* Type Offset=1720 */

	/* Parameter pwQualities */

/* 1842 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1844 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 1846 */	NdrFcShort( 0x6d2 ),	/* Type Offset=1746 */

	/* Parameter pftTimeStamps */

/* 1848 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1850 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 1852 */	NdrFcShort( 0x6e2 ),	/* Type Offset=1762 */

	/* Parameter pErrors */

/* 1854 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1856 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
/* 1858 */	NdrFcShort( 0x6a8 ),	/* Type Offset=1704 */

	/* Return value */

/* 1860 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1862 */	NdrFcShort( 0x2c ),	/* x86 Stack size/offset = 44 */
/* 1864 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnReadComplete */

/* 1866 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1868 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1872 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1874 */	NdrFcShort( 0x30 ),	/* x86 Stack size/offset = 48 */
/* 1876 */	NdrFcShort( 0x28 ),	/* 40 */
/* 1878 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1880 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0xb,		/* 11 */
/* 1882 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1884 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1886 */	NdrFcShort( 0x25 ),	/* 37 */
/* 1888 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dwTransid */

/* 1890 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1892 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1894 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter hGroup */

/* 1896 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1898 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1900 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter hrMasterquality */

/* 1902 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1904 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1906 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter hrMastererror */

/* 1908 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1910 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1912 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter dwCount */

/* 1914 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1916 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1918 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter phClientItems */

/* 1920 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1922 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1924 */	NdrFcShort( 0x6a8 ),	/* Type Offset=1704 */

	/* Parameter pvValues */

/* 1926 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1928 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 1930 */	NdrFcShort( 0x6b8 ),	/* Type Offset=1720 */

	/* Parameter pwQualities */

/* 1932 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1934 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 1936 */	NdrFcShort( 0x6d2 ),	/* Type Offset=1746 */

	/* Parameter pftTimeStamps */

/* 1938 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1940 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 1942 */	NdrFcShort( 0x6e2 ),	/* Type Offset=1762 */

	/* Parameter pErrors */

/* 1944 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1946 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
/* 1948 */	NdrFcShort( 0x6a8 ),	/* Type Offset=1704 */

	/* Return value */

/* 1950 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1952 */	NdrFcShort( 0x2c ),	/* x86 Stack size/offset = 44 */
/* 1954 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnWriteComplete */

/* 1956 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1958 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1962 */	NdrFcShort( 0x5 ),	/* 5 */
/* 1964 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 1966 */	NdrFcShort( 0x20 ),	/* 32 */
/* 1968 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1970 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x7,		/* 7 */
/* 1972 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1974 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1976 */	NdrFcShort( 0x2 ),	/* 2 */
/* 1978 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dwTransid */

/* 1980 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1982 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1984 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter hGroup */

/* 1986 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1988 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1990 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter hrMastererr */

/* 1992 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1994 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1996 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter dwCount */

/* 1998 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2000 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2002 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pClienthandles */

/* 2004 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2006 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2008 */	NdrFcShort( 0x6f6 ),	/* Type Offset=1782 */

	/* Parameter pErrors */

/* 2010 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2012 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 2014 */	NdrFcShort( 0x6f6 ),	/* Type Offset=1782 */

	/* Return value */

/* 2016 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2018 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 2020 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnCancelComplete */

/* 2022 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2024 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2028 */	NdrFcShort( 0x6 ),	/* 6 */
/* 2030 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2032 */	NdrFcShort( 0x10 ),	/* 16 */
/* 2034 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2036 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 2038 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2040 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2042 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2044 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dwTransid */

/* 2046 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2048 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2050 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter hGroup */

/* 2052 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2054 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2056 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2058 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2060 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2062 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Read */

/* 2064 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2066 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2070 */	NdrFcShort( 0x3 ),	/* 3 */
/* 2072 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 2074 */	NdrFcShort( 0x10 ),	/* 16 */
/* 2076 */	NdrFcShort( 0x24 ),	/* 36 */
/* 2078 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 2080 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 2082 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2084 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2086 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dwCount */

/* 2088 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2090 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2092 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter phServer */

/* 2094 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2096 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2098 */	NdrFcShort( 0x4f8 ),	/* Type Offset=1272 */

	/* Parameter dwTransactionID */

/* 2100 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2102 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2104 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pdwCancelID */

/* 2106 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2108 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2110 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter ppErrors */

/* 2112 */	NdrFcShort( 0x2013 ),	/* Flags:  must size, must free, out, srv alloc size=8 */
/* 2114 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2116 */	NdrFcShort( 0x52c ),	/* Type Offset=1324 */

	/* Return value */

/* 2118 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2120 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 2122 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Write */

/* 2124 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2126 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2130 */	NdrFcShort( 0x4 ),	/* 4 */
/* 2132 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 2134 */	NdrFcShort( 0x10 ),	/* 16 */
/* 2136 */	NdrFcShort( 0x24 ),	/* 36 */
/* 2138 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x7,		/* 7 */
/* 2140 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 2142 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2144 */	NdrFcShort( 0x22 ),	/* 34 */
/* 2146 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dwCount */

/* 2148 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2150 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2152 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter phServer */

/* 2154 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2156 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2158 */	NdrFcShort( 0x4f8 ),	/* Type Offset=1272 */

	/* Parameter pItemValues */

/* 2160 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2162 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2164 */	NdrFcShort( 0x516 ),	/* Type Offset=1302 */

	/* Parameter dwTransactionID */

/* 2166 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2168 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2170 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pdwCancelID */

/* 2172 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2174 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2176 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter ppErrors */

/* 2178 */	NdrFcShort( 0x2013 ),	/* Flags:  must size, must free, out, srv alloc size=8 */
/* 2180 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 2182 */	NdrFcShort( 0x52c ),	/* Type Offset=1324 */

	/* Return value */

/* 2184 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2186 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 2188 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Refresh2 */

/* 2190 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2192 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2196 */	NdrFcShort( 0x5 ),	/* 5 */
/* 2198 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2200 */	NdrFcShort( 0xe ),	/* 14 */
/* 2202 */	NdrFcShort( 0x24 ),	/* 36 */
/* 2204 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x4,		/* 4 */
/* 2206 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2208 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2210 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2212 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dwSource */

/* 2214 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2216 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2218 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Parameter dwTransactionID */

/* 2220 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2222 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2224 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pdwCancelID */

/* 2226 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2228 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2230 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2232 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2234 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2236 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetEnable */

/* 2238 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2240 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2244 */	NdrFcShort( 0x7 ),	/* 7 */
/* 2246 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2248 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2250 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2252 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2254 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2256 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2258 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2260 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter bEnable */

/* 2262 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2264 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2266 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2268 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2270 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2272 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetEnable */

/* 2274 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2276 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2280 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2282 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2284 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2286 */	NdrFcShort( 0x24 ),	/* 36 */
/* 2288 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2290 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2292 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2294 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2296 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pbEnable */

/* 2298 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2300 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2302 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2304 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2306 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2308 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure QueryAvailableProperties */

/* 2310 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2312 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2316 */	NdrFcShort( 0x3 ),	/* 3 */
/* 2318 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 2320 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2322 */	NdrFcShort( 0x24 ),	/* 36 */
/* 2324 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 2326 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 2328 */	NdrFcShort( 0x3 ),	/* 3 */
/* 2330 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2332 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter szItemID */

/* 2334 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2336 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2338 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pdwCount */

/* 2340 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2342 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2344 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter ppPropertyIDs */

/* 2346 */	NdrFcShort( 0x2013 ),	/* Flags:  must size, must free, out, srv alloc size=8 */
/* 2348 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2350 */	NdrFcShort( 0x702 ),	/* Type Offset=1794 */

	/* Parameter ppDescriptions */

/* 2352 */	NdrFcShort( 0x2013 ),	/* Flags:  must size, must free, out, srv alloc size=8 */
/* 2354 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2356 */	NdrFcShort( 0x716 ),	/* Type Offset=1814 */

	/* Parameter ppvtDataTypes */

/* 2358 */	NdrFcShort( 0x2013 ),	/* Flags:  must size, must free, out, srv alloc size=8 */
/* 2360 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2362 */	NdrFcShort( 0x73e ),	/* Type Offset=1854 */

	/* Return value */

/* 2364 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2366 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 2368 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetItemProperties */

/* 2370 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2372 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2376 */	NdrFcShort( 0x4 ),	/* 4 */
/* 2378 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 2380 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2382 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2384 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 2386 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 2388 */	NdrFcShort( 0x22 ),	/* 34 */
/* 2390 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2392 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter szItemID */

/* 2394 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2396 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2398 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter dwCount */

/* 2400 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2402 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2404 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pdwPropertyIDs */

/* 2406 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2408 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2410 */	NdrFcShort( 0xc0 ),	/* Type Offset=192 */

	/* Parameter ppvData */

/* 2412 */	NdrFcShort( 0x2013 ),	/* Flags:  must size, must free, out, srv alloc size=8 */
/* 2414 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2416 */	NdrFcShort( 0x752 ),	/* Type Offset=1874 */

	/* Parameter ppErrors */

/* 2418 */	NdrFcShort( 0x2013 ),	/* Flags:  must size, must free, out, srv alloc size=8 */
/* 2420 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2422 */	NdrFcShort( 0x4ec ),	/* Type Offset=1260 */

	/* Return value */

/* 2424 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2426 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 2428 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure LookupItemIDs */

/* 2430 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2432 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2436 */	NdrFcShort( 0x5 ),	/* 5 */
/* 2438 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 2440 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2442 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2444 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 2446 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 2448 */	NdrFcShort( 0x2 ),	/* 2 */
/* 2450 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2452 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter szItemID */

/* 2454 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2456 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2458 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter dwCount */

/* 2460 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2462 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2464 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pdwPropertyIDs */

/* 2466 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2468 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2470 */	NdrFcShort( 0xc0 ),	/* Type Offset=192 */

	/* Parameter ppszNewItemIDs */

/* 2472 */	NdrFcShort( 0x2013 ),	/* Flags:  must size, must free, out, srv alloc size=8 */
/* 2474 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2476 */	NdrFcShort( 0x770 ),	/* Type Offset=1904 */

	/* Parameter ppErrors */

/* 2478 */	NdrFcShort( 0x2013 ),	/* Flags:  must size, must free, out, srv alloc size=8 */
/* 2480 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2482 */	NdrFcShort( 0x4ec ),	/* Type Offset=1260 */

	/* Return value */

/* 2484 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2486 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 2488 */	0x8,		/* FC_LONG */
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
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/*  8 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 10 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 12 */	0xa,		/* FC_FLOAT */
			0x5c,		/* FC_PAD */
/* 14 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 16 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 18 */	
			0x11, 0x0,	/* FC_RP */
/* 20 */	NdrFcShort( 0x8 ),	/* Offset= 8 (28) */
/* 22 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 24 */	NdrFcShort( 0x8 ),	/* 8 */
/* 26 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 28 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 30 */	NdrFcShort( 0x10 ),	/* 16 */
/* 32 */	0x8,		/* FC_LONG */
			0x6,		/* FC_SHORT */
/* 34 */	0x6,		/* FC_SHORT */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 36 */	0x0,		/* 0 */
			NdrFcShort( 0xfff1 ),	/* Offset= -15 (22) */
			0x5b,		/* FC_END */
/* 40 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 42 */	NdrFcShort( 0x2 ),	/* Offset= 2 (44) */
/* 44 */	
			0x2f,		/* FC_IP */
			0x5c,		/* FC_PAD */
/* 46 */	0x28,		/* Corr desc:  parameter, FC_LONG */
			0x0,		/*  */
/* 48 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
/* 50 */	NdrFcShort( 0x5 ),	/* Corr flags:  early, iid_is, */
/* 52 */	
			0x11, 0x14,	/* FC_RP [alloced_on_stack] [pointer_deref] */
/* 54 */	NdrFcShort( 0x2 ),	/* Offset= 2 (56) */
/* 56 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 58 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 60 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 62 */	NdrFcShort( 0x2 ),	/* Offset= 2 (64) */
/* 64 */	
			0x2f,		/* FC_IP */
			0x5c,		/* FC_PAD */
/* 66 */	0x28,		/* Corr desc:  parameter, FC_LONG */
			0x0,		/*  */
/* 68 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 70 */	NdrFcShort( 0x5 ),	/* Corr flags:  early, iid_is, */
/* 72 */	
			0x11, 0x14,	/* FC_RP [alloced_on_stack] [pointer_deref] */
/* 74 */	NdrFcShort( 0x2 ),	/* Offset= 2 (76) */
/* 76 */	
			0x13, 0x0,	/* FC_OP */
/* 78 */	NdrFcShort( 0xa ),	/* Offset= 10 (88) */
/* 80 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 82 */	NdrFcShort( 0x8 ),	/* 8 */
/* 84 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 86 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 88 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 90 */	NdrFcShort( 0x30 ),	/* 48 */
/* 92 */	NdrFcShort( 0x0 ),	/* 0 */
/* 94 */	NdrFcShort( 0x18 ),	/* Offset= 24 (118) */
/* 96 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 98 */	NdrFcShort( 0xffee ),	/* Offset= -18 (80) */
/* 100 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 102 */	NdrFcShort( 0xffea ),	/* Offset= -22 (80) */
/* 104 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 106 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (80) */
/* 108 */	0xd,		/* FC_ENUM16 */
			0x8,		/* FC_LONG */
/* 110 */	0x8,		/* FC_LONG */
			0x6,		/* FC_SHORT */
/* 112 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 114 */	0x6,		/* FC_SHORT */
			0x36,		/* FC_POINTER */
/* 116 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 118 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 120 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 122 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 124 */	NdrFcShort( 0x2 ),	/* Offset= 2 (126) */
/* 126 */	
			0x2f,		/* FC_IP */
			0x5c,		/* FC_PAD */
/* 128 */	0x28,		/* Corr desc:  parameter, FC_LONG */
			0x0,		/*  */
/* 130 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 132 */	NdrFcShort( 0x5 ),	/* Corr flags:  early, iid_is, */
/* 134 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 136 */	NdrFcShort( 0x2 ),	/* Offset= 2 (138) */
/* 138 */	
			0x2f,		/* FC_IP */
			0x5c,		/* FC_PAD */
/* 140 */	0x28,		/* Corr desc:  parameter, FC_LONG */
			0x0,		/*  */
/* 142 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 144 */	NdrFcShort( 0x5 ),	/* Corr flags:  early, iid_is, */
/* 146 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 148 */	0xd,		/* FC_ENUM16 */
			0x5c,		/* FC_PAD */
/* 150 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 152 */	NdrFcShort( 0x2 ),	/* Offset= 2 (154) */
/* 154 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 156 */	NdrFcLong( 0x101 ),	/* 257 */
/* 160 */	NdrFcShort( 0x0 ),	/* 0 */
/* 162 */	NdrFcShort( 0x0 ),	/* 0 */
/* 164 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 166 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 168 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 170 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 172 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 174 */	0xa,		/* FC_FLOAT */
			0x5c,		/* FC_PAD */
/* 176 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 178 */	NdrFcShort( 0x2 ),	/* Offset= 2 (180) */
/* 180 */	
			0x2f,		/* FC_IP */
			0x5c,		/* FC_PAD */
/* 182 */	0x28,		/* Corr desc:  parameter, FC_LONG */
			0x0,		/*  */
/* 184 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 186 */	NdrFcShort( 0x5 ),	/* Corr flags:  early, iid_is, */
/* 188 */	
			0x11, 0x0,	/* FC_RP */
/* 190 */	NdrFcShort( 0x2 ),	/* Offset= 2 (192) */
/* 192 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 194 */	NdrFcShort( 0x4 ),	/* 4 */
/* 196 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 198 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 200 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 202 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 204 */	
			0x11, 0x14,	/* FC_RP [alloced_on_stack] [pointer_deref] */
/* 206 */	NdrFcShort( 0x2 ),	/* Offset= 2 (208) */
/* 208 */	
			0x13, 0x0,	/* FC_OP */
/* 210 */	NdrFcShort( 0x404 ),	/* Offset= 1028 (1238) */
/* 212 */	
			0x13, 0x0,	/* FC_OP */
/* 214 */	NdrFcShort( 0x3ce ),	/* Offset= 974 (1188) */
/* 216 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 218 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 220 */	NdrFcShort( 0xfff8 ),	/* -8 */
/* 222 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 224 */	NdrFcShort( 0x2 ),	/* Offset= 2 (226) */
/* 226 */	NdrFcShort( 0x10 ),	/* 16 */
/* 228 */	NdrFcShort( 0x2f ),	/* 47 */
/* 230 */	NdrFcLong( 0x14 ),	/* 20 */
/* 234 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 236 */	NdrFcLong( 0x3 ),	/* 3 */
/* 240 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 242 */	NdrFcLong( 0x11 ),	/* 17 */
/* 246 */	NdrFcShort( 0x8001 ),	/* Simple arm type: FC_BYTE */
/* 248 */	NdrFcLong( 0x2 ),	/* 2 */
/* 252 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 254 */	NdrFcLong( 0x4 ),	/* 4 */
/* 258 */	NdrFcShort( 0x800a ),	/* Simple arm type: FC_FLOAT */
/* 260 */	NdrFcLong( 0x5 ),	/* 5 */
/* 264 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 266 */	NdrFcLong( 0xb ),	/* 11 */
/* 270 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 272 */	NdrFcLong( 0xa ),	/* 10 */
/* 276 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 278 */	NdrFcLong( 0x6 ),	/* 6 */
/* 282 */	NdrFcShort( 0xe8 ),	/* Offset= 232 (514) */
/* 284 */	NdrFcLong( 0x7 ),	/* 7 */
/* 288 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 290 */	NdrFcLong( 0x8 ),	/* 8 */
/* 294 */	NdrFcShort( 0xe2 ),	/* Offset= 226 (520) */
/* 296 */	NdrFcLong( 0xd ),	/* 13 */
/* 300 */	NdrFcShort( 0xf6 ),	/* Offset= 246 (546) */
/* 302 */	NdrFcLong( 0x9 ),	/* 9 */
/* 306 */	NdrFcShort( 0x102 ),	/* Offset= 258 (564) */
/* 308 */	NdrFcLong( 0x2000 ),	/* 8192 */
/* 312 */	NdrFcShort( 0x10e ),	/* Offset= 270 (582) */
/* 314 */	NdrFcLong( 0x24 ),	/* 36 */
/* 318 */	NdrFcShort( 0x31c ),	/* Offset= 796 (1114) */
/* 320 */	NdrFcLong( 0x4024 ),	/* 16420 */
/* 324 */	NdrFcShort( 0x316 ),	/* Offset= 790 (1114) */
/* 326 */	NdrFcLong( 0x4011 ),	/* 16401 */
/* 330 */	NdrFcShort( 0x314 ),	/* Offset= 788 (1118) */
/* 332 */	NdrFcLong( 0x4002 ),	/* 16386 */
/* 336 */	NdrFcShort( 0x312 ),	/* Offset= 786 (1122) */
/* 338 */	NdrFcLong( 0x4003 ),	/* 16387 */
/* 342 */	NdrFcShort( 0x310 ),	/* Offset= 784 (1126) */
/* 344 */	NdrFcLong( 0x4014 ),	/* 16404 */
/* 348 */	NdrFcShort( 0x30e ),	/* Offset= 782 (1130) */
/* 350 */	NdrFcLong( 0x4004 ),	/* 16388 */
/* 354 */	NdrFcShort( 0x30c ),	/* Offset= 780 (1134) */
/* 356 */	NdrFcLong( 0x4005 ),	/* 16389 */
/* 360 */	NdrFcShort( 0x30a ),	/* Offset= 778 (1138) */
/* 362 */	NdrFcLong( 0x400b ),	/* 16395 */
/* 366 */	NdrFcShort( 0x2f4 ),	/* Offset= 756 (1122) */
/* 368 */	NdrFcLong( 0x400a ),	/* 16394 */
/* 372 */	NdrFcShort( 0x2f2 ),	/* Offset= 754 (1126) */
/* 374 */	NdrFcLong( 0x4006 ),	/* 16390 */
/* 378 */	NdrFcShort( 0x2fc ),	/* Offset= 764 (1142) */
/* 380 */	NdrFcLong( 0x4007 ),	/* 16391 */
/* 384 */	NdrFcShort( 0x2f2 ),	/* Offset= 754 (1138) */
/* 386 */	NdrFcLong( 0x4008 ),	/* 16392 */
/* 390 */	NdrFcShort( 0x2f4 ),	/* Offset= 756 (1146) */
/* 392 */	NdrFcLong( 0x400d ),	/* 16397 */
/* 396 */	NdrFcShort( 0x2f2 ),	/* Offset= 754 (1150) */
/* 398 */	NdrFcLong( 0x4009 ),	/* 16393 */
/* 402 */	NdrFcShort( 0x2f0 ),	/* Offset= 752 (1154) */
/* 404 */	NdrFcLong( 0x6000 ),	/* 24576 */
/* 408 */	NdrFcShort( 0x2ee ),	/* Offset= 750 (1158) */
/* 410 */	NdrFcLong( 0x400c ),	/* 16396 */
/* 414 */	NdrFcShort( 0x2ec ),	/* Offset= 748 (1162) */
/* 416 */	NdrFcLong( 0x10 ),	/* 16 */
/* 420 */	NdrFcShort( 0x8002 ),	/* Simple arm type: FC_CHAR */
/* 422 */	NdrFcLong( 0x12 ),	/* 18 */
/* 426 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 428 */	NdrFcLong( 0x13 ),	/* 19 */
/* 432 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 434 */	NdrFcLong( 0x15 ),	/* 21 */
/* 438 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 440 */	NdrFcLong( 0x16 ),	/* 22 */
/* 444 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 446 */	NdrFcLong( 0x17 ),	/* 23 */
/* 450 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 452 */	NdrFcLong( 0xe ),	/* 14 */
/* 456 */	NdrFcShort( 0x2ca ),	/* Offset= 714 (1170) */
/* 458 */	NdrFcLong( 0x400e ),	/* 16398 */
/* 462 */	NdrFcShort( 0x2ce ),	/* Offset= 718 (1180) */
/* 464 */	NdrFcLong( 0x4010 ),	/* 16400 */
/* 468 */	NdrFcShort( 0x2cc ),	/* Offset= 716 (1184) */
/* 470 */	NdrFcLong( 0x4012 ),	/* 16402 */
/* 474 */	NdrFcShort( 0x288 ),	/* Offset= 648 (1122) */
/* 476 */	NdrFcLong( 0x4013 ),	/* 16403 */
/* 480 */	NdrFcShort( 0x286 ),	/* Offset= 646 (1126) */
/* 482 */	NdrFcLong( 0x4015 ),	/* 16405 */
/* 486 */	NdrFcShort( 0x284 ),	/* Offset= 644 (1130) */
/* 488 */	NdrFcLong( 0x4016 ),	/* 16406 */
/* 492 */	NdrFcShort( 0x27a ),	/* Offset= 634 (1126) */
/* 494 */	NdrFcLong( 0x4017 ),	/* 16407 */
/* 498 */	NdrFcShort( 0x274 ),	/* Offset= 628 (1126) */
/* 500 */	NdrFcLong( 0x0 ),	/* 0 */
/* 504 */	NdrFcShort( 0x0 ),	/* Offset= 0 (504) */
/* 506 */	NdrFcLong( 0x1 ),	/* 1 */
/* 510 */	NdrFcShort( 0x0 ),	/* Offset= 0 (510) */
/* 512 */	NdrFcShort( 0xffff ),	/* Offset= -1 (511) */
/* 514 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 516 */	NdrFcShort( 0x8 ),	/* 8 */
/* 518 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 520 */	
			0x13, 0x0,	/* FC_OP */
/* 522 */	NdrFcShort( 0xe ),	/* Offset= 14 (536) */
/* 524 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 526 */	NdrFcShort( 0x2 ),	/* 2 */
/* 528 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 530 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 532 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 534 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 536 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 538 */	NdrFcShort( 0x8 ),	/* 8 */
/* 540 */	NdrFcShort( 0xfff0 ),	/* Offset= -16 (524) */
/* 542 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 544 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 546 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 548 */	NdrFcLong( 0x0 ),	/* 0 */
/* 552 */	NdrFcShort( 0x0 ),	/* 0 */
/* 554 */	NdrFcShort( 0x0 ),	/* 0 */
/* 556 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 558 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 560 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 562 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 564 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 566 */	NdrFcLong( 0x20400 ),	/* 132096 */
/* 570 */	NdrFcShort( 0x0 ),	/* 0 */
/* 572 */	NdrFcShort( 0x0 ),	/* 0 */
/* 574 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 576 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 578 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 580 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 582 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 584 */	NdrFcShort( 0x2 ),	/* Offset= 2 (586) */
/* 586 */	
			0x13, 0x0,	/* FC_OP */
/* 588 */	NdrFcShort( 0x1fc ),	/* Offset= 508 (1096) */
/* 590 */	
			0x2a,		/* FC_ENCAPSULATED_UNION */
			0x49,		/* 73 */
/* 592 */	NdrFcShort( 0x18 ),	/* 24 */
/* 594 */	NdrFcShort( 0xa ),	/* 10 */
/* 596 */	NdrFcLong( 0x8 ),	/* 8 */
/* 600 */	NdrFcShort( 0x5a ),	/* Offset= 90 (690) */
/* 602 */	NdrFcLong( 0xd ),	/* 13 */
/* 606 */	NdrFcShort( 0x7e ),	/* Offset= 126 (732) */
/* 608 */	NdrFcLong( 0x9 ),	/* 9 */
/* 612 */	NdrFcShort( 0x9e ),	/* Offset= 158 (770) */
/* 614 */	NdrFcLong( 0xc ),	/* 12 */
/* 618 */	NdrFcShort( 0xc8 ),	/* Offset= 200 (818) */
/* 620 */	NdrFcLong( 0x24 ),	/* 36 */
/* 624 */	NdrFcShort( 0x124 ),	/* Offset= 292 (916) */
/* 626 */	NdrFcLong( 0x800d ),	/* 32781 */
/* 630 */	NdrFcShort( 0x12e ),	/* Offset= 302 (932) */
/* 632 */	NdrFcLong( 0x10 ),	/* 16 */
/* 636 */	NdrFcShort( 0x148 ),	/* Offset= 328 (964) */
/* 638 */	NdrFcLong( 0x2 ),	/* 2 */
/* 642 */	NdrFcShort( 0x162 ),	/* Offset= 354 (996) */
/* 644 */	NdrFcLong( 0x3 ),	/* 3 */
/* 648 */	NdrFcShort( 0x17c ),	/* Offset= 380 (1028) */
/* 650 */	NdrFcLong( 0x14 ),	/* 20 */
/* 654 */	NdrFcShort( 0x196 ),	/* Offset= 406 (1060) */
/* 656 */	NdrFcShort( 0xffff ),	/* Offset= -1 (655) */
/* 658 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 660 */	NdrFcShort( 0x4 ),	/* 4 */
/* 662 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 664 */	NdrFcShort( 0x0 ),	/* 0 */
/* 666 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 668 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 670 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 672 */	NdrFcShort( 0x4 ),	/* 4 */
/* 674 */	NdrFcShort( 0x0 ),	/* 0 */
/* 676 */	NdrFcShort( 0x1 ),	/* 1 */
/* 678 */	NdrFcShort( 0x0 ),	/* 0 */
/* 680 */	NdrFcShort( 0x0 ),	/* 0 */
/* 682 */	0x13, 0x0,	/* FC_OP */
/* 684 */	NdrFcShort( 0xff6c ),	/* Offset= -148 (536) */
/* 686 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 688 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 690 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 692 */	NdrFcShort( 0x8 ),	/* 8 */
/* 694 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 696 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 698 */	NdrFcShort( 0x4 ),	/* 4 */
/* 700 */	NdrFcShort( 0x4 ),	/* 4 */
/* 702 */	0x11, 0x0,	/* FC_RP */
/* 704 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (658) */
/* 706 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 708 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 710 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 712 */	NdrFcShort( 0x0 ),	/* 0 */
/* 714 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 716 */	NdrFcShort( 0x0 ),	/* 0 */
/* 718 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 720 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 724 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 726 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 728 */	NdrFcShort( 0xff4a ),	/* Offset= -182 (546) */
/* 730 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 732 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 734 */	NdrFcShort( 0x8 ),	/* 8 */
/* 736 */	NdrFcShort( 0x0 ),	/* 0 */
/* 738 */	NdrFcShort( 0x6 ),	/* Offset= 6 (744) */
/* 740 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 742 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 744 */	
			0x11, 0x0,	/* FC_RP */
/* 746 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (710) */
/* 748 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 750 */	NdrFcShort( 0x0 ),	/* 0 */
/* 752 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 754 */	NdrFcShort( 0x0 ),	/* 0 */
/* 756 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 758 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 762 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 764 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 766 */	NdrFcShort( 0xff36 ),	/* Offset= -202 (564) */
/* 768 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 770 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 772 */	NdrFcShort( 0x8 ),	/* 8 */
/* 774 */	NdrFcShort( 0x0 ),	/* 0 */
/* 776 */	NdrFcShort( 0x6 ),	/* Offset= 6 (782) */
/* 778 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 780 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 782 */	
			0x11, 0x0,	/* FC_RP */
/* 784 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (748) */
/* 786 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 788 */	NdrFcShort( 0x4 ),	/* 4 */
/* 790 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 792 */	NdrFcShort( 0x0 ),	/* 0 */
/* 794 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 796 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 798 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 800 */	NdrFcShort( 0x4 ),	/* 4 */
/* 802 */	NdrFcShort( 0x0 ),	/* 0 */
/* 804 */	NdrFcShort( 0x1 ),	/* 1 */
/* 806 */	NdrFcShort( 0x0 ),	/* 0 */
/* 808 */	NdrFcShort( 0x0 ),	/* 0 */
/* 810 */	0x13, 0x0,	/* FC_OP */
/* 812 */	NdrFcShort( 0x178 ),	/* Offset= 376 (1188) */
/* 814 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 816 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 818 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 820 */	NdrFcShort( 0x8 ),	/* 8 */
/* 822 */	NdrFcShort( 0x0 ),	/* 0 */
/* 824 */	NdrFcShort( 0x6 ),	/* Offset= 6 (830) */
/* 826 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 828 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 830 */	
			0x11, 0x0,	/* FC_RP */
/* 832 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (786) */
/* 834 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 836 */	NdrFcLong( 0x2f ),	/* 47 */
/* 840 */	NdrFcShort( 0x0 ),	/* 0 */
/* 842 */	NdrFcShort( 0x0 ),	/* 0 */
/* 844 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 846 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 848 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 850 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 852 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 854 */	NdrFcShort( 0x1 ),	/* 1 */
/* 856 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 858 */	NdrFcShort( 0x4 ),	/* 4 */
/* 860 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 862 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 864 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 866 */	NdrFcShort( 0x10 ),	/* 16 */
/* 868 */	NdrFcShort( 0x0 ),	/* 0 */
/* 870 */	NdrFcShort( 0xa ),	/* Offset= 10 (880) */
/* 872 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 874 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 876 */	NdrFcShort( 0xffd6 ),	/* Offset= -42 (834) */
/* 878 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 880 */	
			0x13, 0x0,	/* FC_OP */
/* 882 */	NdrFcShort( 0xffe2 ),	/* Offset= -30 (852) */
/* 884 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 886 */	NdrFcShort( 0x4 ),	/* 4 */
/* 888 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 890 */	NdrFcShort( 0x0 ),	/* 0 */
/* 892 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 894 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 896 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 898 */	NdrFcShort( 0x4 ),	/* 4 */
/* 900 */	NdrFcShort( 0x0 ),	/* 0 */
/* 902 */	NdrFcShort( 0x1 ),	/* 1 */
/* 904 */	NdrFcShort( 0x0 ),	/* 0 */
/* 906 */	NdrFcShort( 0x0 ),	/* 0 */
/* 908 */	0x13, 0x0,	/* FC_OP */
/* 910 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (864) */
/* 912 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 914 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 916 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 918 */	NdrFcShort( 0x8 ),	/* 8 */
/* 920 */	NdrFcShort( 0x0 ),	/* 0 */
/* 922 */	NdrFcShort( 0x6 ),	/* Offset= 6 (928) */
/* 924 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 926 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 928 */	
			0x11, 0x0,	/* FC_RP */
/* 930 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (884) */
/* 932 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 934 */	NdrFcShort( 0x18 ),	/* 24 */
/* 936 */	NdrFcShort( 0x0 ),	/* 0 */
/* 938 */	NdrFcShort( 0xa ),	/* Offset= 10 (948) */
/* 940 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 942 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 944 */	NdrFcShort( 0xfc6c ),	/* Offset= -916 (28) */
/* 946 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 948 */	
			0x11, 0x0,	/* FC_RP */
/* 950 */	NdrFcShort( 0xff10 ),	/* Offset= -240 (710) */
/* 952 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 954 */	NdrFcShort( 0x1 ),	/* 1 */
/* 956 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 958 */	NdrFcShort( 0x0 ),	/* 0 */
/* 960 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 962 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 964 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 966 */	NdrFcShort( 0x8 ),	/* 8 */
/* 968 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 970 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 972 */	NdrFcShort( 0x4 ),	/* 4 */
/* 974 */	NdrFcShort( 0x4 ),	/* 4 */
/* 976 */	0x13, 0x0,	/* FC_OP */
/* 978 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (952) */
/* 980 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 982 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 984 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 986 */	NdrFcShort( 0x2 ),	/* 2 */
/* 988 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 990 */	NdrFcShort( 0x0 ),	/* 0 */
/* 992 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 994 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 996 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 998 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1000 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 1002 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 1004 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1006 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1008 */	0x13, 0x0,	/* FC_OP */
/* 1010 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (984) */
/* 1012 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 1014 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 1016 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 1018 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1020 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 1022 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1024 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1026 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 1028 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 1030 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1032 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 1034 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 1036 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1038 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1040 */	0x13, 0x0,	/* FC_OP */
/* 1042 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (1016) */
/* 1044 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 1046 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 1048 */	
			0x1b,		/* FC_CARRAY */
			0x7,		/* 7 */
/* 1050 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1052 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 1054 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1056 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1058 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 1060 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 1062 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1064 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 1066 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 1068 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1070 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1072 */	0x13, 0x0,	/* FC_OP */
/* 1074 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (1048) */
/* 1076 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 1078 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 1080 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 1082 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1084 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 1086 */	NdrFcShort( 0xffd8 ),	/* -40 */
/* 1088 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1090 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1092 */	NdrFcShort( 0xfc0c ),	/* Offset= -1012 (80) */
/* 1094 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1096 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1098 */	NdrFcShort( 0x28 ),	/* 40 */
/* 1100 */	NdrFcShort( 0xffec ),	/* Offset= -20 (1080) */
/* 1102 */	NdrFcShort( 0x0 ),	/* Offset= 0 (1102) */
/* 1104 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1106 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1108 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1110 */	NdrFcShort( 0xfdf8 ),	/* Offset= -520 (590) */
/* 1112 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1114 */	
			0x13, 0x0,	/* FC_OP */
/* 1116 */	NdrFcShort( 0xff04 ),	/* Offset= -252 (864) */
/* 1118 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1120 */	0x1,		/* FC_BYTE */
			0x5c,		/* FC_PAD */
/* 1122 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1124 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 1126 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1128 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 1130 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1132 */	0xb,		/* FC_HYPER */
			0x5c,		/* FC_PAD */
/* 1134 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1136 */	0xa,		/* FC_FLOAT */
			0x5c,		/* FC_PAD */
/* 1138 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1140 */	0xc,		/* FC_DOUBLE */
			0x5c,		/* FC_PAD */
/* 1142 */	
			0x13, 0x0,	/* FC_OP */
/* 1144 */	NdrFcShort( 0xfd8a ),	/* Offset= -630 (514) */
/* 1146 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 1148 */	NdrFcShort( 0xfd8c ),	/* Offset= -628 (520) */
/* 1150 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 1152 */	NdrFcShort( 0xfda2 ),	/* Offset= -606 (546) */
/* 1154 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 1156 */	NdrFcShort( 0xfdb0 ),	/* Offset= -592 (564) */
/* 1158 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 1160 */	NdrFcShort( 0xfdbe ),	/* Offset= -578 (582) */
/* 1162 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 1164 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1166) */
/* 1166 */	
			0x13, 0x0,	/* FC_OP */
/* 1168 */	NdrFcShort( 0x14 ),	/* Offset= 20 (1188) */
/* 1170 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 1172 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1174 */	0x6,		/* FC_SHORT */
			0x1,		/* FC_BYTE */
/* 1176 */	0x1,		/* FC_BYTE */
			0x8,		/* FC_LONG */
/* 1178 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 1180 */	
			0x13, 0x0,	/* FC_OP */
/* 1182 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (1170) */
/* 1184 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1186 */	0x2,		/* FC_CHAR */
			0x5c,		/* FC_PAD */
/* 1188 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x7,		/* 7 */
/* 1190 */	NdrFcShort( 0x20 ),	/* 32 */
/* 1192 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1194 */	NdrFcShort( 0x0 ),	/* Offset= 0 (1194) */
/* 1196 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1198 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1200 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1202 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1204 */	NdrFcShort( 0xfc24 ),	/* Offset= -988 (216) */
/* 1206 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1208 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1210 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1212 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1214 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1216 */	NdrFcShort( 0xfc14 ),	/* Offset= -1004 (212) */
/* 1218 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1220 */	NdrFcShort( 0x20 ),	/* 32 */
/* 1222 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1224 */	NdrFcShort( 0x0 ),	/* Offset= 0 (1224) */
/* 1226 */	0x8,		/* FC_LONG */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 1228 */	0x0,		/* 0 */
			NdrFcShort( 0xfb83 ),	/* Offset= -1149 (80) */
			0x6,		/* FC_SHORT */
/* 1232 */	0x6,		/* FC_SHORT */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 1234 */	0x0,		/* 0 */
			NdrFcShort( 0xffe5 ),	/* Offset= -27 (1208) */
			0x5b,		/* FC_END */
/* 1238 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 1240 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1242 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 1244 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1246 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1248 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 1252 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 1254 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1256 */	NdrFcShort( 0xffda ),	/* Offset= -38 (1218) */
/* 1258 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1260 */	
			0x11, 0x14,	/* FC_RP [alloced_on_stack] [pointer_deref] */
/* 1262 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1264) */
/* 1264 */	
			0x13, 0x0,	/* FC_OP */
/* 1266 */	NdrFcShort( 0xfbce ),	/* Offset= -1074 (192) */
/* 1268 */	
			0x11, 0x0,	/* FC_RP */
/* 1270 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1272) */
/* 1272 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 1274 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1276 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 1278 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1280 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1282 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 1284 */	
			0x11, 0x0,	/* FC_RP */
/* 1286 */	NdrFcShort( 0x10 ),	/* Offset= 16 (1302) */
/* 1288 */	
			0x12, 0x0,	/* FC_UP */
/* 1290 */	NdrFcShort( 0xff9a ),	/* Offset= -102 (1188) */
/* 1292 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1294 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1296 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1298 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1300 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (1288) */
/* 1302 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 1304 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1306 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 1308 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1310 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1312 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 1316 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 1318 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1320 */	NdrFcShort( 0xffe4 ),	/* Offset= -28 (1292) */
/* 1322 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1324 */	
			0x11, 0x14,	/* FC_RP [alloced_on_stack] [pointer_deref] */
/* 1326 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1328) */
/* 1328 */	
			0x13, 0x0,	/* FC_OP */
/* 1330 */	NdrFcShort( 0xffc6 ),	/* Offset= -58 (1272) */
/* 1332 */	
			0x11, 0x0,	/* FC_RP */
/* 1334 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1336) */
/* 1336 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 1338 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1340 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 1342 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1344 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1346 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 1348 */	
			0x11, 0x14,	/* FC_RP [alloced_on_stack] [pointer_deref] */
/* 1350 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1352) */
/* 1352 */	
			0x13, 0x0,	/* FC_OP */
/* 1354 */	NdrFcShort( 0xffee ),	/* Offset= -18 (1336) */
/* 1356 */	
			0x11, 0x0,	/* FC_RP */
/* 1358 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1360) */
/* 1360 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 1362 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1364 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 1366 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1368 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1370 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 1374 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 1376 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1378 */	NdrFcShort( 0xffaa ),	/* Offset= -86 (1292) */
/* 1380 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1382 */	
			0x11, 0x0,	/* FC_RP */
/* 1384 */	NdrFcShort( 0x3c ),	/* Offset= 60 (1444) */
/* 1386 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 1388 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1390 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 1392 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1394 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1396 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 1398 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 1400 */	NdrFcShort( 0x1c ),	/* 28 */
/* 1402 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 1404 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 1406 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1408 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1410 */	0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1412 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 1414 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 1416 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1418 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1420 */	0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1422 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 1424 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 1426 */	NdrFcShort( 0x14 ),	/* 20 */
/* 1428 */	NdrFcShort( 0x14 ),	/* 20 */
/* 1430 */	0x12, 0x0,	/* FC_UP */
/* 1432 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (1386) */
/* 1434 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 1436 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1438 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1440 */	0x8,		/* FC_LONG */
			0x6,		/* FC_SHORT */
/* 1442 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 1444 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 1446 */	NdrFcShort( 0x1c ),	/* 28 */
/* 1448 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 1450 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1452 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1454 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 1456 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 1458 */	NdrFcShort( 0x1c ),	/* 28 */
/* 1460 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1462 */	NdrFcShort( 0x3 ),	/* 3 */
/* 1464 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1466 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1468 */	0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1470 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 1472 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1474 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1476 */	0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1478 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 1480 */	NdrFcShort( 0x14 ),	/* 20 */
/* 1482 */	NdrFcShort( 0x14 ),	/* 20 */
/* 1484 */	0x12, 0x0,	/* FC_UP */
/* 1486 */	NdrFcShort( 0xff9c ),	/* Offset= -100 (1386) */
/* 1488 */	
			0x5b,		/* FC_END */

			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 1490 */	0x0,		/* 0 */
			NdrFcShort( 0xffa3 ),	/* Offset= -93 (1398) */
			0x5b,		/* FC_END */
/* 1494 */	
			0x11, 0x14,	/* FC_RP [alloced_on_stack] [pointer_deref] */
/* 1496 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1498) */
/* 1498 */	
			0x13, 0x0,	/* FC_OP */
/* 1500 */	NdrFcShort( 0x26 ),	/* Offset= 38 (1538) */
/* 1502 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 1504 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1506 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 1508 */	NdrFcShort( 0xc ),	/* 12 */
/* 1510 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1512 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 1514 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 1516 */	NdrFcShort( 0x14 ),	/* 20 */
/* 1518 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 1520 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 1522 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1524 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1526 */	0x13, 0x0,	/* FC_OP */
/* 1528 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (1502) */
/* 1530 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 1532 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1534 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1536 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 1538 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 1540 */	NdrFcShort( 0x14 ),	/* 20 */
/* 1542 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 1544 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1546 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1548 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 1550 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 1552 */	NdrFcShort( 0x14 ),	/* 20 */
/* 1554 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1556 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1558 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1560 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1562 */	0x13, 0x0,	/* FC_OP */
/* 1564 */	NdrFcShort( 0xffc2 ),	/* Offset= -62 (1502) */
/* 1566 */	
			0x5b,		/* FC_END */

			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 1568 */	0x0,		/* 0 */
			NdrFcShort( 0xffc9 ),	/* Offset= -55 (1514) */
			0x5b,		/* FC_END */
/* 1572 */	
			0x11, 0x0,	/* FC_RP */
/* 1574 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1576) */
/* 1576 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 1578 */	NdrFcShort( 0x2 ),	/* 2 */
/* 1580 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 1582 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1584 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1586 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 1588 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 1590 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1592) */
/* 1592 */	
			0x2f,		/* FC_IP */
			0x5c,		/* FC_PAD */
/* 1594 */	0x28,		/* Corr desc:  parameter, FC_LONG */
			0x0,		/*  */
/* 1596 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1598 */	NdrFcShort( 0x5 ),	/* Corr flags:  early, iid_is, */
/* 1600 */	
			0x11, 0x14,	/* FC_RP [alloced_on_stack] [pointer_deref] */
/* 1602 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1604) */
/* 1604 */	
			0x13, 0x0,	/* FC_OP */
/* 1606 */	NdrFcShort( 0x32 ),	/* Offset= 50 (1656) */
/* 1608 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 1610 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1612 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 1614 */	NdrFcShort( 0x18 ),	/* 24 */
/* 1616 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1618 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 1620 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1622 */	NdrFcShort( 0x38 ),	/* 56 */
/* 1624 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1626 */	NdrFcShort( 0x12 ),	/* Offset= 18 (1644) */
/* 1628 */	0x36,		/* FC_POINTER */
			0x36,		/* FC_POINTER */
/* 1630 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1632 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1634 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 1636 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1638 */	0xd,		/* FC_ENUM16 */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 1640 */	0x0,		/* 0 */
			NdrFcShort( 0xfe4f ),	/* Offset= -433 (1208) */
			0x5b,		/* FC_END */
/* 1644 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1646 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 1648 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1650 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 1652 */	
			0x13, 0x0,	/* FC_OP */
/* 1654 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (1608) */
/* 1656 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 1658 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1660 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x54,		/* FC_DEREFERENCE */
/* 1662 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1664 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 1666 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 1670 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 1672 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1674 */	NdrFcShort( 0xffca ),	/* Offset= -54 (1620) */
/* 1676 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1678 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 1680 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1682) */
/* 1682 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 1684 */	NdrFcLong( 0x39c13a55 ),	/* 968964693 */
/* 1688 */	NdrFcShort( 0x11e ),	/* 286 */
/* 1690 */	NdrFcShort( 0x11d0 ),	/* 4560 */
/* 1692 */	0x96,		/* 150 */
			0x75,		/* 117 */
/* 1694 */	0x0,		/* 0 */
			0x20,		/* 32 */
/* 1696 */	0xaf,		/* 175 */
			0xd8,		/* 216 */
/* 1698 */	0xad,		/* 173 */
			0xb3,		/* 179 */
/* 1700 */	
			0x11, 0x0,	/* FC_RP */
/* 1702 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1704) */
/* 1704 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 1706 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1708 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 1710 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1712 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1714 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 1716 */	
			0x11, 0x0,	/* FC_RP */
/* 1718 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1720) */
/* 1720 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 1722 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1724 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 1726 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1728 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1730 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 1734 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 1736 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1738 */	NdrFcShort( 0xfe42 ),	/* Offset= -446 (1292) */
/* 1740 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1742 */	
			0x11, 0x0,	/* FC_RP */
/* 1744 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1746) */
/* 1746 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 1748 */	NdrFcShort( 0x2 ),	/* 2 */
/* 1750 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 1752 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1754 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1756 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 1758 */	
			0x11, 0x0,	/* FC_RP */
/* 1760 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1762) */
/* 1762 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 1764 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1766 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 1768 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1770 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1772 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1774 */	NdrFcShort( 0xf962 ),	/* Offset= -1694 (80) */
/* 1776 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1778 */	
			0x11, 0x0,	/* FC_RP */
/* 1780 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1782) */
/* 1782 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 1784 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1786 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 1788 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1790 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1792 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 1794 */	
			0x11, 0x14,	/* FC_RP [alloced_on_stack] [pointer_deref] */
/* 1796 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1798) */
/* 1798 */	
			0x13, 0x0,	/* FC_OP */
/* 1800 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1802) */
/* 1802 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 1804 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1806 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x54,		/* FC_DEREFERENCE */
/* 1808 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1810 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1812 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 1814 */	
			0x11, 0x14,	/* FC_RP [alloced_on_stack] [pointer_deref] */
/* 1816 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1818) */
/* 1818 */	
			0x13, 0x0,	/* FC_OP */
/* 1820 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1822) */
/* 1822 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 1824 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1826 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x54,		/* FC_DEREFERENCE */
/* 1828 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1830 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1832 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 1834 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 1836 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1838 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1840 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1842 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1844 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1846 */	0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1848 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 1850 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 1852 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1854 */	
			0x11, 0x14,	/* FC_RP [alloced_on_stack] [pointer_deref] */
/* 1856 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1858) */
/* 1858 */	
			0x13, 0x0,	/* FC_OP */
/* 1860 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1862) */
/* 1862 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 1864 */	NdrFcShort( 0x2 ),	/* 2 */
/* 1866 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x54,		/* FC_DEREFERENCE */
/* 1868 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1870 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1872 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 1874 */	
			0x11, 0x14,	/* FC_RP [alloced_on_stack] [pointer_deref] */
/* 1876 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1878) */
/* 1878 */	
			0x13, 0x0,	/* FC_OP */
/* 1880 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1882) */
/* 1882 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 1884 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1886 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 1888 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1890 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1892 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 1896 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 1898 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1900 */	NdrFcShort( 0xfd4c ),	/* Offset= -692 (1208) */
/* 1902 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1904 */	
			0x11, 0x14,	/* FC_RP [alloced_on_stack] [pointer_deref] */
/* 1906 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1908) */
/* 1908 */	
			0x13, 0x0,	/* FC_OP */
/* 1910 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1912) */
/* 1912 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 1914 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1916 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 1918 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1920 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1922 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 1924 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 1926 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1928 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1930 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1932 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1934 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1936 */	0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1938 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 1940 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 1942 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */

			0x0
        }
    };

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ] = 
        {
            
            {
            VARIANT_UserSize
            ,VARIANT_UserMarshal
            ,VARIANT_UserUnmarshal
            ,VARIANT_UserFree
            }

        };



/* Standard interface: __MIDL_itf_OPCDA_0000, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IOPCServer, ver. 0.0,
   GUID={0x39c13a4d,0x011e,0x11d0,{0x96,0x75,0x00,0x20,0xaf,0xd8,0xad,0xb3}} */

#pragma code_seg(".orpc")
static const unsigned short IOPCServer_FormatStringOffsetTable[] =
    {
    0,
    96,
    144,
    192,
    228,
    270
    };

static const MIDL_STUBLESS_PROXY_INFO IOPCServer_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IOPCServer_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IOPCServer_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IOPCServer_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(9) _IOPCServerProxyVtbl = 
{
    &IOPCServer_ProxyInfo,
    &IID_IOPCServer,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* IOPCServer::AddGroup */ ,
    (void *) (INT_PTR) -1 /* IOPCServer::GetErrorString */ ,
    (void *) (INT_PTR) -1 /* IOPCServer::GetGroupByName */ ,
    (void *) (INT_PTR) -1 /* IOPCServer::GetStatus */ ,
    (void *) (INT_PTR) -1 /* IOPCServer::RemoveGroup */ ,
    (void *) (INT_PTR) -1 /* IOPCServer::CreateGroupEnumerator */
};

const CInterfaceStubVtbl _IOPCServerStubVtbl =
{
    &IID_IOPCServer,
    &IOPCServer_ServerInfo,
    9,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: IOPCServerPublicGroups, ver. 0.0,
   GUID={0x39c13a4e,0x011e,0x11d0,{0x96,0x75,0x00,0x20,0xaf,0xd8,0xad,0xb3}} */

#pragma code_seg(".orpc")
static const unsigned short IOPCServerPublicGroups_FormatStringOffsetTable[] =
    {
    318,
    366
    };

static const MIDL_STUBLESS_PROXY_INFO IOPCServerPublicGroups_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IOPCServerPublicGroups_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IOPCServerPublicGroups_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IOPCServerPublicGroups_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(5) _IOPCServerPublicGroupsProxyVtbl = 
{
    &IOPCServerPublicGroups_ProxyInfo,
    &IID_IOPCServerPublicGroups,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* IOPCServerPublicGroups::GetPublicGroupByName */ ,
    (void *) (INT_PTR) -1 /* IOPCServerPublicGroups::RemovePublicGroup */
};

const CInterfaceStubVtbl _IOPCServerPublicGroupsStubVtbl =
{
    &IID_IOPCServerPublicGroups,
    &IOPCServerPublicGroups_ServerInfo,
    5,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: IOPCBrowseServerAddressSpace, ver. 0.0,
   GUID={0x39c13a4f,0x011e,0x11d0,{0x96,0x75,0x00,0x20,0xaf,0xd8,0xad,0xb3}} */

#pragma code_seg(".orpc")
static const unsigned short IOPCBrowseServerAddressSpace_FormatStringOffsetTable[] =
    {
    408,
    444,
    486,
    546,
    588
    };

static const MIDL_STUBLESS_PROXY_INFO IOPCBrowseServerAddressSpace_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IOPCBrowseServerAddressSpace_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IOPCBrowseServerAddressSpace_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IOPCBrowseServerAddressSpace_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(8) _IOPCBrowseServerAddressSpaceProxyVtbl = 
{
    &IOPCBrowseServerAddressSpace_ProxyInfo,
    &IID_IOPCBrowseServerAddressSpace,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* IOPCBrowseServerAddressSpace::QueryOrganization */ ,
    (void *) (INT_PTR) -1 /* IOPCBrowseServerAddressSpace::ChangeBrowsePosition */ ,
    (void *) (INT_PTR) -1 /* IOPCBrowseServerAddressSpace::BrowseOPCItemIDs */ ,
    (void *) (INT_PTR) -1 /* IOPCBrowseServerAddressSpace::GetItemID */ ,
    (void *) (INT_PTR) -1 /* IOPCBrowseServerAddressSpace::BrowseAccessPaths */
};

const CInterfaceStubVtbl _IOPCBrowseServerAddressSpaceStubVtbl =
{
    &IID_IOPCBrowseServerAddressSpace,
    &IOPCBrowseServerAddressSpace_ServerInfo,
    8,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: IOPCGroupStateMgt, ver. 0.0,
   GUID={0x39c13a50,0x011e,0x11d0,{0x96,0x75,0x00,0x20,0xaf,0xd8,0xad,0xb3}} */

#pragma code_seg(".orpc")
static const unsigned short IOPCGroupStateMgt_FormatStringOffsetTable[] =
    {
    630,
    708,
    780,
    816
    };

static const MIDL_STUBLESS_PROXY_INFO IOPCGroupStateMgt_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IOPCGroupStateMgt_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IOPCGroupStateMgt_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IOPCGroupStateMgt_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(7) _IOPCGroupStateMgtProxyVtbl = 
{
    &IOPCGroupStateMgt_ProxyInfo,
    &IID_IOPCGroupStateMgt,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* IOPCGroupStateMgt::GetState */ ,
    (void *) (INT_PTR) -1 /* IOPCGroupStateMgt::SetState */ ,
    (void *) (INT_PTR) -1 /* IOPCGroupStateMgt::SetName */ ,
    (void *) (INT_PTR) -1 /* IOPCGroupStateMgt::CloneGroup */
};

const CInterfaceStubVtbl _IOPCGroupStateMgtStubVtbl =
{
    &IID_IOPCGroupStateMgt,
    &IOPCGroupStateMgt_ServerInfo,
    7,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: IOPCPublicGroupStateMgt, ver. 0.0,
   GUID={0x39c13a51,0x011e,0x11d0,{0x96,0x75,0x00,0x20,0xaf,0xd8,0xad,0xb3}} */

#pragma code_seg(".orpc")
static const unsigned short IOPCPublicGroupStateMgt_FormatStringOffsetTable[] =
    {
    864,
    900
    };

static const MIDL_STUBLESS_PROXY_INFO IOPCPublicGroupStateMgt_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IOPCPublicGroupStateMgt_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IOPCPublicGroupStateMgt_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IOPCPublicGroupStateMgt_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(5) _IOPCPublicGroupStateMgtProxyVtbl = 
{
    &IOPCPublicGroupStateMgt_ProxyInfo,
    &IID_IOPCPublicGroupStateMgt,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* IOPCPublicGroupStateMgt::GetState */ ,
    (void *) (INT_PTR) -1 /* IOPCPublicGroupStateMgt::MoveToPublic */
};

const CInterfaceStubVtbl _IOPCPublicGroupStateMgtStubVtbl =
{
    &IID_IOPCPublicGroupStateMgt,
    &IOPCPublicGroupStateMgt_ServerInfo,
    5,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: IOPCSyncIO, ver. 0.0,
   GUID={0x39c13a52,0x011e,0x11d0,{0x96,0x75,0x00,0x20,0xaf,0xd8,0xad,0xb3}} */

#pragma code_seg(".orpc")
static const unsigned short IOPCSyncIO_FormatStringOffsetTable[] =
    {
    930,
    990
    };

static const MIDL_STUBLESS_PROXY_INFO IOPCSyncIO_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IOPCSyncIO_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IOPCSyncIO_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IOPCSyncIO_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(5) _IOPCSyncIOProxyVtbl = 
{
    &IOPCSyncIO_ProxyInfo,
    &IID_IOPCSyncIO,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* IOPCSyncIO::Read */ ,
    (void *) (INT_PTR) -1 /* IOPCSyncIO::Write */
};

const CInterfaceStubVtbl _IOPCSyncIOStubVtbl =
{
    &IID_IOPCSyncIO,
    &IOPCSyncIO_ServerInfo,
    5,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: IOPCAsyncIO, ver. 0.0,
   GUID={0x39c13a53,0x011e,0x11d0,{0x96,0x75,0x00,0x20,0xaf,0xd8,0xad,0xb3}} */

#pragma code_seg(".orpc")
static const unsigned short IOPCAsyncIO_FormatStringOffsetTable[] =
    {
    1044,
    1110,
    1176,
    1224
    };

static const MIDL_STUBLESS_PROXY_INFO IOPCAsyncIO_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IOPCAsyncIO_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IOPCAsyncIO_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IOPCAsyncIO_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(7) _IOPCAsyncIOProxyVtbl = 
{
    &IOPCAsyncIO_ProxyInfo,
    &IID_IOPCAsyncIO,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* IOPCAsyncIO::Read */ ,
    (void *) (INT_PTR) -1 /* IOPCAsyncIO::Write */ ,
    (void *) (INT_PTR) -1 /* IOPCAsyncIO::Refresh */ ,
    (void *) (INT_PTR) -1 /* IOPCAsyncIO::Cancel */
};

const CInterfaceStubVtbl _IOPCAsyncIOStubVtbl =
{
    &IID_IOPCAsyncIO,
    &IOPCAsyncIO_ServerInfo,
    7,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: IOPCItemMgt, ver. 0.0,
   GUID={0x39c13a54,0x011e,0x11d0,{0x96,0x75,0x00,0x20,0xaf,0xd8,0xad,0xb3}} */

#pragma code_seg(".orpc")
static const unsigned short IOPCItemMgt_FormatStringOffsetTable[] =
    {
    1260,
    1314,
    1374,
    1422,
    1476,
    1530,
    1584
    };

static const MIDL_STUBLESS_PROXY_INFO IOPCItemMgt_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IOPCItemMgt_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IOPCItemMgt_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IOPCItemMgt_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(10) _IOPCItemMgtProxyVtbl = 
{
    &IOPCItemMgt_ProxyInfo,
    &IID_IOPCItemMgt,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* IOPCItemMgt::AddItems */ ,
    (void *) (INT_PTR) -1 /* IOPCItemMgt::ValidateItems */ ,
    (void *) (INT_PTR) -1 /* IOPCItemMgt::RemoveItems */ ,
    (void *) (INT_PTR) -1 /* IOPCItemMgt::SetActiveState */ ,
    (void *) (INT_PTR) -1 /* IOPCItemMgt::SetClientHandles */ ,
    (void *) (INT_PTR) -1 /* IOPCItemMgt::SetDatatypes */ ,
    (void *) (INT_PTR) -1 /* IOPCItemMgt::CreateEnumerator */
};

const CInterfaceStubVtbl _IOPCItemMgtStubVtbl =
{
    &IID_IOPCItemMgt,
    &IOPCItemMgt_ServerInfo,
    10,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: IEnumOPCItemAttributes, ver. 0.0,
   GUID={0x39c13a55,0x011e,0x11d0,{0x96,0x75,0x00,0x20,0xaf,0xd8,0xad,0xb3}} */

#pragma code_seg(".orpc")
static const unsigned short IEnumOPCItemAttributes_FormatStringOffsetTable[] =
    {
    1626,
    1674,
    1710,
    1740
    };

static const MIDL_STUBLESS_PROXY_INFO IEnumOPCItemAttributes_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IEnumOPCItemAttributes_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IEnumOPCItemAttributes_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IEnumOPCItemAttributes_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(7) _IEnumOPCItemAttributesProxyVtbl = 
{
    &IEnumOPCItemAttributes_ProxyInfo,
    &IID_IEnumOPCItemAttributes,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* IEnumOPCItemAttributes::Next */ ,
    (void *) (INT_PTR) -1 /* IEnumOPCItemAttributes::Skip */ ,
    (void *) (INT_PTR) -1 /* IEnumOPCItemAttributes::Reset */ ,
    (void *) (INT_PTR) -1 /* IEnumOPCItemAttributes::Clone */
};

const CInterfaceStubVtbl _IEnumOPCItemAttributesStubVtbl =
{
    &IID_IEnumOPCItemAttributes,
    &IEnumOPCItemAttributes_ServerInfo,
    7,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: IOPCDataCallback, ver. 0.0,
   GUID={0x39c13a70,0x011e,0x11d0,{0x96,0x75,0x00,0x20,0xaf,0xd8,0xad,0xb3}} */

#pragma code_seg(".orpc")
static const unsigned short IOPCDataCallback_FormatStringOffsetTable[] =
    {
    1776,
    1866,
    1956,
    2022
    };

static const MIDL_STUBLESS_PROXY_INFO IOPCDataCallback_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IOPCDataCallback_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IOPCDataCallback_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IOPCDataCallback_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(7) _IOPCDataCallbackProxyVtbl = 
{
    &IOPCDataCallback_ProxyInfo,
    &IID_IOPCDataCallback,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* IOPCDataCallback::OnDataChange */ ,
    (void *) (INT_PTR) -1 /* IOPCDataCallback::OnReadComplete */ ,
    (void *) (INT_PTR) -1 /* IOPCDataCallback::OnWriteComplete */ ,
    (void *) (INT_PTR) -1 /* IOPCDataCallback::OnCancelComplete */
};

const CInterfaceStubVtbl _IOPCDataCallbackStubVtbl =
{
    &IID_IOPCDataCallback,
    &IOPCDataCallback_ServerInfo,
    7,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: IOPCAsyncIO2, ver. 0.0,
   GUID={0x39c13a71,0x011e,0x11d0,{0x96,0x75,0x00,0x20,0xaf,0xd8,0xad,0xb3}} */

#pragma code_seg(".orpc")
static const unsigned short IOPCAsyncIO2_FormatStringOffsetTable[] =
    {
    2064,
    2124,
    2190,
    1224,
    2238,
    2274
    };

static const MIDL_STUBLESS_PROXY_INFO IOPCAsyncIO2_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IOPCAsyncIO2_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IOPCAsyncIO2_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IOPCAsyncIO2_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(9) _IOPCAsyncIO2ProxyVtbl = 
{
    &IOPCAsyncIO2_ProxyInfo,
    &IID_IOPCAsyncIO2,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* IOPCAsyncIO2::Read */ ,
    (void *) (INT_PTR) -1 /* IOPCAsyncIO2::Write */ ,
    (void *) (INT_PTR) -1 /* IOPCAsyncIO2::Refresh2 */ ,
    (void *) (INT_PTR) -1 /* IOPCAsyncIO2::Cancel2 */ ,
    (void *) (INT_PTR) -1 /* IOPCAsyncIO2::SetEnable */ ,
    (void *) (INT_PTR) -1 /* IOPCAsyncIO2::GetEnable */
};

const CInterfaceStubVtbl _IOPCAsyncIO2StubVtbl =
{
    &IID_IOPCAsyncIO2,
    &IOPCAsyncIO2_ServerInfo,
    9,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: IOPCItemProperties, ver. 0.0,
   GUID={0x39c13a72,0x011e,0x11d0,{0x96,0x75,0x00,0x20,0xaf,0xd8,0xad,0xb3}} */

#pragma code_seg(".orpc")
static const unsigned short IOPCItemProperties_FormatStringOffsetTable[] =
    {
    2310,
    2370,
    2430
    };

static const MIDL_STUBLESS_PROXY_INFO IOPCItemProperties_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IOPCItemProperties_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IOPCItemProperties_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IOPCItemProperties_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(6) _IOPCItemPropertiesProxyVtbl = 
{
    &IOPCItemProperties_ProxyInfo,
    &IID_IOPCItemProperties,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* IOPCItemProperties::QueryAvailableProperties */ ,
    (void *) (INT_PTR) -1 /* IOPCItemProperties::GetItemProperties */ ,
    (void *) (INT_PTR) -1 /* IOPCItemProperties::LookupItemIDs */
};

const CInterfaceStubVtbl _IOPCItemPropertiesStubVtbl =
{
    &IID_IOPCItemProperties,
    &IOPCItemProperties_ServerInfo,
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
    0x50002, /* Ndr library version */
    0,
    0x600016e, /* MIDL Version 6.0.366 */
    0,
    UserMarshalRoutines,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0   /* Reserved5 */
    };

const CInterfaceProxyVtbl * _OPCDA_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IOPCServerProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IOPCServerPublicGroupsProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IOPCBrowseServerAddressSpaceProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IOPCGroupStateMgtProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IOPCPublicGroupStateMgtProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IOPCSyncIOProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IOPCAsyncIOProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IOPCItemMgtProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IEnumOPCItemAttributesProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IOPCDataCallbackProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IOPCAsyncIO2ProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IOPCItemPropertiesProxyVtbl,
    0
};

const CInterfaceStubVtbl * _OPCDA_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IOPCServerStubVtbl,
    ( CInterfaceStubVtbl *) &_IOPCServerPublicGroupsStubVtbl,
    ( CInterfaceStubVtbl *) &_IOPCBrowseServerAddressSpaceStubVtbl,
    ( CInterfaceStubVtbl *) &_IOPCGroupStateMgtStubVtbl,
    ( CInterfaceStubVtbl *) &_IOPCPublicGroupStateMgtStubVtbl,
    ( CInterfaceStubVtbl *) &_IOPCSyncIOStubVtbl,
    ( CInterfaceStubVtbl *) &_IOPCAsyncIOStubVtbl,
    ( CInterfaceStubVtbl *) &_IOPCItemMgtStubVtbl,
    ( CInterfaceStubVtbl *) &_IEnumOPCItemAttributesStubVtbl,
    ( CInterfaceStubVtbl *) &_IOPCDataCallbackStubVtbl,
    ( CInterfaceStubVtbl *) &_IOPCAsyncIO2StubVtbl,
    ( CInterfaceStubVtbl *) &_IOPCItemPropertiesStubVtbl,
    0
};

PCInterfaceName const _OPCDA_InterfaceNamesList[] = 
{
    "IOPCServer",
    "IOPCServerPublicGroups",
    "IOPCBrowseServerAddressSpace",
    "IOPCGroupStateMgt",
    "IOPCPublicGroupStateMgt",
    "IOPCSyncIO",
    "IOPCAsyncIO",
    "IOPCItemMgt",
    "IEnumOPCItemAttributes",
    "IOPCDataCallback",
    "IOPCAsyncIO2",
    "IOPCItemProperties",
    0
};


#define _OPCDA_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _OPCDA, pIID, n)

int __stdcall _OPCDA_IID_Lookup( const IID * pIID, int * pIndex )
{
    IID_BS_LOOKUP_SETUP

    IID_BS_LOOKUP_INITIAL_TEST( _OPCDA, 12, 8 )
    IID_BS_LOOKUP_NEXT_TEST( _OPCDA, 4 )
    IID_BS_LOOKUP_NEXT_TEST( _OPCDA, 2 )
    IID_BS_LOOKUP_NEXT_TEST( _OPCDA, 1 )
    IID_BS_LOOKUP_RETURN_RESULT( _OPCDA, 12, *pIndex )
    
}

const ExtendedProxyFileInfo OPCDA_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _OPCDA_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _OPCDA_StubVtblList,
    (const PCInterfaceName * ) & _OPCDA_InterfaceNamesList,
    0, // no delegation
    & _OPCDA_IID_Lookup, 
    12,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#pragma optimize("", on )
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/

