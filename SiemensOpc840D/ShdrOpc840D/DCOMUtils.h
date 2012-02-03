// **************************************************************************
// DCOMUtils.h
//
// Description:
//
// DISCLAIMER:
// This software was developed by U.S. Government employees as part of
// their official duties and is not subject to copyright. No warranty implied 
// or intended.
// **************************************************************************

#pragma once
#include "StdStringEx.h"
#include <winsock2.h>
#include <iphlpapi.h>
#include <icmpapi.h>


//http://www.geekpage.jp/en/programming/iphlpapi/send-icmp.php
//Please note that IcmpSendEcho() can only be used with IPv4. When using ICMP with IPv6, please use Icmp6CreateFile() and Icmp6SendEcho2(). 
inline bool TestPing(std::string host)
{
	std::string tmp;

	//convert the address into a long representation
	DWORD dwAddress = inet_addr(host.c_str());

	HANDLE hIcmp;
	char *SendData = "ICMP SEND DATA";
	LPVOID ReplyBuffer;
	DWORD dwRetVal;
	DWORD buflen;
	PICMP_ECHO_REPLY pIcmpEchoReply;

	hIcmp = IcmpCreateFile();
	if(hIcmp == INVALID_HANDLE_VALUE)
		return "IcmpCreateFile Failed";

	buflen = sizeof(ICMP_ECHO_REPLY) + strlen(SendData) + 1;

	ReplyBuffer = (VOID*) malloc(buflen);
	if (ReplyBuffer == NULL) 
		return _T("Fail");
	memset(ReplyBuffer, 0, buflen);

	pIcmpEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;

	dwRetVal = IcmpSendEcho(hIcmp, 
		dwAddress,			//DestinationAddress
		SendData,			//RequestData
		strlen(SendData), //RequestSize
		NULL,		//RequestOptions
		ReplyBuffer,  //ReplyBuffer
		buflen,  //ReplySize
		1000);  // Timeout

#if 1
	delete ReplyBuffer;
	return (dwRetVal != 0) ;

#else
	if (dwRetVal != 0) 
	{
		// Reply from 127.0.0.1: bytes=32 time<1ms 
		tmp+="Ping Reply from " + host + ":" 
			+ "bytes=" + ConvertToString(pIcmpEchoReply->DataSize) 
			+ " time=" + ((pIcmpEchoReply->RoundTripTime==0) ? std::string("<1ms") :  ConvertToString(pIcmpEchoReply->RoundTripTime))
			+ "\n";

	} else 
	{
		int err = GetLastError(); // useless error message...
		tmp+="Ping Request timed out\n"; // +  ErrorFormatMessage(err);
	}
	delete ReplyBuffer;
	return tmp;
#endif
}

#ifdef UNICODE
typedef unsigned short *  WINSTRING;
#else
typedef unsigned char *   WINSTRING;
#endif
class Auth_Identity
{
public:

		SEC_WINNT_AUTH_IDENTITY idn;
		_bstr_t _strDomain, _strUserName, _strPassword;
		Auth_Identity(_bstr_t strDomain, _bstr_t strUserName, _bstr_t strPassword) :
		_strDomain(strDomain), _strUserName(strUserName), _strPassword(strPassword)
		{

			ZeroMemory(&idn, sizeof(idn));

			idn.UserLength = (unsigned long) _strUserName.length();
			idn.DomainLength = (unsigned long) _strDomain.length();
			idn.PasswordLength = (unsigned long) _strPassword.length();

			idn.User = (WINSTRING) (BSTR) _strUserName; // (unsigned short) new WCHAR[idn.UserLength + 1];
			idn.Domain = (WINSTRING)  (BSTR) _strDomain; // new WCHAR[idn.DomainLength + 1];
			idn.Password = (WINSTRING) (BSTR)  _strPassword; // new WCHAR[idn.PasswordLength + 1];

			idn.Flags = SEC_WINNT_AUTH_IDENTITY_UNICODE;
		}
		COAUTHIDENTITY* GetAuth_Identity() { return (COAUTHIDENTITY*)  &idn; }
		operator COAUTHIDENTITY* () { return (COAUTHIDENTITY*)  &idn; }
		COAUTHIDENTITY* GetNullAuth_Identity() { return NULL; }
};
class Auth_Info
{
public:
	DWORD dwAuthnSvc ;
	DWORD dwAuthzSvc ;
	LPWSTR pwszServerPrincName ;
	DWORD dwAuthnLevel;
	DWORD dwImpersonationLevel ;
	DWORD dwCapabilities ;
	COAUTHIDENTITY *pAuthIdentityData;

	Auth_Info(COAUTHIDENTITY * idn = NULL)
	{
		ZeroMemory(&athn, sizeof(athn));
		dwAuthnSvc = RPC_C_AUTHN_WINNT;
		dwAuthzSvc = RPC_C_AUTHZ_NONE;
		pwszServerPrincName = NULL;
		dwAuthnLevel = RPC_C_AUTHN_LEVEL_CONNECT;
		dwImpersonationLevel = RPC_C_IMP_LEVEL_IMPERSONATE;
		dwCapabilities = EOAC_NONE;
		pAuthIdentityData=idn;
	}
	void Build()
	{
		athn.dwAuthnSvc = dwAuthnSvc;
		athn.dwAuthzSvc = dwAuthzSvc;
		athn.pwszServerPrincName = pwszServerPrincName;
		athn.dwAuthnLevel = dwAuthnLevel;
		athn.dwImpersonationLevel = RPC_C_IMP_LEVEL_IMPERSONATE;
		athn.dwCapabilities = dwCapabilities;
		athn.pAuthIdentityData=pAuthIdentityData; // idn.GetNullAuth_Identity();
	}
	void SetComAuthority ( COAUTHIDENTITY * idn)
	{
		pAuthIdentityData=idn;

	}
	operator COAUTHINFO *() { return GetComAuth_Info(); }
	COAUTHINFO * GetComAuth_Info() 
	{ 
		Build();
		return &athn; 
	}
private:
		COAUTHINFO athn;

};

// http://msdn.microsoft.com/en-us/library/ms738563(VS.85).aspx
//htonl, htons, ntohl, ntohs functions can be used to convert between network and local byte orders.
//	CIPAddressCtrl *ip = (CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS1);
// ip->GetAddress(ipAddr.S_un.S_addr);
inline bstr_t ConvertIpAddress(struct in_addr ipAddr)
{
	USES_CONVERSION;
	// convert the network address byte to host address byte order
	ipAddr.S_un.S_addr = ntohl(ipAddr.S_un.S_addr); 
	return A2W(inet_ntoa(ipAddr));
}

class ComInitializeSecuity
{
public:

	PSECURITY_DESCRIPTOR pSecDesc;
	LONG cAuthSvc;
	SOLE_AUTHENTICATION_SERVICE *asAuthSvc;
	 void *pReserved1;
	DWORD dwAuthnLevel;
	DWORD dwImpLevel;
	 void *pAuthList;
	 DWORD dwCapabilities;
	void *pReserved3;
	ComInitializeSecuity()
	{
		pReserved1 = pReserved3 = NULL; // Immutable
		//PSECURITY_DESCRIPTOR, GUID(dwCapabilities == EOAC_APPID )  or IAccessControl (dwCapabilities ==  EOAC_ACCESS_CONTROL )
		// If pSecDesc is null, then no ACL checking will be performed.
		pSecDesc= NULL;

		//cAuthSvc, specifies the number of authentication services being registered. Zero means that no authentication services are being registered and the process will not be able to receive secure calls; 
		// a value of -1 instructs COM to choose which authentication services to register. 
		cAuthSvc=-1;
		asAuthSvc=NULL;

		 //specifies the default authentication level
		// Client applications set the dwAuthnLevel parameter to determine the default authentication level used for outgoing calls. 
		// The dwAuthnLevel setting specified in the component's call to CoInitializeSecurity becomes the minimum level at which client calls will be accepted. 
		// Any calls arriving at an authentication level below the minimum watermark specified by the component will fail.
		dwAuthnLevel=RPC_C_AUTHN_LEVEL_NONE;

		// specifies the default impersonation level for proxies that the client grants to the component. Not used on the server side.
		// Applications should set this value carefully, since, by default, all IUnknown calls are made at the impersonation level set by the client's call to CoInitializeSecurity.
		dwImpLevel=RPC_C_IMP_LEVEL_IDENTIFY;

		// list contains the default authentication information to use with each authentication service.
		// If the pAuthInfo parameter for the desired authentication service is null, COM will use the process identity to represent the client. 
		pAuthList=NULL;

		// additional client-side and server-side capabilities. This value can be composed of a combination of the values from the EOLE_AUTHENTICATION_CAPABILITIES enumeration 
		dwCapabilities=EOAC_NONE;

	}
	/* __in_opt  PSECURITY_DESCRIPTOR pSecDesc,
	__in      LONG cAuthSvc,
	__in_opt  SOLE_AUTHENTICATION_SERVICE *asAuthSvc,
	__in_opt  void *pReserved1,
	__in      DWORD dwAuthnLevel,
	__in      DWORD dwImpLevel,
	__in_opt  void *pAuthList,
	__in      DWORD dwCapabilities,
	__in_opt  void *pReserved3
	*/
	HRESULT InitializeSecurity()
	{

		return ::CoInitializeSecurity(
			pSecDesc,
			cAuthSvc,
			asAuthSvc,
			pReserved1,
			dwAuthnLevel,
			dwImpLevel,
			pAuthList,
			dwCapabilities,
			pReserved3);
	}

	void SetImpersonationLevel(DWORD _dwImpLevel)
	{
		dwImpLevel=_dwImpLevel;
		return;

		//RPC_C_IMP_LEVEL_DEFAULT
		//anonymous (RPC_C_IMP_LEVEL_ANONYMOUS)
		//The client is anonymous to the server. The server process can impersonate the client, but the impersonation token does not contain any information about the client. This level is only supported over the local interprocess communication transport. All other transports silently promote this level to identify.
		dwImpLevel=RPC_C_IMP_LEVEL_ANONYMOUS;

		//identify (RPC_C_IMP_LEVEL_IDENTIFY)
		//The system default level. The server can obtain the client's identity, and the server can impersonate the client to do ACL checks. 
		dwImpLevel=RPC_C_IMP_LEVEL_IDENTIFY;

		//impersonate (RPC_C_IMP_LEVEL_IMPERSONATE)
		//The server can impersonate the client's security context while acting on behalf of the client. The server can access local resources as the client. If the server is local, it can access network resources as the client. If the server is remote, it can access only resources that are on the same computer as the server.
		dwImpLevel=RPC_C_IMP_LEVEL_IMPERSONATE;

		//delegate (RPC_C_IMP_LEVEL_DELEGATE) ONLY IN SAME DOMAIN
	}

	
};

typedef std::map<bstr_t, int > EnumMap;
typedef std::vector<tstring> tstringVector;

class CDCOMEnums
{
public:
	std::map<bstr_t, int > _enums;
	std::vector<bstr_t> _authenums;
	tstring authn;
	tstring authz;
	tstring RPC_C_IMP_LEVEL;
	tstring RPC_C_AUTHN_LEVEL;
	tstringVector authnitems;
	tstringVector authzitems;
	tstringVector authenlevelitems;
	tstringVector implevelitems;

	CDCOMEnums()
	{

		_authenums.push_back("RPC_C_AUTHN_LEVEL_DEFAULT");
		_authenums.push_back("RPC_C_AUTHN_LEVEL_NONE");
		_authenums.push_back("RPC_C_AUTHN_LEVEL_CONNECT");
		_authenums.push_back("RPC_C_AUTHN_LEVEL_CALL");
		_authenums.push_back("RPC_C_AUTHN_LEVEL_PKT");
		_authenums.push_back("RPC_C_AUTHN_LEVEL_PKT_INTEGRITY");
		_authenums.push_back("RPC_C_AUTHN_LEVEL_PKT_PRIVACY");
		for(int i=0; i< _authenums.size(); i++)
			_enums[_authenums[i]]=      i;

		// COM Security Initialize:
		// Authentication is usually encountered when a user attempts to log on to a network 
		// or call a process on another machine. 
		_enums["RPC_C_AUTHN_NONE"]         =   0;
		_enums["RPC_C_AUTHN_DCE_PRIVATE"]   =  1;
		_enums["RPC_C_AUTHN_DCE_PUBLIC"]    =  2;
		_enums["RPC_C_AUTHN_DEC_PUBLIC"]  =    4;
		_enums["RPC_C_AUTHN_GSS_NEGOTIATE"]   =9;
		_enums["RPC_C_AUTHN_WINNT"]        =  10;
		_enums["RPC_C_AUTHN_GSS_SCHANNEL"]   =14;
		_enums["RPC_C_AUTHN_GSS_KERBEROS"]   =16;
		_enums["RPC_C_AUTHN_DPA"]       =     17;
		_enums["RPC_C_AUTHN_MSN"]       =     18;
		_enums["RPC_C_AUTHN_DIGEST"]     =    21;
		_enums["RPC_C_AUTHN_MQ"]      =      100;
		_enums["RPC_C_AUTHN_DEFAULT"]    =   0xFFFFFFFFL;

		authn="RPC_C_AUTHN_NONE,RPC_C_AUTHN_DCE_PRIVATE,RPC_C_AUTHN_DCE_PUBLIC,RPC_C_AUTHN_DEC_PUBLIC,RPC_C_AUTHN_GSS_NEGOTIATE,"
			"RPC_C_AUTHN_WINNT,RPC_C_AUTHN_GSS_SCHANNEL,RPC_C_AUTHN_GSS_KERBEROS,RPC_C_AUTHN_DPA,RPC_C_AUTHN_MSN,RPC_C_AUTHN_DIGEST,"
			"RPC_C_AUTHN_DEFAULT";
		authnitems = Tokenize(authn, ",");
	

		_enums["RPC_C_AUTHZ_NONE"]         =     0;
		_enums["RPC_C_AUTHZ_NAME"]         =     1;
		_enums["RPC_C_AUTHZ_DCE"]         =      2;
		_enums["RPC_C_AUTHZ_DEFAULT"]         =  0xffffffff;
		authz="RPC_C_AUTHZ_NONE,RPC_C_AUTHZ_NAME,RPC_C_AUTHZ_DCE,RPC_C_AUTHZ_DEFAULT";
		authzitems = Tokenize(authz, ",");

		_enums["RPC_C_AUTHN_LEVEL_DEFAULT"]=         0;
		_enums["RPC_C_AUTHN_LEVEL_NONE"]=            1;
		_enums["RPC_C_AUTHN_LEVEL_CONNECT"]=         2;
		_enums["RPC_C_AUTHN_LEVEL_CALL"]=            3;
		_enums["RPC_C_AUTHN_LEVEL_PKT"]=             4;
		_enums["RPC_C_AUTHN_LEVEL_PKT_INTEGRITY"]=   5;
		_enums["RPC_C_AUTHN_LEVEL_PKT_PRIVACY"]=     6;
		RPC_C_AUTHN_LEVEL="RPC_C_AUTHN_LEVEL_DEFAULT,RPC_C_AUTHN_LEVEL_NONE,RPC_C_AUTHN_LEVEL_CONNECT,"
			"RPC_C_AUTHN_LEVEL_CALL,RPC_C_AUTHN_LEVEL_PKT,RPC_C_AUTHN_LEVEL_PKT_INTEGRITY,RPC_C_AUTHN_LEVEL_PKT_PRIVACY";
		authenlevelitems = Tokenize(RPC_C_AUTHN_LEVEL, ",");


		_enums["RPC_C_IMP_LEVEL_DEFAULT"]=      0;
		_enums["RPC_C_IMP_LEVEL_ANONYMOUS"]=    1;
		_enums["RPC_C_IMP_LEVEL_IDENTIFY"]=     2;
		_enums["RPC_C_IMP_LEVEL_IMPERSONATE"]=  3;
		_enums["RPC_C_IMP_LEVEL_DELEGATE"]=     4;
		RPC_C_IMP_LEVEL="RPC_C_IMP_LEVEL_DEFAULT,RPC_C_IMP_LEVEL_ANONYMOUS,RPC_C_IMP_LEVEL_IDENTIFY,"
			"RPC_C_IMP_LEVEL_IMPERSONATE,RPC_C_IMP_LEVEL_DELEGATE";
		implevelitems = Tokenize(RPC_C_IMP_LEVEL, ",");


		_enums["RPC_C_QOS_IDENTITY_STATIC"]=    0;
		_enums["RPC_C_QOS_IDENTITY_DYNAMIC"]=   1;

		_enums["RPC_C_QOS_CAPABILITIES_DEFAULT"]=                        0x0;
		_enums["RPC_C_QOS_CAPABILITIES_MUTUAL_AUTH"]=                    0x1;
		_enums["RPC_C_QOS_CAPABILITIES_MAKE_FULLSIC"]=                   0x2;
		_enums["RPC_C_QOS_CAPABILITIES_ANY_AUTHORITY "]=                 0x4;


	}
	bool Find(bstr_t szEnum) { return  _enums.find(szEnum) != _enums.end() ; }
	long Value(bstr_t szEnum) 
	{ 
		EnumMap::iterator it;
		if( (it= _enums.find(szEnum)) != _enums.end()) 
			return (*it).second;
		return 0;
	}
	int Get(int n, tstringVector & items) 
	{ 
		for(int i=0; i< items.size(); i++) 
			if(Value( items[i].c_str())==n) 
				return i; 
		return 0; 
	}
};
__declspec(selectany)  CDCOMEnums DCOMEnums;
