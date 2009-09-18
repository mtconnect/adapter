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
#include "device_datum.hpp"
#include "string_buffer.hpp"

/*
 * Data value methods.
 */
DeviceDatum::DeviceDatum(const char *aName)
{
  strncpy(mName, aName, NAME_LEN);
  mName[NAME_LEN - 1] = '\0';
  mChanged = false;
  mHasValue = false;
}

DeviceDatum::~DeviceDatum()
{
}

bool DeviceDatum::append(StringBuffer &aBuffer)
{
  char buffer[1024];
  aBuffer.append(toString(buffer, 1024));
  mChanged = false;
  return mChanged;
}

bool DeviceDatum::hasInitialValue()
{
  return mHasValue;
}

bool DeviceDatum::requiresFlush()
{
  return false;
}


/*
 * Event methods
 */
Event::Event(const char* aName) :
  DeviceDatum(aName)
{
  mValue[0] = 0;
}

bool Event::setValue(const char *aValue)
{
  if (strncmp(aValue, mValue, EVENT_VALUE_LEN) != 0 || !mHasValue)
  {
    mChanged = true;
    strncpy(mValue, aValue, EVENT_VALUE_LEN);
    mValue[EVENT_VALUE_LEN - 1] = '\0';
    mHasValue = true;
  }
  return mChanged;
}

char *Event::toString(char *aBuffer, int aMaxLen)
{
  snprintf(aBuffer, aMaxLen, "|%s|", mName);
  size_t len = strlen(aBuffer);
  size_t max = aMaxLen - len;
  char *cp = mValue, *dp = aBuffer + len;
  for (size_t i = len; i < aMaxLen && *cp != '\0'; i++)
  {
	if (*cp == '\n' || *cp == '\r')
		*dp = ' ';
	else
		*dp = *cp;
	dp++; cp++;
  }
  *dp = '\0';
  return aBuffer;
}

/*
 * IntEvent methods
 */
IntEvent::IntEvent(const char *aName)
  : DeviceDatum(aName)
{
  mValue = 0;
}

bool IntEvent::setValue(int aValue)
{
  if (aValue !=  mValue || !mHasValue)
  {
    mChanged = true;
    mValue = aValue;
    mHasValue = true;
  }
  return mChanged;
}

char *IntEvent::toString(char *aBuffer, int aMaxLen)
{
  snprintf(aBuffer, aMaxLen, "|%s|%d", mName, mValue);
  return aBuffer;
}

/*
 * Sample methods
 */
Sample::Sample(const char *aName)
  : DeviceDatum(aName)
{
  mValue = 0.0;
}
 
bool Sample::setValue(double aValue)
{
  if (fabs(aValue - mValue) > 0.000001 || !mHasValue)
  {
      mChanged = true;
      mValue = aValue;
      mHasValue = true;
  }
  return mChanged;
}

char *Sample::toString(char *aBuffer, int aMaxLen)
{
  snprintf(aBuffer, aMaxLen, "|%s|%.10f", mName, mValue);
  return aBuffer;
}

/*
 * Alarm methods
 */
Alarm::Alarm(const char *aName) :
  DeviceDatum(aName), mCode(eOTHER), mState(eCLEARED),
  mSeverity(eWARNING)
{
  
  mNativeCode[0] = 0;
  mDescription[0] = 0;
}

const char *Alarm::getCodeText()
{
  switch (mCode)
  {
  case eFAILURE: return "FAILURE";
  case eFAULT: return "FAULT";
  case eCRASH: return "CRASH";
  case eJAM: return "JAM";
  case eOVERLOAD: return "OVERLOAD";
  case eESTOP: return "ESTOP";
  case eMATERIAL: return "MATERIAL";
  case eMESSAGE: return "MESSAGE";
  case eOTHER: return "OTHER";
  }
  return "";
}

const char *Alarm::getSeverityText()
{
  switch (mSeverity)
  {
    case eCRITICAL: return "CRITICAL";
    case eERROR: return "ERROR";
    case eWARNING: return "WARNING";
    case eINFO: return "INFO";
  }
  return "";
}

const char *Alarm::getStateText()
{
  switch (mState)
  {
    case eINSTANT: return "INSTANT";
    case eACTIVE: return "ACTIVE";
    case eCLEARED: return "CLEARED";
  }
  return "";
}

bool Alarm::setValue(enum ECode aCode, const char *aNativeCode, enum ESeverity aSeverity,
	                  enum EState aState, const char *aDescription)
{
  if (mCode != aCode || strcmp(mNativeCode, aNativeCode) != 0 ||
      mSeverity != aSeverity || strcmp(mDescription, aDescription) != 0 ||
      mState != aState || !mHasValue)
  {
    mCode = aCode;
    mSeverity = aSeverity;
    mState = aState;

    strncpy(mNativeCode, aNativeCode, NATIVE_CODE_LEN);
    mNativeCode[NATIVE_CODE_LEN - 1] = '\0';

    strncpy(mDescription, aDescription, DESCRIPTION_LEN);
    mDescription[DESCRIPTION_LEN - 1] = '\0';
    for (size_t i = 0; i < strlen(mDescription); i++)
    {
      if (mDescription[i] == '\n' || mDescription[i] == '\r')
	mDescription[i] = ' ';
    }

    mChanged = true;
    mHasValue = true;
  }
   
  return mChanged;
}

char *Alarm::toString(char *aBuffer, int aMaxLen)
{
  snprintf(aBuffer, aMaxLen, "|%s|%s|%s|%s|%s|%s", mName, getCodeText(), mNativeCode, 
            getSeverityText(), getStateText(), mDescription);
  return aBuffer;
}

bool Alarm::hasInitialValue()
{
  return mState == eACTIVE && mHasValue;
}

/* Alarms require since they need to be on their own line */
bool Alarm::requiresFlush()
{
  return true;
}

/* StatefullAlarm */
StatefullAlarm::StatefullAlarm(const char *aName, enum ECode aCode, const char *aNativeCode,
			       enum ESeverity aSeverity, const char *aDescription)
  : Alarm(aName)
{
  mCode = aCode;
  strcpy(mNativeCode, aNativeCode);
  mSeverity = aSeverity;
  strcpy(mDescription, aDescription);
}

bool StatefullAlarm::setState(enum EState aState)
{
  if (mState != aState || !mHasValue)
  {
    mState = aState;

    mChanged = true;
    mHasValue = true;
  }
   
  return mChanged;
}

bool StatefullAlarm::hasInitialValue()
{
  return mHasValue;
}



/* Power */

bool Power::setValue(enum EPowerStatus aStatus)
{
  if (mStatus != aStatus || !mHasValue)
  {
    mStatus = aStatus;
    mChanged = true;
    mHasValue = true;
  }
  return mChanged;
}

char *Power::toString(char *aBuffer, int aMaxLen)
{
  const char *text;
  switch(mStatus)
  {
  case eON: text = "ON"; break;
  case eOFF: text = "OFF"; break;
  default: text = ""; break;
  }
  snprintf(aBuffer, aMaxLen, "|%s|%s", mName, text);
  return aBuffer;
}

/* Execution */

bool Execution::setValue(enum EExecutionState aState)
{
  if (mState != aState || !mHasValue)
  {
    mState = aState;
    mChanged = true;
    mHasValue = true;
  }
  
  return mChanged;
}

char *Execution::toString(char *aBuffer, int aMaxLen)
{
  const char *text;
  switch(mState)
  {
  case eACTIVE: text = "ACTIVE"; break;
  case eREADY: text = "READY"; break;
  case eINTERRUPTED: text = "INTERRUPTED"; break;
  case eSTOPPED: text = "STOPPED"; break;
  default: text = ""; break;
  }
  snprintf(aBuffer, aMaxLen, "|%s|%s", mName, text);
  return aBuffer;
}

/* ControllerMode */

char *ControllerMode::toString(char *aBuffer, int aMaxLen)
{
  const char *text;
  switch(mMode)
  {
  case eAUTOMATIC: text = "AUTOMATIC"; break;
  case eMANUAL: text = "MANUAL"; break;
  case eMANUAL_DATA_INPUT: text = "MANUAL_DATA_INPUT"; break;
  default: text = ""; break;
  }
  snprintf(aBuffer, aMaxLen, "|%s|%s", mName, text);
  return aBuffer;
}

bool ControllerMode::setValue(enum EMode aMode)
{
  if (mMode != aMode || !mHasValue)
  {
    mMode = aMode;
    mChanged = true;
    mHasValue = true;
  }
  return false;
}

/* Direction */

char *Direction::toString(char *aBuffer, int aMaxLen)
{
  const char *text;
  switch(mDirection)
  {
  case eCLOCKWISE: text = "CLOCKWISE"; break;
  case eCOUNTER_CLOCKWISE: text = "COUNTER_CLOCKWISE"; break;
  default: ""; break;
  }
  snprintf(aBuffer, aMaxLen, "|%s|%s", mName, text);
  return aBuffer;
}

bool Direction::setValue(enum ERotationDirection aDirection)
{
  if (mDirection != aDirection || !mHasValue)
  {
    mDirection = aDirection;
    mChanged = true;
    mHasValue = true;
  }
  return false;
}


  
