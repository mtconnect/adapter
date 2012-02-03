//
// SymbolTable.cpp
//

// This software was developed by U.S. Government employees as part of
// their official duties and is not subject to copyright. No warranty implied 
// or intended.




template <class SymbolT>
_variant_t CSymbolTableT<SymbolT>::GetValue(tstring tag)
{
	SymbolsIterator it = m_symbols.find(tag);
	if(it!=m_symbols.end())
		return (*it).second->m_value;
	return _variant_t((BSTR) "");
}

/// returns list of parameters in a section. Parameter name is returned, not with section prepended.
template <class SymbolT>
std::vector<tstring> CSymbolTableT<SymbolT>::GetSectionSymbols(tstring section)
{
	std::vector<tstring> symbols;
		ifstream fin(_config->configfile().c_str());
		config_reader::kernel_1a main_cr;
		main_cr.load_from(fin);
        if (!main_cr.is_block_defined("MachineMonitor"))
			return;
		
		const config_reader::kernel_1a& cr = main_cr.block("MachineMonitor");

#if 0
	// FIXME: this is wrong. Maybe reason for tree...
	SymbolsIterator it =  m_symbols.find(section);
	if(it != m_symbols.end())
	{
		it++;
		for(; it!=m_symbols.end(); it++)
		{
			if((*it).second->m_type!=SymbolTable::LEAF)
				break;
			symbols.push_back((*it).second->m_name);
		}

	}
#endif
	return symbols;
}


template <class SymbolT>
_variant_t CSymbolTableT<SymbolT>::GetSymbolValue(tstring section, tstring tag, variant_t var)
{
	tstring szTag = section + "." + tag;
	CSymbolPtr symbol = GetSymbol(szTag);
	if(symbol!=NULL)
	{
		return symbol->m_value; 
	}
	return  var;
}



template <class SymbolT>
HRESULT CSymbolTableT<SymbolT>::SetSymbolValue(tstring section, tstring tag, variant_t var)
{	
	tstring szTag = section + "." + tag;
	CSymbolPtr symbol;
	if((symbol=GetSymbol(szTag))!=NULL)
	{
		symbol->m_value=var;
		return S_OK;
	}
	return E_FAIL;

}


template <class SymbolT>
tstring CSymbolTableT<SymbolT>::ToString(tstring section)
{

	//// FIXME:
	tstring tmp;
	SymbolsIterator it, endit;
	if(section.IsEmpty())
	{
		it=m_symbols.begin();
		endit=m_symbols.end();

	}
	else
	{
		it =  m_symbols.find(section);
		if(it==m_symbols.end() ||  endit==m_symbols.end())
			return _T("");
	}

	for(; it!=endit; it++)
	{
		if((*it).second->m_type== SymbolTable::ROOT )
		{
			continue;
		}
		else if((*it).second->m_type== SymbolTable::BRANCH )
		{
			tmp+=StrFormat(_T("[%s]\n"),   (*it).second->m_section);
		}	
		else
		{
			try {
					tmp+=StrFormat(_T("%s=%s\n"),   (*it).second->m_name, (LPCTSTR) (_bstr_t)  (*it).second->m_value);
			}
			catch(...){}

		}

	}
	return tmp;
}

template <class SymbolT>
HRESULT CSymbolTableT<SymbolT>::AddSymbolItem(tstring section, tstring parameter, tstring value, VARTYPE vt, tstring sDefaultValue)
{
	CSymbolPtr symbol = new SymbolT(SymbolTable::LEAF,section, parameter) ;
	try 
	{
		symbol->m_value= _variant_t((LPCTSTR) value);
		//m_tree
		if(vt!=VT_EMPTY)
			symbol->vt = vt;

		if(symbol->vt != VT_USERDEFINED )
			symbol->m_value.ChangeType(symbol->vt);

		if(sDefaultValue.GetLength()>0)
		{
			symbol->m_defaultValue=_variant_t((LPCTSTR)sDefaultValue);
			symbol->m_defaultValue.ChangeType(symbol->vt);
		}
		m_symbols[parameter] = symbol;
	}
	catch(...)
	{
		return E_FAIL;
	}
	return S_OK;
}
#if 0
template <class SymbolPtr>
tstring CSymbolTableT<SymbolPtr>::ParseSection(tstring iniSection)
{
	tstring errstr="";
	//iniSection.Replace(";", "\n", "");
	unsigned int i;

	tstring tmp = iniSection.Mid(1, iniSection.Find(_T("]")) +1);
	tmp.Trim();

	tstring section = GetIniSectionName(tmp);

	m_symbols[section] = new SymbolT(CSymbol::BRANCH, section, section) ;

	if( OnBeforeParseSection(section, iniSection))
		return errstr;

	tstringVector lines = _Tokenize(iniSection,"\n");
	for(i=0; i< lines.size(); i++)
	{
		GetIniSetting(section, lines[i]);

	}
	return errstr;
}
#endif
template <class SymbolT>
tstring CSymbolTableT<SymbolT>::GetIniSectionName(tstring &line)
{
	line.Trim();
	if(line.Find(L'[')!=0)
		return _T("");
	if(line.ReverseFind(L']')<0)
		return _T("");
	line.Replace(_T("["), _T(""));
	line.Replace(_T("]"), _T(""));
	line.Trim();
	return line;

}

template <class SymbolT>
std::vector<tstring> CSymbolTableT<SymbolT>::ParseIni(tstring &contents)
{
	int last =0;
	tstring section;
	int i;

	vector<string> blocks;
    main_cr.get_blocks(blocks);
    
    vector<string>::iterator block;
    for (block = blocks.begin(); block != blocks.end(); ++block)
    {
		tstring section = (*block).c_str();
		// Add section to symbols
		Trim(section);
		_szSections.push_back(section);
		//_Sections.push_back(Section ( section, start));
		m_symbols[section]= new SymbolT(SymbolTable::BRANCH,section, _T("")) ;


		const config_reader::kernel_1a &secttionkey = main_cr.block(*block);
		vector<string> keys;
		secttionkey.get_keys(keys);
		for (int i =0; i < keys.end(); i++)
		{
			CSymbolPtr symbol;
			tstring parameter = keys[i];
			symbol = new SymbolT(SymbolTable::LEAF,section, parameter) ;
			tstring value = secttionkey [keys[i]];
			Trim(value);
			symbol->m_value= _variant_t(_bstr_t( value.c_str()));
			m_symbols[section+ _T(".") + parameter] = symbol;
		}
	return _szSections;
}

//template <class SymbolT>
//tstring CSymbolTableT<SymbolT>::GetIniSection(tstring section)
//{
//	for(int i=0; i< _Sections.size(); i++)
//	{
//		if(_Sections[i]._name == section)
//			return _contents.substr(_Sections[i]._first,_Sections[i]._end-_Sections[i]._first+1);
//	}
//	return "";
//}
template <class SymbolT>
tstring CSymbolTableT<SymbolT>::Fromfile(tstring szProgramIniFile)
{
	tstring sTmp;

	try {
		if(ATL::ATLPath::FileExists(szProgramIniFile.c_str()))

		//if(GetFileExists(szProgramIniFile))
		{
			ifstream fin(_config->configfile().c_str());
			main_cr.load_from(fin);
#if 0
			ReadFile(szProgramIniFile, _contents);
			ReplacePattern(_contents, _T("#"), _T("\n"), _T("\n"));
			ReplacePattern(_contents, _T(";"), _T("\n"), _T("\n"));
			//ReplaceAll(_contents, _T("\n\n"), _T("\n")); // Doesn't work

			if(_contents.size()<1)
				throw StdStringFormat(_T("Items Ini file %s is empty?\n"), szProgramIniFile.c_str());
#endif
			ParseIni( _contents);
		}
	}catch(tstring err)
	{
		Logger(FATAL) << err;
	}
	return sTmp;
}

template <class SymbolT>
HRESULT CSymbolTableT<SymbolT>::SetItemValue(tstring tag,_variant_t val)
{
	CSymbolPtr item = GetSymbol(tag);
	if(item==NULL)
		return E_FAIL;
	item->m_lastvalue=item->m_value;
	item->m_value=val;
	return S_OK;
}
template <class SymbolT>
_variant_t CSymbolTableT<SymbolT>::GetItemValue(tstring tag)
{
	CSymbolPtr item ;
	try {
		item = GetSymbol(tag);
		if(item==NULL)
			return _variant_t();
	}
	catch(...)
	{
		OutputDebugString(StdStringFormat(_T("GetItemValue %s Failed\n"), tag.c_str()).c_str());
		return _variant_t();
	}
	return item->m_value;

}
