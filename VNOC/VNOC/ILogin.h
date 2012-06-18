#pragma once
#include "IModule.h"

interface ILoginUI : public IModule
{
	STDMETHOD( SetAccount(LPCTSTR username) = 0);
	STDMETHOD( OnLoginResult(BOOL success) = 0);
};

interface ILogin : public IModule
{
public:
	STDMETHOD( Show(BOOL bShow=TRUE) = 0 );
	// 指定要显示的登陆的账户
	STDMETHOD( SetAccount(LPCTSTR userName) = 0);
	// 当前登陆情况。如果没有用户登陆，username将是空
	STDMETHOD( GetCurrentUser(CString& userName,CString& cookie) = 0);
	// 注销。目前是单用户设计，所以可以不填用户名
	STDMETHOD( Logout(LPCTSTR username=NULL) = 0);
};