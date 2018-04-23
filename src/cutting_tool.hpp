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
#include <string>
#include <map>
#include <vector>
#include <float.h>


class CuttingToolProperty
{
public:
	CuttingToolProperty(
		const std::string &aName,
		std::map<std::string, std::string> &aAttributes,
		const std::string &aValue)
		:
		mName(aName),
		mAttributes(aAttributes),
		mValue(aValue)
	{
	}

	CuttingToolProperty(std::string aName) :
		mName(aName)
	{
	}

	CuttingToolProperty(const CuttingToolProperty &aProp) :
		mName(aProp.mName),
		mAttributes(aProp.mAttributes),
		mValue(aProp.mValue)
	{
	}

	virtual std::string toXML() const;

public:
	std::string mName;
	std::map<std::string,std::string> mAttributes;
	std::string mValue;
};


class CuttingToolStatus : public CuttingToolProperty
{
public:
	CuttingToolStatus(std::vector<std::string> &status) :
		CuttingToolProperty("CutterStatus"),
		mStatus(status)
	{
	}

	CuttingToolStatus(const CuttingToolStatus &status) :
		CuttingToolProperty(status),
		mStatus(status.mStatus)
	{
	}

	std::string toXML() const override;

public:
	std::vector<std::string> mStatus;
};


static const double CT_NO_VALUE = DBL_MAX;


class CuttingToolMeasurement : public CuttingToolProperty
{
public:
	CuttingToolMeasurement(
		std::string name,
		std::string code,
		double value,
		double nominal = CT_NO_VALUE,
		double min = CT_NO_VALUE,
		double max = CT_NO_VALUE,
		std::string nativeUnits = "",
		std::string units = "");

	CuttingToolMeasurement(const CuttingToolMeasurement &measurement) :
		CuttingToolProperty(measurement)
	{
	}
};


class CuttingItem
{
public:
	std::vector<CuttingToolProperty> mProperties;
	std::vector<CuttingToolMeasurement> mMeasurements;
};


class CuttingTool
{
public:
	CuttingTool(
		std::string &aAssetId, 
		int aToolNumber,
		std::string &aDescription,
		CuttingToolStatus &status);

	void add(CuttingToolProperty &aProp) {
		mProperties.push_back(aProp); }
	void add(CuttingToolMeasurement &aProp) {
		mMeasurements.push_back(aProp); }

	virtual std::string toString();

	const std::string &getAssetId() const {
		return mAssetId; }
	int getToolNumber() const { return mToolNumber; }

	bool isValid() const {
		return !mAssetId.empty(); }

protected:
	CuttingToolStatus mStatus;
	std::vector<CuttingToolProperty> mProperties;
	std::vector<CuttingToolMeasurement> mMeasurements;
	std::string mAssetId;
	int mToolNumber;
	std::string mDescription;
};
