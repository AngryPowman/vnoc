#include "FileUserStorage.h"

fUserStorage::fUserStorage()
{
}

fUserStorage::~fUserStorage()
{
}



bool fUserStorage::IfUserExist(const char* pUser)
{
	for (int i = 0; i < nUserCount; i++)
	{
		if ( strcmp(pUserTable[i].strUser, pUser) == 0)
		{
			return true;
		}
	}
	return false;
}
bool fUserStorage::GetPassword(const char* pUser, char* pPassWordBuff, size_t cbBuff)
{
	for (int i = 0; i < nUserCount; i++)
	{
		if ( strcmp(pUserTable[i].strUser, pUser) == 0)
		{
			memcpy(pPassWordBuff, pUserTable[i].passwordData, cbBuff);
			return true;
		}
	}
	return false;
}
bool fUserStorage::GetUserInfo(const char* pUser, userinfo* pUserInfo)
{
	userinfo *p = pUserTable;
	for (int i = 0; i < nUserCount; i++)
	{
		if ( strcmp(pUserTable[i].strUser, pUser) == 0)
		{
			memcpy(pUserInfo, p, sizeof(userinfo));
			return true;
		}
		++p;
	}
	return false;
}