// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>


#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit
#pragma warning ( disable : 4018 )	// warning C4018: '<' : signed/unsigned mismatch
#pragma warning ( disable : 4996 )	// 'sprintf': This function or variable may be unsafe. Consider using sprintf_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.

#include <atlbase.h>
#include <atlstr.h>
#include <comdef.h>

#define ALARMS