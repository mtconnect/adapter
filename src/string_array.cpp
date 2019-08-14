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
#include "string_array.hpp"

#define BLOCK_SIZE 256


StringArray::StringArray()
{
	mLength = 0;
	mSize = BLOCK_SIZE;
	mArray = (char **) malloc(sizeof(char *) * mSize);
	mArray[mLength = 0];
}


StringArray::~StringArray()
{
	clear();
	free(mArray);
	mArray = nullptr;
}


void StringArray::clear()
{
	for (int i = 0; i < mLength; i++)
		free(mArray[i]);

	mLength = 0;
}


void StringArray::append(const char *string)
{
	char *dup = strdup(string);

	if (!dup)
	{
		perror("StringArray::append");
		exit(2);
	}

	if (mLength >= mSize - 1)
	{
		mSize += BLOCK_SIZE;
		mArray = (char **) realloc(mArray, mSize * sizeof(char *));

		if (!mArray)
		{
			perror("StringArray::append");
			exit(2);
		}
	}

	mArray[mLength++] = dup;
}


int StringArray::readFile(const char *fileName)
{
	// First clear out existing contents
	clear();

	// Parse file.
	auto file = fopen(fileName, "r");

	if (!file)
		printf("Could not open file: %s\n", fileName);
	else
	{
		printf("Parsing file: %s\n", fileName);
		char buffer[1024] = {0};

		while (fgets(buffer, 1024, file) != 0)
		{
			int last = strlen(buffer) - 1;

			if (last > 0 && buffer[last] == '\n')
				buffer[last] = 0;

			append(buffer);
		}
	}

	return mLength;
}

