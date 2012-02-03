TODO:

1) SHDR Tags to finish:

1.	uuid
2.	manufacturer
3.	station
4.	serialNumber

2) Connecting to the Siemens 840D to receive shutdown notifications.
#import "IregieSvr.dll"

	CComPtr<IREGIESVRLib::IRegieSvr> server;
	
	OutputDebugString("Connecting to Siemens 840D Regie Server");
	CComVariant v1=hwndMainFrame;
	CComVariant v2;
	::ShowWindow(hwndMainFrame, SW_MINIMIZE);
	::ShowWindow(hwndMainFrame, SW_SHOW);
	if(FAILED(hr=server.CoCreateInstance(__uuidof(RegieSvr), NULL, CLSCTX_SERVER)))
		throw CString (_T("CoCreateInstance Siemens 830D RegieSvr FAILED"));
	if(FAILED(hr=server->InitSvr(v1,v2)))
		throw CString (_T("InitSvr(v1,v2) FAILED"));
	if(FAILED(hr=server->InitCompleteEx()))
		throw CString (_T("InitCompleteEx FAILED"));

