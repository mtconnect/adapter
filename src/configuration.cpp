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

#include "configuration.hpp"
#include "yaml.h"

using namespace std;

Configuration::Configuration()
  : mPort(7878), mScanDelay(1000), mTimeout(10000)
{
}

void Configuration::parse(istream &aStream, int aPort, int aDelay, int aTimeout, const char *aService)
{
  YAML::Parser parser(aStream);
  YAML::Node doc;
  parser.GetNextDocument(doc);
  parse(doc, aPort, aDelay, aTimeout, aService);
}

void Configuration::parse(YAML::Node &aDoc, int aPort, int aDelay, int aTimeout, const char *aService)
{
  if (aDoc.FindValue("adapter") != NULL)
  {
    const YAML::Node &adapter = aDoc["adapter"];
    SET_WITH_DEFAULT(adapter, "port", mPort, aPort);
    SET_WITH_DEFAULT(adapter, "scanDelay", mScanDelay, aDelay);
    SET_WITH_DEFAULT(adapter, "timeout", mTimeout, aTimeout);
    SET_WITH_DEFAULT(adapter, "service", mServiceName, aService);
  }
  else
  {
  	mPort = aPort;
  	mScanDelay = aDelay;
  	mTimeout = aTimeout;
  	mServiceName = aService;
  }
}

Configuration::~Configuration()
{
}

RegisterSet *Configuration::getRegisters(string &aName)
{
  return mRegisters[aName];
}



