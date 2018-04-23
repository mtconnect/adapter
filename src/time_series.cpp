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
#include "time_series.hpp"
#include "string_buffer.hpp"

using namespace std;


TimeSeries::TimeSeries(const char *name, float epsilon, float rate) :
	DeviceDatum(name),
	mUnavailable(false),
	mEpsilon(epsilon),
	mRate(rate)
{
}


void TimeSeries::addValue(float value)
{
	mValues.push_back(value);

	mChanged = true;
	mHasValue = true;
	mUnavailable = false;
}


bool TimeSeries::setValue(std::vector<float> values)
{
	mValues = values;

	mChanged = true;
	mHasValue = true;
	mUnavailable = false;

	return true;
}


bool TimeSeries::append(StringBuffer &stringBuffer)
{
	char buffer[1024] = {0};
	if (mUnavailable)
	{
		snprintf(buffer, 1023u, "|%s|0||UNAVAILABLE", mName);
		stringBuffer.append(buffer);
	}
	else
	{
		if (mRate > 0)
			snprintf(buffer, 1023u, "|%s|%d|%g|", mName, (int) mValues.size(), mRate);
		else
			snprintf(buffer, 1023u, "|%s|%d||", mName, (int) mValues.size());
		stringBuffer.append(buffer);
		for (auto i = 0u; i < mValues.size(); i++)
		{
			snprintf(buffer, 1023, "%.10g ", mValues[i]);
			stringBuffer.append(buffer);
		}
	}

	mChanged = false;
	return true;
}


char *TimeSeries::toString(char *buffer, int maxLen)
{
	return buffer;
}


bool TimeSeries::unavailable()
{
	if (!mUnavailable)
	{
		mChanged = true;
		mUnavailable = true;
		mHasValue = true;
	}

	return mChanged;
}


bool TimeSeries::requiresFlush() const
{
	return true;
}
