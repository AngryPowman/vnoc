#pragma once
#include <fstream>
#include "UserStorage.h"


class fUserStorage  :public UserStorage

{
public:
	fUserStorage();
	~fUserStorage();
		//返回密码的长度
	bool IfUserExist(const char* pUser);
	bool GetPassword(const char* pUser, char* pPassWordBuff, size_t cbBuff);
	bool GetUserInfo(const char* pUser, userinfo* pUserInfo);

private:
		int nUserCount;
		userinfo *pUserTable;
};

