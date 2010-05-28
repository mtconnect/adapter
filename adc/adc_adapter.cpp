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
#include "adc_adapter.hpp"

using namespace std;

AdcAdapter::AdcAdapter(int aPort, AdcSerial *aSerial)
  : Adapter(aPort, 1000), 
    mValue1("value")
{
  addDatum(mValue1);
  
  mSerial = aSerial;
}

bool AdcAdapter::connect()
{
  bool ret = mSerial->connect();
  if (ret)
  {
    mSerial->flushInput();
  }
  
  return ret;    
}

void AdcAdapter::disconnect()
{
  mSerial->disconnect();
}

AdcAdapter::~AdcAdapter()
{
  disconnect();
}

void AdcAdapter::gatherDeviceData()
{
  try
  {
    if (!mSerial->connected())
    {
      if (!connect())
	sleep(5);
    }
    else
    {
      mSerial->flushInput();

      // Test connection and make sure we're synched before we start
      string res = mSerial->sendCommand("Q1");
      if (!res.empty() && res.length() == 6)
      {
	int value = strtoul(res.substr(2,3).c_str(), 0, 16);
	mValue1.setValue(value);
	cout << res;
      }
      else
      {
	mValue1.unavailable();
	disconnect();
	sleep(1);
      }
    }
  }
  
  catch (Serial::SerialError &e)
  {
    printf("SerialError: %s\n", e.message());
    disconnect();
  }
}

