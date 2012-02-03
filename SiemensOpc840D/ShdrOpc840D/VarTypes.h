//
// VarTypes.h
//

// This software was developed by U.S. Government employees as part of
// their official duties and is not subject to copyright. No warranty implied 
// or intended.


#pragma once
#include <map>
#include <string>

class VarTypes
{
public:
	typedef std::map<std::string ,VARTYPE> VARTYPES;
	VARTYPES types;
	typedef std::map<std::string ,VARTYPE>::iterator VarTypeIterator;
	void Trim(std::string &svt)
	{
		svt.erase(svt.begin(), std::find_if(svt.begin(), svt.end(), std::not1(std::pointer_to_unary_function<int, int>(isspace))));
		svt.erase(std::find_if(svt.rbegin(), svt.rend(), std::not1(std::pointer_to_unary_function<int, int>(isspace))).base(), svt.end());
	}
	bool ValidVT(std::string  svt)
	{
		svt.erase(svt.begin(), std::find_if(svt.begin(), svt.end(), std::not1(std::pointer_to_unary_function<int, int>(isspace))));
		svt.erase(std::find_if(svt.rbegin(), svt.rend(), std::not1(std::pointer_to_unary_function<int, int>(isspace))).base(), svt.end());
		Trim(svt);
		return types.find( svt)==types.end();
	}

	std::string Lookup(VARTYPE  vt)
	{
		VARTYPES::iterator it;
		for(it=types.begin(); it!=types.end(); it++)
			if((*it).second == vt)
				return (*it).first;
		return "VT_UNDEFINED";
	}
	VARTYPE Lookup(std::string  svt)
	{
		VARTYPES::iterator it;
		Trim(svt);
		it=types.find(svt);
		if(it==types.end())
			return 0;
		return (*it).second;
	}
	VarTypes()
	{
		types["VT_EMPTY"]  = 0;
		types["VT_NUL"]  = 1;
		types["VT_I2"]  = 2;
		types["VT_I4"]  = 3;
		types["VT_R4"]  = 4;
		types["VT_R8"]  = 5;
		types["VT_CY"]  = 6;
		types["VT_DATE"]  = 7;
		types["VT_BSTR"]  = 8;
		types["VT_DISPATCH"]  = 9;
		types["VT_ERROR"]  = 10;
		types["VT_BOO"]  = 11;
		types["VT_VARIANT"]  = 12;
		types["VT_UNKNOWN"]  = 13;
		types["VT_DECIMA"]  = 14;
		types["VT_I1"]  = 16;
		types["VT_UI1"]  = 17;
		types["VT_UI2"]  = 18;
		types["VT_UI4"]  = 19;
		types["VT_I8"]  = 20;
		types["VT_UI8"]  = 21;
		types["VT_INT"]  = 22;
		types["VT_UINT"]  = 23;
		types["VT_VOID"]  = 24;
		types["VT_HRESULT"]   = 25;
		types["VT_PTR"]  = 26;
		types["VT_SAFEARRAY"]  = 27;
		types["VT_CARRAY"]  = 28;
		types["VT_USERDEFINED"]  = 29;
		types["VT_LPSTR"]  = 30;
		types["VT_LPWSTR"]  = 31;
		types["VT_FILETIME"]  = 64;
		types["VT_BLOB"]  = 65;
		types["VT_STREAM"]  = 66;
		types["VT_STORAGE"]  = 67;
		types["VT_STREAMED_OBJECT"]  = 68;
		types["VT_STORED_OBJECT"]  = 69;
		types["VT_BLOB_OBJECT"]  = 70;
		types["VT_CF"]  = 71;
		types["VT_CLSID"]  = 72;
		types["VT_VECTOR"]  = 0x1000;
		types["VT_ARRAY"]  = 0x2000;
		types["VT_BYREF"]  = 0x4000;
		types["VT_RESERVED"]  = 0x8000;
		types["VT_ILLEGA"]  = 0xffff;
		types["VT_ILLEGALMASKED"]  = 0xfff;
		types["VT_TYPEMASK"] = 0xfff;
	}
};