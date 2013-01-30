#pragma once
#include "../../Base/IModule.h"

interface ILogin : public IModule
{
public:
	// ָ��Ҫ��ʾ�ĵ�½���˻�
	STDMETHOD( SetAccount(LPCTSTR userName) = 0);
	// ��ǰ��½��������û���û���½��username���ǿ�
	STDMETHOD( GetCurrentUser(CString& userName,CString& cookie) = 0);
	// ��½
	STDMETHOD( Login(LPCTSTR username,LPCTSTR pwd) = 0);
	// ע����Ŀǰ�ǵ��û���ƣ����Կ��Բ����û���
	STDMETHOD( Logout(LPCTSTR username=NULL) = 0);
};