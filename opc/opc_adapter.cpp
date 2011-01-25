/*
* Copyright (c) 2008, AMT â€“ The Association For Manufacturing Technology (â€œAMTâ€‌)
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
#include "opc_adapter.hpp"
#include "opc_item.hpp"

char complete_Tag_Name[80];

char complete_Tag_Name0[80];
char complete_Tag_Name1[80];
char complete_Tag_Name2[80];
char complete_Tag_Name3[80];
char complete_Tag_Name4[80];
char complete_Tag_Name5[80];
char complete_Tag_Name6[80];
char complete_Tag_Name7[80];
char complete_Tag_Name8[80];
char complete_Tag_Name9[80];
char complete_Tag_Name10[80];
char complete_Tag_Name11[80];
char complete_Tag_Name12[80];
char complete_Tag_Name13[80];
char complete_Tag_Name14[80];
char complete_Tag_Name15[80];
char complete_Tag_Name16[80];
char complete_Tag_Name17[80];
char complete_Tag_Name18[80];
char complete_Tag_Name19[80];
char complete_Tag_Name20[80];
char complete_Tag_Name21[80];
char complete_Tag_Name22[80];
char complete_Tag_Name23[80];
char complete_Tag_Name24[80];
char complete_Tag_Name25[80];
char complete_Tag_Name26[80];
char complete_Tag_Name27[80];
char complete_Tag_Name28[80];
char complete_Tag_Name29[80];

char PLCStatus_Tag[80], PLCProgStatus_Tag[80], ErrorStatus_Tag[80], InfoStatus_Tag[80], WarningStatus_Tag[80];
char ProgramName_Tag[80], ChannelStatus_Tag[80], NCBlockNo_Tag[80];
string tmp1, tmp2, tmp3;

char name1[30] ="usingVar";
char name2[80] =" ";
string token1 = "addOpcItem(mOpcTest = new COPCItem(mOPCServer,";
string token2 = "!R8,IndraMotion_MTX_P60,NC.Chan.AxisPosAcs,1 {0,7}";

OpcAdapter::OpcAdapter(int aPort, const char *aName)
  : Adapter(aPort, 200),
    //mPower("power"), j------------------------------------------------
	//mAlarm("contr_alarm"), j------------------------------------------------
	mExecution("execution"),
	mBlock("block"), // should be block
	mProgram("program_name"),
	mMode("mode"),
	mTest(name1), // -----------------------
	mTtag0("default_name"), // -----------------------
	mTtag1("default_name"), // -----------------------
	mTtag2("default_name"), // -----------------------
	mTtag3("default_name"), // -----------------------
	mTtag4("default_name"), // -----------------------
	mTtag5("default_name"), // -----------------------
	mTtag6("default_name"), // -----------------------
	mTtag7("default_name"), // -----------------------
	mTtag8("default_name"), // -----------------------
	mTtag9("default_name"), // -----------------------
	mTtag10("default_name"), // -----------------------
	mTtag11("default_name"), // -----------------------
	mTtag12("default_name"), // -----------------------
	mTtag13("default_name"), // -----------------------
	mTtag14("default_name"), // -----------------------
	mTtag15("default_name"), // -----------------------
	mTtag16("default_name"), // -----------------------
	mTtag17("default_name"), // -----------------------
	mTtag18("default_name"), // -----------------------
	mTtag19("default_name"), // -----------------------
	mTtag20("default_name"), // -----------------------
	mTtag21("default_name"), // -----------------------
	mTtag22("default_name"), // -----------------------
	mTtag23("default_name"), // -----------------------
	mTtag24("default_name"), // -----------------------
	mTtag25("default_name"), // -----------------------
	mTtag26("default_name"), // -----------------------
	mTtag27("default_name"), // -----------------------
	mTtag28("default_name"), // -----------------------
	mTtag29("default_name") // -----------------------
{
	
	ReadUserFile(); // Read config.ini file

	tmp1 = "!BOOL,";
	tmp2 = ",Plc.PVL,.bPower";
	tmp3 = tmp1 + deviceName[0] + tmp2;
	tmp3.copy(PLCStatus_Tag, 80);

	tmp1 = "!I4,";
	tmp2 = ",NC.Chan.ModeOfOperation,1 {0,0}";
	tmp3 = tmp1 + deviceName[0] + tmp2;
	tmp3.copy(PLCProgStatus_Tag, 80);

	tmp1 = "!BOOL,";
	tmp2 = ",System.ErrorStatus,-1,-1 {0,0}";
	tmp3 = tmp1 + deviceName[0] + tmp2;
	tmp3.copy(ErrorStatus_Tag, 80);

	tmp2 = ",System.InfoStatus,-1,-1 {0,0}";
	tmp3 = tmp1 + deviceName[0] + tmp2;
	tmp3.copy(InfoStatus_Tag, 80);

	tmp2 = ",System.WarningStatus,-1,-1 {0,0}";
	tmp3 = tmp1 + deviceName[0] + tmp2;
	tmp3.copy(WarningStatus_Tag, 80);

	tmp1 = "!BSTR,";
	tmp2 = ",NC.Chan.ActMainProgram,1 {0,0}";
	tmp3 = tmp1 + deviceName[0] + tmp2;
	tmp3.copy(ProgramName_Tag, 80);

	tmp1 = "!I4,";
	tmp2 = ",NC.Chan.ChanState,1 {0,0}";
	tmp3 = tmp1 + deviceName[0] + tmp2;
	tmp3.copy(ChannelStatus_Tag, 80);

	tmp1 = "!BSTR,";
	tmp2 = ",NC.Chan.ActNcBlock,1 {0,0}";
	tmp3 = tmp1 + deviceName[0] + tmp2;
	tmp3.copy(NCBlockNo_Tag, 80);

	for (int t=0; t<tagCount; t++)
	{
		memset( name1, '\0', 80 );
		tagName[t].copy(name1,30);
		mTest = name1;
		if (t == 0)
			mTtag0 = name1;
		if (t == 1)
			mTtag1 = name1;
		if (t == 2)
			mTtag2 = name1;
		if (t == 3)
			mTtag3 = name1;
		if (t == 4)
			mTtag4 = name1;
		if (t == 5)
			mTtag5 = name1;
		if (t == 6)
			mTtag6 = name1;
		if (t == 7)
			mTtag7 = name1;
		if (t == 8)
			mTtag8 = name1;
		if (t == 9)
			mTtag9 = name1;
		if (t == 10)
			mTtag10 = name1;
		if (t == 11)
			mTtag11 = name1;
		if (t == 12)
			mTtag12 = name1;
		if (t == 13)
			mTtag13 = name1;
		if (t == 14)
			mTtag14 = name1;
		if (t == 15)
			mTtag15 = name1;
		if (t == 16)
			mTtag16 = name1;
		if (t == 17)
			mTtag17 = name1;
		if (t == 18)
			mTtag18 = name1;
		if (t == 19)
			mTtag19 = name1;
		if (t == 20)
			mTtag20 = name1;
		if (t == 21)
			mTtag21 = name1;
		if (t == 22)
			mTtag22 = name1;
		if (t == 23)
			mTtag23 = name1;
		if (t == 24)
			mTtag24 = name1;
		if (t == 25)
			mTtag25 = name1;
		if (t == 26)
			mTtag26 = name1;
		if (t == 27)
			mTtag27 = name1;
		if (t == 28)
			mTtag28 = name1;
		if (t == 29)
			mTtag29 = name1;

	}

	for (int q=0; q<completeTagCount; q++)
	{
		memset( name2, '\0', 80 );
		completeTagStr[q].copy(name2,80);
		if (q == 0)
			strcpy(complete_Tag_Name0,name2);
		if (q == 1)
			strcpy(complete_Tag_Name1,name2);
		if (q == 2)
			strcpy(complete_Tag_Name2,name2);
		if (q == 3)
			strcpy(complete_Tag_Name3,name2);
		if (q == 4)
			strcpy(complete_Tag_Name4,name2);
		if (q == 5)
			strcpy(complete_Tag_Name5,name2);
		if (q == 6)
			strcpy(complete_Tag_Name6,name2);
		if (q == 7)
			strcpy(complete_Tag_Name7,name2);
		if (q == 8)
			strcpy(complete_Tag_Name8,name2);
		if (q == 9)
			strcpy(complete_Tag_Name9,name2);
		if (q == 10)
			strcpy(complete_Tag_Name10,name2);
		if (q == 11)
			strcpy(complete_Tag_Name11,name2);
		if (q == 12)
			strcpy(complete_Tag_Name12,name2);
		if (q == 13)
			strcpy(complete_Tag_Name13,name2);
		if (q == 14)
			strcpy(complete_Tag_Name14,name2);
		if (q == 15)
			strcpy(complete_Tag_Name15,name2);
		if (q == 16)
			strcpy(complete_Tag_Name16,name2);
		if (q == 17)
			strcpy(complete_Tag_Name17,name2);
		if (q == 18)
			strcpy(complete_Tag_Name18,name2);
		if (q == 19)
			strcpy(complete_Tag_Name19,name2);
		if (q == 20)
			strcpy(complete_Tag_Name20,name2);
		if (q == 21)
			strcpy(complete_Tag_Name21,name2);
		if (q == 22)
			strcpy(complete_Tag_Name22,name2);
		if (q == 23)
			strcpy(complete_Tag_Name23,name2);
		if (q == 24)
			strcpy(complete_Tag_Name24,name2);
		if (q == 25)
			strcpy(complete_Tag_Name25,name2);
		if (q == 26)
			strcpy(complete_Tag_Name26,name2);
		if (q == 27)
			strcpy(complete_Tag_Name27,name2);
		if (q == 28)
			strcpy(complete_Tag_Name28,name2);
		if (q == 29)
			strcpy(complete_Tag_Name29,name2);

	}
	
	completeTagStr[0].copy(complete_Tag_Name,80);
	//strcpy(complete_Tag_Name, "!R8,IndraMotion_MTX_P60,NC.Chan.AxisPosAcs,1 {1,7}");
	
	if (!localInit())
		exit(1);

	if (!createLocalServer(aName))
		exit(1);

	createGroup();

//    addDatum(mAlarm); j------------------------------------------------
//	addDatum(mPower); j------------------------------------------------
	addDatum(mExecution);
	addDatum(mBlock);

	addDatum(mTest); //-----------------
	addDatum(mTtag0); //-----------------
	addDatum(mTtag1); //-----------------
	addDatum(mTtag2); //-----------------
	addDatum(mTtag3); //-----------------
	addDatum(mTtag4); //-----------------
	addDatum(mTtag5); //-----------------
	addDatum(mTtag6); //-----------------
	addDatum(mTtag7); //-----------------
	addDatum(mTtag8); //-----------------
	addDatum(mTtag9); //-----------------
	addDatum(mTtag10); //-----------------
	addDatum(mTtag11); //-----------------
	addDatum(mTtag12); //-----------------
	addDatum(mTtag13); //-----------------
	addDatum(mTtag14); //-----------------
	addDatum(mTtag15); //-----------------
	addDatum(mTtag16); //-----------------
	addDatum(mTtag17); //-----------------
	addDatum(mTtag18); //-----------------
	addDatum(mTtag19); //-----------------
	addDatum(mTtag20); //-----------------
	addDatum(mTtag21); //-----------------
	addDatum(mTtag22); //-----------------
	addDatum(mTtag23); //-----------------
	addDatum(mTtag24); //-----------------
	addDatum(mTtag25); //-----------------
	addDatum(mTtag26); //-----------------
	addDatum(mTtag27); //-----------------
	addDatum(mTtag28); //-----------------
	addDatum(mTtag29); //-----------------

	addDatum(mProgram);
	addDatum(mMode);

	addOpcItem(mOpcTag0 = new COPCItem(mOPCServer, complete_Tag_Name0));
	addOpcItem(mOpcTag1 = new COPCItem(mOPCServer, complete_Tag_Name1));
	addOpcItem(mOpcTag2 = new COPCItem(mOPCServer, complete_Tag_Name2));
	// The following lines are added by Jamal
	addOpcItem(mOpcTag3 = new COPCItem(mOPCServer, complete_Tag_Name3));
	addOpcItem(mOpcTag4 = new COPCItem(mOPCServer, complete_Tag_Name4)); 
	addOpcItem(mOpcTag5 = new COPCItem(mOPCServer, complete_Tag_Name5));
	addOpcItem(mOpcTag6 = new COPCItem(mOPCServer, complete_Tag_Name6));
	addOpcItem(mOpcTag7 = new COPCItem(mOPCServer, complete_Tag_Name7));
	
	addOpcItem(mPLCStatus = new COPCItem(mOPCServer, PLCStatus_Tag));
	addOpcItem(mPLCProgStatus = new COPCItem(mOPCServer, PLCProgStatus_Tag ));
	addOpcItem(mErrorStatus = new COPCItem(mOPCServer, ErrorStatus_Tag));
	addOpcItem(mInfoStatus = new COPCItem(mOPCServer, InfoStatus_Tag));
	addOpcItem(mWarningStatus = new COPCItem(mOPCServer, WarningStatus_Tag));
	addOpcItem(mProgramName = new COPCItem(mOPCServer, ProgramName_Tag));
	addOpcItem(mChannelStatus = new COPCItem(mOPCServer, ChannelStatus_Tag));

	//addOpcItem(mNCBlockNo = new COPCItem(mOPCServer, "!BSTR,IndraMotion_MTX_P60,NC.Chan.ActNcBlock,1 {0,0}"));
	addOpcItem(mNCBlockNo = new COPCItem(mOPCServer, NCBlockNo_Tag));


	addOpcItem(mOpcTag8 = new COPCItem(mOPCServer, complete_Tag_Name8));
	addOpcItem(mOpcTag9 = new COPCItem(mOPCServer, complete_Tag_Name9));
	addOpcItem(mOpcTag10 = new COPCItem(mOPCServer, complete_Tag_Name10));
	addOpcItem(mOpcTag11 = new COPCItem(mOPCServer, complete_Tag_Name11));
	addOpcItem(mOpcTag12 = new COPCItem(mOPCServer, complete_Tag_Name12));
	addOpcItem(mOpcTag13 = new COPCItem(mOPCServer, complete_Tag_Name13));
	addOpcItem(mOpcTag14 = new COPCItem(mOPCServer, complete_Tag_Name14));
	addOpcItem(mOpcTag15 = new COPCItem(mOPCServer, complete_Tag_Name15));
	addOpcItem(mOpcTag16 = new COPCItem(mOPCServer, complete_Tag_Name16));
	addOpcItem(mOpcTag17 = new COPCItem(mOPCServer, complete_Tag_Name17));
	addOpcItem(mOpcTag18 = new COPCItem(mOPCServer, complete_Tag_Name18));
	addOpcItem(mOpcTag19 = new COPCItem(mOPCServer, complete_Tag_Name19));
	addOpcItem(mOpcTag20 = new COPCItem(mOPCServer, complete_Tag_Name20));
	addOpcItem(mOpcTag21 = new COPCItem(mOPCServer, complete_Tag_Name21));
	addOpcItem(mOpcTag22 = new COPCItem(mOPCServer, complete_Tag_Name22));
	addOpcItem(mOpcTag23 = new COPCItem(mOPCServer, complete_Tag_Name23));
	addOpcItem(mOpcTag24 = new COPCItem(mOPCServer, complete_Tag_Name24));
	addOpcItem(mOpcTag25 = new COPCItem(mOPCServer, complete_Tag_Name25));
	addOpcItem(mOpcTag26 = new COPCItem(mOPCServer, complete_Tag_Name26));
	addOpcItem(mOpcTag27 = new COPCItem(mOPCServer, complete_Tag_Name27));
	addOpcItem(mOpcTag28 = new COPCItem(mOPCServer, complete_Tag_Name28));
	addOpcItem(mOpcTag29 = new COPCItem(mOPCServer, complete_Tag_Name29));
	addOpcItem(mOpcTest = new COPCItem(mOPCServer, complete_Tag_Name));
	
}

OpcAdapter::~OpcAdapter()
{
	CoUninitialize();
}

void OpcAdapter::gatherDeviceData()
{
	syncReadIO();
	//mPower.setValue(mPLCStatus->getBoolValue() ? Power::eON : Power::eOFF); j-------------

	//----mProgram.setValue((char*)mProgramName->getBSTRValue());
	//----mProgram.setValue(mProgramName->getBSTRValue());	
	USES_CONVERSION; // ATL conversion macros uses local vars - they are allocated here
	char* complete_Tag_Name = W2A(mProgramName->getBSTRValue()); // The new string are allocated on the stack
	mProgram.setValue(complete_Tag_Name);

	char* str2 = W2A(mNCBlockNo->getBSTRValue()); // The new string are allocated on the stack
	mBlock.setValue(str2);

	gatherPositions();
	getSpindleValues();// Added by Jamal
	getFeedRate(); // Added by Jamal
	getControllerMode();
}

void OpcAdapter::gatherPositions()
{
	
	//mXact.setValue(mOpcXact->getDoubleValue());
	//mYact.setValue(mOpcYact->getDoubleValue());
	//mZact.setValue(mOpcZact->getDoubleValue());
	//mXcom.setValue(mOpcXcom->getDoubleValue()); // Added by Jamal
	//mYcom.setValue(mOpcYcom->getDoubleValue()); // Added by Jamal
	//mZcom.setValue(mOpcZcom->getDoubleValue()); // Added by Jamal

	mTest.setValue(mOpcTest->getDoubleValue()); // ------------------------
	mTtag0.setValue(mOpcTag0->getDoubleValue()); // ------------------------
	mTtag1.setValue(mOpcTag1->getDoubleValue()); // ------------------------
	mTtag2.setValue(mOpcTag2->getDoubleValue()); // ------------------------
	mTtag3.setValue(mOpcTag3->getDoubleValue()); // ------------------------
	mTtag4.setValue(mOpcTag4->getDoubleValue()); // ------------------------
	mTtag5.setValue(mOpcTag5->getDoubleValue()); // ------------------------
	mTtag6.setValue(mOpcTag6->getDoubleValue()); // ------------------------
	mTtag7.setValue(mOpcTag7->getDoubleValue()); // ------------------------
	mTtag8.setValue(mOpcTag8->getDoubleValue()); // ------------------------
	mTtag9.setValue(mOpcTag9->getDoubleValue()); // ------------------------
	mTtag10.setValue(mOpcTag10->getDoubleValue()); // ------------------------
	mTtag11.setValue(mOpcTag11->getDoubleValue()); // ------------------------
	mTtag12.setValue(mOpcTag12->getDoubleValue()); // ------------------------
	mTtag13.setValue(mOpcTag13->getDoubleValue()); // ------------------------
	mTtag14.setValue(mOpcTag14->getDoubleValue()); // ------------------------
	mTtag15.setValue(mOpcTag15->getDoubleValue()); // ------------------------
	mTtag16.setValue(mOpcTag16->getDoubleValue()); // ------------------------
	mTtag17.setValue(mOpcTag17->getDoubleValue()); // ------------------------
	mTtag18.setValue(mOpcTag18->getDoubleValue()); // ------------------------
	mTtag19.setValue(mOpcTag19->getDoubleValue()); // ------------------------
	mTtag20.setValue(mOpcTag20->getDoubleValue()); // ------------------------
	mTtag21.setValue(mOpcTag21->getDoubleValue()); // ------------------------
	mTtag22.setValue(mOpcTag22->getDoubleValue()); // ------------------------
	mTtag23.setValue(mOpcTag23->getDoubleValue()); // ------------------------
	mTtag24.setValue(mOpcTag24->getDoubleValue()); // ------------------------
	mTtag25.setValue(mOpcTag25->getDoubleValue()); // ------------------------
	mTtag26.setValue(mOpcTag26->getDoubleValue()); // ------------------------
	mTtag27.setValue(mOpcTag27->getDoubleValue()); // ------------------------
	mTtag28.setValue(mOpcTag28->getDoubleValue()); // ------------------------
	mTtag29.setValue(mOpcTag29->getDoubleValue()); // ------------------------


}

void OpcAdapter::getSpindleValues() // Added by Jamal
{
	//mSspeed.setValue(mOpcSspeed->getDoubleValue());
}

void OpcAdapter::getFeedRate() // Added by Jamal
{
	//mPathFeedrate.setValue(mOpcPathFeedrate->getDoubleValue());

	//mXfr.setValue(mOpcXfr->getDoubleValue());
	//mYfr.setValue(mOpcXfr->getDoubleValue());
	//mZfr.setValue(mOpcXfr->getDoubleValue());
}

void OpcAdapter::getControllerMode()
{
	if (mPLCProgStatus->getIntValue() == 1)
		mMode.setValue(ControllerMode::eMANUAL);
	else if (mPLCProgStatus->getIntValue() == 4)
		mMode.setValue(ControllerMode::eMANUAL_DATA_INPUT);
	else if (mPLCProgStatus->getIntValue() == 5)
		mMode.setValue(ControllerMode::eAUTOMATIC);

	// Get error status
	//if (mErrorStatus->getIntValue()) j---------------------------------------------
	//	mAlarm.setValue(Alarm::eFAULT, " ", Alarm::eERROR, Alarm::eINSTANT, "");
	//else if (mWarningStatus->getIntValue()) j------------------------------------
	//	mAlarm.setValue(Alarm::eOTHER, " ", Alarm::eWARNING, Alarm::eINSTANT, "");
	//else if (mInfoStatus->getIntValue()) j--------------------------------------
		//mAlarm.setValue(Alarm::eOTHER, " ", Alarm::eINFO, Alarm::eINSTANT, "");

	// Get Execution status
	if (mChannelStatus->getIntValue() == 0) 
		mExecution.setValue(Execution::eREADY);
	else if (mChannelStatus->getIntValue() == 1) 
		mExecution.setValue(Execution::eINTERRUPTED);
	else if (mChannelStatus->getIntValue() == 2) 
		mExecution.setValue(Execution::eSTOPPED);
	else if (mChannelStatus->getIntValue() == 3) 
		mExecution.setValue(Execution::eACTIVE);
	
}

void OpcAdapter::syncReadIO()
{
//	mPower.setValue(Power::eON); j------------------------------------------------

    CComPtr< IOPCSyncIO > poOPCSyncIO;

    HRESULT hRes;

    hRes = mOPCServer->GetGroupByName(L"MTConnect", 
                                        __uuidof(IOPCSyncIO),
                                        (IUnknown**)&poOPCSyncIO);
    if (FAILED(hRes))
    {
        MakeErrorMsg(hRes, "GetGroupByName");
        return;
    }

	DWORD dwSize = mOPCItemList.size();
    OPCHANDLE  *phServer = new OPCHANDLE[dwSize];
    if (NULL == phServer)
    {
        MakeErrorMsg(E_OUTOFMEMORY, "OPC Server Handle");
        return;
    }
    int nIndex = 0;
    for(DWORD i = 0; i <dwSize; i++)
    {
        if (NULL != mOPCItemList[i]->mItemResult &&
            OPC_READABLE == (mOPCItemList[i]->mItemResult->dwAccessRights & OPC_READABLE))
        {
            phServer[nIndex++] = mOPCItemList[i]->mItemResult->hServer;
        }
    }

    HRESULT         *phError;
    OPCITEMSTATE    *ptState;

	hRes = poOPCSyncIO->Read(OPC_DS_DEVICE, nIndex, phServer, &ptState, &phError);
    delete [] phServer;

    if (FAILED(hRes))
    {
        MakeErrorMsg(E_OUTOFMEMORY, "Read");
        return;
    }

    COPCItem *poItem = NULL;
    while (nIndex != 0)
    {
        --nIndex;
        if (phError[nIndex] == S_OK)
        {
            poItem = (COPCItem*)ptState[nIndex].hClient;
            if (NULL != poItem)
            {
                poItem->setCurrentResult(ptState[nIndex], phError[nIndex]);
            }
        }
    }
    CoTaskMemFree(ptState);
    CoTaskMemFree(phError);
}

bool OpcAdapter::localInit()
{
	HRESULT	r1;
	r1 = CoInitialize(NULL);
	return !FAILED(r1);
}

bool OpcAdapter::createLocalServer(const char *aName)
{
    HRESULT hRes;
    CLSID clsid;
	WCHAR *wname = WSTRFromSBCS(aName);
    hRes = ::CLSIDFromProgID(wname, &clsid);
	delete wname;
    if (FAILED(hRes))
    {
        MakeErrorMsg(hRes, "CLSIDFromProgID");
        return false;
    }
    hRes = ::CoCreateInstance(clsid, NULL, CLSCTX_SERVER, __uuidof(IOPCServer), (void**)&mOPCServer);  
	
    if (FAILED(hRes))
    {
        MakeErrorMsg(hRes, "CoCreateInstance");
        return false;
    }
	return true;
}

void OpcAdapter::MakeErrorMsg(HRESULT hError, const char* szText)
{
    CString strMsg = szText;
    LPVOID lpMsgBuf;
    FormatMessage( 
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM | 
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        hError,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
        (LPTSTR) &lpMsgBuf,
        0,
        NULL 
        );
    strMsg += "\n";
    strMsg += LPCTSTR(lpMsgBuf);
	puts(strMsg);
    LocalFree( lpMsgBuf );

}

bool OpcAdapter::createGroup()
{
    HRESULT hRes;
    DWORD       dwRevisedUpdateRate;
    OPCHANDLE   hServer;
    hRes = mOPCServer->AddGroup(L"MTConnect", 
                                  TRUE, 
                                  250, 
                                  1234, 
                                  NULL, 
                                  NULL, 
                                  GetUserDefaultLCID(),
                                  &hServer,
                                  &dwRevisedUpdateRate,
                                  __uuidof(IOPCItemMgt),
                                  (IUnknown**)&mOPCItemMgt);
    if (FAILED(hRes))
    {
        MakeErrorMsg(hRes, "AddGroup");
        return false;
    }

	return true;
}

bool OpcAdapter::addOpcItem(COPCItem *aItem)
{
    mOPCItemList.push_back(aItem);
   
    HRESULT         *phError, hRes;
    hRes = mOPCItemMgt->AddItems(1, &aItem->mItemDef, &aItem->mItemResult, &phError);
    if (FAILED(hRes))
    {   
        MakeErrorMsg(hRes, "AddItem");
        return false;
    }
    aItem->mError = *phError;
    CoTaskMemFree(phError);

	return true;
}

void OpcAdapter::ReadUserFile()
{
	  ifstream infile("config.ini");
	  completeTagCount = 0;
	  k = 0;
	  axisCount = 0;
	  tagCount = 0;
	  deviceCount = 0;
	  string temp;

   if (!infile)
   {
      cerr << "Could not open file." << endl;
   }

   vector<string> lines;

   while (getline(infile, line) && completeTagCount < 30)
   {
      lines.push_back(line);
	  //buffer = line;
	  memset( buffer, '\0', 30 );
	  line.copy(buffer, 30);

	  removeAllWhite(line);  // remove white spaces
	  
	  buffer1 = line;
	  //found = buffer1.find("Device_Name");

	  if (buffer1.find("Device_Name") !=string::npos)
	  {
		  pos = buffer1.find("=");    // position of "=" in buffer1
		  buffer2 = buffer1.substr (pos+1);   // get from "=" +1 to the end
		  //buffer2.copy(deviceName, 30);
		  deviceName[deviceCount] = buffer2;
	  }

	  else if (buffer1.find("Channel_Number") !=string::npos)
	  {
		  pos = buffer1.find("=");    // position of "=" in buffer1
		  buffer2 = buffer1.substr (pos+1);   // get from "=" +1 to the end
		  buffer2.copy(channelNum[0], 30);

	  }

	  else if ((buffer1.find("_End") !=string::npos)|| (buffer1.find("_end") !=string::npos))
	  {
		 ;//----------
	  }


	  else if (buffer1.find("Axis_Name") !=string::npos)
	  {
		  pos = buffer1.find("=");    // position of "=" in buffer1
		  buffer2 = buffer1.substr (pos+1);   // get from "=" +1 to the end
		  //buffer2.copy(axisName[axisCount], 20);
		  axisName[axisCount] = buffer2;
		  axisCount++;
	  }

	  else if (buffer1.find("Tag_Name") !=string::npos)
	  {
		  pos = buffer1.find("=");    // position of "=" in buffer1
		  buffer2 = buffer1.substr (pos+1);   // get from "=" +1 to the end
		  tagName[tagCount] = buffer2;
		  //int str_length = 0;
		  //string temp;
		   tagCount++;
	  }

	  else
		{
			completeTagStr[completeTagCount] = line;
			completeTagCount++;
		}
   }

   infile.close();
}

void OpcAdapter::removeAllWhite(std::string &str)
{
    std::string temp;
    for (unsigned int z = 0; z < str.length(); z++)
        if (str[z] != ' ') temp += str[z];
    str = temp;
}

void OpcAdapter::clientsDisconnected()
{
	printf("closing...\n");
	exit(0);
}
