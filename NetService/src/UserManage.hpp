#pragma once

#include "UserInfo.hpp"
#include "UserStorage.h"
#include <string>
#include <string.h>
#define NULLPOINT			-1
#define LOGIN_OK			1
#define TEST_FALSE			2


class CUserManage
{

public:
	UserStorage *_us;
	static CUserManage* GetInstance()
	{
		return &_instance;
	}
	//返回值： LOGIN_OK 登陆成功 TEST_FALSE 验证失败 NULLPOINT 指针无效
	//如果登陆成功则获得用户信息
	void initial(UserStorage *us)
	{
		_us = us;
	}
	int Authenticate(char* szUser, char* pPassword, userinfo* pUserInfo, int nPassLen = 40)
	{
		if (szUser == NULL)
		{
			return TEST_FALSE;
		}
        strncpy(pUserInfo->strUser, szUser, 40);
        return LOGIN_OK;
		if ( !_us->IfUserExist(szUser) )//账号是否存在 查数据库
		{
			return TEST_FALSE;
		}
		char strPass[41] = {0};	
		_us->GetPassword(szUser, strPass, 40);

		int i = 0; //密码验证
		do
		{
			if (strPass[i] != pPassword[i])
			{
				return TEST_FALSE;
			}
			++i;
		} while (i < nPassLen);

		if (pUserInfo == 0)
		{
			return NULLPOINT;
		}

		//获得用户信息
		_us->GetUserInfo(szUser, pUserInfo);

		return LOGIN_OK;
	}	
private:
	static CUserManage _instance;
};

