#pragma once
#include "ILogin.h"
#include "../../FrameWork/FrameBase.hpp"
#include "../../net/INet.h"

class CLoginImpl
	: public ILogin
	, public CFrameBase
	, public INetListener
{
public:
	CLoginImpl(void);
	~CLoginImpl(void);
public:
	virtual ResultCode Initialize(IModule* UpperFrame=NULL);
	virtual ResultCode UnInitialize();
	virtual ResultCode Run();
	virtual ResultCode Terminate();
	virtual ResultCode SetAccount(LPCTSTR userName);
	virtual ResultCode Login(LPCTSTR username,LPCTSTR pwd);
	virtual ResultCode GetCurrentUser(CString& userName,CString& cookie);
	virtual ResultCode Logout(LPCTSTR username=NULL);

	virtual ResultCode OnNetMessage(const CMessage &msg);
protected:
	VOID OnLogin(XMessage_Login* pMsg);
	BOOL CheckLoginRequest(XMessage_Login* pMsg);

private:
	IFrameWork* m_frame;
	CNetListenerHelper netHelper;
public:
	Begin_XMessage(CLoginImpl)
		OnXMessage(XMessage_Login,OnLogin)
	End_XMessage()
};

