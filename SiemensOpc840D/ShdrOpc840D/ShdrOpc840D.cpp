// ShdrOpc840D.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <exception>
#include "MtcOpcAdapter.hpp"
#include "Logger.h"

#pragma comment(lib, "Ws2_32.lib")

#pragma comment(linker, "/SUBSYSTEM:WINDOWS")
#pragma warning(disable: 4247) //warning C4297: 'WinMain' : function assumed not to throw an exception but does

int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPTSTR    lpCmdLine,
					 int       nCmdShow)
{
	HRESULT hr ;
	try 
	{


		if(FAILED(hr=::CoInitialize(NULL)))
			throw  std::exception("CoInitialize failed\n");

		if(FAILED(hr= ::CoInitializeSecurity( NULL, //Points to security descriptor 
			-1, //Count of entries in asAuthSvc 
			NULL, //Array of names to register 
			NULL, //Reserved for future use 
			RPC_C_AUTHN_LEVEL_NONE, //The default authentication //level for proxies 
			RPC_C_IMP_LEVEL_IDENTIFY, //The default impersonation //level for proxies 
			NULL, //Reserved; must be set to  NULL
			EOAC_NONE, //Additional client or //server-side capabilities 
			NULL //Reserved for future use 
			)))  
			throw std::exception("CoInitializeSecurity failed\n");

		MtcOpcAdapter *adapter = new MtcOpcAdapter(7878);
		adapter->setName("MtcOpcAdapter");  // sets the service name for install
		return adapter->main(__argc,(const char **) __argv);

	}
	catch(std::exception e)
	{
		OutputDebugString(e.what());
	}
	catch(...)
	{
		OutputDebugString("Service terminated abnormally in main\n");
	}
	::CoUninitialize();
	return -1;
}
