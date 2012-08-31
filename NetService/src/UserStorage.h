#pragma once
#include "UserInfo.hpp"




class UserStorage
{
public:
	UserStorage();
	~UserStorage();
	virtual bool IfUserExist(const char* pUser);
	virtual bool GetPassword(const char* pUser, char* pPassWordBuff, long cbBuff) ;
	virtual bool GetUserInfo(const char* pUser, userinfo* pUserInfo) ;
private:

};