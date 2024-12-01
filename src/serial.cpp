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
#include "serial.hpp"
#include "logger.hpp"

#include <stdio.h>


Serial::SerialError::SerialError(const char *aMessage)
{
	strncpy(mMessage, aMessage, 1023);
	mMessage[1023] = '\0';
}


Serial::Serial(
	const char *aDevice,
	int aBaud,
	const char *aParity,
	int aDataBit,
	int aStopBit)
{
	mBaud = aBaud;
	strncpy(mDevice, aDevice, sizeof(mDevice) - 1);
	mParity[sizeof(mDevice) - 1] = '\0';
	strncpy(mParity, aParity, sizeof(mParity) - 1);
	mParity[sizeof(mParity) - 1] = '\0';
	mDataBit = aDataBit;
	mStopBit = aStopBit;
	mFlow = eNONE;

#ifdef WIN32
	mFd = INVALID_HANDLE_VALUE;
#else
	mFd = -1;
#endif

	mConnected = false;
}


Serial::~Serial()
{
	disconnect();
}


int Serial::readUntil(const char *aUntil, char *aBuffer, int aLength)
{
	if (!mConnected)
	{
		gLogger->error("Trying to read when not connected");
		return -1;
	}

	gLogger->debug("Reading upto %d bytes or '%c' we get a match", aLength, aUntil);

	int len = 0, count = 0;
	char *cp = aBuffer;
	const char *match = aUntil;

	do
	{
		int ret = read(cp, 1);

		if (ret == -1)
			throw SerialError("Couldn't read");

		if (ret == 0)
		{
			usleep(10 * 1000); // 10 msec

			if (count++ > 10)
			{
				gLogger->info("Read timed out\n");
				return -1;
			}
		}
		else
		{
			count = 0;

			//printf("Received: %d == match: %d\n", *cp, *match);
			if (*match == *cp)
				match++;
			else
				match = aUntil;

			// See if we can match the beginnig of the string again.
			if (*match == *cp)
				match++;

			//printf("Match now: %d\n", *match);
			cp++;
			len++;
		}
	}
	while (len <= aLength && *match != '\0');

	*cp = '\0';

	gLogger->debug("Read returned: %d - '%s'", len, aBuffer);

	return len;
}


int Serial::write(const char *aBuffer)
{
	gLogger->debug("Writing '%s'\n", aBuffer);

	int ret = write(aBuffer, (int) strlen(aBuffer));

	if (ret < 0)
		throw SerialError("Couldn't write");

	gLogger->debug("Write returned: %d\n", ret);

	return ret;
}


bool Serial::flushInput()
{
	char buffer[2];
	int ret;

	do
	{
		ret = read(buffer, 1);

		if (ret < 0)
			throw SerialError("Couldn't read");
	}
	while (ret > 0);

#ifdef WIN32
	DWORD errors;
	COMSTAT stat;
	ClearCommError(mFd, &errors, &stat);
#else
	tcflush(mFd, TCIFLUSH);
#endif

	return true;
}


int Serial::readFully(char *aBuffer, int len, uint32_t timeout)
{
	int consumed = 0;
	uint64_t start = getTimestamp();

	while (consumed < len && (getTimestamp() - start) < timeout)
	{
		int res = wait(100);

		if (res > 0)
		{
			int cnt = read(aBuffer + consumed, len - consumed);

			if (cnt < 0)
			{
				gLogger->debug("Read returned: %d\n", res);
				return -1;
			}

			consumed += cnt;
		}
		else if (res < 0)
		{
			gLogger->debug("Wait returned: %d\n", res);
			return res;
		}
	}

	return consumed;
}


int Serial::writeFully(const char *aBuffer, int len, uint32_t timeout)
{
	int written = 0;

	uint64_t start = getTimestamp();

	while (written < len && (getTimestamp() - start) < timeout)
	{
		int res = wait(100, Serial::WRITE);

		if (res > 0)
		{
			int cnt = write(aBuffer + written, len - written);

			if (cnt > 0)
				written += cnt;
			else
			{
				gLogger->debug("Write returned: %d\n", res);
				return -1;
			}
		}
		else if (res < 0)
		{
			gLogger->debug("Wait returned: %d\n", res);
			return res;
		}
	}

	return written;
}


#ifdef WIN32
	#include "serial.win32.inc"
#else
	#include "serial.unix.inc"
#endif

