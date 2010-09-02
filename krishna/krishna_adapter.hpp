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

#ifndef KRISHNA_ADAPTER_HPP
#define KRISHNA_ADAPTER_HPP

#include "adapter.hpp"
#include "service.hpp"
#include "device_datum.hpp"
#include "serial.hpp"
#include "XBee.h"
#include "krishna_data.hpp"
#include "krishna_meter.hpp"
#include <string>
#include <vector>

class KrishnaAdapter : public Adapter, public MTConnectService
{
public:
  KrishnaAdapter(int aPort);
  ~KrishnaAdapter();
  
  virtual void initialize(int aArgc, const char *aArgv[]);
  virtual void start();
  virtual void stop();
  
  void initializeMeter(KrishnaMeter *aMeter);
  void requestData(KrishnaMeter *aMeter);
  virtual void gatherDeviceData();
  
protected:
  /* Events */
  std::vector<KrishnaMeter*> mMeters; 

  int mConnected;
  std::string mSerialPort;
  std::string mParity;
  int mBaud, mDataBits, mStopBits;
  int mTimeout;

  Serial *mSerial;
  XBee   mXBee;  
};

#endif

