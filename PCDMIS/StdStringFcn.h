//
// StdStringFcn.h
//
//
// StdStringFcn.h
//

#pragma once

#include <algorithm> 
#include <functional> 
#include <locale>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iosfwd>

#include <string>
#include <iomanip>
#include <vector>
#include <map>

#ifdef UNICODE
typedef std::wstring  tstring;
typedef	std::wifstream tifstream;
typedef std::wstringstream  tstringstream;
typedef std::wstreambuf tstreambuf;
typedef std::wfstream tfstream;
typedef	std::wofstream tofstream;
#define t_str w_str
#else
typedef std::string  tstring;
typedef	std::ifstream tifstream;
typedef std::stringstream  tstringstream;
typedef std::streambuf tstreambuf;
typedef std::fstream tfstream;
#define t_str c_str
typedef	 std::ofstream tofstream;
#define A2T(X) X

#endif


// Convert the string to lowercase
inline void MakeUpper(tstring &str)
{
	std::transform(str.begin(),str.end(),str.begin(),toupper);
}

inline void MakeLower(tstring &str)
{
	std::transform(str.begin(),str.end(),str.begin(),tolower);
}

inline std::string UrlEncode(const std::string& input)
{
    std::ostringstream ssOut;
    ssOut << std::setbase(16);
    for(std::string::const_iterator i = input.begin(); i != input.end(); ++i)
    {
        if(isalnum(*i))
            ssOut << *i;
        else
            ssOut << '%' << std::setw(2) << std::setfill('0') << ((unsigned int)(unsigned char)*i);
    }
    return ssOut.str();
} 

// std::string test = urldecode("http%3A%2F%2F");
inline std::string UrlDecode ( const std::string& str ) 
{
	std::string result;
	std::string::size_type i;
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
inline std::string & RemoveSpaces(std::string& str)
{
	str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
	return str;
}
inline std::string & RemoveChar(std::string& str, char c)
{
	std::string::iterator end_pos = std::remove(str.begin(), str.end(), c); 
	str.erase(end_pos, str.end()); 
	return str;
}

inline std::string StdStringFormat(const TCHAR* format, ...)
{
	va_list ap;
	va_start(ap, format);
	
	int m;
	int n= strlen(format) + 1028;
	std::string tmp(n,'0');	


	// Kind of a bogus way to insure that we don't
	// exceed the limit of our buffer
	while((m=_vsnprintf(&tmp[0], n-1, format, ap))<0)
	{
		n=n+1028;
		tmp.resize(n,'0');
	}
	va_end(ap);
	return tmp.substr(0,m);

}

//////////////////////////////////////////////////
// std::string Exensions
//////////////////////////////////////////////////



// trim from start
inline std::string &LeftTrim(std::string &str) 
{

	size_t startpos = str.find_first_not_of(" \t\r\n"); 
	if( std::string::npos != startpos )
		str = str.substr( startpos ); 
	return str;
}

// trim from end
inline std::string &RightTrim(std::string &str) 
{
 		size_t endpos = str.find_last_not_of(" \t\r\n"); 
		if(std::string::npos != endpos ) 
			str = str.substr( 0, endpos+1 ); 		 
		return str;
}

// trim from both ends
inline std::string &Trim(std::string &s) 
{
        return LeftTrim(RightTrim(s));
}

inline bool EndsWith (std::string const &fullString, std::string const &ending) 
{ 
    if (fullString.length() > ending.length()) { 
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending)); 
    } else { 
        return false; 
    } 
} 
inline std::string ExtractDirectory( const std::string& path )
  {
  return path.substr( 0, path.find_last_of( '\\' ) +1 );
  }
inline std::string ExtractFilename( const std::string& path )
  {
  return path.substr( path.find_last_of( '\\' ) +1 );
  }
inline std::string ChangeExtension( const std::string& path, const std::string& ext )
  {
  std::string filename = ExtractFilename( path );
  return ExtractDirectory( path ) +filename.substr( 0, filename.find_last_of( '.' ) ) +ext;
  }
inline std::string nowtimestamp()
{
  SYSTEMTIME st;
  GetSystemTime(&st);
  return  StdStringFormat("%4d-%02d-%02dT%02d:%02d:%02d.%04d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
}
inline std::string fileToText( const std::string &filename )
{
   std::ifstream fin(filename.c_str());
   return std::string((std::istreambuf_iterator<char>(fin)),std::istreambuf_iterator<char>());
}
inline void ReadFile(std::string filename, std::string & contents)
{
	std::ifstream in(filename.c_str()); 
	std::stringstream buffer; 
	buffer << in.rdbuf(); 
	contents= buffer.str();
} 
inline void WriteFile(std::string filename, std::string & contents)
{
	std::ofstream outFile(filename.c_str());
	//Write the string and its null terminator !!
	outFile << contents.c_str();
}

inline std::vector<std::string> &Split(const std::string &s, char delim, std::vector<std::string> &elems, bool bKeepEmpty=false) 
{ 
	std::stringstream ss(s); 
	std::string item; 
	while(std::getline(ss, item, delim)) { 
		if(item.empty() && !bKeepEmpty)
			continue;
		elems.push_back(item); 
	} 
	return elems; 
} 


inline std::vector<std::string> Split(const std::string &s, char delim) { 
	std::vector<std::string> elems; 
	return Split(s, delim, elems); 
} 



inline std::map<std::string, std::string> ParseIniSection(std::string filename, std::string section)
{
	std::string contents;
	std::map<std::string, std::string> symbols;
	ReadFile(filename, contents);
	int n,m;
	if((n=contents.find("["+section +"]"))==std::string::npos)
		return symbols;
	contents=contents.substr(n+section.size()+2);
	if((m=contents.find("["))==std::string::npos)
		m=contents.size();
	std::string s = Trim(contents.substr(0, m-1));
	std::vector<std::string> lines = Split(s, '\n');
	for(unsigned int i=0; i< lines.size(); i++)
	{
		std::vector<std::string> tokens=Split(lines[i], '=');
		if(tokens.size()<2)
			continue;
		symbols[Trim(tokens[0])]=Trim(tokens[1]);
		
	}
	return symbols;

}
inline std::string GetIniProfileString(std::string filename, std::string section, std::string key)
{
	std::map<std::string, std::string> symbols = ParseIniSection(filename, section);
	return symbols[key];
}

template<typename T>
inline T ConvertString(std::string data, T defaultval)
{
	T result;
	std::istringstream stream(data);

	if(stream >> result)
		return result;
	return defaultval;
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

inline std::string CenterString(const std::string& text, const int& fieldwidth,
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
  std::string pad1;
  if (p1 > 0) pad1 = std::string(p1, ' ');
  std::string pad2;
  if (p2 > 0) {
    pad2 = std::string(p2, ' ');
  }
  return pad1+text+pad2;
}
// Return string of length Max(fieldwidth, Length(text))
inline std::string PadString(const std::string& text, const int& fieldwidth)
{
  int len = text.size();
  if (len >= fieldwidth) return text;
  std::string pad1((fieldwidth-len), ' ');
  return text+pad1;
}
// Return right-justified string
inline std::string RightString(const std::string& text, const int& fieldwidth)
{
  int len = text.size();
  if (len >= fieldwidth) return text;
  std::string pad1((fieldwidth-len), ' ');
  return pad1+text;
}
// Return left-justified string
inline std::string LeftString(const std::string& text, const int& fieldwidth)
{
  int len = text.size();
  if (len >= fieldwidth) return text;
  std::string pad1((fieldwidth-len), ' ');
  return text+pad1;
}
// <tag><data</tag>
inline std::string MakeXMLtag(const std::string& tag, const std::string& data)
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
