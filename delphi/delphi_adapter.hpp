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

#ifndef DELPHI_ADAPTER_HPP
#define DELPHI_ADAPTER_HPP

#include "adapter.hpp"
#include "device_datum.hpp"

#define STDCALL __stdcall

#ifndef DELPHI_EXPORTS
#define EXTERN __declspec(dllimport) 
#else
#define EXTERN __declspec(dllexport)
#endif

class AbstractDatum { 
public:
  virtual DeviceDatum* STDCALL getDatum() = 0;
};

class AbstractAdapter {
public:
  virtual void STDCALL addDatum_(AbstractDatum *aValue) = 0;
  virtual void STDCALL startServer_() = 0;
  virtual bool STDCALL startServerThread_() = 0;
  virtual void STDCALL sendChangedData_() = 0;
  virtual void STDCALL flush_() = 0;
  virtual void STDCALL timestamp_() = 0;
  virtual void STDCALL unavailable_() = 0;
  virtual void STDCALL stopServer_() = 0;
  
  virtual void STDCALL free() = 0;
};

class DelphiAdapter : public AbstractAdapter, private Adapter
{
public:
  virtual void STDCALL addDatum_(AbstractDatum *aValue) { Adapter::addDatum(*aValue->getDatum()); }
  virtual void STDCALL startServer_() { Adapter::startServer(); }
  virtual bool STDCALL startServerThread_() { return Adapter::startServerThread(); }
  virtual void STDCALL sendChangedData_() { Adapter::sendChangedData(); }
  virtual void STDCALL flush_() { Adapter::flush(); }
  virtual void STDCALL timestamp_();
  virtual void STDCALL unavailable_() { Adapter::unavailable(); }
  virtual void STDCALL stopServer_() { Adapter::stopServer(); }
  virtual void STDCALL free() { delete this; }

  virtual void gatherDeviceData();  
  DelphiAdapter(int aPort);
  ~DelphiAdapter();
};

extern "C" EXTERN AbstractAdapter* __cdecl NewAdapter(int aPort);

// Define the class wrappers

#define DELPHI_WRAPPER(klass, type)                                   \
class Abstract ## klass : public AbstractDatum {               \
public:                                                    \
  virtual bool STDCALL setValue(type) = 0;   \
  virtual type STDCALL getValue() = 0;              \
  virtual void STDCALL free() = 0;                         \
};                                                         \
                                                           \
class Delphi ## klass : public Abstract ## klass, private klass {  \
public:                                                    \
  Delphi ## klass(const char *aName) : klass(aName) { }        \
  virtual bool STDCALL setValue(type aValue) { return klass::setValue(aValue); } \
  virtual type STDCALL getValue() { return klass::getValue(); }           \
  virtual void STDCALL free() { delete this; }                                   \
  virtual DeviceDatum* STDCALL getDatum() { return (klass*) this; }              \
};                                                                               \
                                                                                 \
extern "C" EXTERN Abstract ## klass* __cdecl New ## klass(const char *aName)

#define DELPHI_INST(klass) \
Abstract ## klass * __cdecl New ## klass(const char *aName) \
{                                                   \
  return new Delphi ## klass(aName);                    \
}

// Create the wrapper classes

DELPHI_WRAPPER(Event, const char *);
DELPHI_WRAPPER(Sample, double);
DELPHI_WRAPPER(IntEvent, int);
DELPHI_WRAPPER(Execution, Execution::EExecutionState);
DELPHI_WRAPPER(ControllerMode, ControllerMode::EMode);
DELPHI_WRAPPER(Direction, Direction::ERotationDirection);
DELPHI_WRAPPER(EmergencyStop, EmergencyStop::EValues);
DELPHI_WRAPPER(AxisCoupling, AxisCoupling::EValues);
DELPHI_WRAPPER(DoorState, DoorState::EValues);
DELPHI_WRAPPER(PathMode, PathMode::EValues);
DELPHI_WRAPPER(RotaryMode, RotaryMode::EValues);

class AbstractCondition : public AbstractDatum {
public:
  virtual bool STDCALL setValue(Condition::ELevels aLevel, const char *aText, const char *aCode,
		const char *aQualifier, const char *aSeverity) = 0;
  virtual void STDCALL free() = 0;
};

class DelphiCondition : public AbstractCondition, private Condition { 
public:
  DelphiCondition(const char *aName) : Condition(aName) { }
  virtual bool STDCALL setValue(Condition::ELevels aLevel, const char *aText, const char *aCode,
		const char *aQualifier, const char *aSeverity) { 
		  return Condition::setValue(aLevel, aText, aCode, aQualifier, aSeverity); 
	}
  virtual void STDCALL free() { delete this; }
  virtual DeviceDatum* STDCALL getDatum() { return (Condition*) this; }
};

extern "C" EXTERN AbstractCondition* __cdecl NewCondition(const char *aName);

class AbstractMessage : public AbstractDatum {
public:
  virtual bool STDCALL setValue(const char *aText, const char *aCode) = 0;
  virtual void STDCALL free() = 0;
};

class DelphiMessage : public AbstractMessage, private Message { 
public:
  DelphiMessage(const char *aName) : Message(aName) { }
  virtual bool STDCALL setValue(const char *aText, const char *aCode) { 
		  return Message::setValue(aText, aCode); 
	}
  virtual void STDCALL free() { delete this; }
  virtual DeviceDatum* STDCALL getDatum() { return (Message*) this; }
};

extern "C" EXTERN AbstractMessage* __cdecl NewMessage(const char *aName);

class AbstractAvailability : public AbstractDatum {
public:
  virtual bool STDCALL available() = 0;
  virtual bool STDCALL unavailable_() = 0;
  virtual void STDCALL free() = 0;
};

class DelphiAvailability : public AbstractAvailability, private Availability { 
public:
  DelphiAvailability(const char *aName) : Availability(aName) { }
  virtual bool STDCALL available() { return Availability::available(); }
  virtual bool STDCALL unavailable_() { return Availability::unavailable(); }
  virtual void STDCALL free() { delete this; }
  virtual DeviceDatum* STDCALL getDatum() { return (Availability*) this; }
};

extern "C" EXTERN AbstractAvailability* __cdecl NewAvailability(const char *aName);


#endif

