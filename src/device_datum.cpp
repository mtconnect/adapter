//
// Copyright (c) 2008, AMT - The Association For Manufacturing Technology ("AMT")
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the AMT nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// DISCLAIMER OF WARRANTY. ALL MTCONNECT MATERIALS AND SPECIFICATIONS PROVIDED
// BY AMT, MTCONNECT OR ANY PARTICIPANT TO YOU OR ANY PARTY ARE PROVIDED "AS IS"
// AND WITHOUT ANY WARRANTY OF ANY KIND. AMT, MTCONNECT, AND EACH OF THEIR
// RESPECTIVE MEMBERS, OFFICERS, DIRECTORS, AFFILIATES, SPONSORS, AND AGENTS
// (COLLECTIVELY, THE "AMT PARTIES") AND PARTICIPANTS MAKE NO REPRESENTATION OR
// WARRANTY OF ANY KIND WHATSOEVER RELATING TO THESE MATERIALS, INCLUDING, WITHOUT
// LIMITATION, ANY EXPRESS OR IMPLIED WARRANTY OF NONINFRINGEMENT,
// MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
//
// LIMITATION OF LIABILITY. IN NO EVENT SHALL AMT, MTCONNECT, ANY OTHER AMT
// PARTY, OR ANY PARTICIPANT BE LIABLE FOR THE COST OF PROCURING SUBSTITUTE GOODS
// OR SERVICES, LOST PROFITS, LOSS OF USE, LOSS OF DATA OR ANY INCIDENTAL,
// CONSEQUENTIAL, INDIRECT, SPECIAL OR PUNITIVE DAMAGES OR OTHER DIRECT DAMAGES,
// WHETHER UNDER CONTRACT, TORT, WARRANTY OR OTHERWISE, ARISING IN ANY WAY OUT OF
// THIS AGREEMENT, USE OR INABILITY TO USE MTCONNECT MATERIALS, WHETHER OR NOT
// SUCH PARTY HAD ADVANCE NOTICE OF THE POSSIBILITY OF SUCH DAMAGES.
//
#include "internal.hpp"
#include "device_datum.hpp"
#include "string_buffer.hpp"

static const char *sUnavailable = "UNAVAILABLE";


DeviceDatum::DeviceDatum(const char *name)
{
	strncpy(mName, name, NAME_LEN);
	mName[NAME_LEN - 1] = '\0';
	strcpy(mOrigName, mName);
	mChanged = false;
	mHasValue = false;
}


DeviceDatum::~DeviceDatum()
{
}


bool DeviceDatum::prefixName(const char *name)
{
	// Check for overflow.
	auto len = strlen(name);
	if (strlen(mOrigName) + len >= (size_t) NAME_LEN)
		return false;

	strcpy(mName, name);
	mName[len++] = ':';
	strcpy(mName + len, mOrigName);

	// Make sure the whole thing is terminated
	mName[NAME_LEN - 1] = '\0';
	return true;
}


void DeviceDatum::setName(const char *name)
{
	strncpy(mName, name, NAME_LEN);
	mName[NAME_LEN - 1] = '\0';
	strcpy(mOrigName, mName);
}


void DeviceDatum::setNativeUnits(const char *units)
{
	strncpy(mNativeUnits, units, UNITS_LEN);
	mNativeUnits[UNITS_LEN - 1] = '\0';
}


bool DeviceDatum::append(StringBuffer &stringBuffer)
{
	char buffer[1024];
	stringBuffer.append(toString(buffer, 1024));
	mChanged = false;
	return mChanged;
}


bool DeviceDatum::hasInitialValue() const
{
	return mHasValue;
}


bool DeviceDatum::requiresFlush() const
{
	return false;
}


// Append text to the buffer and remove all <CR> and <NL> characters.
void DeviceDatum::appendText(char *buffer, char *value, int maxLen)
{
	auto len = strlen(buffer);
	auto cp = value;
	auto dp = buffer + len;

	for (auto i = len; i < (size_t)maxLen && *cp != '\0'; i++)
	{
		if (*cp == '\n' || *cp == '\r')
			*dp = ' ';
		else
			*dp = *cp;

		dp++;
		cp++;
	}

	*dp = '\0';
}


Event::Event(const char *name) :
	DeviceDatum(name),
	mValue{0}
{
}


bool Event::setValue(const char *value)
{
	if (strncmp(value, mValue, EVENT_VALUE_LEN) != 0 || !mHasValue)
	{
		mChanged = true;
		strncpy(mValue, value, EVENT_VALUE_LEN);
		mValue[EVENT_VALUE_LEN - 1] = '\0';
		mHasValue = true;
	}

	return mChanged;
}


char *Event::toString(char *buffer, int maxLen)
{
	snprintf(buffer, maxLen, "|%s|", mName);
	appendText(buffer, mValue, maxLen);
	return buffer;
}


bool Event::unavailable()
{
	return setValue(sUnavailable);
}


IntEvent::IntEvent(const char *name) :
	DeviceDatum(name),
	mValue{0},
	mUnavailable(false)
{
}


bool IntEvent::setValue(int value)
{
	if (value !=  mValue || !mHasValue || mUnavailable)
	{
		mChanged = true;
		mValue = value;
		mHasValue = true;
		mUnavailable = false;
	}

	return mChanged;
}


char *IntEvent::toString(char *buffer, int maxLen)
{
	if (mUnavailable)
		snprintf(buffer, maxLen, "|%s|UNAVAILABLE", mName);
	else
		snprintf(buffer, maxLen, "|%s|%d", mName, mValue);

	return buffer;
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


Sample::Sample(const char *name, double epsilon) :
	DeviceDatum(name),
	mValue(0.0),
	mUnavailable(false),
	mEpsilon(epsilon)
{
}


bool Sample::setValue(double value)
{
	if (fabs(value - mValue) > mEpsilon ||
		!mHasValue ||
		mUnavailable )
	{
		mChanged = true;
		mValue = value;
		mHasValue = true;
		mUnavailable = false;
	}

	return mChanged;
}


char *Sample::toString(char *buffer, int maxLen)
{
	if (mUnavailable)
		snprintf(buffer, maxLen, "|%s|UNAVAILABLE", mName);
	else
		snprintf(buffer, maxLen, "|%s|%.10g", mName, mValue);

	return buffer;
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


bool PowerState::setValue(enum EPowerState state)
{
	if (mState != state || !mHasValue)
	{
		mState = state;
		mChanged = true;
		mHasValue = true;
	}

	return mChanged;
}


char *PowerState::toString(char *buffer, int maxLen)
{
	const char *text(nullptr);

	switch (mState)
	{
	case eUNAVAILABLE:
		text = sUnavailable;
		break;

	case eON:
		text = "ON";
		break;

	case eOFF:
		text = "OFF";
		break;

	default:
		text = "";
		break;
	}

	snprintf(buffer, maxLen, "|%s|%s", mName, text);
	return buffer;
}


bool PowerState::unavailable()
{
	return setValue(eUNAVAILABLE);
}


bool Execution::setValue(enum EExecutionState state)
{
	if (mState != state || !mHasValue)
	{
		mState = state;
		mChanged = true;
		mHasValue = true;
	}

	return mChanged;
}


char *Execution::toString(char *buffer, int maxLen)
{
	const char *text(nullptr);

	switch (mState)
	{
	case eUNAVAILABLE:
		text = sUnavailable;
		break;

	case eACTIVE:
		text = "ACTIVE";
		break;

	case eREADY:
		text = "READY";
		break;

	case eINTERRUPTED:
		text = "INTERRUPTED";
		break;

	case eSTOPPED:
		text = "STOPPED";
		break;

	case eFEED_HOLD:
		text = "FEED_HOLD";
		break;

	default:
		text = "";
		break;
	}

	snprintf(buffer, maxLen, "|%s|%s", mName, text);
	return buffer;
}


bool Execution::unavailable()
{
	return setValue(eUNAVAILABLE);
}


char *ControllerMode::toString(char *buffer, int maxLen)
{
	const char *text(nullptr);

	switch (mMode)
	{
	case eUNAVAILABLE:
		text = sUnavailable;
		break;

	case eSEMI_AUTOMATIC:
		text = "SEMI_AUTOMATIC";
		break;

	case eAUTOMATIC:
		text = "AUTOMATIC";
		break;

	case eMANUAL:
		text = "MANUAL";
		break;

	case eMANUAL_DATA_INPUT:
		text = "MANUAL_DATA_INPUT";
		break;

	default:
		text = "";
		break;
	}

	snprintf(buffer, maxLen, "|%s|%s", mName, text);
	return buffer;
}


bool ControllerMode::setValue(enum EMode mode)
{
	if (mMode != mode || !mHasValue)
	{
		mMode = mode;
		mChanged = true;
		mHasValue = true;
	}

	return mChanged;
}


bool ControllerMode::unavailable()
{
	return setValue(eUNAVAILABLE);
}


char *Direction::toString(char *buffer, int maxLen)
{
	const char *text(nullptr);

	switch (mDirection)
	{
	case eUNAVAILABLE:
		text = sUnavailable;
		break;

	case eCLOCKWISE:
		text = "CLOCKWISE";
		break;

	case eCOUNTER_CLOCKWISE:
		text = "COUNTER_CLOCKWISE";
		break;

	default:
		text = "";
		break;
	}

	snprintf(buffer, maxLen, "|%s|%s", mName, text);
	return buffer;
}


bool Direction::setValue(enum ERotationDirection direction)
{
	if (mDirection != direction || !mHasValue)
	{
		mDirection = direction;
		mChanged = true;
		mHasValue = true;
	}

	return mChanged;
}


bool Direction::unavailable()
{
	return setValue(eUNAVAILABLE);
}


char *EmergencyStop::toString(char *buffer, int maxLen)
{
	const char *text(nullptr);

	switch (mValue)
	{
	case eUNAVAILABLE:
		text = sUnavailable;
		break;

	case eTRIGGERED:
		text = "TRIGGERED";
		break;

	case eARMED:
		text = "ARMED";
		break;

	default:
		text = "";
		break;
	}

	snprintf(buffer, maxLen, "|%s|%s", mName, text);
	return buffer;
}


bool EmergencyStop::setValue(enum EValues value)
{
	if (mValue != value || !mHasValue)
	{
		mValue = value;
		mChanged = true;
		mHasValue = true;
	}

	return mChanged;
}


bool EmergencyStop::unavailable()
{
	return setValue(eUNAVAILABLE);
}


char *AxisCoupling::toString(char *buffer, int maxLen)
{
	const char *text(nullptr);

	switch (mValue)
	{
	case eUNAVAILABLE:
		text = sUnavailable;
		break;

	case eTANDEM:
		text = "TANDEM";
		break;

	case eSYNCHRONOUS:
		text = "SYNCHRONOUS";
		break;

	case eMASTER:
		text = "MASTER";
		break;

	case eSLAVE:
		text = "SLAVE";
		break;

	default:
		text = "";
		break;
	}

	snprintf(buffer, maxLen, "|%s|%s", mName, text);
	return buffer;
}


bool AxisCoupling::setValue(enum EValues value)
{
	if (mValue != value || !mHasValue)
	{
		mValue = value;
		mChanged = true;
		mHasValue = true;
	}

	return mChanged;
}


bool AxisCoupling::unavailable()
{
	return setValue(eUNAVAILABLE);
}


char *DoorState::toString(char *buffer, int maxLen)
{
	const char *text(nullptr);

	switch (mValue)
	{
	case eUNAVAILABLE:
		text = sUnavailable;
		break;

	case eOPEN:
		text = "CLOSED";
		break;

	case eCLOSED:
		text = "OPEN";
		break;

	default:
		text = "";
		break;
	}

	snprintf(buffer, maxLen, "|%s|%s", mName, text);
	return buffer;
}


bool DoorState::setValue(enum EValues value)
{
	if (mValue != value || !mHasValue)
	{
		mValue = value;
		mChanged = true;
		mHasValue = true;
	}

	return mChanged;
}


bool DoorState::unavailable()
{
	return setValue(eUNAVAILABLE);
}


char *PathMode::toString(char *buffer, int maxLen)
{
	const char *text(nullptr);

	switch (mValue)
	{
	case eUNAVAILABLE:
		text = sUnavailable;
		break;

	case eINDEPENDENT:
		text = "INDEPENDENT";
		break;

	case eSYNCHRONOUS:
		text = "SYNCHRONOUS";
		break;

	case eMIRROR:
		text = "MIRROR";
		break;

	default:
		text = "";
		break;
	}

	snprintf(buffer, maxLen, "|%s|%s", mName, text);
	return buffer;
}


bool PathMode::setValue(enum EValues value)
{
	if (mValue != value || !mHasValue)
	{
		mValue = value;
		mChanged = true;
		mHasValue = true;
	}

	return mChanged;
}


bool PathMode::unavailable()
{
	return setValue(eUNAVAILABLE);
}


char *RotaryMode::toString(char *buffer, int maxLen)
{
	const char *text(nullptr);

	switch (mValue)
	{
	case eUNAVAILABLE:
		text = sUnavailable;
		break;

	case eSPINDLE:
		text = "SPINDLE";
		break;

	case eINDEX:
		text = "INDEX";
		break;

	case eCONTOUR:
		text = "CONTOUR";
		break;

	default:
		text = "";
		break;
	}

	snprintf(buffer, maxLen, "|%s|%s", mName, text);
	return buffer;
}


bool RotaryMode::setValue(enum EValues value)
{
	if (mValue != value || !mHasValue)
	{
		mValue = value;
		mChanged = true;
		mHasValue = true;
	}

	return mChanged;
}


bool RotaryMode::unavailable()
{
	return setValue(eUNAVAILABLE);
}



Message::Message(const char *name) :
	DeviceDatum(name),
	mNativeCode{0}
{
}


char *Message::toString(char *buffer, int maxLen)
{
	snprintf(buffer, maxLen, "|%s|%s|", mName, mNativeCode);
	appendText(buffer, mText, maxLen);
	return buffer;
}


bool Message::setValue(const char *text, const char *code)
{
	if (!mHasValue ||
		strncmp(code, mNativeCode, EVENT_VALUE_LEN) ||
		strncmp(text, mText, EVENT_VALUE_LEN) )

	{
		strncpy(mNativeCode, code, EVENT_VALUE_LEN);
		mNativeCode[EVENT_VALUE_LEN - 1] = '\0';

		strncpy(mText, text, EVENT_VALUE_LEN);
		mText[EVENT_VALUE_LEN - 1] = '\0';

		mChanged = true;
		mHasValue = true;
	}

	return mChanged;
}


bool Message::unavailable()
{
	return setValue(sUnavailable);
}


PathPosition::PathPosition(const char *name, double epsilon)
	: DeviceDatum(name),
	mX(0.0),
	mY(0.0),
	mZ(0.0),
	mUnavailable(false),
	mEpsilon(epsilon)
{
}


bool PathPosition::setValue(double x, double y, double z)
{
	if (!mHasValue ||
		fabs(x- mX) > mEpsilon ||
		fabs(y - mY) > mEpsilon ||
		fabs(z - mZ) > mEpsilon ||
		mUnavailable)
	{
		mChanged = true;
		mX = x;
		mY = y;
		mZ = z;
		mHasValue = true;
		mUnavailable = false;
	}

	return mChanged;
}


char *PathPosition::toString(char *buffer, int maxLen)
{
	if (mUnavailable)
		snprintf(buffer, maxLen, "|%s|UNAVAILABLE", mName);
	else
		snprintf(buffer, maxLen, "|%s|%.10f %0.10f %0.10f", mName, mX, mY, mZ);

	return buffer;
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


Availability::Availability(const char *name) :
	DeviceDatum(name),
	mUnavailable(false)
{
	mHasValue = true;
}


char *Availability::toString(char *buffer, int maxLen)
{
	if (mUnavailable)
		snprintf(buffer, maxLen, "|%s|UNAVAILABLE", mName);
	else
		snprintf(buffer, maxLen, "|%s|AVAILABLE", mName);

	return buffer;
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
