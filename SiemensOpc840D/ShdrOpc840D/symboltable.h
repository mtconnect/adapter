// **************************************************************************
// SymbolTable.h
//
// Description:
//
// DISCLAIMER:
// This software was developed by U.S. Government employees as part of
// their official duties and is not subject to copyright. No warranty implied 
// or intended.
// **************************************************************************

#pragma once

#include <map>
#include "comutil.h"
#include <atlpath.h>
#include "Utils.h"
#include "stdafx.h"
#include <dlib/config_reader.h>

namespace SymbolTable
{
	typedef enum { ROOT, BRANCH, LEAF} SymbolType;
};

template <class T>
class CSymbolT : public ISmartPointer<T >
{
public:
	SymbolTable::SymbolType m_type;
	tstring m_name;
	tstring m_section;
	tstring m_parameter;

	VARTYPE vt;
	_variant_t m_value;
	_variant_t m_lastvalue;
	_variant_t m_defaultValue;
	FILETIME m_time;

	// Application specific
	CSymbolT(SymbolTable::SymbolType type=LEAF, tstring section="", tstring tag="")
	{
		m_type=type;
		m_section = section;
		m_name=tag;
		m_value.Clear();
		m_lastvalue.vt=VT_EMPTY;
	}

};

//#define CSYMBOLPTR CSymbol *
//#define CSYMBOLPTR CSymbolPtr
//typedef boost::intrusive_ptr<CSymbol> CSYMBOLPTR;              
//typedef boost::intrusive_ptr<const CSymbol> CSymbolConstPtr;

template <class SymbolT>
class CSymbolTableT
{
public:
	typedef boost::intrusive_ptr<SymbolT> CSymbolPtr;              
	typedef boost::intrusive_ptr<const SymbolT> CSymbolConstPtr;

	typedef std::map<tstring,CSymbolPtr> SymbolsMap;
	typedef typename SymbolsMap::iterator SymbolsIterator;

	CSymbolTableT(){}
	virtual ~CSymbolTableT(){}


	HRESULT			SetItemValue(tstring tag,_variant_t val);
	_variant_t		GetItemValue(tstring tag);

	_variant_t		GetSymbolValue(tstring section, tstring tag, variant_t var=0);
	HRESULT			SetSymbolValue(tstring section, tstring tag, variant_t var);

	std::vector<tstring>	GetSectionSymbols(tstring section);

	template<typename T>
	bool GetValue(tstring section, tstring parameter, T & val);


	_variant_t		GetValue(tstring tag);

	//tstring			ParseSection(tstring iniSection);
	tstring			ToString(tstring section=L"");
	tstring			Fromfile(tstring filename);
	std::vector<tstring>	ParseIni(tstring &contents);
	HRESULT			AddSymbolItem(tstring section, 	tstring parameter, 	tstring value, 	VARTYPE vt, tstring sDefaultValue);

	void			Clear(){ _Sections.clear(); _szSections.clear(); m_symbols.clear(); _contents.clear(); }


	CSymbolPtr GetSymbol(tstring tag)
	{
		SymbolsIterator it =  m_symbols.find(tag);
		if(it != m_symbols.end())
			return (*it).second;
		return NULL;

	}
	template <typename T>
	T GetSymbolValue(tstring section, tstring tag, T defaultval)
	{
		T val=defaultval;
		tstring szTag = section + "." + tag;

		try 
		{
			_variant_t var = GetItemValue(szTag);
			val =(T) var;
		}
		catch(...)
		{
			return defaultval;
		}
		return val; 
	}

	tstring GetSymbolValue(tstring section, tstring tag, const char * defaultval)
	{
		tstring val=defaultval;
		tstring szTag = section + "." + tag;

		try 
		{
			_variant_t var = GetItemValue(szTag);
			val = (LPCTSTR)(_bstr_t) var;
		}
		catch(...)
		{
			return defaultval;
		}
		return val; 
	}

private:
	void IniParseIt(tstring filename);
	tstring GetIniSectionName(tstring &line);


	struct Section
	{
		Section(tstring name, int start, int first=-1, int end=-1) : _name(name) ,  _start(start) ,  _first(first),  _end(end) {}
		tstring _name;
		int _start;
		int _first;
		int _end;
	};
	std::vector<Section> _Sections;
	std::vector<tstring> _szSections;

	// Map rep
	SymbolsMap m_symbols;

	tstring _contents;
	config_reader::kernel_1a main_cr;

};
#if 1
class CSymbol : public CSymbolT<CSymbol> 
{
public:
	CSymbol(SymbolTable::SymbolType type=SymbolTable::LEAF, tstring section="", tstring tag="") :
	  CSymbolT<CSymbol>(type, section,  tag)
	  {

	  }
};
typedef CSymbolTableT<CSymbol> CSymbolTable;
#else
#include "SymbolTableWithScripting.h"
#endif

#include "SymbolTable.cpp"