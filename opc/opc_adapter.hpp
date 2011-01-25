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

#ifndef Opc_ADAPTER_HPP
#define Opc_ADAPTER_HPP

#include <atlbase.h>
#include <vector>
#include "adapter.hpp"
#include "device_datum.hpp"
#include "opc_item.hpp"

//----------------------------------------------
// Added by Jamal
#include <iostream>
#include <string>
#include <vector>

using namespace std;
#include <stdio.h>
#include <fstream>

//---------------------------------------------

inline static WCHAR * WSTRFromSBCS(const CHAR *buf)
{
	size_t	length, i;
	WCHAR	*temp;

	length = strlen(buf) + 1;
	temp = new WCHAR[strlen(buf) + 1];
	
	if(temp)
	{
		for(i=0; i<length; i++) temp[i] = (WCHAR) buf[i];
	}
	return temp;
}

class OpcAdapter : public Adapter 
{
protected:
	/* Define all the data values here */

	/* Events */
//	Alarm mAlarm; ------------------------------------------------

//	Power mPower; ------------------------------------------------
	Execution mExecution;
	Event mBlock; // Changed it from IntEvent to Event
	Event mProgram;
	ControllerMode mMode;

	/* Samples */

	//Sample mXact;
	//Sample mYact;
	//Sample mZact;  
	Sample mTest; // Added by Jamal 
	Sample mTtag0; // Added by Jamal
	Sample mTtag1; // Added by Jamal
	Sample mTtag2; // Added by Jamal
	Sample mTtag3; // Added by Jamal
	Sample mTtag4; // Added by Jamal
	Sample mTtag5; // Added by Jamal
	Sample mTtag6; // Added by Jamal
	Sample mTtag7; // Added by Jamal
	Sample mTtag8; // Added by Jamal
	Sample mTtag9; // Added by Jamal
	Sample mTtag10; // Added by Jamal
	Sample mTtag11; // Added by Jamal
	Sample mTtag12; // Added by Jamal
	Sample mTtag13; // Added by Jamal
	Sample mTtag14; // Added by Jamal
	Sample mTtag15; // Added by Jamal
	Sample mTtag16; // Added by Jamal
	Sample mTtag17; // Added by Jamal
	Sample mTtag18; // Added by Jamal
	Sample mTtag19; // Added by Jamal
	Sample mTtag20; // Added by Jamal
	Sample mTtag21; // Added by Jamal
	Sample mTtag22; // Added by Jamal
	Sample mTtag23; // Added by Jamal
	Sample mTtag24; // Added by Jamal
	Sample mTtag25; // Added by Jamal
	Sample mTtag26; // Added by Jamal
	Sample mTtag27; // Added by Jamal
	Sample mTtag28; // Added by Jamal
	Sample mTtag29; // Added by Jamal



	/* Spindle */
	//Sample mSspeed;

	/* Path Feedrate */
	//Sample mPathFeedrate;
	//Sample mXfr; // Added by Jamal
	//Sample mYfr; // Added by Jamal
	//Sample mZfr; // Added by Jamal

	/* OPC Items */
	COPCItem *mOpcTag0;
	COPCItem *mOpcTag1; 
	COPCItem *mOpcTag2;
	COPCItem *mOpcTag3; // Added by Jamal
	COPCItem *mOpcTag4; // Added by Jamal
	COPCItem *mOpcTag5; // Added by Jamal
	COPCItem *mOpcTag6;
	COPCItem *mPLCStatus;
	COPCItem *mPLCProgStatus; // Added by Jamal

	COPCItem *mOpcTest; // ---------------------------

	COPCItem *mErrorStatus; // Added by Jamal
	COPCItem *mInfoStatus; // Added by Jamal
	COPCItem *mWarningStatus; // Added by Jamal 
	COPCItem *mProgramName; // Added by Jamal 
	COPCItem *mChannelStatus; // Added by Jamal 
	COPCItem *mNCBlockNo; // Added by Jamal
	COPCItem *mOpcTag7;// Added by Jamal 

	COPCItem *mOpcTag8;// Added by Jamal
	COPCItem *mOpcTag9;// Added by Jamal
	COPCItem *mOpcTag10;// Added by Jamal
	COPCItem *mOpcTag11;// Added by Jamal
	COPCItem *mOpcTag12;// Added by Jamal
	COPCItem *mOpcTag13;// Added by Jamal
	COPCItem *mOpcTag14;// Added by Jamal
	COPCItem *mOpcTag15;// Added by Jamal
	COPCItem *mOpcTag16;// Added by Jamal
	COPCItem *mOpcTag17;// Added by Jamal
	COPCItem *mOpcTag18;// Added by Jamal
	COPCItem *mOpcTag19;// Added by Jamal
	COPCItem *mOpcTag20;// Added by Jamal
	COPCItem *mOpcTag21;// Added by Jamal
	COPCItem *mOpcTag22;// Added by Jamal
	COPCItem *mOpcTag23;// Added by Jamal
	COPCItem *mOpcTag24;// Added by Jamal
	COPCItem *mOpcTag25;// Added by Jamal
	COPCItem *mOpcTag26;// Added by Jamal
	COPCItem *mOpcTag27;// Added by Jamal
	COPCItem *mOpcTag28;// Added by Jamal
	COPCItem *mOpcTag29;// Added by Jamal


  /* COM Objects */
    CComPtr< IOPCServer >   mOPCServer;
	CComPtr< IOPCItemMgt >  mOPCItemMgt;
    std::vector< COPCItem* > mOPCItemList;

public:
  OpcAdapter(int aPort, const char *aName);
  ~OpcAdapter();
  //--------------------------------------------------------
	void ReadUserFile(); // Added by Jamal to read config file
	void removeAllWhite(std::string &str);
	char buffer[30];
	string buffer1, buffer2, axisName[20], tagName[30], deviceName[5];
	char channelNum[5][2];
	int axisCount, tagCount, deviceCount;
	string completeTagStr[30];
	FILE *infile, *outfile;
	//int i;
	int completeTagCount;
	int k;
	size_t found, pos;
	string line;
  //--------------------------------------------------------

  virtual void gatherDeviceData();
  virtual void clientsDisconnected();

protected:
  void gatherPositions();
  void getSpindleValues(); // Added by Jamal 
  void getFeedRate(); // Added by Jamal
  void getControllerMode();
  void syncReadIO();

  bool localInit();
  bool createLocalServer(const char *aName);
  bool createGroup();
  bool addOpcItem(COPCItem *aItem);
  bool addPositionItems();
  void MakeErrorMsg(HRESULT hError, const char* szText);
};

#endif


