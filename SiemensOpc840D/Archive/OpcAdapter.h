// **************************************************************************
// OpcAdapter.h : Declaration of the CMachineState
//
// Description:
//
// DISCLAIMER:
// This software was developed by U.S. Government employees as part of
// their official duties and is not subject to copyright. No warranty implied 
// or intended.
// **************************************************************************
#pragma once
#include "OPCBackEnd.h"
#include "wininet.h"
#include <algorithm>
//#import "IregieSvr.dll"
#include <fstream>
//#include "atlhttp.h"
#include "Config.h"
#include "atlutil.h"

class MtcOpcAdapter;

enum CNCOPCServerStatus { CONNECTED=0, WAITING=-1, TRYING = -2, CNC_NOTRUNNING=-2,  REMOTE_CONNECTION_FAILED=-3, 
ATTEMPTING_REMOTE_CONNECTION=-4, DISCONNECTED=-5 , RECONFIGURING=-6} ;

class COpcAdapter : public  COPCBackEnd<COPCItem>, public IWorkerThreadClient
{
public:

	typedef  COPCBackEnd<COPCItem> COPCBaseClass;

	COpcAdapter(MtcOpcAdapter * config, // mtconnect agent
								 std::string machine,  // ip address or pc name
								 std::string device,  // mtconnect device name
								 std::string tagsection); // tag name for opc mapping
	~COpcAdapter();
	virtual void OnApplicationInit();
	virtual void OnApplicationAction();
	virtual void OnApplicationClose();
	virtual int ReadOPCSettings(tstring filename);

	virtual std::string GetSymbolValue(tstring section, tstring tag, const char * defaultval) ;
	virtual std::vector<std::string>  GetSectionKeys(tstring section);


	virtual void Stop(); 
	virtual void Start();
	virtual bool Join(int nWait);
	void OnValueChanged(ItemList & _opcitems);
	//void SetMTCTagValue(tstring device, tstring tag, tstring value);
	long &DelayTime() { return _nDelayTime; }

	CWorkerThread _workerthread;
	HRESULT Initialize();
	HRESULT Execute(DWORD_PTR dwParam, HANDLE hObject);

	//LRESULT OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//static UINT __stdcall entry(void *ptr);
	bool						_bRunning;	// replace with handle
	CHandle						_hRunMutex;  // "Keep Running" mutex 
	CHandle						_hJoinMutex;
	CHandle						_hThread ;
	unsigned int				_threadID;
	long						_nDelayTime;

	/////////////////////////////////////////////////////////////////////////
	std::string					_szConfigFile;

	Utilities::Config           _config;
	CNCOPCServerStatus			_eOPCStatus,_eOPCLastStatus;
	std::string					_status;
	std::string					_lastblock;
	MtcOpcAdapter *		        _mtcadapter;
	std::string					_device;



#ifdef ALARMS
	Utilities::Config			_alarmconfig;
#endif
};