#pragma once

struct userinfo
{
	//�����кܶ��û���Ϣ
};

class UserStorage
{
public:
	UserStorage();
	~UserStorage();
	//��������ĳ���
	bool IfUserExist(const char* pUser);
	int GetPassword(const char* pUser, char* pPassWordBuff, long cbBuff);
	int GetUserInfo(const char* pUser, userinfo* pUserInfo);
private:

};

UserStorage::UserStorage()
{
}

UserStorage::~UserStorage()
{
}

bool UserStorage::IfUserExist(const char* pUser)
{
	return true;
}
int UserStorage::GetPassword(const char* pUser, char* pPassWordBuff, long cbBuff)
{
	return 0;
}
int UserStorage::GetUserInfo(const char* pUser, userinfo* pUserInfo)
{
	return 0;
}