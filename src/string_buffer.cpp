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
#include "string_buffer.hpp"

StringBuffer::StringBuffer(const char *aString)
{
  mLength = mSize = 0;
  mTimestamp[0] = 0;
  if (aString != 0)
  {
    append(aString);
  }
  else
  {
    mBuffer = 0;
  }
}

StringBuffer::~StringBuffer()
{
  if (mBuffer != 0)
    free(mBuffer);
}

const char *StringBuffer::append(const char* aString)
{
  /* Include additional length for timestamp */
  size_t len = strlen(aString);
  size_t totalLength = mLength + len;
  size_t tsLen = strlen(mTimestamp);
  if (mLength == 0)
    totalLength += tsLen;
  if (totalLength >= mSize)
  {
    size_t newLen = ((totalLength / 1024) + 1) * 1024;
    char *newBuffer = (char*) malloc(newLen);
    memcpy(newBuffer, mBuffer, mLength);
    
    free(mBuffer);
    mBuffer = newBuffer;
    mSize = newLen;
  }
  
  if (mLength == 0 && tsLen > 0)
  {
    strcpy(mBuffer, mTimestamp);
    mLength += tsLen;
  }

  strcpy(mBuffer + mLength, aString);
  mLength += len;
  
  return mBuffer;
}

void StringBuffer::newline()
{
  append("\n");
  append(mTimestamp);
}

void StringBuffer::reset()
{
  if (mBuffer != 0)
  {
    mBuffer[0] = 0;
    mLength = 0;
  }
}

void StringBuffer::timestamp()
{
#ifdef WIN32
  SYSTEMTIME st;
  GetSystemTime(&st);
  sprintf(mTimestamp, "%4d-%02d-%02dT%02d:%02d:%02d.%03dZ", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
#else
  struct timeval tv;
  struct timezone tz;
  
  gettimeofday(&tv, &tz);
  
  strftime(mTimestamp, 64, "%Y-%m-%dT%H:%M:%S", gmtime(&tv.tv_sec));
  sprintf(mTimestamp + strlen(mTimestamp), ".%06ldZ", tv.tv_usec);
#endif
}



