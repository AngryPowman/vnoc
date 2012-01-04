
#ifndef VNOC_ANALYSIS
#define VNOC_ANALYSIS

#define MAX_ACCOUNT  256
#define MAX_VERLEN   12
#define MAX_CONTROL  256
#define MAX_REMSGLEN   25

#include "tinyxml.h"
#include <tchar.h>

class Login
{
public:
	Login(){
		_Name[0] = _T('\0');
		_Password[0] = _T('\0');
	}
public:
	TCHAR _Name[MAX_ACCOUNT];
	TCHAR _Password[MAX_ACCOUNT];
};


class ReturnMessage
{
public:
	ReturnMessage():MsgID(0),MsgType(0){}

public:
	int MsgType;
	int MsgID;
};

class Version
{
public:
	Version()
	{
		_ver[0] = _T('\0');
	}

public:
    TCHAR _ver[MAX_VERLEN];
};

class Update
{
public:
	Update(){
		_ver[0] = _T('\0');
		Control[0] = _T('\0');
	}

public:
	TCHAR _ver[MAX_VERLEN];
	TCHAR Control[MAX_CONTROL];
};

class XmlAnalysis
{
public:
	XmlAnalysis(){};

	XmlAnalysis(LPCTSTR lpszStr,Login* login){ 
		this->Resolve(lpszStr,login);
	}
	XmlAnalysis(LPCTSTR lpszStr,ReturnMessage* retmessage){
		this->Resolve(lpszStr,retmessage);
	}	
	XmlAnalysis(LPCTSTR lpszStr,Version* version){
		this->Resolve(lpszStr,version);
	}
	XmlAnalysis(LPCTSTR lpszStr,Update* update){
		this->Resolve(lpszStr,update);
	}

	~XmlAnalysis(){

	}

	bool Resolve(LPCTSTR lpszStr, Login* login);
	bool Resolve(LPCTSTR lpszStr, ReturnMessage* retmessage);
	bool Resolve(LPCTSTR lpszStr, Version* version);
	bool Resolve(LPCTSTR lpszStr, Update* update);


protected:
	TiXmlElement* TiXml;
};

bool XmlAnalysis::Resolve(LPCTSTR lpszStr, Login* login)
{
	if (!lpszStr||!login)
	{
		return false;
	}
	if (_tcscmp(lpszStr,"") == 0)
	{
		return false;
	}
	memset(login->_Name,0,MAX_ACCOUNT);
	memset(login->_Password,0,MAX_ACCOUNT);

	TiXml = new TiXmlElement("Login");

	TiXml->Parse(lpszStr,0,TIXML_ENCODING_UNKNOWN);
	_tcscpy_s(login->_Name,_tcslen(TiXml->Attribute("Name")) + 1,TiXml->Attribute("Name"));
	_tcscpy_s(login->_Password,_tcslen(TiXml->Attribute("Password")) + 1,TiXml->Attribute("Password"));
	if ((_tcscmp(login->_Name,"") == 0 ) || (_tcscmp(login->_Password,"") == 0 ))
	{
		return false;
	}
	if (TiXml != NULL)
	{
		delete TiXml;
	}
	return true;
}

bool XmlAnalysis::Resolve(LPCTSTR lpszStr, Version* version)
{
	if (!lpszStr||!version)
	{
		return false;
	}
	if (_tcscmp(lpszStr,"") == 0)
	{
		return false;
	}

	memset(version->_ver,0,MAX_VERLEN);

	TiXml = new TiXmlElement("Version");

	TiXml->Parse(lpszStr,0,TIXML_ENCODING_UNKNOWN);
	_tcscpy_s(version->_ver,_tcslen(TiXml->Attribute("ver")) + 1,TiXml->Attribute("ver"));
	if (_tcscmp(version->_ver,"") == 0 )
	{
		return false;
	}
	if (TiXml != NULL)
	{
		delete TiXml;
	}
	return true;
}

bool XmlAnalysis::Resolve(LPCTSTR lpszStr, ReturnMessage* retmessage)
{
	if (!lpszStr||!retmessage)
	{
		return false;
	}
	if (_tcscmp(lpszStr,"") == 0)
	{
		return false;
	}
	
	retmessage->MsgID = 0;
	retmessage->MsgType = 0;

	TiXml = new TiXmlElement("ReturnMessage");

	TCHAR szMsgID[MAX_REMSGLEN] = {0};
	TCHAR szMsgType[MAX_REMSGLEN] = {0};
	
	TiXml->Parse(lpszStr,0,TIXML_ENCODING_UNKNOWN);
	_tcscpy_s(szMsgID,_tcslen(TiXml->Attribute("MsgID")) + 1,TiXml->Attribute("MsgID"));
	_tcscpy_s(szMsgType,_tcslen(TiXml->Attribute("MsgType")) + 1,TiXml->Attribute("MsgType"));

	if ((_tcscmp(szMsgID,"") == 0 ) || (_tcscmp(szMsgType,"") == 0 ))
	{
		return false;
	}
	
	retmessage->MsgID = _ttoi(szMsgID);
	retmessage->MsgType = _ttoi(szMsgType);

	if (TiXml != NULL)
	{
		delete TiXml;
	}
	return true;
}

bool XmlAnalysis::Resolve(LPCTSTR lpszStr, Update* update)
{
	if (!lpszStr||!update)
	{
		return false;
	}
	if (_tcscmp(lpszStr,"") == 0)
	{
		return false;
	}

	memset(update->_ver,0,MAX_VERLEN);
	memset(update->Control,0,MAX_CONTROL);

	TiXml = new TiXmlElement("Update");

	TiXml->Parse(lpszStr,0,TIXML_ENCODING_UNKNOWN);
	_tcscpy_s(update->_ver,_tcslen(TiXml->Attribute("version")) + 1,TiXml->Attribute("version"));
	_tcscpy_s(update->Control,_tcslen(TiXml->Attribute("Control")) + 1,TiXml->Attribute("Control"));
	
	if ((_tcscmp(update->_ver,"") == 0 ) || (_tcscmp(update->Control,"") == 0 ))
	{
		return false;
	}

	if (TiXml != NULL)
	{
		delete TiXml;
	}
	return true;
}

#endif