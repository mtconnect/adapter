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

string encodeForXml( const string &aSrc )
{
	ostringstream ret;

    for( string::const_iterator iter = aSrc.begin(); iter != aSrc.end(); iter++ )
	{
         unsigned char c = (unsigned char)*iter;

         switch( c )
		{
             case '&': ret << "&amp;"; break;
             case '<': ret << "&lt;"; break;
             case '>': ret << "&gt;"; break;
             case '"': ret << "&quot;"; break;
             case '\'': ret << "&apos;"; break;

		default:
              if ( c<32 || c>127 )
              {
				ret << "&#" << (unsigned int)c << ";";
              }
			else
              {
				ret << c;
		}
	}
    }

	return ret.str();
}

std::string CuttingToolProperty::toXML()
{
	ostringstream xml;
	xml << '<' << mName;

  map<std::string,std::string>::iterator iter;
  for (iter = mAttributes.begin(); iter != mAttributes.end(); iter++) {
    xml << ' ' << iter->first << "=\"" << encodeForXml(iter->second) << '"';
  }
  if (!mValue.empty()) {
		xml << '>' << encodeForXml(mValue) << "</" << mName;
  } else {
		xml << '/';
  }

	xml << '>';
  
	return xml.str();
}

std::string CuttingToolStatus::toXML()
{
	ostringstream xml;
	xml << '<' << mName << '>';
  vector<std::string>::iterator iter;
  for (iter = mStatus.begin(); iter != mStatus.end(); iter++) {
    xml << "<Status>" << *iter << "</Status>";
  }

	xml << "</" << mName << '>';

	return xml.str();
}

static char *doubleToString(double aDouble, char *aBuffer)
{
  sprintf(aBuffer, "%.10g", aDouble);
  return aBuffer;
}

CuttingToolMeasurement::CuttingToolMeasurement(std::string aName, std::string aCode, 
                       double aValue, double aNominal, double aMin, double aMax,  
                       std::string aNativeUnits, std::string aUnits)
  : CuttingToolProperty(aName)
{
	char buffer[80];
  mAttributes["code"] = aCode;

  if (aNominal != CT_NO_VALUE) mAttributes["nominal"] = doubleToString(aNominal, buffer);
  if (aMin != CT_NO_VALUE) mAttributes["minimum"] = doubleToString(aMin, buffer);
  if (aMax != CT_NO_VALUE) mAttributes["maximum"] = doubleToString(aMax, buffer);
  if (!aNativeUnits.empty()) mAttributes["nativeUnits"] = aNativeUnits;
  if (!aNativeUnits.empty()) mAttributes["units"] = aUnits;

  if (aValue != CT_NO_VALUE) mValue = doubleToString(aValue, buffer);
}

CuttingTool::CuttingTool(std::string &aAssetId, int aToolNumber, std::string &aDescription, CuttingToolStatus &aStatus)
  : mAssetId(encodeForXml(aAssetId)), mToolNumber(aToolNumber), mDescription(encodeForXml(aDescription)), 
    mStatus(aStatus)
{
}

string CuttingTool::toString()
{
	// Agent takes care of reordering the properties, so we can cheat...
	std::ostringstream xml;

	// Open tag...
  xml << "<CuttingTool serialNumber=\"" << mAssetId << 
    "\" toolId=\"" << mToolNumber << "\" assetId=\"" << mAssetId << "\">";  
	xml << "<Description>" << mDescription << "</Description>";
	xml << "<CuttingToolLifeCycle>";

	// Properties
  vector<CuttingToolProperty>::iterator prop;
  for (prop = mProperties.begin(); prop != mProperties.end(); prop++) {
    xml << prop->toXML();
  }

	xml << "<Measurements>";

  vector<CuttingToolMeasurement>::iterator measure;
  for (measure = mMeasurements.begin(); measure != mMeasurements.end(); measure++) {
    xml << measure->toXML();
  }

	xml << "</Measurements>";

	// Items...
	xml << "</CuttingToolLifeCycle></CuttingTool>";

	return xml.str();
}