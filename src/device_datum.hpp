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

#ifndef DEVICE_DATUM_HPP
#define DEVICE_DATUM_HPP

/* Forward class definitions */
class StringBuffer;

/* Some constants for field lengths */
const int NAME_LEN = 32;
const int CODE_LEN = 32;
const int UNITS_LEN = 32;
const int NATIVE_CODE_LEN = 32;
const int SEVERITY_LEN = 32;
const int STATE_LEN = 32;
const int DESCRIPTION_LEN = 512;
const int EVENT_VALUE_LEN = 512;

/*
 * An abstract data value that knows its name and tracks when it has changed. 
 * 
 * The data value will be set in the subclasses.
 */
class DeviceDatum {
protected:
  /* The name of the Data Value */
  char mName[NAME_LEN];
  char mOrigName[NAME_LEN];
  char mNativeUnits[UNITS_LEN];
  
  /* A changed flag to indicated that the value has changed since last append. */
  bool mChanged;
  
  /* Has this data value been initialized? */
  bool mHasValue;

protected:
  void appendText(char *aBuffer, char *aValue, int aMaxLen);

public:
  // The name will be supplied later...
  DeviceDatum(const char *aName = "");

  virtual ~DeviceDatum();
  
  virtual bool changed() { return mChanged; }
  void reset() { mChanged = false; }

  const char *getNativeUnits() { return mNativeUnits; }
  void setNativeUnits(const char *aNativeUnits);

  bool prefixName(const char *aPrefix);
  bool hasValue() const { return mHasValue; }
  char *getName() { return mName; }
  void setName(const char *aName = "");
  virtual char *toString(char *aBuffer, int aMaxLen) = 0;
  virtual bool append(StringBuffer &aBuffer);
  virtual bool hasInitialValue();
  virtual bool requiresFlush();

  virtual bool unavailable() = 0;
  virtual void begin() { }
  virtual void prepare() { }
  virtual void cleanup() { }
  virtual void initialize() { }
};

/*
 * An event is a data value with a string value.
 */
class Event : public DeviceDatum 
{
protected:
  char mValue[EVENT_VALUE_LEN];

public:
  Event(const char *aName = "");
  bool setValue(const char *aValue);
  const char *getValue() { return mValue; }
  virtual char *toString(char *aBuffer, int aMaxLen);

  virtual bool unavailable();
};

/*
 * An int event is an event with an integer value. This can be used
 * for line number events.
 */
class IntEvent : public DeviceDatum 
{
protected:
  int mValue;
  bool mUnavailable;

public:
  IntEvent(const char *aName = "");
  bool setValue(int aValue);
  int getValue() { return mValue; }
  virtual char *toString(char *aBuffer, int aMaxLen);
  
  virtual bool unavailable();
};

/*
 * A sample event is used for floating point samples.
 */

class Sample : public DeviceDatum 
{
protected:
  double mValue;
  bool mUnavailable;
  double mEpsilon;

public:
  Sample(const char *aName = "", double aEpsilon = 0.000001);
  bool setValue(double aValue);
  double getValue() { return mValue; }
  virtual char *toString(char *aBuffer, int aMaxLen);

  virtual bool unavailable();
};

/* Power status data value */

class PowerState : public DeviceDatum 
{
public:
  enum EPowerState {
    eUNAVAILABLE,
    eON,
    eOFF,
  };
  
protected:
  EPowerState mState;
  
public:
  PowerState(const char *aName = "") : DeviceDatum(aName) { }
  bool setValue(enum EPowerState aState);
  EPowerState getValue() { return mState; }
  virtual char *toString(char *aBuffer, int aMaxLen);
  
  virtual bool unavailable();
};

/* Executaion state */

class Execution : public DeviceDatum 
{
public:
  enum EExecutionState {
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
  Execution(const char *aName = "") : DeviceDatum(aName) { }
  bool setValue(enum EExecutionState aState);
  EExecutionState getValue() { return mState; }
  virtual char *toString(char *aBuffer, int aMaxLen);
  
  virtual bool unavailable();
};

/* ControllerMode  */

class ControllerMode : public DeviceDatum
{
public:
  enum EMode {
    eUNAVAILABLE,
    eAUTOMATIC,
    eMANUAL,
    eMANUAL_DATA_INPUT,
    eSEMI_AUTOMATIC
  };
  
protected:
  EMode mMode;
  
public:
  ControllerMode(const char *aName = "") : DeviceDatum(aName) { }
  bool setValue(enum EMode aState);
  EMode getValue() { return mMode; }
  virtual char *toString(char *aBuffer, int aMaxLen);

  virtual bool unavailable();
};


/* Direction  */

class Direction : public DeviceDatum
{
public:
  enum ERotationDirection {
    eUNAVAILABLE,
    eCLOCKWISE,
    eCOUNTER_CLOCKWISE
  };
  
protected:
  ERotationDirection mDirection;
  
public:
  Direction(const char *aName = "") : DeviceDatum(aName) { }
  bool setValue(enum ERotationDirection aDirection);
  ERotationDirection getValue() { return mDirection; }
  virtual char *toString(char *aBuffer, int aMaxLen);

  virtual bool unavailable();
};

// Version 1.1

/* Emergency Stop */

class EmergencyStop : public DeviceDatum
{
public:
  enum EValues {
    eUNAVAILABLE,
    eTRIGGERED,
    eARMED
  };
  
protected:
  EValues mValue;
  
public:
  EmergencyStop(const char *aName = "") : DeviceDatum(aName) { }
  bool setValue(enum EValues aValue);
  EValues getValue() { return mValue; }
  virtual char *toString(char *aBuffer, int aMaxLen);

  virtual bool unavailable();
};


class AxisCoupling : public DeviceDatum
{
public:
  enum EValues {
    eUNAVAILABLE,
    eTANDEM,
    eSYNCHRONOUS,
    eMASTER,
    eSLAVE
  };
  
protected:
  EValues mValue;
  
public:
  AxisCoupling(const char *aName = "") : DeviceDatum(aName) { }
  bool setValue(enum EValues aValue);
  EValues getValue() { return mValue; }
  virtual char *toString(char *aBuffer, int aMaxLen);

  virtual bool unavailable();
};

class DoorState : public DeviceDatum
{
public:
  enum EValues {
    eUNAVAILABLE,
    eOPEN,
    eCLOSED
  };
  
protected:
  EValues mValue;
  
public:
  DoorState(const char *aName = "") : DeviceDatum(aName) { }
  bool setValue(enum EValues aValue);
  EValues getValue() { return mValue; }
  virtual char *toString(char *aBuffer, int aMaxLen);

  virtual bool unavailable();
};

class PathMode : public DeviceDatum
{
public:
  enum EValues {
    eUNAVAILABLE,
    eINDEPENDENT,
    eSYNCHRONOUS,
    eMIRROR
  };
  
protected:
  EValues mValue;
  
public:
  PathMode(const char *aName = "") : DeviceDatum(aName) { }
  bool setValue(enum EValues aValue);
  EValues getValue() { return mValue; }
  virtual char *toString(char *aBuffer, int aMaxLen);

  virtual bool unavailable();
};

class RotaryMode : public DeviceDatum
{
public:
  enum EValues {
    eUNAVAILABLE,
    eSPINDLE,
    eINDEX,
    eCONTOUR
  };
  
protected:
  EValues mValue;
  
public:
  RotaryMode(const char *aName = "") : DeviceDatum(aName) { }
  bool setValue(enum EValues aValue);
  EValues getValue() { return mValue; }
  virtual char *toString(char *aBuffer, int aMaxLen);

  virtual bool unavailable();
};

class Message : public DeviceDatum {
  char mText[EVENT_VALUE_LEN];
  char mNativeCode[EVENT_VALUE_LEN];

public:
  Message(const char *aName = "");
  bool setValue(const char *aText, const char *aCode = ""); 
  virtual char *toString(char *aBuffer, int aMaxLen);
  const char *getNativeCode() { return mNativeCode; }
  
  virtual bool requiresFlush();  
  virtual bool unavailable();
};
  
class PathPosition : public DeviceDatum {
protected:
  double mX, mY, mZ;
  bool mUnavailable;
  double mEpsilon;

public:
  PathPosition(const char *aName = "", double aEpsilon = 0.000001);
  bool setValue(double aX, double aY, double aZ);
  double getX() { return mX; }
  double getY() { return mY; }
  double getZ() { return mZ; }
  virtual char *toString(char *aBuffer, int aMaxLen);

  virtual bool unavailable();  
};

class Availability : public DeviceDatum 
{
protected:
  bool mUnavailable;

public:
  Availability(const char *aName = "");
  virtual char *toString(char *aBuffer, int aMaxLen);
  bool available();
  virtual bool unavailable();  
};
  
#endif
