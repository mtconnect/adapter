// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef DEBUG
#define DEBUG
#endif

#ifndef WINVER                          // Specifies that the minimum required platform is Windows Vista.
#define WINVER 0x0600           // Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT            // Specifies that the minimum required platform is Windows Vista.
#define _WIN32_WINNT 0x0600     // Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINDOWS          // Specifies that the minimum required platform is Windows 98.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE                       // Specifies that the minimum required platform is Internet Explorer 7.0.
#define _WIN32_IE 0x0700        // Change this to the appropriate value to target other versions of IE.
#endif


#include <stdio.h>
#include <tchar.h>


#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#include <atlbase.h>
#include <atlstr.h>

#pragma warning ( disable : 4018 )	// warning C4018: '<' : signed/unsigned mismatch
#pragma warning ( disable : 4996 )	// 'sprintf': This function or variable may be unsafe. Consider using sprintf_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
#include "comdef.h"
#include <exception>
#include <string>

extern DWORD FindProcessId(std::string processName);
extern _bstr_t ErrorFormatMessage(HRESULT dwLastErr );
struct HRESULT_EX
{
private:
    HRESULT _hr;
public:
    void operator = (const HRESULT& hr)
    { 
         _hr = hr;
 
        if(FAILED(hr))
			throw std::exception((LPCSTR) ErrorFormatMessage(hr));
    }
 
    operator HRESULT() {
         return _hr;
    }
};


#include <iostream>
#define OutputDebugString(X)   std::cerr << X
