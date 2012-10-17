#pragma once
#include "UserStorage.h"
#include "mysql/MysqlWrapper.h"
#pragma comment(lib, "../lib/libmysql.lib")

class sUserStorage :public UserStorage
{
public:
	sUserStorage ();
	~sUserStorage ();
	bool IfUserExist(const char* pUser);
	bool GetPassword(const char* pUser, char* pPassWordBuff, size_t cbBuff);
	bool GetUserInfo(const char* pUser, userinfo* pUserInfo);
private:
	void SaveConnLog(string host,string name,string password,string db,int port /*= 3306*/ );
	MWConnection conn;
	bool bIsConn;
};

