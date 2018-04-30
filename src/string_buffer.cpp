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
#include "string_buffer.hpp"
#ifdef min
	#undef min
#endif
#ifdef max
	#undef max
#endif
#include <date/date.h>

StringBuffer::StringBuffer(const char *string) :
	mBuffer(nullptr),
	mSize(0u),
	mLength(0u)
{
	if (string)
		append(string);
}


StringBuffer::~StringBuffer()
{
	if (mBuffer)
	{
		free(mBuffer);
		mBuffer = nullptr;
	}
}


const char *StringBuffer::append(const char *string)
{
	// Include additional length for timestamp
	auto len = strlen(string);
	auto totalLength = mLength + len;
	auto tsLen = mTimestamp.size();

	if (!mLength)
		totalLength += tsLen;

	if (totalLength >= mSize)
	{
		auto newLen = ((totalLength / 1024u) + 1u) * 1024u;
		auto newBuffer = (char *) malloc(newLen);
		memcpy(newBuffer, mBuffer, mLength);

		free(mBuffer);
		mBuffer = newBuffer;
		mSize = newLen;
	}

	if (!mLength && tsLen > 0)
	{
		strcpy(mBuffer, mTimestamp.c_str());
		mLength += tsLen;
	}

	strcpy(mBuffer + mLength, string);
	mLength += len;

	return mBuffer;
}


void StringBuffer::newline()
{
	append("\n");
	append(mTimestamp.c_str());
}


void StringBuffer::reset()
{
	if (mBuffer)
	{
		mBuffer[0] = 0;
		mLength = 0u;
	}
}


void StringBuffer::timestamp()
{
	mTimestamp = date::format("%FT%TZ",
				std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()));
}
