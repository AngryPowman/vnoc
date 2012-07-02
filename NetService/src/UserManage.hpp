#pragma once

#include "UserStorage.hpp"
#include <string>
#define NULLPOINT			-1
#define LOGIN_OK			1
#define TEST_FALSE			2

using namespace std;

class CUserManage
{
private:
	UserStorage m_us;
public:
	CUserManage(UserStorage _UserStorage)
	{
		m_us = _UserStorage;
	}
	//����ֵ�� LOGIN_OK ��½�ɹ� TEST_FALSE ��֤ʧ�� NULLPOINT ָ����Ч
	//�����½�ɹ������û���Ϣ
	int Authenticate(string sUser, char* pPassword, userinfo* pUserInfo, int nPassLen = 20)
{
	char strPass[20] = {0};
	m_us.GetPassword(sUser.c_str(), strPass, 20);

	if ( !m_us.IfUserExist(sUser.c_str()) )//�˺��Ƿ���� �����ݿ�
	{
		return TEST_FALSE;
	}
	
	int i = 0; //������֤
	do
	{
		if (strPass[i] != pPassword[i])
		{
			return TEST_FALSE;
		}
		++i;
	} while (i < nPassLen);

	if ((int)pUserInfo == 0)
	{
		return NULLPOINT;
	}

	//����û���Ϣ
	m_us.GetUserInfo(sUser.c_str(), pUserInfo);

	return LOGIN_OK;
}

};

