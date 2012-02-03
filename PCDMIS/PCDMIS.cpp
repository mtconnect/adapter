// PCDMIS.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "internal.hpp"
#include "pcdmis_adapter.hpp"
#include "server.hpp"
#include "string_buffer.hpp"
#include <exception>

// Winsock library
#pragma comment(lib, "Ws2_32.lib")

//int _tmain(int argc, const _TCHAR* argv[])
//{
//  /* Construct the adapter and start the server */
//  PcDmisAdapter *adapter = new PcDmisAdapter(7878);
//  adapter->setName("Fake MTConnect Adapter");  // sets the service name for install
//  return adapter->main(argc,  argv);
//}

#include "guicon.h"

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
		// This will allow you to kill the process by killing the cmd console
		AttachConsole(ATTACH_PARENT_PROCESS);
		RedirectIOToConsole();
		std::cout << "hello world\n"; 
		//config.configfile() = ExeDirectory() + INI_FILENAME;

		//config.configureLogger();
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

		PcDmisAdapter *adapter = new PcDmisAdapter(7878);
		adapter->setName("PcDmisMTConnectAdapter");  // sets the service name for install
		return adapter->main(__argc,(const char **) __argv);
	}
	catch(std::exception e)
	{
		OutputDebugString(e.what());
	}
	catch(...)
	{
		OutputDebugString("Service terminated abnormally in WinMain\n");
	}
	::CoUninitialize();
	return 0;
}