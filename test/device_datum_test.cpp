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

#include "device_datum_test.hpp"
#include <string>

using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(DeviceDatumTest);

void DeviceDatumTest::setUp()
{
}

void DeviceDatumTest::tearDown()
{
}

void DeviceDatumTest::testEvent()
{
  Event event("Test");
  char buffer[256];

  CPPUNIT_ASSERT_EQUAL(string("Test"), string(event.getName()));

  CPPUNIT_ASSERT(!event.changed());
  CPPUNIT_ASSERT(!event.hasInitialValue());
  CPPUNIT_ASSERT_EQUAL(string(""), string(event.getValue()));

  event.unavailable();
  CPPUNIT_ASSERT(event.changed());
  CPPUNIT_ASSERT(event.hasInitialValue());
  
  CPPUNIT_ASSERT(event.toString(buffer, 255) != 0);
  CPPUNIT_ASSERT_EQUAL(string("|Test|UNAVAILABLE"), string(buffer));

  event.reset();
  CPPUNIT_ASSERT(!event.changed());
  CPPUNIT_ASSERT(event.hasInitialValue());
  
  CPPUNIT_ASSERT(event.setValue("Hello"));
  CPPUNIT_ASSERT(event.changed());
  CPPUNIT_ASSERT(event.toString(buffer, 255) != 0);
  CPPUNIT_ASSERT_EQUAL(string("|Test|Hello"), string(buffer));
  CPPUNIT_ASSERT_EQUAL(string("Hello"), string(event.getValue()));

  event.reset();
  CPPUNIT_ASSERT(!event.changed());

  CPPUNIT_ASSERT(!event.setValue("Hello"));
}

void DeviceDatumTest::testSample()
{
  Sample sample("Test");
  char buffer[256];

  CPPUNIT_ASSERT_EQUAL(0, strcmp(sample.getName(), "Test"));

  CPPUNIT_ASSERT(!sample.changed());
  CPPUNIT_ASSERT(!sample.hasInitialValue());
  CPPUNIT_ASSERT_EQUAL(0.0, sample.getValue());

  sample.unavailable();
  CPPUNIT_ASSERT(sample.changed());
  CPPUNIT_ASSERT(sample.hasInitialValue());

  CPPUNIT_ASSERT(sample.toString(buffer, 255) != 0);
  CPPUNIT_ASSERT_EQUAL(string("|Test|UNAVAILABLE"), string(buffer));

  CPPUNIT_ASSERT(sample.setValue(1.234567));
  CPPUNIT_ASSERT(sample.changed());
  CPPUNIT_ASSERT(sample.hasInitialValue());

  CPPUNIT_ASSERT(sample.toString(buffer, 255) != 0);
  CPPUNIT_ASSERT_EQUAL(string("|Test|1.234567"), string(buffer));

  CPPUNIT_ASSERT(sample.setValue(1.234568));
  CPPUNIT_ASSERT(sample.changed());
  CPPUNIT_ASSERT(sample.hasInitialValue());

  CPPUNIT_ASSERT(sample.toString(buffer, 255) != 0);
  CPPUNIT_ASSERT_EQUAL(string("|Test|1.234568"), string(buffer));

  sample.reset();

  // Check epsilon change does not change value.
  CPPUNIT_ASSERT(!sample.setValue(1.2345681));
  CPPUNIT_ASSERT(!sample.changed());

  CPPUNIT_ASSERT(sample.toString(buffer, 255) != 0);
  CPPUNIT_ASSERT_EQUAL(string("|Test|1.234568"), string(buffer));
}
