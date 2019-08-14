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
#pragma once
#include <vector>
#include <string>
#include <map>
#include <istream>

// Forward Declarations
class DeviceDatum;
namespace YAML
{
	class Parser;
	class Node;
}


// A register represents a PLC/PMC register as an accessible unit. The
// value of the register will manifest as a typed value or a
// conditional that will map to an event and be represented by a
// state.
class Register
{
public:
	enum EType
	{
		FLOAT_64,
		FLOAT_32,
		INTEGER_32,
		INTEGER_16,
		INTEGER_8,
		BOOL,
		BIT,
		CONDITION,
		TEXT
	};

	Register(EType type, int offset, bool timeSeries = false) :
		mType(type),
		mOffset(offset),
		mTimeseries(timeSeries),
		mScaler(0.0),
		mScalerOffset(0),
		mCount(0),
		mDatum(nullptr)
	{
	}

	Register(Register &anotherRegister) : 
		mType(anotherRegister.mType),
		mOffset(anotherRegister.mOffset),
		mTimeseries(anotherRegister.mTimeseries),
		mScaler(0.0),
		mScalerOffset(0),
		mCount(0),
		mDatum(nullptr)
	{
	}

protected:
	EType mType;
	int mOffset;
	bool mTimeseries;
	double mScaler;
	int mScalerOffset;
	int mCount;

	DeviceDatum *mDatum;
};


class RegisterSet
{
	void addRegister(Register &newRegister) {
		mRegisters.push_back(&newRegister); }

protected:
	int mAddress;
	int mLength;
	int mCount;

	std::vector<Register *> mRegisters;
};


class Configuration
{
public:
	Configuration();
	virtual ~Configuration();

	virtual void parse(
		std::istream &stream,
		int port = 7878,
		int delay = 1000,
		int timeout = 10000,
		const char *service = "MTConnect Adapter");

	int getPort() const {
		return mPort; }
	int getScanDelay() const {
		return mScanDelay; }
	int getTimeout() const {
		return mTimeout; }
	const std::string &getServiceName() const {
		return mServiceName; }

	void setPort(int aPort) {
		mPort = aPort; }

	RegisterSet *getRegisters(std::string &name) const;

protected:
	virtual void parse(
		YAML::Node &aDoc,
		int aPort,
		int aDelay,
		int aTimeout,
		const char *aService);

protected:
	int mPort;
	int mScanDelay;
	int mTimeout;
	std::string mServiceName;
	std::map<std::string, RegisterSet *> mRegisters;
};

