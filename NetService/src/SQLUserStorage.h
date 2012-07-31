#include "UserStorage.h"
#include "mysql\MysqlWrapper.h"
#pragma comment(lib, "../lib/libmysql.lib")

class sUserStorage :public UserStorage
{
public:
	sUserStorage ();
	~sUserStorage ();
	bool IfUserExist(const char* pUser);
	bool GetPassword(const char* pUser, char* pPassWordBuff, long cbBuff);
	bool GetUserInfo(const char* pUser, userinfo* pUserInfo);
private:
	MWConnection conn;
	bool bIsConn;
};

