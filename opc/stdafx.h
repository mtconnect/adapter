// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__E42FB3BF_2FE1_11D4_BF9E_00C04F5178B5__INCLUDED_)
#define AFX_STDAFX_H__E42FB3BF_2FE1_11D4_BF9E_00C04F5178B5__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC OLE automation classes
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

///////////////////////////////////
//
//  Step 3
//  include von externen Headerfiles 
#include <atlbase.h>
#include "opcda.h"
#include "opccomn.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__E42FB3BF_2FE1_11D4_BF9E_00C04F5178B5__INCLUDED_)
