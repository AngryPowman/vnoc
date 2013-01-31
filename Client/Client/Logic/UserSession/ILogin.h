#pragma once
#include "IModule.h"

interface ILogin : public IModule
{
public:
	// ָ��Ҫ��ʾ�ĵ�½���˻�
	virtual ResultCode SetAccount(LPCTSTR userName)=0;
	// ��ǰ��½��������û���û���½��username���ǿ�
	virtual ResultCode GetCurrentUser(CString& userName,CString& cookie)=0;
	// ��½
	virtual ResultCode Login(LPCTSTR username,LPCTSTR pwd)=0;
	// ע����Ŀǰ�ǵ��û���ƣ����Կ��Բ����û���
	virtual ResultCode Logout(LPCTSTR username=NULL)=0;
};