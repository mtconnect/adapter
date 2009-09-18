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

#include "internal.hpp"
#include "wattnode_adapter.hpp"

WattnodeAdapter::WattnodeAdapter(const char *aSerialPort, int aPort)
  : Adapter(aPort, 1000), 
	mPowerSum("power_sum"), mPowerA("power_a"), mPowerB("power_b"), mPowerC("power_c"),
	mVoltAvg("volt_avg"), mVoltA("volt_a"), mVoltB("volt_b"), mVoltC("volt_c"),
	mFrequency("frequency"),
	mCurrentA("amp_a"), mCurrentB("amp_b"), mCurrentC("amp_c")
{
  addDatum(mPowerSum);
  addDatum(mPowerA);
  addDatum(mPowerB);
  addDatum(mPowerC);
  addDatum(mVoltAvg);
  addDatum(mVoltA);
  addDatum(mVoltB);
  addDatum(mVoltC);
  addDatum(mFrequency);
  addDatum(mCurrentA);
  addDatum(mCurrentB);
  addDatum(mCurrentC);

  mConnected = 0;
  strcpy(mSerialPort, aSerialPort);
}

WattnodeAdapter::~WattnodeAdapter()
{
	modbus_close(&mb_param);
}

void WattnodeAdapter::gatherDeviceData()
{
	if (!mConnected) {
	  modbus_init_rtu(&mb_param, mSerialPort, 19200, "none", 8, 1);
	  modbus_set_debug(&mb_param, TRUE);
	  if (modbus_connect(&mb_param) == -1) {
		printf("ERROR Connection failed\n");
		Sleep(5000);
		return;
	  } else {
	    mConnected = 1;
	  }
	}

	uint16_t reg[128];

	// Get the power scaling factor at 41609
	int ret = read_holding_registers(&mb_param, 1, 1608, 1, reg);
	if (ret < 0) {
	  mConnected = 0;
	  modbus_close(&mb_param);
	  return;
	}

	double scaling_factor = (double) reg[0];

	if (read_holding_registers(&mb_param, 1, 1208, 8, reg) > 0) {
		mPowerSum.setValue((double) reg[0] / scaling_factor);
		mPowerA.setValue((double) reg[1] / scaling_factor);
		mPowerB.setValue((double) reg[2] / scaling_factor);
		mPowerC.setValue((double) reg[3] / scaling_factor);
		
		mVoltAvg.setValue((double) reg[4] / 10.0);
		mVoltA.setValue((double) reg[5] / 10.0);
		mVoltB.setValue((double) reg[6] / 10.0);
		mVoltC.setValue((double) reg[7] / 10.0);
	}

	read_holding_registers(&mb_param, 1, 1220, 1, reg);
	mFrequency.setValue((double) reg[0] / 10.0);

	if (read_holding_registers(&mb_param, 1, 1350, 3, reg) > 0) {
		mCurrentA.setValue((double) reg[0]);
		mCurrentB.setValue((double) reg[1]);
		mCurrentC.setValue((double) reg[2]);
	}
}

