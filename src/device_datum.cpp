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

static const char *sUnavailable = "UNAVAILABLE";

/*
 * Data value methods.
 */
DeviceDatum::DeviceDatum(const char *aName)
{
  strncpy(mName, aName, NAME_LEN);
  mName[NAME_LEN - 1] = '\0';
  strcpy(mOrigName, mName);
  mChanged = false;
  mHasValue = false;
}


DeviceDatum::~DeviceDatum()
{
}

bool DeviceDatum::prefixName(const char *aName)
{
  // Check for overflow.
  int len = strlen(aName);
  if (strlen(mOrigName) + len >= (size_t) NAME_LEN)
    return false;

  strcpy(mName, aName);
  mName[len++] = ':';
  strcpy(mName + len, mOrigName);

  // Make sure the whole thing is terminated
  mName[NAME_LEN - 1] = '\0';

  return true;
}

void DeviceDatum::setName(const char *aName)
{
  strncpy(mName, aName, NAME_LEN);
  mName[NAME_LEN - 1] = '\0';
  strcpy(mOrigName, mName);
}

void DeviceDatum::setNativeUnits(const char *aUnits)
{
  strncpy(mNativeUnits, aUnits, UNITS_LEN);
  mNativeUnits[UNITS_LEN - 1] = '\0';
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

// Append text to the buffer and remove all <CR> and <NL> characters. 
void DeviceDatum::appendText(char *aBuffer, char *aValue, int aMaxLen)
{
  size_t len = strlen(aBuffer);
  char *cp = aValue, *dp = aBuffer + len;
  for (size_t i = len; i < (size_t) aMaxLen && *cp != '\0'; i++)
  {
	if (*cp == '\n' || *cp == '\r')
		*dp = ' ';
	else
		*dp = *cp;
	dp++; cp++;
  }
  *dp = '\0';
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
  appendText(aBuffer, mValue, aMaxLen);
  return aBuffer;
}

bool Event::unavailable()
{
  return setValue(sUnavailable);
}

/*
 * IntEvent methods
 */
IntEvent::IntEvent(const char *aName)
  : DeviceDatum(aName)
{
  mValue = 0;
  mUnavailable = false;
}

bool IntEvent::setValue(int aValue)
{
  if (aValue !=  mValue || !mHasValue || mUnavailable)
  {
    mChanged = true;
    mValue = aValue;
    mHasValue = true;
    mUnavailable = false;
  }
  
  return mChanged;
}

char *IntEvent::toString(char *aBuffer, int aMaxLen)
{
  if (mUnavailable)
    snprintf(aBuffer, aMaxLen, "|%s|UNAVAILABLE", mName);
  else
    snprintf(aBuffer, aMaxLen, "|%s|%d", mName, mValue);

  return aBuffer;
}

bool IntEvent::unavailable()
{
  if (!mUnavailable)
  {
    mChanged = true;
    mUnavailable = true;
  }
  
  return mChanged;
}

/*
 * Sample methods
 */
Sample::Sample(const char *aName, double aEpsilon)
  : DeviceDatum(aName), mEpsilon(aEpsilon)
{
  mValue = 0.0;
  mUnavailable = false;
}
 
bool Sample::setValue(double aValue)
{
  if (fabs(aValue - mValue) > mEpsilon || !mHasValue ||
      mUnavailable)
  {
      mChanged = true;
      mValue = aValue;
      mHasValue = true;
      mUnavailable = false;
  }
  return mChanged;
}

char *Sample::toString(char *aBuffer, int aMaxLen)
{
  if (mUnavailable)
    snprintf(aBuffer, aMaxLen, "|%s|UNAVAILABLE", mName);
  else
    snprintf(aBuffer, aMaxLen, "|%s|%.10g", mName, mValue);
  return aBuffer;
}

bool Sample::unavailable()
{
  if (!mUnavailable)
  {
    mChanged = true;
    mUnavailable = true;
    mHasValue = true;
  }
  
  return mChanged;
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
  case eUNAVAILABLE: text = sUnavailable; break;
  case eON: text = "ON"; break;
  case eOFF: text = "OFF"; break;
  default: text = ""; break;
  }
  snprintf(aBuffer, aMaxLen, "|%s|%s", mName, text);
  return aBuffer;
}

bool PowerState::unavailable()
{
  return setValue(eUNAVAILABLE);
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
  case eUNAVAILABLE: text = sUnavailable; break;
  case eACTIVE: text = "ACTIVE"; break;
  case eREADY: text = "READY"; break;
  case eINTERRUPTED: text = "INTERRUPTED"; break;
  case eSTOPPED: text = "STOPPED"; break;
  case eFEED_HOLD: text = "FEED_HOLD"; break;
  default: text = ""; break;
  }
  snprintf(aBuffer, aMaxLen, "|%s|%s", mName, text);
  return aBuffer;
}

bool Execution::unavailable()
{
  return setValue(eUNAVAILABLE);
}

/* ControllerMode */

char *ControllerMode::toString(char *aBuffer, int aMaxLen)
{
  const char *text;
  switch(mMode)
  {
  case eUNAVAILABLE: text = sUnavailable; break;
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

bool ControllerMode::unavailable()
{
  return setValue(eUNAVAILABLE);
}

/* Direction */

char *Direction::toString(char *aBuffer, int aMaxLen)
{
  const char *text;
  switch(mDirection)
  {
  case eUNAVAILABLE: text = sUnavailable; break;
  case eCLOCKWISE: text = "CLOCKWISE"; break;
  case eCOUNTER_CLOCKWISE: text = "COUNTER_CLOCKWISE"; break;
  default: text = ""; break;
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

bool Direction::unavailable()
{
  return setValue(eUNAVAILABLE);
}

/* Emergency Stop */

char *EmergencyStop::toString(char *aBuffer, int aMaxLen)
{
  const char *text;
  switch(mValue)
  {
  case eUNAVAILABLE: text = sUnavailable; break;
  case eTRIGGERED: text = "TRIGGERED"; break;
  case eARMED: text = "ARMED"; break;
  default: text = ""; break;
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

bool EmergencyStop::unavailable()
{
  return setValue(eUNAVAILABLE);
}

/* Axis Coupling */

char *AxisCoupling::toString(char *aBuffer, int aMaxLen)
{
  const char *text;
  switch(mValue)
  {
  case eUNAVAILABLE: text = sUnavailable; break;
  case eTANDEM: text = "TANDEM"; break;
  case eSYNCHRONOUS: text = "SYNCHRONOUS"; break;
  case eMASTER: text = "MASTER"; break;
  case eSLAVE: text = "SLAVE"; break;
  default: text = ""; break;
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

bool AxisCoupling::unavailable()
{
  return setValue(eUNAVAILABLE);
}

/* Door State */

char *DoorState::toString(char *aBuffer, int aMaxLen)
{
  const char *text;
  switch(mValue)
  {
  case eUNAVAILABLE: text = sUnavailable; break;
  case eOPEN: text = "CLOSED"; break;
  case eCLOSED: text = "OPEN"; break;
  default: text = ""; break;
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

bool DoorState::unavailable()
{
  return setValue(eUNAVAILABLE);
}

// Path Mode

char *PathMode::toString(char *aBuffer, int aMaxLen)
{
  const char *text;
  switch(mValue)
  {
  case eUNAVAILABLE: text = sUnavailable; break;
  case eINDEPENDENT: text = "INDEPENDENT"; break;
  case eSYNCHRONOUS: text = "SYNCHRONOUS"; break;
  case eMIRROR: text = "MIRROR"; break;
  default: text = ""; break;
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

bool PathMode::unavailable()
{
  return setValue(eUNAVAILABLE);
}

// Rotary Mode

char *RotaryMode::toString(char *aBuffer, int aMaxLen)
{
  const char *text;
  switch(mValue)
  {
  case eUNAVAILABLE: text = sUnavailable; break;
  case eSPINDLE: text = "SPINDLE"; break;
  case eINDEX: text = "INDEX"; break;
  case eCONTOUR: text = "CONTOUR"; break;
  default: text = ""; break;
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

bool RotaryMode::unavailable()
{
  return setValue(eUNAVAILABLE);
}


// Message

Message::Message(const char *aName) :
  DeviceDatum(aName)
{
  mNativeCode[0] = 0;
}

char *Message::toString(char *aBuffer, int aMaxLen)
{
  snprintf(aBuffer, aMaxLen, "|%s|%s|", mName, mNativeCode);
  appendText(aBuffer, mText, aMaxLen);
  return aBuffer;
}

 bool Message::setValue(const char *aText, const char *aCode)
{
  if (!mHasValue ||
      strncmp(aCode, mNativeCode, EVENT_VALUE_LEN) != 0 ||
      strncmp(aText, mText, EVENT_VALUE_LEN) != 0)
    
  {
    strncpy(mNativeCode, aCode, EVENT_VALUE_LEN);
    mNativeCode[EVENT_VALUE_LEN - 1] = '\0';
    
    strncpy(mText, aText, EVENT_VALUE_LEN);
    mText[EVENT_VALUE_LEN - 1] = '\0';
    
    mChanged = true;
    mHasValue = true;
  }
  
  return mChanged;
}

bool Message::requiresFlush()
{
  return true;
}

bool Message::unavailable()
{
  return setValue(sUnavailable);
}

/*
 * PathPosition methods
 */
PathPosition::PathPosition(const char *aName, double aEpsilon)
  : DeviceDatum(aName), mEpsilon(aEpsilon)
{
  mX = mY = mZ = 0.0;
  mUnavailable = false;
}
 
bool PathPosition::setValue(double aX, double aY, double aZ)
{
  if (!mHasValue ||
      fabs(aX - mX) > mEpsilon ||
      fabs(aY - mY) > mEpsilon ||
      fabs(aZ - mZ) > mEpsilon ||
      mUnavailable)
  {
      mChanged = true;
      mX = aX; mY = aY; mZ = aZ;
      mHasValue = true;
      mUnavailable = false;
  }
  return mChanged;
}

char *PathPosition::toString(char *aBuffer, int aMaxLen)
{
  if (mUnavailable)
    snprintf(aBuffer, aMaxLen, "|%s|UNAVAILABLE", mName);
  else
    snprintf(aBuffer, aMaxLen, "|%s|%.10f %0.10f %0.10f", mName, mX, mY, mZ);
  return aBuffer;
}

bool PathPosition::unavailable()
{
  if (!mUnavailable)
  {
    mChanged = true;
    mUnavailable = true;
  }
  
  return mChanged;
}

/*
 *  Availability methods
 */

Availability::Availability(const char *aName)
  : DeviceDatum(aName)
{
  mHasValue = true;
  mUnavailable = false;
}
 
char *Availability::toString(char *aBuffer, int aMaxLen)
{
  if (mUnavailable)
    snprintf(aBuffer, aMaxLen, "|%s|UNAVAILABLE", mName);
  else
    snprintf(aBuffer, aMaxLen, "|%s|AVAILABLE", mName);
  return aBuffer;
}

bool Availability::unavailable()
{
  if (!mUnavailable)
  {
    mChanged = true;
    mUnavailable = true;
  }
  
  return mChanged;
}

bool Availability::available()
{
  if (mUnavailable)
  {
    mChanged = true;
    mUnavailable = false;
  }
  
  return mChanged;
}
