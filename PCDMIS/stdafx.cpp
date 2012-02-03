// stdafx.cpp : source file that includes just the standard includes
// PCDMIS.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#include <string>
#include <algorithm> 
_bstr_t ErrorFormatMessage(HRESULT dwLastErr )
{
	TCHAR * pTemp;
	_bstr_t errmsg(L"");
	int nLen = ::FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_IGNORE_INSERTS |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, 
		dwLastErr,
		MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
		(LPTSTR) &pTemp, 
		1, 
		NULL );

	errmsg=(LPCTSTR) pTemp;
	::LocalFree( pTemp );
	return errmsg;
}


static void MakeLower(std::string &str)
{
	std::transform(str.begin(),str.end(),str.begin(),tolower);
}

#include <tlhelp32.h>


/*
	DWORD processID = FindProcessId(processName);

	if ( processID == 0 )
		std::wcout << "Could not find " << processName.c_str() << std::endl;
	else
		std::wcout << "Process ID is " << processID << std::endl;
*/

DWORD FindProcessId(std::string processName)
{
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if ( processesSnapshot == INVALID_HANDLE_VALUE )
		return 0;

	Process32First(processesSnapshot, &processInfo);
	std::string nextprocessname = processInfo.szExeFile;
	MakeLower(processName);
	MakeLower(nextprocessname);

	if ( !processName.compare(nextprocessname) )
	{
		CloseHandle(processesSnapshot);
		return processInfo.th32ProcessID;
	}

	while ( Process32Next(processesSnapshot, &processInfo) )
	{
		nextprocessname = processInfo.szExeFile;
		MakeLower(nextprocessname);
		if ( !processName.compare(nextprocessname) )
		{
			CloseHandle(processesSnapshot);
			return processInfo.th32ProcessID;
		}
	}
	
	CloseHandle(processesSnapshot);
	return 0;
}