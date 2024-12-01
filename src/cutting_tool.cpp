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
#include "cutting_tool.hpp"
#include <sstream>

using namespace std;

string encodeForXml(const string &source)
{
	ostringstream ret;

	for (auto iter = source.cbegin(); iter != source.cend(); iter++)
	{
		unsigned char c = (unsigned char) * iter;

		switch (c)
		{
		case '&':
			ret << "&amp;";
			break;

		case '<':
			ret << "&lt;";
			break;

		case '>':
			ret << "&gt;";
			break;

		case '"':
			ret << "&quot;";
			break;

		case '\'':
			ret << "&apos;";
			break;

		default:
			if (c < 32 || c > 127)
				ret << "&#" << (unsigned int)c << ";";
			else
				ret << c;
		}
	}

	return ret.str();
}


std::string CuttingToolProperty::toXML() const
{
	ostringstream xml;
	xml << '<' << mName;

	for(auto const &mapPair : mAttributes)
		xml << ' ' << mapPair.first << "=\"" << encodeForXml(mapPair.second) << '"';

	if (!mValue.empty())
		xml << '>' << encodeForXml(mValue) << "</" << mName;
	else
		xml << '/';

	xml << '>';
	return xml.str();
}


std::string CuttingToolStatus::toXML() const
{
	ostringstream xml;
	xml << '<' << mName << '>';

	for (auto const &status : mStatus)
		xml << "<Status>" << status << "</Status>";

	xml << "</" << mName << '>';

	return xml.str();
}


static char *doubleToString(double value, char *buffer)
{
	sprintf(buffer, "%.10g", value);
	return buffer;
}


CuttingToolMeasurement::CuttingToolMeasurement(
	std::string name,
	std::string code,
	double value,
	double nominal,
	double min,
	double max,
	std::string nativeUnits,
	std::string units)
	:
	CuttingToolProperty(name)
{
	char buffer[80];
	mAttributes["code"] = code;

	if (nominal != CT_NO_VALUE)
		mAttributes["nominal"] = doubleToString(nominal, buffer);

	if (min != CT_NO_VALUE)
		mAttributes["minimum"] = doubleToString(min, buffer);

	if (max != CT_NO_VALUE)
		mAttributes["maximum"] = doubleToString(max, buffer);

	if (!nativeUnits.empty())
		mAttributes["nativeUnits"] = nativeUnits;

	if (!units.empty())
		mAttributes["units"] = units;

	if (value != CT_NO_VALUE)
		mValue = doubleToString(value, buffer);
}


CuttingTool::CuttingTool(
	std::string &assetId,
	int toolNumber,
	std::string &description,
	CuttingToolStatus &status)
	:
	mAssetId(encodeForXml(assetId)),
	mToolNumber(toolNumber),
	mDescription(encodeForXml(description)),
	mStatus(status)
{
}


string CuttingTool::toString()
{
	// Agent takes care of reordering the properties, so we can cheat...
	std::ostringstream xml;

	// Open tag...
	xml << "<CuttingTool serialNumber=\"" << mAssetId
		<< "\" toolId=\"" << mToolNumber
		<< "\" assetId=\"" << mAssetId
		<< "\">";
	xml << "<Description>" << mDescription << "</Description>";
	xml << "<CuttingToolLifeCycle>";

	// Properties
	for (auto const &prop  : mProperties)
		xml << prop.toXML();

	xml << "<Measurements>";
	for (auto const &measure : mMeasurements)
		xml << measure.toXML();
	xml << "</Measurements>";

	// Items...
	xml << "</CuttingToolLifeCycle></CuttingTool>";

	return xml.str();
}