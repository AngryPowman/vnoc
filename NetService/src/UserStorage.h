#pragma once
#include "UserInfo.hpp"
#include <string.h>




class UserStorage
{
public:
	UserStorage();
	~UserStorage();
	virtual bool IfUserExist(const char* pUser);
	virtual bool GetPassword(const char* pUser, char* pPassWordBuff, size_t cbBuff) ;
	virtual bool GetUserInfo(const char* pUser, userinfo* pUserInfo) ;
private:

};