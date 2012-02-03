// **************************************************************************
// StdStringFcn.h
//
// Description:
//
// DISCLAIMER:
// This software was developed by U.S. Government employees as part of
// their official duties and is not subject to copyright. No warranty implied 
// or intended.
// **************************************************************************

#pragma once

#include <algorithm> 
#include <functional> 
#include <locale>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <string>
#include <iomanip>


// more at: http://www.richelbilderbeek.nl/CppString.htm
//#ifdef UNICODE
//typedef std::wstring  tstring;
//typedef	std::wifstream tifstream;
//typedef std::wstringstream  tstringstream;
//typedef std::wstreambuf tstreambuf;
//typedef std::wfstream tfstream;
//typedef	std::wofstream tofstream;
//#define t_str w_str
//#else
//typedef std::string  tstring;
//typedef	std::ifstream tifstream;
//typedef std::stringstream  tstringstream;
//typedef std::streambuf tstreambuf;
//typedef std::fstream tfstream;
//#define t_str c_str
//typedef	 std::ofstream tofstream;
//#define A2T(X) X
//
//#endif

#define tstring std::string
#define tstringstream std::stringstream 
#include <iosfwd> 
#include <vector>
#include <map>


// Convert the string to lowercase
inline tstring& MakeUpper(tstring &str)
{
	std::transform(str.begin(),str.end(),str.begin(),toupper);
	return str;
}

inline tstring & MakeLower(tstring &str)
{
	std::transform(str.begin(),str.end(),str.begin(),tolower);
	return str;
}
template<typename T>
inline T ConvertFromString(tstring data, T defaultval)
{
	T result;
	std::istringstream stream(data);
	try{
		stream >> result;
		return result;
	}catch(...){}
	return defaultval;
}

template<>
inline bool ConvertFromString(tstring data, bool defaultval)
{
	if(MakeLower(data)!="true" && MakeLower(data)!="false" )
		return defaultval;
	if(MakeLower(data)=="true")
		return true;
	return false;
}

template<typename T>
inline std::string ConvertToString(T data)
{
	std::stringstream stream;
	try{
		stream << data;
	}catch(...){}

	return stream.str();
}
inline tstring UrlEncode(const tstring& input)
{
	std::ostringstream ssOut;
	ssOut << std::setbase(16);
	for(tstring::const_iterator i = input.begin(); i != input.end(); ++i)
	{
		if(isalnum(*i))
			ssOut << *i;
		else
			ssOut << '%' << std::setw(2) << std::setfill('0') << ((unsigned int)(unsigned char)*i);
	}
	return ssOut.str();
} 

// tstring test = urldecode("http%3A%2F%2F");
inline tstring UrlDecode ( const tstring& str ) 
{
	tstring result;
	tstring::size_type i;
	for (i = 0; i < str.size(); ++i)
	{
		if (str[i] == '+')
		{
			result += ' ';
		}
		else if (str[i] == '%' && str.size() > i+2)
		{
			std::istringstream iss("0x" + str.substr(i+1,2));
			long h=0;
			if(iss>>std::hex>>h)
				result += (unsigned char) h;
			i += 2;
		}
		else
		{
			result += str[i];
		}
	}
	return result;
}
inline tstring & RemoveSpaces(tstring& str)
{
	str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
	return str;
}
inline tstring & RemoveChar(tstring& str, char c)
{
	tstring::iterator end_pos = std::remove(str.begin(), str.end(), c); 
	str.erase(end_pos, str.end()); 
	return str;
}

inline tstring StdStringFormat(const TCHAR* format, ...)
{
	va_list ap;
	va_start(ap, format);

	int m;
	int n= strlen(format) + 1028;
	tstring tmp(n,'0');	


	// Kind of a bogus way to insure that we don't
	// exceed the limit of our buffer
	//_vsnprintf_s
	while((m=_vsnprintf_s(&tmp[0], n-1, n-1, format, ap))<0)
	{
		n=n+1028;
		tmp.resize(n,'0');
	}
	va_end(ap);
	return tmp.substr(0,m);

}



//////////////////////////////////////////////////
// tstring Exensions
//////////////////////////////////////////////////
//??? http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring

// trim from start
inline std::string &LeftTrim(tstring &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::pointer_to_unary_function<int, int>(isspace))));
	return s;
}

// trim from end
inline std::string &RightTrim(tstring &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::pointer_to_unary_function<int, int>(isspace))).base(), s.end());
	return s;
}


// trim from both ends
inline tstring &Trim(tstring &s) 
{
	return LeftTrim(RightTrim(s));
}

inline bool EndsWith (tstring const &fullString, tstring const &ending) 
{ 
	if (fullString.length() > ending.length()) { 
		return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending)); 
	} else { 
		return false; 
	} 
} 
inline tstring ExtractDirectory( const tstring& path )
{
	return path.substr( 0, path.find_last_of( '\\' ) +1 );
}
inline tstring ExtractFilename( const tstring& path )
{
	return path.substr( path.find_last_of( '\\' ) +1 );
}
inline tstring ChangeExtension( const tstring& path, const tstring& ext )
{
	tstring filename = ExtractFilename( path );
	return ExtractDirectory( path ) +filename.substr( 0, filename.find_last_of( '.' ) ) +ext;
}
inline tstring nowtimestamp()
{
	SYSTEMTIME st;
	GetSystemTime(&st);
	return  StdStringFormat("%4d-%02d-%02dT%02d:%02d:%02d.%04d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
}
inline tstring fileToText( const tstring &filename )
{
	std::ifstream fin(filename.c_str());
	return tstring((std::istreambuf_iterator<char>(fin)),std::istreambuf_iterator<char>());
}
inline void ReadFile(tstring filename, tstring & contents)
{
	std::ifstream in(filename.c_str()); 
	tstringstream buffer; 
	buffer << in.rdbuf(); 
	contents= buffer.str();
} 
inline void WriteFile(tstring filename, tstring & contents)
{
	std::ofstream outFile(filename.c_str());
	//Write the string and its null terminator !!
	outFile << contents.c_str();
}

inline std::vector<tstring> &Split(const tstring &s, char delim, std::vector<tstring> &elems, bool bKeepEmpty=false) 
{ 
	tstringstream ss(s); 
	tstring item; 
	while(std::getline(ss, item, delim)) { 
		if(item.empty() && !bKeepEmpty)
			continue;
		elems.push_back(item); 
	} 
	return elems; 
} 


inline std::vector<tstring> Split(const tstring &s, char delim) { 
	std::vector<tstring> elems; 
	return Split(s, delim, elems); 
} 


typedef std::map<tstring, tstring>::iterator StrMapIterator;

inline tstring GetIniSection(tstring filename, tstring section)
{
	int n,m;
	tstring contents;
	ReadFile(filename, contents);
	if((n=contents.find("["+section +"]"))==tstring::npos)
		return "";
	contents=contents.substr(n+section.size()+2);
	if((m=contents.find("["))==tstring::npos)
		m=contents.size();
	return Trim(contents.substr(0, m-1));
}

inline std::map<tstring, tstring> ParseIniString(tstring s)
{	
	std::map<tstring, tstring> symbols;
	std::vector<tstring> lines = Split(s, '\n');
	for(int i=0; i< lines.size(); i++)
	{
		std::vector<tstring> tokens=Split(lines[i], '=');
		if(tokens.size()<2)
			continue;
		symbols[Trim(tokens[0])]=Trim(tokens[1]);

	}
	return symbols;

}


inline std::map<tstring, tstring> ParseIniSection(tstring filename, tstring section)
{
	tstring s=GetIniSection(filename, section);
	return ParseIniString(s);
}
inline tstring ExeDirectory()
{
	TCHAR buf[1000];
	GetModuleFileName(NULL, buf, 1000);
	tstring path(buf);
	path=path.substr( 0, path.find_last_of( '\\' ) +1 );
	return path;
}
inline tstring GetIniProfileString(tstring filename, tstring section, tstring key)
{
	std::map<tstring, tstring> symbols = ParseIniSection(filename, section);
	return symbols[key];
}


// From: http://www.ccp4.ac.uk/ccp4bin/viewcvs/pointless/string_util.cpp?rev=1.1&content-type=text/vnd.viewcvs-markup

// Return string of length <fieldwidth> with text centred on position
// cenpos (numbered from 0)
#ifndef MAX
#define MAX(X,Y) ((X>Y) ?  X : Y)
#endif
#ifndef MIN
#define MIN(X,Y) ((X<Y) ?  X : Y)
#endif

inline tstring CenterString(const tstring& text, const int& fieldwidth,
							const int& cenposition)
{
	int cenpos = cenposition;
	if (cenpos == 0) cenpos = fieldwidth/2;
	//  ASSERT (cenpos < fieldwidth);
	int len = text.size();
	if (len >= fieldwidth) return text;
	// number of trailing spaces
	int p2 = MAX(0,MIN(fieldwidth-1-(cenpos+len/2), fieldwidth-1)); 
	int p1 = fieldwidth - len - p2; // number of leading spaces
	tstring pad1;
	if (p1 > 0) pad1 = tstring(p1, ' ');
	tstring pad2;
	if (p2 > 0) {
		pad2 = tstring(p2, ' ');
	}
	return pad1+text+pad2;
}
// Return string of length Max(fieldwidth, Length(text))
inline tstring PadString(const tstring& text, const int& fieldwidth)
{
	int len = text.size();
	if (len >= fieldwidth) return text;
	tstring pad1((fieldwidth-len), ' ');
	return text+pad1;
}
// Return right-justified string
inline tstring RightString(const tstring& text, const int& fieldwidth)
{
	int len = text.size();
	if (len >= fieldwidth) return text;
	tstring pad1((fieldwidth-len), ' ');
	return pad1+text;
}
// Return left-justified string
inline tstring LeftString(const tstring& text, const int& fieldwidth)
{
	int len = text.size();
	if (len >= fieldwidth) return text;
	tstring pad1((fieldwidth-len), ' ');
	return text+pad1;
}
// <tag><data</tag>
inline tstring MakeXMLtag(const tstring& tag, const tstring& data)
{
	return "<"+tag+">"+data+"</"+tag+">";
}



#include <streambuf>
#include <ostream>

template <class cT, class traits = std::char_traits<cT> >
class basic_nullbuf: public std::basic_streambuf<cT, traits> {
	typename traits::int_type overflow(typename traits::int_type c)
	{
		return traits::not_eof(c); // indicate success
	}
};

template <class cT, class traits = std::char_traits<cT> >
class basic_onullstream: public std::basic_ostream<cT, traits> {
public:
	basic_onullstream():
	  std::basic_ios<cT, traits>(&m_sbuf),
		  std::basic_ostream<cT, traits>(&m_sbuf)
	  {
		  init(&m_sbuf);
	  }

private:
	basic_nullbuf<cT, traits> m_sbuf;
};

typedef basic_onullstream<char> onullstream;
typedef basic_onullstream<wchar_t> wonullstream;


inline std::string ReplaceOnce(
							   std::string result, 
							   const std::string& replaceWhat, 
							   const std::string& replaceWithWhat)
{
	const int pos = result.find(replaceWhat);
	if (pos==-1) return result;
	result.replace(pos,replaceWhat.size(),replaceWithWhat);
	return result;
}


// usual one can have infinite loop (e.g., \r\n for \n
inline void ReplaceAll( tstring& tInput, tstring tFind, tstring tReplace ) 
{ 
	size_t uPos = 0; 
	size_t uFindLen = tFind.length(); 
	size_t uReplaceLen = tReplace.length();
	if( uFindLen == 0 )
	{
		return;
	}

	for( ;(uPos = tInput.find( tFind, uPos )) != tstring::npos; )
	{
		tInput.replace( uPos, uFindLen, tReplace );
		uPos += uReplaceLen;
	}

}

inline boolean ReplacePattern(tstring & inStr, tstring oldBeginningStr, tstring oldEndingStr, tstring newStr)
{
	int i,j,k;

	// Sanity checks
	if(inStr.size()<1) return false;
	if(oldBeginningStr.size()<1) return false;
	if(oldEndingStr.size()<1) return false;

	// Setup up StringEx sizes
	int newCnt = newStr.size();  

	i=0; // Let find determine where to start.

	int cnt=0;
	for(;i<inStr.size();){
		if((j=inStr.find(oldBeginningStr,  i))<0)break;
		if((k=inStr.find(oldEndingStr, j))<0) break;
		inStr = inStr.substr(0,j)+ newStr + inStr.substr(k+1);
		i=j+newCnt;
		cnt++;
	}
	if(cnt>0) return true;
	return false;
}

inline std::vector<std::string> Tokenize(const std::string& str,const std::string& delimiters)
{
	std::vector<std::string> tokens;
	std::string::size_type delimPos = 0, tokenPos = 0, pos = 0;

	if(str.length()<1)  return tokens;
	while(1){
		delimPos = str.find_first_of(delimiters, pos);
		tokenPos = str.find_first_not_of(delimiters, pos);

		if(std::string::npos != delimPos){
			if(std::string::npos != tokenPos){
				if(tokenPos<delimPos){
					tokens.push_back(str.substr(pos,delimPos-pos));
				}else{
					tokens.push_back("");
				}
			}else{
				tokens.push_back("");
			}
			pos = delimPos+1;
		} else {
			if(std::string::npos != tokenPos){
				tokens.push_back(str.substr(pos));
			} else {
				tokens.push_back("");
			}
			break;
		}
	}
	return tokens;
}

inline std::string ErrorFormatMessage(HRESULT dwLastErr )
{
	TCHAR * pTemp;
	std::string errmsg("");
	int nLen = ::FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_IGNORE_INSERTS |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, 
		dwLastErr,
		MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
		(LPTSTR) &pTemp, 
		1, 
		NULL );
	if(pTemp==NULL)
		return "";

#ifdef UNICODE
	errmsg=(BSTR) pTemp;
#else
	errmsg=(char *) pTemp;
#endif

	::LocalFree( pTemp );

	return errmsg;
}