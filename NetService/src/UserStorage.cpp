#include "UserStorage.h"

UserStorage::UserStorage()
{
}

UserStorage::~UserStorage()
{
	
}

bool UserStorage::IfUserExist(const char* pUser)
{
	return false;
}
bool UserStorage::GetPassword(const char* pUser, char* pPassWordBuff, long cbBuff)
{
	return false;
}
bool UserStorage::GetUserInfo(const char* pUser, userinfo* pUserInfo) 
{
	return false;
}