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
#pragma once

// Forward declarations
class StringBuffer;

// Some constants for field lengths
const int NAME_LEN = 32;
const int CODE_LEN = 32;
const int UNITS_LEN = 32;
const int NATIVE_CODE_LEN = 32;
const int SEVERITY_LEN = 32;
const int STATE_LEN = 32;
const int DESCRIPTION_LEN = 512;
const int EVENT_VALUE_LEN = 512;

//
// An abstract data value that knows its name and tracks when it has changed.
//
// The data value will be set in the subclasses.
//
class DeviceDatum
{
protected:
	// The name of the Data Value
	char mName[NAME_LEN];
	char mOrigName[NAME_LEN];
	char mNativeUnits[UNITS_LEN];

	// A changed flag to indicated that the value has changed since last append.
	bool mChanged;

	// Has this data value been initialized?
	bool mHasValue;

protected:
	void appendText(char *buffer, char *value, int maxLen);

public:
	// The name will be supplied later...
	DeviceDatum(const char *name = "");

	virtual ~DeviceDatum();

	void reset() {
		mChanged = false; }

	const char *getNativeUnits() const {
		return mNativeUnits; }
	void setNativeUnits(const char *nativeUnits);

	bool prefixName(const char *prefix);
	bool hasValue() const {
		return mHasValue; }
	const char *getName() const {
		return mName; }
	void setName(const char *name = "");

// Pure virtual and overridable methods
public:
	virtual char *toString(char *buffer, int maxLen) = 0;
	virtual bool unavailable() = 0;

	virtual bool changed() const {
		return mChanged; }
	virtual bool append(StringBuffer &buffer);
	virtual bool hasInitialValue() const ;
	virtual bool requiresFlush() const;

	virtual void begin() { }
	virtual void prepare() { }
	virtual void cleanup() { }
	virtual void initialize() { }
};


//
// An event is a data value with a string value.
//
class Event : public DeviceDatum
{
protected:
	char mValue[EVENT_VALUE_LEN];

public:
	Event(const char *name = "");

	bool setValue(const char *value);
	const char *getValue() const {
		return mValue; }

// Overrides from DeviceDatum
public:
	char *toString(char *buffer, int maxLen) override;
	bool unavailable() override;
};


//
// An int event is an event with an integer value. This can be used
// for line number events.
//
class IntEvent : public DeviceDatum
{
protected:
	int mValue;
	bool mUnavailable;

public:
	IntEvent(const char *name = "");

	bool setValue(int value);
	int getValue() const {
		return mValue; }

// Overrides from DeviceDatum
public:
	char *toString(char *buffer, int maxLen) override;
	bool unavailable() override;
};


//
// A sample event is used for floating point samples.
//
class Sample : public DeviceDatum
{
protected:
	double mValue;
	bool mUnavailable;
	double mEpsilon;

public:
	Sample(const char *name = "", double aEpsilon = 0.000001);

	bool setValue(double value);
	double getValue() const {
		return mValue; }

// Overrides from DeviceDatum
public:
	char *toString(char *buffer, int maxLen) override;
	bool unavailable() override;
};


class PowerState : public DeviceDatum
{
public:
	enum EPowerState
	{
		eUNAVAILABLE,
		eON,
		eOFF,
	};

protected:
	EPowerState mState;

public:
	PowerState(const char *name = "") : DeviceDatum(name) { }
	bool setValue(enum EPowerState state);
	EPowerState getValue() const {
		return mState; }

// Overrides from DeviceDatum
public:
	char *toString(char *buffer, int maxLen) override;
	bool unavailable() override;
};


class Execution : public DeviceDatum
{
public:
	enum EExecutionState
	{
		eUNAVAILABLE,
		eREADY,
		eINTERRUPTED,
		eSTOPPED,
		eACTIVE,
		eFEED_HOLD
	};

protected:
	EExecutionState mState;

public:
	Execution(const char *name = "") :
		DeviceDatum(name)
	{
	}

	bool setValue(enum EExecutionState state);
	EExecutionState getValue() const {
		return mState; }

// Overrides from DeviceDatum
public:
	char *toString(char *buffer, int maxLen) override;
	bool unavailable() override;
};


class ControllerMode : public DeviceDatum
{
public:
	enum EMode
	{
		eUNAVAILABLE,
		eAUTOMATIC,
		eMANUAL,
		eMANUAL_DATA_INPUT,
		eSEMI_AUTOMATIC
	};

protected:
	EMode mMode;

public:
	ControllerMode(const char *name = "") :
		DeviceDatum(name)
	{
	}

	bool setValue(enum EMode state);
	EMode getValue() const {
		return mMode; }

// Overrides from DeviceDatum
public:
	char *toString(char *buffer, int maxLen) override;
	bool unavailable() override;
};


class Direction : public DeviceDatum
{
public:
	enum ERotationDirection
	{
		eUNAVAILABLE,
		eCLOCKWISE,
		eCOUNTER_CLOCKWISE
	};

protected:
	ERotationDirection mDirection;

public:
	Direction(const char *name = "") :
		DeviceDatum(name)
	{
	}

	bool setValue(enum ERotationDirection direction);
	ERotationDirection getValue() const {
		return mDirection; }

// Overrides from DeviceDatum
public:
	char *toString(char *buffer, int maxLen) override;
	bool unavailable() override;
};


class EmergencyStop : public DeviceDatum
{
public:
	enum EValues
	{
		eUNAVAILABLE,
		eTRIGGERED,
		eARMED
	};

protected:
	EValues mValue;

public:
	EmergencyStop(const char *name = "") :
		DeviceDatum(name)
	{
	}

	bool setValue(enum EValues value);
	EValues getValue() const {
		return mValue; }

// Overrides from DeviceDatum
public:
	char *toString(char *buffer, int maxLen) override;
	bool unavailable() override;
};


class AxisCoupling : public DeviceDatum
{
public:
	enum EValues
	{
		eUNAVAILABLE,
		eTANDEM,
		eSYNCHRONOUS,
		eMASTER,
		eSLAVE
	};

protected:
	EValues mValue;

public:
	AxisCoupling(const char *name = "") :
		DeviceDatum(name)
	{
	}

	bool setValue(enum EValues value);
	EValues getValue() const {
		return mValue; }

// Overrides from DeviceDatum
public:
	char *toString(char *buffer, int maxLen) override;
	bool unavailable() override;
};


class DoorState : public DeviceDatum
{
public:
	enum EValues
	{
		eUNAVAILABLE,
		eOPEN,
		eCLOSED
	};

protected:
	EValues mValue;

public:
	DoorState(const char *name = "") :
		DeviceDatum(name)
	{
	}

	bool setValue(enum EValues value);
	EValues getValue() const {
		return mValue; }

// Overrides from DeviceDatum
public:
	char *toString(char *buffer, int maxLen) override;
	bool unavailable() override;
};


class PathMode : public DeviceDatum
{
public:
	enum EValues
	{
		eUNAVAILABLE,
		eINDEPENDENT,
		eSYNCHRONOUS,
		eMIRROR
	};

protected:
	EValues mValue;

public:
	PathMode(const char *name = "") :
		DeviceDatum(name)
	{
	}

	bool setValue(enum EValues value);
	EValues getValue() const {
		return mValue; }

// Overrides from DeviceDatum
public:
	char *toString(char *buffer, int maxLen) override;
	bool unavailable() override;
};


class RotaryMode : public DeviceDatum
{
public:
	enum EValues
	{
		eUNAVAILABLE,
		eSPINDLE,
		eINDEX,
		eCONTOUR
	};

protected:
	EValues mValue;

public:
	RotaryMode(const char *name = "") : DeviceDatum(name) { }
	bool setValue(enum EValues value);
	EValues getValue() const {
		return mValue; }

// Overrides from DeviceDatum
public:
	char *toString(char *buffer, int maxLen) override;
	bool unavailable() override;
};


class Message : public DeviceDatum
{
	char mText[EVENT_VALUE_LEN];
	char mNativeCode[EVENT_VALUE_LEN];

public:
	Message(const char *name = "");

	bool setValue(const char *text, const char *code = "");
	const char *getNativeCode() const {
		return mNativeCode; }

// Overrides from DeviceDatum
public:
	char *toString(char *buffer, int maxLen) override;
	bool unavailable() override;
	bool requiresFlush() const override {
		return true; }
};


class PathPosition : public DeviceDatum
{
protected:
	double mX, mY, mZ;
	bool mUnavailable;
	double mEpsilon;

public:
	PathPosition(const char *name = "", double aEpsilon = 0.000001);
	bool setValue(double x, double y, double z);
	double getX() const {
		return mX; }
	double getY() const {
		return mY; }
	double getZ() const {
		return mZ; }

// Overrides from DeviceDatum
public:
	char *toString(char *buffer, int maxLen) override;
	bool unavailable() override;
};


class Availability : public DeviceDatum
{
protected:
	bool mUnavailable;

public:
	Availability(const char *name = "");
	bool available();

// Overrides from DeviceDatum
public:
	char *toString(char *buffer, int maxLen) override;
	bool unavailable() override;
};

