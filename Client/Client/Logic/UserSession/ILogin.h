#pragma once
#include "IModule.h"

interface ILogin : public IModule
{
public:
	// 指定要显示的登陆的账户
	virtual ResultCode SetAccount(LPCTSTR userName)=0;
	// 当前登陆情况。如果没有用户登陆，username将是空
	virtual ResultCode GetCurrentUser(CString& userName,CString& cookie)=0;
	// 登陆
	virtual ResultCode Login(LPCTSTR username,LPCTSTR pwd)=0;
	// 注销。目前是单用户设计，所以可以不填用户名
	virtual ResultCode Logout(LPCTSTR username=NULL)=0;
};