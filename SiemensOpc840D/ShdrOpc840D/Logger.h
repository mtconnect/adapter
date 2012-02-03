// **************************************************************************
// Logger.h 
//
// Description:
//
// DISCLAIMER:
// This software was developed by U.S. Government employees as part of
// their official duties and is not subject to copyright. No warranty implied 
// or intended.
// **************************************************************************
#pragma once
#include "atlstr.h"
#include <fstream>
#include <iostream>
#include "StdStringEx.h"

#ifdef LOGLISTENERS
// This is for notifying listeners to logged messages - mnany to many relationship
#include <boost/function.hpp> 
#include <boost/function_equal.hpp> 
typedef boost::function<void (const TCHAR * msg)> FcnLognotify;
#include <vector>
#endif

// This is useful for preventing endless repeated error messages 
#define LOGONCE  static long nLog##__LINE__=0; if( 0 == nLog##__LINE__++) 
//#define LOGONCE  

/** Allow redirected of std::cout to vc console output
std::cout.rdbuf(&Logger);
std::cout << "Hello World\n";
std::cout.flush();
*/
class basic_debugbuf : public std::streambuf
{
public:
	basic_debugbuf(int bufferSize=100) 
	{
		if (bufferSize)
		{
			char *ptr = new char[bufferSize];
			setp(ptr, ptr + bufferSize);
		}
		else
			setp(0, 0);
	}
	virtual ~basic_debugbuf() 
	{
		sync();
		delete[] pbase();
	}

	virtual void writeString(const std::string &str)
	{
		OutputDebugString(str.c_str());
	}
private:
	int	overflow(int c)
	{
		sync();
		if (c != EOF)
		{
			if (pbase() == epptr())
			{
				std::string temp;
				temp += char(c);
				writeString(temp);
			}
			else
				sputc(c);
		}
		return 0;
	}
	int	sync()
	{
		if (pbase() != pptr())
		{
			int len = int(pptr() - pbase());
			std::string temp(pbase(), len);
			writeString(temp);
			setp(pbase(), epptr());
		}
		return 0;
	}
};

struct CDebugLevel
{
	CDebugLevel(int n, char * name) : _debug(n), _name(name) {}
	int operator ()() { return _debug; }
	operator int () { return _debug; }
	operator std::string () { return _name; }
	int _debug;
	std::string _name;
};

__declspec(selectany)  	  CDebugLevel ENTRY      (0, "ENTRY  ");
__declspec(selectany)  	  CDebugLevel FATAL      (0, "FATAL  ");
__declspec(selectany)  	  CDebugLevel ZERROR     (1, "ERROR  ");
__declspec(selectany)  	  CDebugLevel WARNING    (2, "WARNING");
__declspec(selectany)  	  CDebugLevel INFO       (3, "INFO   ");
__declspec(selectany)  	  CDebugLevel LDEBUG     (4, "DEBUG  ");
__declspec(selectany)  	  CDebugLevel DETAILED   (5, "DETAIL ");

/**
Sample use:
CLogger logger;
logger.DebugString()=true;
logger << FATAL << "Fatal Message1\n"; 
logger << FATAL << "Fatal Message2\n"; 
logger.DebugString()=false;
logger.Open(::ExeDirectory() + "debug.txt");
logger << FATAL << "Fatal Message\n"; 
logger << DETAILED << "DETAILED Message\n"; 
logger << INFO << "INFO Message\n"; 
logger().close();
*/

class CLogger : public basic_debugbuf
{
	struct nullstream: std::fstream {
		nullstream(): std::ios(0),  std::fstream((_Filet *) NULL) {}
	};
public:
	CLogger() 
	{
		DebugLevel()=3;
		filename=::ExeDirectory() + "debug.txt";
		std::cout.rdbuf(&_outputdebugstream);
		Timestamping()=false;
		DebugString()=false;
	}
	~CLogger()
	{
		if(DebugFile.is_open())
			DebugFile.close();
	}
	
	int & DebugLevel()				{ return _debuglevel; }
	bool & Timestamping()			{ return  bTimestamp; }
	bool & DebugString()			{ return  bDebugString; }
	std::fstream & operator()(void) { return this->DebugFile; }

	void Open(std::string filename, int bAppend=false)
	{
		this->filename=filename;
		Open(bAppend);
	}
	void Open(int bAppend)
	{	
		int opMode = std::fstream::out;
		if(bAppend)
			opMode |= std::fstream::app;

		DebugFile.open(filename.c_str(), opMode, OF_SHARE_DENY_NONE);
	}

	std::ostream  &operator<<( CDebugLevel level) 
	{ 
		std::fstream tmp;

		if( DebugLevel() < (int) level)
			return this->devnull;
		if(DebugString())
		{
			return PrintTimestamp(std::cout,level );
		}
		return PrintTimestamp(this->DebugFile,level);
	}
	std::ostream  & PrintTimestamp(std::ostream  & s, CDebugLevel level)
	{
		if(Timestamping())
		{
			s << Timestamp() << ": ";
			s << (std::string) level <<  " [" << (int ) level <<  "] ";
		}
		return s;
	} 
	void LogMessage(std::string msg, int level=-1)
	{
		if( DebugLevel() < level)
			return;

		OutputDebugString(msg.c_str());

		if(!DebugFile.is_open())
			return;
		if(Timestamping())
			DebugFile << Timestamp();
		DebugFile << msg;
		// LOGLISTENERS is for notifying windows or other listeners
#ifdef LOGLISTENERS
		for(int i=0; i< listeners.size(); i++)
			listeners[i](msg);
#endif
		DebugFile.flush();
	}

	/////////////////////////////////////////////////////////////////////////////

	static std::string Timestamp()
	{
		return (LPCSTR) COleDateTime::GetCurrentTime().Format();
	}
protected:
	int _debuglevel;
	bool bTimestamp;
	bool bDebugString;  
	std::fstream DebugFile;
	int nDebugReset;
	std::string filename;
	nullstream devnull;
	basic_debugbuf _outputdebugstream;
#ifdef LOGLISTENERS
	void AddListener(FcnLognotify notify)
	{
		listeners.push_back(notify);
	}
	std::vector<FcnLognotify> listeners;
#endif
};
__declspec(selectany)  CLogger GLogger;

