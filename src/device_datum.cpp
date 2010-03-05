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

/* Power */

bool PowerState::setValue(enum EPowerState aState)
{
  if (mState != aState || !mHasValue)
  {
    mState = aState;
    mChanged = true;
    mHasValue = true;
  }
  return mChanged;
}

char *PowerState::toString(char *aBuffer, int aMaxLen)
{
  const char *text;
  switch(mState)
  {
  case eUNAVAILABLE: text = "UNAVAILABLE"; break;
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
  case eUNAVAILABLE: text = "UNAVAILABLE"; break;
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
  case eUNAVAILABLE: text = "UNAVAILABLE"; break;
  case eSEMI_AUTOMATIC: text = "SEMI_AUTOMATIC"; break;
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

  return mChanged;
}

/* Direction */

char *Direction::toString(char *aBuffer, int aMaxLen)
{
  const char *text;
  switch(mDirection)
  {
  case eUNAVAILABLE: text = "UNAVAILABLE"; break;
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

  return mChanged;
}

/* Emergency Stop */

char *EmergencyStop::toString(char *aBuffer, int aMaxLen)
{
  const char *text;
  switch(mValue)
  {
  case eUNAVAILABLE: text = "UNAVAILABLE"; break;
  case eACTIVE: text = "ACTIVE"; break;
  case eRESET: text = "RESET"; break;
  default: ""; break;
  }
  snprintf(aBuffer, aMaxLen, "|%s|%s", mName, text);
  return aBuffer;
}

bool EmergencyStop::setValue(enum EValues aValue)
{
  if (mValue != aValue || !mHasValue)
  {
    mValue = aValue;
    mChanged = true;
    mHasValue = true;
  }

  return mChanged;
}

/* Axis Coupling */

char *AxisCoupling::toString(char *aBuffer, int aMaxLen)
{
  const char *text;
  switch(mValue)
  {
  case eUNAVAILABLE: text = "UNAVAILABLE"; break;
  case eTANDEM: text = "TANDEM"; break;
  case eSYNCHRONOUS: text = "SYNCHRONOUS"; break;
  case eMASTER: text = "MASTER"; break;
  case eSLAVE: text = "SLAVE"; break;
  default: ""; break;
  }
  snprintf(aBuffer, aMaxLen, "|%s|%s", mName, text);
  return aBuffer;
}

bool AxisCoupling::setValue(enum EValues aValue)
{
  if (mValue != aValue || !mHasValue)
  {
    mValue = aValue;
    mChanged = true;
    mHasValue = true;
  }
  return mChanged;
}

/* Door State */

char *DoorState::toString(char *aBuffer, int aMaxLen)
{
  const char *text;
  switch(mValue)
  {
  case eUNAVAILABLE: text = "UNAVAILABLE"; break;
  case eOPEN: text = "CLOSED"; break;
  case eCLOSED: text = "OPEN"; break;
  default: ""; break;
  }
  snprintf(aBuffer, aMaxLen, "|%s|%s", mName, text);
  return aBuffer;
}

bool DoorState::setValue(enum EValues aValue)
{
  if (mValue != aValue || !mHasValue)
  {
    mValue = aValue;
    mChanged = true;
    mHasValue = true;
  }
  return mChanged;
}

// Path Mode

char *PathMode::toString(char *aBuffer, int aMaxLen)
{
  const char *text;
  switch(mValue)
  {
  case eUNAVAILABLE: text = "UNAVAILABLE"; break;
  case eINDEPENDENT: text = "INDEPENDENT"; break;
  case eSYNCHRONOUS: text = "SYNCHRONOUS"; break;
  case eMIRROR: text = "MIRROR"; break;
  default: ""; break;
  }
  snprintf(aBuffer, aMaxLen, "|%s|%s", mName, text);
  return aBuffer;
}

bool PathMode::setValue(enum EValues aValue)
{
  if (mValue != aValue || !mHasValue)
  {
    mValue = aValue;
    mChanged = true;
    mHasValue = true;
  }
  return mChanged;
}

// Rotary Mode

char *RotaryMode::toString(char *aBuffer, int aMaxLen)
{
  const char *text;
  switch(mValue)
  {
  case eUNAVAILABLE: text = "UNAVAILABLE"; break;
  case eSPINDLE: text = "SPINDLE"; break;
  case eINDEX: text = "INDEX"; break;
  case eCONTOUR: text = "CONTOUR"; break;
  default: ""; break;
  }
  snprintf(aBuffer, aMaxLen, "|%s|%s", mName, text);
  return aBuffer;
}

bool RotaryMode::setValue(enum EValues aValue)
{
  if (mValue != aValue || !mHasValue)
  {
    mValue = aValue;
    mChanged = true;
    mHasValue = true;
  }
  return mChanged;
}

// Condition

Condition::Condition(const char *aName) :
  DeviceDatum(aName), mLevel(eUNAVAILABLE)
{
  mNativeCode[0] = mNativeSeverity[0] = mText[0] =
   mQualifier[0] = 0;
}

char *Condition::toString(char *aBuffer, int aMaxLen)
{
  const char *text;
  switch(mLevel)
  {
  case eUNAVAILABLE: text = "UNAVAILABLE"; break;
  case eNORMAL: text = "NORMAL"; break;
  case eWARNING: text = "WARNING"; break;
  case eFAULT: text = "FAULT"; break;
  default: ""; break;
  }
  snprintf(aBuffer, aMaxLen, "|%s|%s|%s|%s|%s|%s", mName, text, mNativeCode, mNativeSeverity,
	   mQualifier, mText);
  return aBuffer;
}

 bool Condition::setValue(ELevels aLevel, const char *aText, const char *aCode,
			  const char *aQualifier, const char *aSeverity)
{
  if (!mHasValue ||
      mLevel != aLevel ||
      strncmp(aCode, mNativeCode, EVENT_VALUE_LEN) != 0 ||
      strncmp(aQualifier, mQualifier, EVENT_VALUE_LEN) != 0 ||
      strncmp(aSeverity, mNativeSeverity, EVENT_VALUE_LEN) != 0 ||
      strncmp(aText, mText, EVENT_VALUE_LEN) != 0)
    
  {
    mLevel = aLevel;
    
    strncpy(mNativeCode, aCode, EVENT_VALUE_LEN);
    mNativeCode[EVENT_VALUE_LEN - 1] = '\0';
    
    strncpy(mQualifier, aQualifier, EVENT_VALUE_LEN);
    mQualifier[EVENT_VALUE_LEN - 1] = '\0';

    strncpy(mNativeSeverity, aSeverity, EVENT_VALUE_LEN);
    mNativeSeverity[EVENT_VALUE_LEN - 1] = '\0';

    strncpy(mText, aText, EVENT_VALUE_LEN);
    mText[EVENT_VALUE_LEN - 1] = '\0';
    
    mChanged = true;
    mHasValue = true;
  }
  
  return mChanged;
}

bool Condition::requiresFlush()
{
  return true;
}







  
