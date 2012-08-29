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
	// ָ��Ҫ��ʾ�ĵ�½���˻�
	STDMETHOD( SetAccount(LPCTSTR userName) = 0);
	// ��ǰ��½��������û���û���½��username���ǿ�
	STDMETHOD( GetCurrentUser(CString& userName,CString& cookie) = 0);
	// ע����Ŀǰ�ǵ��û���ƣ����Կ��Բ����û���
	STDMETHOD( Logout(LPCTSTR username=NULL) = 0);
};