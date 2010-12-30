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

class Condition;

class ConditionList 
{
protected:
  struct Node 
  {
    Condition *mCondition;
    Node *mNext;
    Node *mPrevious;
  };

  Node *mHead;
    
public:
  class Iterator 
  {
  protected:
    Node *mCurrent;
    ConditionList *mList;
  public:
    Iterator(const Iterator &aIterator) {
      mList = aIterator.mList;
      mCurrent = aIterator.mCurrent;
    }
    
    Iterator(ConditionList *aObj) {
      mList = aObj;
      mCurrent = aObj->mHead;
    }
    Iterator(ConditionList &aObj) {
      mList = &aObj;
      mCurrent = aObj.mHead;
    }
    Condition *operator++() {
      if (mCurrent == 0)
	return 0;

      mCurrent = mCurrent->mNext;
      if (mCurrent != 0)
	return mCurrent->mCondition;
      else
	return 0;
    }
    Condition *operator++(int) {
      if (mCurrent == 0)
	return 0;
      
      Condition *obj = mCurrent->mCondition;
      mCurrent = mCurrent->mNext;
      return obj;
    }
    operator Condition *() {
      if (mCurrent == 0)
	return 0;
      else
	return mCurrent->mCondition;
    }
    bool end() { return mCurrent == 0; }
    
    bool remove() {
      if (mCurrent == 0)
	return false;

      if (mCurrent->mPrevious)
	mCurrent->mPrevious->mNext = mCurrent->mNext;
      else
	mList->mHead = mCurrent->mNext;
      if (mCurrent->mNext)
	mCurrent->mNext->mPrevious = mCurrent->mPrevious;

      Node *old = mCurrent;
      mCurrent = mCurrent->mNext;
      delete old;

      return true;
    }
  };
    
  
  ConditionList() : mHead(0) {}
  ~ConditionList();
  void add(Condition *aCondition);
  Iterator begin() { return Iterator(this); }
};

#endif
