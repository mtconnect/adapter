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

#include "condition_test.hpp"
#include "string_buffer.hpp"
#include <string>

using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(ConditionTest);

void ConditionTest::setUp()
{
}

void ConditionTest::tearDown()
{
}

void ConditionTest::testInitial()
{
	Condition cond("Test");
	StringBuffer buffer;

	CPPUNIT_ASSERT_EQUAL(string("Test"), string(cond.getName()));

	CPPUNIT_ASSERT(cond.changed());
	CPPUNIT_ASSERT(cond.hasInitialValue());
	CPPUNIT_ASSERT(cond.requiresFlush());

	cond.append(buffer);
  CPPUNIT_ASSERT_EQUAL(string("|Test|UNAVAILABLE||||"), string((const char*) buffer));
}

void ConditionTest::testInitialNormal()
{
	Condition cond("Test");
	StringBuffer buffer;

	CPPUNIT_ASSERT(cond.normal());

	cond.append(buffer);
  CPPUNIT_ASSERT_EQUAL(string("|Test|NORMAL||||"), string((const char*) buffer));
}

void ConditionTest::testAdd()
{
	Condition cond("Test");
	StringBuffer buffer;

	cond.begin();
	CPPUNIT_ASSERT(cond.add(Condition::eFAULT, "A Fault", "123", "HIGH", "1"));
	CPPUNIT_ASSERT(cond.append(buffer));
  CPPUNIT_ASSERT_EQUAL(string("|Test|FAULT|123|1|HIGH|A Fault"), string((const char*) buffer));
	cond.cleanup();
}

void ConditionTest::testActive()
{
	Condition cond("Test");
	StringBuffer buffer;

	cond.begin();
	CPPUNIT_ASSERT(cond.add(Condition::eFAULT, "A Fault", "123", "HIGH", "1"));
	CPPUNIT_ASSERT(cond.append(buffer));
  CPPUNIT_ASSERT_EQUAL(string("|Test|FAULT|123|1|HIGH|A Fault"), string((const char*) buffer));

	CPPUNIT_ASSERT(cond.isActive("123"));
	CPPUNIT_ASSERT(!cond.isActive("1234"));
	cond.cleanup();
}

void ConditionTest::testCleanup()
{
	Condition cond("Test");
	StringBuffer buffer;

	cond.begin();
	CPPUNIT_ASSERT(cond.add(Condition::eFAULT, "A Fault", "123", "HIGH", "1"));
	CPPUNIT_ASSERT(cond.append(buffer));
  CPPUNIT_ASSERT_EQUAL(string("|Test|FAULT|123|1|HIGH|A Fault"), string((const char*) buffer));
	cond.cleanup();

	buffer.reset();

	cond.begin();
	cond.append(buffer);
  CPPUNIT_ASSERT_EQUAL(string("|Test|NORMAL||||"), string((const char*) buffer));
	cond.cleanup();

	buffer.reset();

	cond.begin();
	CPPUNIT_ASSERT(cond.add(Condition::eFAULT, "A Fault", "123", "HIGH", "1"));
	CPPUNIT_ASSERT(cond.add(Condition::eFAULT, "Another Fault", "124", "HIGH", "1"));
	CPPUNIT_ASSERT(cond.append(buffer));
	CPPUNIT_ASSERT_EQUAL(string("|Test|FAULT|124|1|HIGH|Another Fault\n"
				"|Test|FAULT|123|1|HIGH|A Fault"),
		       string((const char*) buffer));

	CPPUNIT_ASSERT(cond.isActive("123"));
	CPPUNIT_ASSERT(cond.isActive("124"));
	cond.cleanup();

	buffer.reset();

	cond.begin();
	CPPUNIT_ASSERT(!cond.add(Condition::eFAULT, "A Fault", "123", "HIGH", "1"));
  
	cond.append(buffer);
  CPPUNIT_ASSERT_EQUAL(string("|Test|NORMAL|124|||"), string((const char*) buffer));
	cond.cleanup();

	CPPUNIT_ASSERT(cond.isActive("123"));
	CPPUNIT_ASSERT(!cond.isActive("124"));
}

