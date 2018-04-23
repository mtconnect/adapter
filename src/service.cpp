//
// Copyright (c) 2008, AMT - The Association For Manufacturing Technology ("AMT")
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the AMT nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// DISCLAIMER OF WARRANTY. ALL MTCONNECT MATERIALS AND SPECIFICATIONS PROVIDED
// BY AMT, MTCONNECT OR ANY PARTICIPANT TO YOU OR ANY PARTY ARE PROVIDED "AS IS"
// AND WITHOUT ANY WARRANTY OF ANY KIND. AMT, MTCONNECT, AND EACH OF THEIR
// RESPECTIVE MEMBERS, OFFICERS, DIRECTORS, AFFILIATES, SPONSORS, AND AGENTS
// (COLLECTIVELY, THE "AMT PARTIES") AND PARTICIPANTS MAKE NO REPRESENTATION OR
// WARRANTY OF ANY KIND WHATSOEVER RELATING TO THESE MATERIALS, INCLUDING, WITHOUT
// LIMITATION, ANY EXPRESS OR IMPLIED WARRANTY OF NONINFRINGEMENT,
// MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
//
// LIMITATION OF LIABILITY. IN NO EVENT SHALL AMT, MTCONNECT, ANY OTHER AMT
// PARTY, OR ANY PARTICIPANT BE LIABLE FOR THE COST OF PROCURING SUBSTITUTE GOODS
// OR SERVICES, LOST PROFITS, LOSS OF USE, LOSS OF DATA OR ANY INCIDENTAL,
// CONSEQUENTIAL, INDIRECT, SPECIAL OR PUNITIVE DAMAGES OR OTHER DIRECT DAMAGES,
// WHETHER UNDER CONTRACT, TORT, WARRANTY OR OTHERWISE, ARISING IN ANY WAY OUT OF
// THIS AGREEMENT, USE OR INABILITY TO USE MTCONNECT MATERIALS, WHETHER OR NOT
// SUCH PARTY HAD ADVANCE NOTICE OF THE POSSIBILITY OF SUCH DAMAGES.
//
#include "service.hpp"
#include "string.h"


MTConnectService::MTConnectService() :
	mName{0},
	mIsService(false),
	mDebug(false)
{
}

void MTConnectService::setName(const char *name)
{
	strncpy_s(mName, sizeof(mName), name,  sizeof(mName) - 1);
}

void MTConnectService::initialize(int aArgc, const char *aArgv[])
{
	if (!gLogger)
	{
		if (mIsService)
			gLogger = new ServiceLogger();
		else
			gLogger = new Logger();
	}

	if (mDebug)
		gLogger->setLogLevel(Logger::eDEBUG);
}

#ifdef WIN32

#include <internal.hpp>
#include <strsafe.h>

#pragma comment(lib, "advapi32.lib")

#define SVC_ERROR		((DWORD)0xC0000001L)
#define SVC_WARNING		((DWORD)0x90000001L)
#define SVC_INFO		((DWORD)0x50000001L)

SERVICE_STATUS			gSvcStatus;
SERVICE_STATUS_HANDLE	gSvcStatusHandle;

VOID WINAPI SvcCtrlHandler(DWORD);
VOID WINAPI SvcMain(DWORD, LPTSTR *);

VOID ReportSvcStatus(DWORD, DWORD, DWORD);
VOID SvcInit(DWORD, LPTSTR *);
VOID SvcReportEvent(LPTSTR);

MTConnectService *gService = nullptr;



//
// Purpose:
//   Entry point for the process
//
// Parameters:
//   None
//
// Return value:
//   None
//
int MTConnectService::main(int argc, const char *argv[])
{
	// If command-line parameter is "install", install the service.
	// Otherwise, the service is probably being started by the SCM.

	if (argc > 1)
	{
		if (!stricmp(argv[1], "debug"))
			mDebug = true;

		initialize(argc - 2, argv + 2);

		if (!stricmp(argv[1], "install"))
		{
			install(argc - 2, argv + 2);
			return 0;
		}
		else if (!stricmp(argv[1], "remove"))
		{
			remove();
			return 0;
		}
		else if (!stricmp(argv[1], "debug"))
		{
			gLogger->setLogLevel(Logger::eDEBUG);
			start();
			return 0;
		}
		else if (!stricmp(argv[1], "run"))
		{
			start();
			return 0;
		}
	}

	mIsService = true;
	SERVICE_TABLE_ENTRY DispatchTable[] =
	{
		{ mName, (LPSERVICE_MAIN_FUNCTION) SvcMain },
		{ nullptr, nullptr }
	};

	gService = this;

	if (!StartServiceCtrlDispatcherA(DispatchTable))
		SvcReportEvent("StartServiceCtrlDispatcher");

	return 0;
}

//
// Purpose:
//   Installs a service in the SCM database
//
// Parameters:
//   None
//
// Return value:
//   None
//
void MTConnectService::install(int argc, const char *argv[])
{
	char szPath[MAX_PATH] = {0};
	if (!GetModuleFileNameA(nullptr, szPath, MAX_PATH))
	{
		printf("Cannot install service (%d)\n", GetLastError());
		return;
	}

	// Get a handle to the SCM database.
	auto schSCManager = OpenSCManagerA(
		nullptr,					// local computer
		nullptr,					// ServicesActive database
		SC_MANAGER_ALL_ACCESS);		// full access rights

	if (!schSCManager)
	{
		printf("OpenSCManager failed (%d)\n", GetLastError());
		return;
	}

	auto schService = OpenServiceA(schSCManager, mName, SC_MANAGER_ALL_ACCESS);
	if (schService)
	{
		if (! ChangeServiceConfigA(
			schService,						// handle of service
			SERVICE_WIN32_OWN_PROCESS |
			SERVICE_INTERACTIVE_PROCESS,	// service type: no change
			SERVICE_AUTO_START,				// service start type
			SERVICE_NO_CHANGE,				// error control: no change
			szPath,							// binary path: no change
			nullptr,						// load order group: no change
			nullptr,						// tag ID: no change
			nullptr,						// dependencies: no change
			nullptr,						// account name: no change
			nullptr,						// password: no change
			nullptr) )						// display name: no change
		{
			printf("ChangeServiceConfig failed (%d)\n", GetLastError());
		}
		else
			printf("Service updated successfully.\n");
	}
	else
	{
		// Create the service
		schService = CreateServiceA(
				 schSCManager,					// SCM database
				 mName,							// name of service
				 mName,							// service name to display
				 SERVICE_ALL_ACCESS,			// desired access
				 SERVICE_WIN32_OWN_PROCESS |
				 SERVICE_INTERACTIVE_PROCESS,	// service type
				 SERVICE_AUTO_START,			// start type
				 SERVICE_ERROR_NORMAL,			// error control type
				 szPath,						// path to service's binary
				 nullptr,						// no load ordering group
				 nullptr,						// no tag identifier
				 nullptr,						// no dependencies
				 nullptr,						// LocalSystem account
				 nullptr);						// no password

		if (!schService)
		{
			printf("CreateService failed (%d)\n", GetLastError());
			CloseServiceHandle(schSCManager);
			return;
		}
		else
			printf("Service installed successfully\n");
	}

	CloseServiceHandle(schService); schService = nullptr;
	CloseServiceHandle(schSCManager); schSCManager = nullptr;

	HKEY software(nullptr);
	auto res = RegOpenKeyA(HKEY_LOCAL_MACHINE, "SOFTWARE", &software);
	if (res != ERROR_SUCCESS)
	{
		printf("Could not open software key: %d\n", res);
		return;
	}

	HKEY mtc(nullptr);
	res = RegOpenKeyA(software, "MTConnect", &mtc);
	if (res != ERROR_SUCCESS)
	{
		//printf("Could not open MTConnect, creating: %d\n", res);
		res = RegCreateKeyA(software, "MTConnect", &mtc);
		if (res != ERROR_SUCCESS)
		{
			RegCloseKey(software);
			printf("Could not create MTConnect: %d\n", res);
			return;
		}
	}

	RegCloseKey(software);

	// Create Service Key
	HKEY adapter(nullptr);
	res = RegOpenKeyA(mtc, mName, &adapter);
	if (res != ERROR_SUCCESS)
	{
		//printf("Could not open %s, creating: %d\n", mName, res);
		res = RegCreateKeyA(mtc, mName, &adapter);
		if (res != ERROR_SUCCESS)
		{
			RegCloseKey(mtc);
			printf("Could not create %s: %d\n", mName, res);
			return;
		}
	}

	RegCloseKey(mtc);

	char arguments[2048] = {0};
	// TODO: create registry entries for arguments to be passed in later to create the adapter multi_sz
	int d = 0;
	for (int i = 0; i < argc; i++)
	{
		strcpy_s(arguments + d, 2048 - d, argv[i]);
		d += strlen(arguments + d) + 1;
	}

	arguments[d] = '\0';
	RegSetValueExA(adapter, "Arguments", 0, REG_MULTI_SZ, (const BYTE *) arguments, d);
	RegCloseKey(adapter);
}


void MTConnectService::remove()
{
	auto manager = OpenSCManagerA(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
	if (!manager)
	{
		printf("Could not open Service Control Manager");
		return;
	}

	auto service = ::OpenServiceA(manager, mName, SERVICE_ALL_ACCESS);
	CloseServiceHandle(manager);

	if (!service)
	{
		printf("Could not open Service ");
		return;
	}

	if (!::DeleteService(service))
		printf("Could delete service %s\n", mName);
	else
		printf("Successfully removed service %s\n", mName);

	::CloseServiceHandle(service);
}


//
// Purpose:
//   Entry point for the service
//
// Parameters:
//   dwArgc - Number of arguments in the lpszArgv array
//   lpszArgv - Array of strings. The first string is the name of
//     the service and subsequent strings are passed by the process
//     that called the StartService function to start the service.
//
// Return value:
//   None.
//
VOID WINAPI SvcMain(DWORD dwArgc, LPTSTR *lpszArgv)
{
	// Register the handler function for the service
	char path[MAX_PATH] = {0};

	if (!GetModuleFileNameA(nullptr, path, MAX_PATH))
	{
		printf("Cannot get path of executable (%d)\n", GetLastError());
		return;
	}

	auto cp = strrchr(path, '\\');
	if (cp != nullptr)
	{
		*cp = '\0';
		SetCurrentDirectoryA(path);
	}

	gService->setName(lpszArgv[0]);
	gSvcStatusHandle = RegisterServiceCtrlHandlerA(
		gService->name(),
		SvcCtrlHandler);

	if (!gSvcStatusHandle)
	{
		SvcReportEvent("RegisterServiceCtrlHandler");
		return;
	}

	// These SERVICE_STATUS members remain as set here
	gSvcStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	gSvcStatus.dwServiceSpecificExitCode = 0;

	// Report initial status to the SCM
	ReportSvcStatus(SERVICE_START_PENDING, NO_ERROR, 10000ul);

	// Perform service-specific initialization and work.
	Sleep(20000);

	SvcInit(dwArgc, lpszArgv);
}


//
// Purpose:
//   The service code
//
// Parameters:
//   dwArgc - Number of arguments in the lpszArgv array
//   lpszArgv - Array of strings. The first string is the name of
//     the service and subsequent strings are passed by the process
//     that called the StartService function to start the service.
//
// Return value:
//   None
//
VOID SvcInit(DWORD dwArgc, LPTSTR *lpszArgv)
{
	// Get the real arguments from the registry
	char key[1024] = {0};
	snprintf(key, 1022, "SOFTWARE\\MTConnect\\%s", gService->name());
	key[1023] = '\0';

	HKEY adapter(nullptr);
	auto res = RegOpenKeyExA(HKEY_LOCAL_MACHINE, key, 0, KEY_READ, &adapter);
	if (res != ERROR_SUCCESS)
	{
		SvcReportEvent("RegOpenKey: Could not open Adapter");
		ReportSvcStatus(SERVICE_STOPPED, 1ul, 0ul);
		return;
	}

	char *argp[64] = {0};
	BYTE arguments[2048] = {0};
	DWORD len = 2047ul, type, argc = 0ul;
	res = RegQueryValueExA(adapter, "Arguments", 0, &type, (BYTE *) arguments, &len);
	if (res == ERROR_SUCCESS)
	{
		DWORD i = 0ul;
		while (i < len)
		{
			argp[argc] = (char *) arguments + i;
			i += strlen((char *) arguments + i) + 1ul;
			argc++;
		}

		argp[argc] = 0;
	}
	else
	{
		SvcReportEvent("RegOpenKey: Could not get Arguments");
		RegCloseKey(adapter);
		ReportSvcStatus(SERVICE_STOPPED, 1ul, 0ul);
		return;
	}

	gService->initialize(argc, (const char **) argp);

	// TO_DO: Declare and set any required variables.
	//   Be sure to periodically call ReportSvcStatus() with
	//   SERVICE_START_PENDING. If initialization fails, call
	//   ReportSvcStatus with SERVICE_STOPPED.

	// Create an event. The control handler function, SvcCtrlHandler,
	// signals this event when it receives the stop control code.

	// Report running status when initialization is complete.
	ReportSvcStatus(SERVICE_RUNNING, NO_ERROR, 0ul);

	// TO_DO: Perform work until service stops.
	gService->start();
	ReportSvcStatus(SERVICE_STOPPED, NO_ERROR, 0ul);
}


//
// Purpose:
//   Sets the current service status and reports it to the SCM.
//
// Parameters:
//   dwCurrentState - The current state (see SERVICE_STATUS)
//   dwWin32ExitCode - The system error code
//   dwWaitHint - Estimated time for pending operation,
//     in milliseconds
//
// Return value:
//   None
//
VOID ReportSvcStatus(
	DWORD dwCurrentState,
	DWORD dwWin32ExitCode,
	DWORD dwWaitHint)
{
	static DWORD dwCheckPoint = 1ul;

	// Fill in the SERVICE_STATUS structure.

	gSvcStatus.dwCurrentState = dwCurrentState;
	gSvcStatus.dwWin32ExitCode = dwWin32ExitCode;
	gSvcStatus.dwWaitHint = dwWaitHint;

	if (dwCurrentState == SERVICE_START_PENDING)
		gSvcStatus.dwControlsAccepted = 0;
	else
		gSvcStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;

	if (dwCurrentState == SERVICE_RUNNING ||
		dwCurrentState == SERVICE_STOPPED )
	{
		gSvcStatus.dwCheckPoint = 0;
	}
	else
		gSvcStatus.dwCheckPoint = dwCheckPoint++;

	// Report the status of the service to the SCM.
	SetServiceStatus(gSvcStatusHandle, &gSvcStatus);
}


//
// Purpose:
//   Called by SCM whenever a control code is sent to the service
//   using the ControlService function.
//
// Parameters:
//   dwCtrl - control code
//
// Return value:
//   None
//
VOID WINAPI SvcCtrlHandler(DWORD dwCtrl)
{
	// Handle the requested control code.

	switch (dwCtrl)
	{
	case SERVICE_CONTROL_STOP:
		ReportSvcStatus(SERVICE_STOP_PENDING, NO_ERROR, 0ul);

		if (gService)
			gService->stop();

		ReportSvcStatus(gSvcStatus.dwCurrentState, NO_ERROR, 0ul);

		return;

	case SERVICE_CONTROL_INTERROGATE:
		break;

	default:
		break;
	}

}


//
// Purpose:
//   Logs messages to the event log
//
// Parameters:
//   szFunction - name of function that failed
//
// Return value:
//   None
//
// Remarks:
//   The service must have an entry in the Application event log.
//
VOID SvcReportEvent(LPTSTR functionName)
{
	HANDLE hEventSource(nullptr);
	LPCTSTR lpszStrings[2] = {nullptr, nullptr};
	char Buffer[80] = {0};

	hEventSource = RegisterEventSourceA(nullptr, gService->name());

	if (hEventSource)
	{
		snprintf(Buffer, _countof(Buffer), "%-60s failed with %d", functionName, GetLastError());

		lpszStrings[0] = gService->name();
		lpszStrings[1] = Buffer;

		ReportEventA(hEventSource,		// event log handle
				EVENTLOG_ERROR_TYPE,	// event type
				0,						// event category
				SVC_ERROR,				// event identifier
				nullptr,				// no security identifier
				2,						// size of lpszStrings array
				0ul,					// no binary data
				lpszStrings,			// array of strings
				nullptr);				// no binary data

		DeregisterEventSource(hEventSource);
	}
}


VOID SvcLogEvent(WORD type, DWORD id, LPSTR text)
{
	HANDLE hEventSource(nullptr);
	LPCTSTR lpszStrings[3] = {nullptr};

	hEventSource = RegisterEventSourceA(nullptr, gService->name());
	if (hEventSource)
	{
		lpszStrings[0] = gService->name();
		lpszStrings[1] = "\n\n";
		lpszStrings[2] = text;

		ReportEventA(hEventSource,	// event log handle
				type,				// event type
				0,					// event category
				id,					// event identifier
				nullptr,			// no security identifier
				3,					// size of lpszStrings array
				0,					// no binary data
				lpszStrings,		// array of strings
				nullptr);			// no binary data

		DeregisterEventSource(hEventSource);
	}
}


void ServiceLogger::error(const char *inputFormat, ...)
{
	char buffer[LOGGER_BUFFER_SIZE] = {0};
	va_list args;
	va_start(args, inputFormat);
	SvcLogEvent(
		EVENTLOG_ERROR_TYPE,
		SVC_ERROR,
		(LPSTR)format(buffer, LOGGER_BUFFER_SIZE, inputFormat, args));
	va_end(args);
}


void ServiceLogger::warning(const char *inputFormat, ...)
{
	char buffer[LOGGER_BUFFER_SIZE] = {0};
	va_list args;
	va_start(args, inputFormat);
	SvcLogEvent(
		EVENTLOG_WARNING_TYPE,
		SVC_WARNING, (LPSTR)format(buffer, LOGGER_BUFFER_SIZE, inputFormat, args));
	va_end(args);
}


void ServiceLogger::info(const char *inputFormat, ...)
{
	char buffer[LOGGER_BUFFER_SIZE] = {0};
	va_list args;
	va_start(args, inputFormat);
	SvcLogEvent(
		EVENTLOG_INFORMATION_TYPE,
		SVC_INFO,
		(LPSTR) format(buffer, LOGGER_BUFFER_SIZE, inputFormat, args));
	va_end(args);
}


void ServiceLogger::debug(const char *inputFormat, ...)
{
	// Debug service logging is not supported
}

#else

int MTConnectService::main(int argc, const char *argv[])
{
	initialize(argc - 1, argv + 1);
	start();
	return 0;
}

void MTConnectService::install(int argc, const char *argv[])
{
}

#endif
