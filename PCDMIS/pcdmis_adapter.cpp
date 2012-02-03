/*
* Copyright (c) 2008, AMT – The Association For Manufacturing Technology (“AMT”)
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of the AMT nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* DISCLAIMER OF WARRANTY. ALL MTCONNECT MATERIALS AND SPECIFICATIONS PROVIDED
* BY AMT, MTCONNECT OR ANY PARTICIPANT TO YOU OR ANY PARTY ARE PROVIDED "AS IS"
* AND WITHOUT ANY WARRANTY OF ANY KIND. AMT, MTCONNECT, AND EACH OF THEIR
* RESPECTIVE MEMBERS, OFFICERS, DIRECTORS, AFFILIATES, SPONSORS, AND AGENTS
* (COLLECTIVELY, THE "AMT PARTIES") AND PARTICIPANTS MAKE NO REPRESENTATION OR
* WARRANTY OF ANY KIND WHATSOEVER RELATING TO THESE MATERIALS, INCLUDING, WITHOUT
* LIMITATION, ANY EXPRESS OR IMPLIED WARRANTY OF NONINFRINGEMENT,
* MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. 

* LIMITATION OF LIABILITY. IN NO EVENT SHALL AMT, MTCONNECT, ANY OTHER AMT
* PARTY, OR ANY PARTICIPANT BE LIABLE FOR THE COST OF PROCURING SUBSTITUTE GOODS
* OR SERVICES, LOST PROFITS, LOSS OF USE, LOSS OF DATA OR ANY INCIDENTAL,
* CONSEQUENTIAL, INDIRECT, SPECIAL OR PUNITIVE DAMAGES OR OTHER DIRECT DAMAGES,
* WHETHER UNDER CONTRACT, TORT, WARRANTY OR OTHERWISE, ARISING IN ANY WAY OUT OF
* THIS AGREEMENT, USE OR INABILITY TO USE MTCONNECT MATERIALS, WHETHER OR NOT
* SUCH PARTY HAD ADVANCE NOTICE OF THE POSSIBILITY OF SUCH DAMAGES.
*/
#include "stdafx.h"
#include "internal.hpp"
#include "pcdmis_adapter.hpp"
#include <string>
#include <comdef.h>
#include "StdStringFcn.h"

static std::string ExeDirectory()
{
	TCHAR buf[1000];
	GetModuleFileName(NULL, buf, 1000);
	std::string path(buf);
	path=path.substr( 0, path.find_last_of( '\\' ) +1 );
	return path;
}

static void trans_func( unsigned int u, EXCEPTION_POINTERS* pExp )
{
	OutputDebugString( StdStringFormat("PcDmisAdapter trans_func - Code = 0x%x\n",  
		pExp->ExceptionRecord->ExceptionCode).c_str() );
	throw std::exception();
}

PcDmisAdapter::PcDmisAdapter(int aPort)
: Adapter(aPort, 1000), 
mAvailability("avail"), 
mSystem("system"), 
mFeedrate("path_feedratefrt"),
mExecution("exec"),
mPower("power"),
mMode("controllermode"),
mProgram("program"),
mPartCount("PartCountAct"),
mBadPartCount("PartCountBad"),
mProbed("probed"),
mSetPoint("SetPoint"),
mActualPoint("ActualPoint"),
mTolerance("Tolerance"),
mPlusTolerance("PlusTolerance"),
mMinusTolerance("MinusTolerance"),
mMinDeviation("MinDeviation"),
mMaxDeviation("MaxDeviation"),
mDeviation("Deviation"),
mOuttol("Outtol"),
mProbeName("ProbeName"),
mCharacteristicType("CharacteristicType"),
mFeatureType("FeatureType"),
mProbedId("ProbedId")
{
	addDatum(mAvailability);
	//addDatum(mSystem);
	addDatum(mFeedrate);
	addDatum(mExecution);
	addDatum(mPower);
	addDatum(mMode);
	addDatum(mProgram);
	addDatum(mPartCount);
	addDatum(mBadPartCount);
	addDatum(mProbed);
	addDatum(mSetPoint);
	addDatum(mActualPoint);
	addDatum(mPlusTolerance);
	addDatum(mMinusTolerance);
	addDatum(mMinDeviation);
	addDatum(mMaxDeviation);
	addDatum(mDeviation);
	addDatum(mOuttol);
	addDatum(mProbeName);
	addDatum(mCharacteristicType);
	addDatum(mFeatureType);
	addDatum(mProbedId);

	m_pAppEventListener=NULL;
	m_pMachineListener=NULL;
	disconnect();
	_nCycleTime=1000;
	clear();
}


PcDmisAdapter::~PcDmisAdapter()
{
}

void PcDmisAdapter::clear()
{
	mPower.setValue("OFF");
	mAvailability.unavailable();
	mPartCount.setValue(0);
	mBadPartCount.setValue(0);
	mExecution.setValue(Execution::eSTOPPED);
	mFeedrate.setValue(0);
	mProgram.setValue("");
	mMode.unavailable();
	mProbed.setValue(0);

	mSetPoint.unavailable();
	mActualPoint.unavailable();
	mTolerance.unavailable();
	mPlusTolerance.unavailable();
	mMinusTolerance.unavailable();
	mMinDeviation.unavailable();
	mMaxDeviation.unavailable();
	mDeviation.unavailable();
	mOuttol.unavailable();
	mProbeName.unavailable();
	mCharacteristicType.unavailable();
	mFeatureType.unavailable();
	mProbedId.unavailable();

}

void PcDmisAdapter::initialize(int aArgc, const char *aArgv[])
{
	USES_CONVERSION;
	HRESULT hr;

	_iniconfig.load(ExeDirectory() + "\\pcdmis.ini");
	std::string _sServiceName = _iniconfig.GetSymbolValue("PCDMIS.ServiceName", "PcDmisMTConnectAdapter");
	setName("PcDmisMTConnectAdapter");  // sets the service name for install


	MTConnectService::initialize(aArgc, aArgv);
	if (aArgc > 1) {
		mPort = atoi(aArgv[1]);
	}

	// In case we eventually have version problems
	_sProgid = _iniconfig.GetSymbolValue("PCDMIS.ProgId", "PCDLRN.Application");

	hr = CLSIDFromProgID(bstr_t(_sProgid.c_str()), &_clsdid ); 
	if(hr!=S_OK)
	{
		gLogger->error("PCDMIS COM component not installed\n");
		//throw std::exception("PCDMIS COM component not installed\n");
	}

	_sPcdmisId = _iniconfig.GetSymbolValue("PCDMIS.ProcessId", "pcdlrn.exe");
	_nCycleTime = _iniconfig.GetSymbolValue("PCDMIS.CycleTime", "1000").toNumber<int>();

}

void PcDmisAdapter::start()
{
	_state=OFF;

	m_pAppEventListener = new CPartProgramSink(this);
	m_pAppEventListener->AddRef();

	m_pMachineListener = new CMachineEvents(this);
	m_pMachineListener->AddRef();
	startServer();
}

void PcDmisAdapter::stop()
{
	disconnect();
	stopServer();
	_state=OFF;
	m_pAppEventListener->Release();
	m_pAppEventListener = NULL;
	m_pMachineListener->Release();
	m_pMachineListener=NULL;
}

bool PcDmisAdapter::IsConnnected()
{
	//	return pPCDmisApp != NULL;
	return ( FindProcessId(_sPcdmisId) != 0 && pPCDmisApp != NULL );

}
void  PcDmisAdapter::connect() 
{
	USES_CONVERSION;
	HRESULT hr;
	mPower.setValue("OFF");

	// see if PCDMIS running, if not, 
	DWORD processID = FindProcessId(_sPcdmisId);
	if ( processID == 0 )
		throw std::exception("PCDMIS not running\n");

	if(FAILED(hr=CoCreateInstance(__uuidof(PCDLRN::Application), //clsid,
		NULL,
		CLSCTX_LOCAL_SERVER,
		__uuidof(	PCDLRN::IApplication),
		(void**)&pPCDmisApp 
		)))
		throw std::exception("CoCreateInstance PCDLRN.Application failed"+ErrorFormatMessage(hr)+"\n");

	bstr_t _bstrDefaultFilePath = pPCDmisApp->DefaultFilePath;
	bstr_t _bstrVersion = pPCDmisApp->VersionString;
	bstr_t _bstrDefaultMachineName = pPCDmisApp->DefaultMachineName;
	bstr_t _bstrName = pPCDmisApp->Name;
	OutputDebugString(StdStringFormat("Version=%s\n", (LPCSTR) _bstrVersion).c_str());
	OutputDebugString(StdStringFormat("DefaultFilePath=%s\n", (LPCSTR) _bstrDefaultFilePath).c_str());
	OutputDebugString(StdStringFormat("Name=%s\n", (LPCSTR) _bstrName).c_str());
	OutputDebugString(StdStringFormat("DefaultMachineName=%s\n", (LPCSTR) _bstrDefaultMachineName).c_str());

	pAppObjEvents = pPCDmisApp->GetApplicationEvents ( );
	if(FAILED(hr=m_pAppEventListener->AttachToSource( pAppObjEvents)))
		throw std::exception("m_pAppEventListener->AttachToSource failed"+ErrorFormatMessage(hr)+"\n");

	mMode.setValue(ControllerMode::eMANUAL);

	ConnectMachine();
	// Probing result exist for programs already scanned.
	//this->UpdateProbingOperations();
}


bool PcDmisAdapter::ConnectMachine()
{
	HRESULT hr;
	m_pMachineListener->DetachFromSource();

	try{
		pMachines= pPCDmisApp->GetMachines( );
		if(pMachines==NULL )
			return false;

		int nMachines = pMachines->GetCount();
		if(nMachines<=0)
			return false;

		// FIXME: this is  hard coded
		pMachine=pMachines->Item(_variant_t(1));
		if(FAILED(hr=m_pMachineListener->AttachToSource( pMachine)))
		{
			OutputDebugString("m_pMachineListener->AttachToSource failed"+ErrorFormatMessage(hr)+"\n");
			return false;
		}
	}
	catch(...)
	{
		OutputDebugString("Error in PcDmisAdapter::ConnectMachine()\n");
		return false;
	}
	return true;
}

void  PcDmisAdapter::disconnect()
{
	try{
		clear();

		pAppObjEvents=NULL;
		pPCDmisApp=NULL;
		if(m_pAppEventListener!=NULL)
		{
			m_pAppEventListener->DetachFromSource(); 
		}
		if(m_pMachineListener!=NULL)
		{
			m_pMachineListener->DetachFromSource(); 
		}

	}
	catch(...)
	{
		OutputDebugString("Error in  PcDmisAdapter::disconnect()\n");
	}

}

void PcDmisAdapter::gatherDeviceData()
{
	USES_CONVERSION;
	_set_se_translator( trans_func );  // correct thread?

	long nLastSpeed=_nSpeed;
	try{


		if(!IsConnnected())
			connect(); // throws exception if cannot connect

		_nSleep=0;
		mAvailability.available();

		//OperatorMode - Represents whether or not you are in operator mode. TRUE when in operator mode
		VARIANT_BOOL vbOperatorMode = pPCDmisApp->OperatorMode;

		// COM Pointer to the currently active part program
		CComPtr<PCDLRN::IPartProgram> pProgram = pPCDmisApp->ActivePartProgram;
		if(pProgram==NULL)
			throw std::exception(" pcdmis disconnected");

		// Fixme: must check to see if changes
		pMachine=pProgram->ActiveMachine;
		if(pMachine!=NULL)
		{
			// Doesn't seem to work
			//  ManAutoSelector Read Only: Returns the MAN/AUTO selector position (-1=Undefined 0=MAN, 1=AUTO)  
			_manauto = pMachine->ManAutoSelector;
		}

		_bstrPartName="";
		_nSpeed=-1;

		// Represents the part name of the part program.
		// The part name is not the same as the file name
		_bstrPartName =pProgram->PartName;
		_nSpeed = pProgram->Speed; // Execution speed (0 if not executing). DOESNT WORK?
		_nPartCount =  pProgram->StatsCount;
		_bstrProbeName=pProgram->CurrentProbeName;

		// Translate PCDMIS values into MTConnect tag values
		mPower.setValue("ON");
		mFeedrate.setValue((double) _nSpeed);
		mProgram.setValue((LPCSTR) _bstrPartName);
		mProbeName.setValue((LPCSTR) _bstrProbeName);
		mPartCount.setValue(_nPartCount);

		// ManAutoSelector Read Only: Returns the MAN/AUTO selector position (-1=Undefined 0=MAN, 1=AUTO)  
		switch(_manauto)
		{
		case 0:
			mMode.setValue(ControllerMode::eMANUAL);
			break;
		case 1:
			mMode.setValue(ControllerMode::eAUTOMATIC);
			break;
		default:
			mMode.setValue(ControllerMode::eMANUAL);
		}

		switch (m_pAppEventListener->_state)
		{
		case CPartProgramSink::READY :  
			mExecution.setValue(Execution::eREADY); 
			break;
		case CPartProgramSink::RUNNING :  
			mExecution.setValue(Execution::eACTIVE); 
			break;
		case CPartProgramSink::PAUSED : 
			mExecution.setValue(Execution::eSTOPPED); 
			break;
		default:  mExecution.setValue(Execution::eREADY); 
		}

		// Doesn't seem to work
		//// Override events if moving
		//if((nLastSpeed!=_nSpeed) && (_nSpeed>0))
		//	mExecution.setValue(Execution::eACTIVE);

	}
	catch(std::exception e)
	{
		OutputDebugString("PcDmisAdapter::gatherDeviceData() exception: "); 
		OutputDebugString(e.what()); OutputDebugString("\n");
		disconnect();
		_nSleep=10*1000;
	}	
	catch(_com_error e)
	{
		OutputDebugString("_com_error:"); 
		OutputDebugString(e.ErrorMessage()); OutputDebugString("\n");
		disconnect();
		_nSleep=10*1000;
	}	
	catch(...)
	{
		disconnect();
		_nSleep=10*1000;
	}
}



/*
C++ Code based on:
http://www.pcdmisforum.com/showthread.php?6058-Count-all-OUTTOLS-regardless-of-Legacy-or-XactMeasure 
and other excel pcdmis vb code.
*/


extern std::string PcDmisEnumString(int n);
int PcDmisAdapter::UpdateProbingOperations()
{

	PCDLRN::IDimensionCmdPtr objDimCmd;
	CComPtr<PCDLRN::IPartProgram> pProgram;
	double dTotalOutTol;
	int nProbings=0;
	try{
		pProgram = pPCDmisApp->ActivePartProgram;
		if(pProgram==NULL)
			return 0;

		PCDLRN::ICommandsPtr pExecutedCommands =  pProgram->Commands;

		// Reset adapter data cycle
		Adapter::begin();

		// assume vb 6 indexing - start from 1, end at count
		for(int i=1; i<= pExecutedCommands->Count; i++)
		{
			CComPtr<PCDLRN::ICommand> pCommand = pExecutedCommands->Item(i);

			if(pCommand==NULL)
				continue;
			mBuffer.timestamp();
			_bstr_t objCmdIDName=pCommand->ID; // capture the ID Name of the command that is being looked at.

			if( pCommand->IsDimension == VARIANT_TRUE)
			{
				// Cast unknown pointer to Dimensional Command
				objDimCmd=pCommand->DimensionCommand;

				if( pCommand->Type == PCDLRN::DIMENSION_TRUE_START_POSITION || pCommand->Type ==  PCDLRN::DIMENSION_START_LOCATION
					|| pCommand->Type ==  PCDLRN::DIMENSION_TRUE_END_POSITION || pCommand->Type ==  PCDLRN::DIMENSION_END_LOCATION)
				{
					continue;
				}
				// indicated that we have another probed measurement
				nProbings++;
				_ProbedId=objDimCmd->ID;

				_SetPoint=objDimCmd->NOMINAL;
				_ActualPoint=objDimCmd->Measured;

				_Deviation = objDimCmd->Deviation ; 
				_MinDeviation = objDimCmd->Min ; 
				_MaxDeviation = objDimCmd->Max ; 

				_PlusTolerance=objDimCmd->Max;
				_MinusTolerance=objDimCmd->Minus;
				_Outtol=objDimCmd->OutTol;

				_CharacteristicType= PcDmisEnumString(pCommand->Type);
				_FeatureType=(LPCSTR) objDimCmd->Feat1;

				dTotalOutTol+=_Outtol;

#ifndef OUTTOLONLY
				mProbed.setValue(mProbed.getValue()+1);
				mSetPoint.setValue(objDimCmd->NOMINAL);
				mActualPoint.setValue(objDimCmd->Measured);
				mPlusTolerance.setValue(_PlusTolerance);
				mMinusTolerance.setValue(_MinusTolerance);
				mMinDeviation.setValue(_MinDeviation);
				mMaxDeviation.setValue(_MaxDeviation);
				mDeviation.setValue(_Deviation);
				mOuttol.setValue(_Outtol);
				mCharacteristicType.setValue(_CharacteristicType.c_str());
				mFeatureType.setValue(_FeatureType.c_str());
#endif

			}
			// This is a callback thread from PCDMIS not sure about any conflicts with adapter socket thread
			Adapter::sendChangedData();
			mBuffer.reset();
			Adapter::cleanup();

		}
	} 
	catch(...)
	{
		OutputDebugString("Exception in PcDmisAdapter::UpdateProbingOperations()\n");

	}
#ifdef OUTTOLONLY
			mProbed.setValue(mProbed.getValue()+1);
			mOuttol.setValue(dTotalOutTol);
			mBuffer.timestamp();
			Adapter::sendChangedData();
			mBuffer.reset();
			Adapter::cleanup();
#endif
	return 0;
}
