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
  
  /* A changed flag to indicated that the value has changed since last append. */
  bool mChanged;
  
  /* Has this data value been initialized? */
  bool mHasValue;

public:
  DeviceDatum(const char *aName);
  virtual ~DeviceDatum();
  
  bool changed() { return mChanged; }
  void reset() { mChanged = false; }
  
  char *getName() { return mName; }
  virtual char *toString(char *aBuffer, int aMaxLen) = 0;
  virtual bool append(StringBuffer &aBuffer);
  virtual bool hasInitialValue();
  virtual bool requiresFlush();
};

/*
 * An event is a data value with a string value.
 */
class Event : public DeviceDatum 
{
protected:
  char mValue[EVENT_VALUE_LEN];

public:
  Event(const char *aName);
  bool setValue(const char *aValue);
  const char *getValue() { return mValue; }
  virtual char *toString(char *aBuffer, int aMaxLen);
};

/*
 * An int event is an event with an integer value. This can be used
 * for line number events.
 */
class IntEvent : public DeviceDatum 
{
protected:
  int mValue;

public:
  IntEvent(const char *aName);
  bool setValue(int aValue);
  int getValue() { return mValue; }
  virtual char *toString(char *aBuffer, int aMaxLen);
};

/*
 * A sample event is used for floating point samples.
 */
class Sample : public DeviceDatum 
{
protected:
  double mValue;

public:
  Sample(const char *aName);
  bool setValue(double aValue);
  double getValue() { return mValue; }
  virtual char *toString(char *aBuffer, int aMaxLen);
};

/*
 * An alarm event.
 */
class Alarm : public DeviceDatum
{
public:
  enum ESeverity {
    eCRITICAL,
    eERROR,
    eWARNING,
    eINFO
  };
  
  enum EState {
    eINSTANT,
    eACTIVE,
    eCLEARED
  };
  
  enum ECode {
    eFAILURE,
    eFAULT,
    eCRASH,
    eJAM,
    eOVERLOAD,
    eESTOP,
    eMATERIAL,
    eMESSAGE,
    eOTHER
  };
  
protected:
  char mNativeCode[NATIVE_CODE_LEN];
  char mDescription[DESCRIPTION_LEN];
  
  ECode     mCode;
  EState    mState;
  ESeverity mSeverity;
  
protected:
  const char *getCodeText();
  const char *getSeverityText();
  const char *getStateText();

public:
  Alarm(const char *aName);
  bool setValue(enum ECode aCode, const char *aNativeCode, enum ESeverity aSeverity,
		            enum EState aState, const char *aDescription);
  ECode getCode() { return mCode; }
  EState getState() { return mState; }
  ESeverity getSeverity() { return mSeverity; }
  const char *getNativeCode() { return mNativeCode; }
  const char *getDescription() { return mDescription; }
  virtual char *toString(char *aBuffer, int aMaxLen);
  virtual bool hasInitialValue();
  virtual bool requiresFlush();
};

class StatefullAlarm : public Alarm
{
public:
  StatefullAlarm(const char *aName, enum ECode aCode, const char *aNativeCode,
		 enum ESeverity aSeverity, const char *aDescription);
  bool setState(enum EState aState);
  bool active() { return setState(Alarm::eACTIVE); }
  bool cleared() { return setState(Alarm::eCLEARED); }
  virtual bool hasInitialValue();
};

/* Power status data value */

class Power : public DeviceDatum 
{
public:
  enum EPowerStatus {
    eON,
    eOFF
  };
  
protected:
  EPowerStatus mStatus;
  
public:
  Power(const char *aName) : DeviceDatum(aName) { }
  bool setValue(enum EPowerStatus aStatus);
  EPowerStatus getValue() { return mStatus; }
  virtual char *toString(char *aBuffer, int aMaxLen);
};

/* Executaion state */

class Execution : public DeviceDatum 
{
public:
  enum EExecutionState {
    eREADY,
    eINTERRUPTED,
    eSTOPPED,
    eACTIVE
  };
  
protected:
  EExecutionState mState;
  
public:
  Execution(const char *aName) : DeviceDatum(aName) { }
  bool setValue(enum EExecutionState aState);
  EExecutionState getValue() { return mState; }
  virtual char *toString(char *aBuffer, int aMaxLen);
};

/* ControllerMode  */

class ControllerMode : public DeviceDatum
{
public:
  enum EMode {
    eAUTOMATIC,
    eMANUAL,
    eMANUAL_DATA_INPUT
  };
  
protected:
  EMode mMode;
  
public:
  ControllerMode(const char *aName) : DeviceDatum(aName) { }
  bool setValue(enum EMode aState);
  EMode getValue() { return mMode; }
  virtual char *toString(char *aBuffer, int aMaxLen);
};


/* ControllerMode  */

class Direction : public DeviceDatum
{
public:
  enum ERotationDirection {
    eCLOCKWISE,
    eCOUNTER_CLOCKWISE
  };
  
protected:
  ERotationDirection mDirection;
  
public:
  Direction(const char *aName) : DeviceDatum(aName) { }
  bool setValue(enum ERotationDirection aDirection);
  ERotationDirection getValue() { return mDirection; }
  virtual char *toString(char *aBuffer, int aMaxLen);
};

#endif
