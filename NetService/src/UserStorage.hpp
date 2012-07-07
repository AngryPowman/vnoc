#include <fstream>
#include "UserInfo.hpp"
using namespace std;
class UserStorage
{
public:
	UserStorage();
	~UserStorage();
	//返回密码的长度
	bool IfUserExist(const char* pUser);
	bool GetPassword(const char* pUser, char* pPassWordBuff, long cbBuff);
	bool GetUserInfo(const char* pUser, userinfo* pUserInfo);
private:
	int nUserCount;
	userinfo *pUserTable;
};

UserStorage::UserStorage()
{
	ifstream file;
	streampos nSize;
	file.open("userinfo", ios_base::in );
	if (file.is_open())
	{
		file.seekg(0, ios::end);
		nSize = file.tellg();
		nUserCount = nSize/148;
		pUserTable = new userinfo[nUserCount];
		file.seekg(0, ios::beg);
		file.read((char *)pUserTable, nSize);
	}
	file.close();
}

UserStorage::~UserStorage()
{
	delete pUserTable;
}

bool UserStorage::IfUserExist(const char* pUser)
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
bool UserStorage::GetPassword(const char* pUser, char* pPassWordBuff, long cbBuff)
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
bool UserStorage::GetUserInfo(const char* pUser, userinfo* pUserInfo)
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