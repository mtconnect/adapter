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
#include "emc_adapter.hpp"

EmcAdapter::EmcAdapter(int aPort, const char *aNmlFile)
  : Adapter(aPort, 10), 
    mAlarm("alarm"), mPower("power"), mExecution("execution"), mLine("line"),
    mXact("Xact"), mYact("Yact"), mZact("Zact"), 
    mXcom("Xcom"), mYcom("Ycom"), mZcom("Zcom"),
    mSpindleSpeed("spindle_speed"), mPathFeedrate("path_feedrate"),
    mProgram("program"), mMode("mode"), mBlock("block"),
    mPathFeedrateOverride("feed_ovr"), mSpindleSpeedOverride("SspeedOvr"),
    mLineMax("line_max")
{
  addDatum(mAlarm);
  addDatum(mPower);
  addDatum(mExecution);
  addDatum(mLine);
  addDatum(mXact);
  addDatum(mYact);
  addDatum(mZact);
  addDatum(mXcom);
  addDatum(mYcom);
  addDatum(mZcom);
  addDatum(mSpindleSpeed);
  addDatum(mPathFeedrate);
  addDatum(mProgram);
  addDatum(mMode);
  addDatum(mBlock);
  addDatum(mPathFeedrateOverride);
  addDatum(mSpindleSpeedOverride);
  addDatum(mLineMax);
    
  
  mErrorString[0] = 0;
  mEmcErrorBuffer = mEmcStatusBuffer = 0;
  strcpy(mNmlFile, aNmlFile);
  mConnected = false;
  mEstop = false;
}

bool EmcAdapter::connect()
{
  int retval = 0;
  if (mEmcStatusBuffer == 0)
  {
    mEmcStatusBuffer = new RCS_STAT_CHANNEL(emcFormat, "emcStatus", "adapter", mNmlFile);
    if (! mEmcStatusBuffer->valid() || EMC_STAT_TYPE != mEmcStatusBuffer->peek())
    {
      rcs_print_error("emcStatus buffer not available\n");
      delete mEmcStatusBuffer;
      
      mEmcStatusBuffer = 0;
      mPower.setValue(Power::eOFF);
      return false;
    }
  }
    
  if (mEmcErrorBuffer == 0)
  {
    mEmcErrorBuffer = new NML(nmlErrorFormat, "emcError", "adapter", mNmlFile);
    if (!mEmcErrorBuffer->valid())
    {
      rcs_print_error("emcError buffer not available\n");
      delete mEmcErrorBuffer;
      mEmcErrorBuffer = 0;
      mPower.setValue(Power::eOFF);
      return false;
    }
  }
  mConnected = true;
  return true;
}

void EmcAdapter::disconnect()
{
  if (mConnected)
  {
    if (mEmcErrorBuffer)
      delete mEmcErrorBuffer;
    mEmcErrorBuffer = 0;

    if (mEmcStatusBuffer)
      delete mEmcStatusBuffer;
    mEmcStatusBuffer = 0;
    mConnected = false;
  }
}

void EmcAdapter::actual()
{
  EmcPose &pose = mEmcStatus.motion.traj.actualPosition;
  mXact.setValue(pose.tran.x);
  mYact.setValue(pose.tran.y);
  mZact.setValue(pose.tran.z);
}

void EmcAdapter::commanded()
{
  EmcPose &pose = mEmcStatus.motion.traj.position;
  mXcom.setValue(pose.tran.x);
  mYcom.setValue(pose.tran.y);
  mZcom.setValue(pose.tran.z);
}

void EmcAdapter::spindle()
{
  mSpindleSpeed.setValue(mEmcStatus.motion.spindle.speed);
  mSpindleSpeedOverride.setValue(mEmcStatus.motion.traj.spindle_scale * 100.0);
}

void EmcAdapter::feedrate()
{
  mPathFeedrate.setValue(mEmcStatus.motion.traj.current_vel);
  mPathFeedrateOverride.setValue(mEmcStatus.motion.traj.scale * 100.0);
}

void EmcAdapter::program()
{
  if (mEmcStatus.task.file[0] != '\0' && mProgram.setValue(mEmcStatus.task.file))
  {
    // Read the file and split it into lines.
    int max_line = mBlocks.readFile(mEmcStatus.task.file);
    mLineMax.setValue(max_line);
  }

  int line = mEmcStatus.motion.traj.id;
  if (mLine.setValue(line))
  {
    if (line <= 0)
      mBlock.setValue("");
    else
      mBlock.setValue(mBlocks[line - 1]);
  }
}

void EmcAdapter::machine()
{
  if (mEmcStatus.task.state == EMC_TASK_STATE_ON)
  {
    mPower.setValue(Power::eON);
  }
  else
  {
    mPower.setValue(Power::eOFF);
  }
}

void EmcAdapter::execution()
{
  if (mEmcStatus.task.state != EMC_TASK_STATE_ON ||
      mEmcStatus.task.mode == EMC_TASK_MODE_MANUAL) 
  {
    mMode.setValue(ControllerMode::eMANUAL);
    mExecution.setValue(Execution::eSTOPPED);  
  }
  else if (mEmcStatus.task.mode == EMC_TASK_MODE_AUTO) 
  {
    mMode.setValue(ControllerMode::eAUTOMATIC);
    if (mEmcStatus.task.interpState == EMC_TASK_INTERP_PAUSED)
      mExecution.setValue(Execution::eINTERRUPTED);
    else if (mEmcStatus.task.interpState == EMC_TASK_INTERP_IDLE ||
	     mEmcStatus.task.interpState == EMC_TASK_INTERP_WAITING)
    {
      if (mEmcStatus.task.execState > 2)
	mExecution.setValue(Execution::eACTIVE);
      else if (mEmcStatus.task.execState == 2)
	mExecution.setValue(Execution::eREADY);
      else
	mExecution.setValue(Execution::eSTOPPED);  
    }
  }
  else if (mEmcStatus.task.mode == EMC_TASK_MODE_MDI) 
  {
    mMode.setValue(ControllerMode::eMANUAL_DATA_INPUT);
    mExecution.setValue(Execution::eSTOPPED);  
  }
}

void EmcAdapter::alarms()
{
  if (mEmcStatus.task.state == EMC_TASK_STATE_ESTOP && !mEstop) 
  {
    mAlarm.setValue(Alarm::eESTOP, "ESTOP", Alarm::eCRITICAL, Alarm::eACTIVE, "ESTOP Pressed");
    sendDatum(&mAlarm);
    mEstop = true;
  }
  else if (mEmcStatus.task.state != EMC_TASK_STATE_ESTOP && mEstop)
  {
    mAlarm.setValue(Alarm::eESTOP, "ESTOP", Alarm::eCRITICAL, Alarm::eCLEARED, "ESTOP Reset");
    sendDatum(&mAlarm);
    mEstop = false;
  }
  
  char error_string[LINELEN];
  char num[10];
  error_string[0] = 0;
  bool error = false;
  
  NMLTYPE type;
  
  switch (type = mEmcErrorBuffer->read()) {
  case -1:
    // error reading channel
    break;
    
  case 0:
    // nothing new
    break;
    
  case EMC_OPERATOR_ERROR_TYPE:
    strncpy(error_string,
	    ((EMC_OPERATOR_ERROR *) (mEmcErrorBuffer->get_address()))->
	    error, LINELEN - 1);
    error_string[LINELEN - 1] = 0;
    error = true;
    sprintf(num, "%d", type);
    mAlarm.setValue(Alarm::eOTHER, num, Alarm::eERROR, Alarm::eINSTANT, error_string);
    break;
    
  case EMC_OPERATOR_TEXT_TYPE:
    strncpy(error_string,
	    ((EMC_OPERATOR_TEXT *) (mEmcErrorBuffer->get_address()))->
	    text, LINELEN - 1);
    error_string[LINELEN - 1] = 0;
    error = true;
    sprintf(num, "%d", type);
    mAlarm.setValue(Alarm::eOTHER, num, Alarm::eINFO, Alarm::eINSTANT, error_string);
    break;
    
  case EMC_OPERATOR_DISPLAY_TYPE:
    strncpy(error_string,
	    ((EMC_OPERATOR_DISPLAY *) (mEmcErrorBuffer->
				       get_address()))->display,
	    LINELEN - 1);
    error_string[LINELEN - 1] = 0;
    error = true;
    sprintf(num, "%d", type);
    mAlarm.setValue(Alarm::eOTHER, num, Alarm::eINFO, Alarm::eINSTANT, error_string);
    break;
    
  case NML_ERROR_TYPE:
    strncpy(error_string,
	    ((NML_ERROR *) (mEmcErrorBuffer->get_address()))->error,
	    NML_ERROR_LEN - 1);
    error_string[NML_ERROR_LEN - 1] = 0;
    error = true;
    sprintf(num, "%d", type);
    mAlarm.setValue(Alarm::eOTHER, num, Alarm::eERROR, Alarm::eINSTANT, error_string);
    break;
    
  case NML_TEXT_TYPE:
    strncpy(error_string,
	    ((NML_TEXT *) (mEmcErrorBuffer->get_address()))->text,
	    NML_TEXT_LEN - 1);
    error_string[NML_TEXT_LEN - 1] = 0;
    error = true;
    sprintf(num, "%d", type);
    mAlarm.setValue(Alarm::eOTHER, num, Alarm::eINFO, Alarm::eINSTANT, error_string);
    break;
    
  case NML_DISPLAY_TYPE:
    strncpy(error_string,
	    ((NML_DISPLAY *) (mEmcErrorBuffer->get_address()))->display,
	    NML_DISPLAY_LEN - 1);
    error_string[NML_DISPLAY_LEN - 1] = 0;
    error = true;
    sprintf(num, "%d", type);
    mAlarm.setValue(Alarm::eOTHER, num, Alarm::eINFO, Alarm::eINSTANT, error_string);
    break;
    
  default:
    // if not recognized, set the error string
    sprintf(error_string, "unrecognized error %ld", type);
    sprintf(num, "%d", type);
    mAlarm.setValue(Alarm::eOTHER, num, Alarm::eWARNING, Alarm::eINSTANT, error_string);
    error = true;
    break;
  }

  if (error)
  {
    sendDatum(&mAlarm);
  }
}

EmcAdapter::~EmcAdapter()
{
  disconnect();
}

void EmcAdapter::gatherDeviceData()
{
  if (!mConnected)
  {
    if (!connect())
      sleep(5);
  }
  else
  {
    if (!mEmcStatusBuffer->valid())
    {
      disconnect();
      return;
    }
    
    if(mEmcStatusBuffer->peek() == EMC_STAT_TYPE) {
      memcpy(&mEmcStatus, mEmcStatusBuffer->get_address(), sizeof(EMC_STAT));
      actual();
      commanded();
      spindle();
      feedrate();
      program();
      machine();
      execution();
      alarms();
    }
  } 
}

