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
#include <fstream>
#include "Config.h"
#include "atlutil.h"

class MtcOpcAdapter;

class COpcAdapter : public  COPCBackEnd<COPCItem>, public IWorkerThreadClient
{
	typedef  COPCBackEnd<COPCItem> COPCBaseClass;
public:
	COpcAdapter(MtcOpcAdapter * config, // mtconnect agent
								 std::string machine,  // ip address or pc name
								 std::string device,  // mtconnect device name
								 std::string tagsection); // tag name for opc mapping
	~COpcAdapter();

	virtual int ReadOPCSettings(tstring filename);
	virtual std::string GetSymbolValue(tstring section, tstring tag, const char * defaultval) ;
	virtual std::vector<std::string>  GetSectionKeys(tstring section);
	
	virtual void ErrorMessage(std::string errmsg) ;
	virtual void DebugMessage(std::string errmsg) ;

	virtual void Stop(); 
	virtual void Start();
	virtual bool Join(int nWait);
	virtual void OnValueChanged(ItemList & _opcitems);
	virtual void ExportValues();
	virtual void Off() ;
	virtual void On() ;
		
	// Threading
	HRESULT Initialize();
	HRESULT Execute(DWORD_PTR dwParam, HANDLE hObject);
	HRESULT CloseHandle(HANDLE);

	std::string & Device() { return _device; }

	bool						_bInitialized;
	CHandle						_hJoinMutex;
	HANDLE						_hTimer;
	CWorkerThread<>				_workerthread;

	/////////////////////////////////////////////////////////////////////////
protected:
	Utilities::Config           _config;
	std::string					_device;
	std::string					_lastblock;
	MtcOpcAdapter *		        _mtcadapter;
	std::string					_status;
	std::string					_szConfigFile;

#ifdef ALARMS
	Utilities::Config			_alarmconfig;
#endif
};