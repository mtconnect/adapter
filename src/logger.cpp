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
#include "logger.hpp"
#ifdef min
	#undef min
#endif
#ifdef max
	#undef max
#endif
#include <date/date.h>
#include <chrono>

Logger *gLogger = nullptr;


void Logger::error(const char *inputformat, ...)
{
	char buffer[LOGGER_BUFFER_SIZE];
	va_list args;
	va_start(args, inputformat);
	fprintf(mFile, "%s - Error: %s\n",
		timestamp().c_str(),
		format(buffer, LOGGER_BUFFER_SIZE, inputformat, args));
	fflush(mFile);
	va_end(args);
}


void Logger::warning(const char *inputformat, ...)
{
	if (mLogLevel > eWARNING)
		return;

	char buffer[LOGGER_BUFFER_SIZE];
	va_list args;
	va_start(args, inputformat);
	fprintf(mFile, "%s - Warning: %s\n",
		timestamp().c_str(),
		format(buffer, LOGGER_BUFFER_SIZE, inputformat, args));
	fflush(mFile);
	va_end(args);
}


void Logger::info(const char *inputformat, ...)
{
	if (mLogLevel > eINFO)
		return;

	char buffer[LOGGER_BUFFER_SIZE];
	va_list args;
	va_start(args, inputformat);
	fprintf(mFile, "%s - Info: %s\n",
		timestamp().c_str(),
		format(buffer, LOGGER_BUFFER_SIZE, inputformat, args));
	fflush(mFile);
	va_end(args);
}


void Logger::debug(const char *inputformat, ...)
{
	if (mLogLevel > eDEBUG)
		return;

	char buffer[LOGGER_BUFFER_SIZE];
	va_list args;
	va_start(args, inputformat);
	fprintf(mFile, "%s - Debug: %s\n",
		timestamp().c_str(),
		format(buffer, LOGGER_BUFFER_SIZE, inputformat, args));
	fflush(mFile);
	va_end(args);
}


const char *Logger::format(char *buffer, int aLen, const char *inputformat, va_list args)
{
	vsprintf(buffer, inputformat, args);
	buffer[aLen - 1] = '\0';
	return buffer;
}


std::string Logger::timestamp()
{
	return date::format("%FT%TZ",
				std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()));
}