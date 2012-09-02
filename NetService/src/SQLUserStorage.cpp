#include "SQLUserStorage.h"
#include "Config.hpp"
#include <ezlogger_headers.hpp>
#include <iostream>
#include <string.h>
void sUserStorage ::SaveConnLog(string host,string name,string password,string db,int port /*= 3306*/ )
{
	EZLOGGERSTREAM<<"MySQL host :"<<host<<std::endl;
	EZLOGGERSTREAM<<"MySQL name :"<<name<<std::endl;
	EZLOGGERSTREAM<<"MySQL password :"<<password<<std::endl;
	EZLOGGERSTREAM<<"MySQL db :"<<db<<std::endl;
	EZLOGGERSTREAM<<"MySQL port :"<<port<<std::endl;
}
sUserStorage ::sUserStorage ()
{
	string host = Config::getInstance()->getValue("sql_host");
	string name = Config::getInstance()->getValue("sql_username");
	string password = Config::getInstance()->getValue("sql_password");
	string db = Config::getInstance()->getValue("user_db");
	int port = Config::getInstance()->getValue("sql_port");

	SaveConnLog(host, name, password, db, port);

	if (conn.Connect(host, name, password, db, port) == MW_SQL_ERR)
	{
		bIsConn = false;
//		EZLOGGERSTREAM<<"Connet MySQL Data Base failure"<<std::endl;
		return;
	}
	bIsConn = true;
}

sUserStorage ::~sUserStorage ()
{
	conn.Shutdown();
}

bool sUserStorage::IfUserExist(const char* pUser)
{
	if (!bIsConn)
	{
		return false;
	}

	char SQL[100] = "select * from vnoc.user_info where user='";
	strcat(SQL, pUser);
	strcat(SQL, "'");
	MWCommand com(&conn, SQL);
	
	com.Execute();
	com.FetchNext();
	if (com.GetLine_Num() == 0)
	{
		return false;
	}
	return true;
}
bool sUserStorage::GetPassword(const char* pUser, char* pPassWordBuff, size_t cbBuff)
{

	if (!bIsConn)
	{
		return false;
	}

	char SQL[100] = "select passwd from vnoc.user_info where user='";
	strcat(SQL, pUser);
	strcat(SQL, "'");
	MWCommand com(&conn, SQL);
	com.Execute();
	com.FetchNext();
	if (com.GetField_Num() == 0)
	{
		return false;
	}

	const char *p = (com.GetOneData()->asString());
	if (strlen(p) > cbBuff)
	{
		return false;
	}
	strcpy(pPassWordBuff, p);

	/*int a=0,b=0;
	for (int i = 0; i < 40; i+=2)
	{
		if ( p[i] >= 'A' && p[i]<='F' )
		{
			a = p[i] - 'A' +10;
		}else if (p[i] >= '0' && p[i]<='9' )
		{
			a = p[i] - '0' ;
		}
		else if ( p[i] >= 'a' && p[i]<='f' )
		{
			a = p[i] - 'a' + 10;
		}
		if ( p[i+1] >= 'A' && p[i+1]<='F' )
		{
			b = p[i+1] - 'A' +10;
		}else if (p[i+1] >= '0' && p[i+1]<='9' )
		{
			b = p[i+1] - '0' ;
		}
		else if ( p[i+1] >= 'a' && p[i+1]<='f' )
		{
			b = p[i+1] - 'a' + 10;
		}

		pPassWordBuff[i/2] = a*16 + b;
	}*/
	return true;
}
bool sUserStorage::GetUserInfo(const char* pUser, userinfo* pUserInfo)
{

	if (!bIsConn)
	{
		return false;
	}
	char SQL[100] = "select passwd from vnoc.user_info where user='";
	strcat(SQL, pUser);
	strcat(SQL, "'");
	MWCommand com(&conn, SQL);
	com.Execute();
	com.FetchNext();
	if (com.GetField_Num() == 0)
	{
		return false;
	}
	
	strcpy(pUserInfo->strUser, pUser);

	int a=0,b=0;

	const char* p = com.GetOneData()->asString();
	for (int i = 0; i < 40; i+=2)
	{
		if ( p[i] >= 'A' && p[i]<='F' )
		{
			a = p[i] - 'A' +10;
		}else if (p[i] >= '0' && p[i]<='9' )
		{
			a = p[i] - '0' ;
		}
		else if ( p[i] >= 'a' && p[i]<='f' )
		{
			a = p[i] - 'a' + 10;
		}
		if ( p[i+1] >= 'A' && p[i+1]<='F' )
		{
			b = p[i+1] - 'A' +10;
		}else if (p[i+1] >= '0' && p[i+1]<='9' )
		{
			b = p[i+1] - '0' ;
		}
		else if ( p[i+1] >= 'a' && p[i+1]<='f' )
		{
			b = p[i+1] - 'a' + 10;
		}

		pUserInfo->passwordData[i/2] = a*16 + b;
	}
	

	char SQL1[100] = "select nickname from vnoc.user_info where user='";
	strcat(SQL1, pUser);
	strcat(SQL1, "'");
	MWCommand com1(&conn, SQL1);
	com1.Execute();
	com1.FetchNext();
	strcpy(pUserInfo->strNickname, com1.GetOneData()->asString());

	char SQL2[100] = "select email from vnoc.user_info where user='";
	strcat(SQL2, pUser);
	strcat(SQL2, "'");
	MWCommand com2(&conn, SQL2);
	com2.Execute();
	com2.FetchNext();
	strcpy(pUserInfo->strEmailAddr, com2.GetOneData()->asString());

	char SQL3[100] = "select gender from vnoc.user_info where user='";
	strcat(SQL3, pUser);
	strcat(SQL3, "'");
	MWCommand com3(&conn, SQL3);
	com3.Execute();
	com3.FetchNext();
	pUserInfo->gender = com3.GetOneData()->asInt();

	char SQL4[100] = "select old from vnoc.user_info where user='";
	strcat(SQL4, pUser);
	strcat(SQL4, "'");
	MWCommand com4(&conn, SQL4);
	com4.Execute();
	com4.FetchNext();
	pUserInfo->uOld = com4.GetOneData()->asInt();
	return true;
}