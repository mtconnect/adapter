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

#ifndef CONDITION_LIST
#define CONDITION_LIST

// The conditon items
#include "device_datum.hpp"


class Condition : public DeviceDatum 
{
public:
  enum ELevels {
    eUNAVAILABLE,
    eNORMAL,
    eWARNING,
    eFAULT
  };
  
protected:
  class ActiveCondition 
  {
  protected:
    ELevels mLevel;
    char mText[EVENT_VALUE_LEN];
    char mNativeCode[EVENT_VALUE_LEN];
    char mNativeSeverity[EVENT_VALUE_LEN];
    char mQualifier[EVENT_VALUE_LEN];
    
    bool mChanged;
    bool mMarked;
    bool mPlaceHolder;
    
  public:
    ActiveCondition() : mChanged(true), mMarked(true), mPlaceHolder(false) {
      mNativeCode[0] = mNativeSeverity[0] = mText[0] =
       mQualifier[0] = 0;
    }
    ActiveCondition(ELevels aLevel, const char *aText = "", const char *aCode = "",
		    const char *aQualifier = "", const char *aSeverity = "") 
      : mChanged(true), mMarked(true), mPlaceHolder(false)
    {
      setValue(aLevel, aText, aCode, aQualifier, aSeverity);
    }
    
    bool setValue(ELevels aLevel, const char *aText = "", const char *aCode = "",
		  const char *aQualifier = "", const char *aSeverity = "");
    
    char *toString(char *aBuffer, int aMaxLen);
    bool hasChanged() { return mChanged; }
    
    ELevels getLevel() { return mLevel; }
    const char *getText() { return mText; }
    const char *getNativeCode() { return mNativeCode; }
    const char *getNativeSeverity() { return mNativeSeverity; }
    const char *getQualifier() { return mQualifier; }

    void clear() { mMarked = false; }
    void mark() { mMarked = true; }
    bool isMarked() { return mMarked; }
    bool isPlaceHolder() { return mPlaceHolder; }
  };
  
protected:
  ActiveCondition **mActiveList;
  int mActiveSize;
  int mActiveCount;
  static const int mInitialActiveListSize = 16;

  bool mBegun;
  bool mPrepared;
  bool mSimple;

  void add(ActiveCondition *aCondition);
  bool removeAt(int i);

  void append(StringBuffer &aStringBuffer, char *aBuffer, ActiveCondition *aCond,
	      bool &aFirst, int aMaxLen);
  
public:
  Condition(const char *aName = "", bool aSimple = false);
  virtual ~Condition();
  virtual char *toString(char *aBuffer, int aMaxLen);

  virtual bool requiresFlush();
  virtual bool unavailable();
  virtual bool append(StringBuffer &aBuffer);

  bool add(ELevels aLevel, const char *aText = "", const char *aCode = "",
	   const char *aQualifier = "", const char *aSeverity = "");
  void remove(const char *aCode);   
   
  void removeAll();
  bool normal() { return add(eNORMAL); }
  bool isActive(const char *aNativeCode);
  void setSimple() { mSimple = true; }

  virtual void begin();
  virtual void prepare();
  virtual void cleanup();
  virtual void initialize();
};

#endif
