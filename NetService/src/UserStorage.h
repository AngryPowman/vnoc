#pragma once
#include "UserInfo.hpp"




class UserStorage
{
public:
	UserStorage();
	~UserStorage();
	virtual bool IfUserExist(const char* pUser) = 0;
	virtual bool GetPassword(const char* pUser, char* pPassWordBuff, long cbBuff) = 0;
	virtual bool GetUserInfo(const char* pUser, userinfo* pUserInfo) = 0;
private:

};

