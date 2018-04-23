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
#include "fake_adapter.hpp"

FakeAdapter::FakeAdapter(int aPort) :
	Adapter(aPort, 1000),
	mAvailability("avail"),
	mSystem("system"),
	mPos("pos"),
	mExecution("exec")
{
	addDatum(mAvailability);
	addDatum(mSystem);
	addDatum(mPos);
	addDatum(mExecution);
}


FakeAdapter::~FakeAdapter()
{
}


void FakeAdapter::initialize(int argc, const char *argv[])
{
	MTConnectService::initialize(argc, argv);

	if (argc > 1)
		mPort = atoi(argv[1]);
}


void FakeAdapter::start()
{
	startServer();
}


void FakeAdapter::stop()
{
	stopServer();
}


static int count = 0;
void FakeAdapter::gatherDeviceData()
{
	mAvailability.available();

	printf("Count: %d\n", count);
	mPos.setValue(count);

	switch (count % 6)
	{
	case 0:
		mExecution.setValue(Execution::eREADY);
		printf("Add 0\n");
		mSystem.add(Condition::eFAULT, "Fault 1", "0");
		break;

	case 1:
		mExecution.setValue(Execution::eACTIVE);
		printf("Add 0, 1\n");
		mSystem.add(Condition::eFAULT, "Fault 1", "0");
		mSystem.add(Condition::eFAULT, "Fault 2", "1");
		break;

	case 2:
		printf("Add 0, 1, 2\n");
		mSystem.add(Condition::eFAULT, "Fault 1", "0");
		mSystem.add(Condition::eFAULT, "Fault 2", "1");
		mSystem.add(Condition::eFAULT, "Fault 3", "2");
		break;

	case 3:
		printf("Add 1, 3\n");
		mSystem.add(Condition::eFAULT, "Fault 2", "1");
		mSystem.add(Condition::eFAULT, "Fault 4", "3");
		break;

	case 4:
		mExecution.setValue(Execution::eSTOPPED);
		printf("Stay the same\n");
		mSystem.add(Condition::eFAULT, "Fault 2", "1");
		mSystem.add(Condition::eFAULT, "Fault 4", "3");
		break;


	case 5:
		// Clear all
		printf("Clear\n");
		mAvailability.unavailable();
		break;
	}

	count++;
}

