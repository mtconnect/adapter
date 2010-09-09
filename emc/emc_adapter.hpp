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

#ifndef EMC_ADAPTER_HPP
#define EMC_ADAPTER_HPP

#include "adapter.hpp"
#include "device_datum.hpp"
#include "string_array.hpp"

#include "emc2/rcs.hh"               // rcs_print_error(), esleep()
#include "emc2/emc.hh"               // EMC NML
#include "emc2/emc_nml.hh"
#include "emc2/emcglb.h"             // EMC_NMLFILE, TRAJ_MAX_VELOCITY
#include "emc2/emccfg.h"             // DEFAULT_TRAJ_MAX_VELOCITY
#include "emc2/inifile.hh"           // INIFILE
#include "emc2/rcs_print.hh"
#include "emc2/nml_oi.hh"
#include "emc2/timer.hh"

class EmcAdapter : public Adapter 
{
protected:
  /* Define all the data values here */
  
  /* Conditions */
  
  
  /* Events */
  Availability mAvail;
  EmergencyStop mEstop;
  PowerState mPower;
  Execution mExecution;
  IntEvent mLine; 
  IntEvent mLineMax; 
  Event mProgram;
  ControllerMode mMode;
  Event mBlock;
  Message mMessage;
  
  /* Samples */
  /* Linear Axis */
  Sample mXact;
  Sample mYact;
  Sample mZact;
  
  Sample mXcom;
  Sample mYcom;
  Sample mZcom;
  
  /* Spindle */
  Sample mSpindleSpeed;  
  Sample mSpindleSpeedOverride;  
  
  /* Path Feedrate */
  Sample mPathFeedrate;
  Sample mPathFeedrateOverride;
  
protected:
  EMC_STAT mEmcStatus;
  NML *mEmcErrorBuffer;
  char mErrorString[LINELEN];
  RCS_STAT_CHANNEL *mEmcStatusBuffer;
  bool mConnected;
  StringArray mBlocks;
  
  char mNmlFile[1024];
  
protected:
  bool connect();
  void disconnect();
  void actual();
  void commanded();
  void spindle();
  void feedrate();
  void program();
  void machine();
  void execution();
  void alarms();
  
public:
  EmcAdapter(int aPort, const char *aNmlFile);
  ~EmcAdapter();
  
  virtual void gatherDeviceData();
};

#endif

