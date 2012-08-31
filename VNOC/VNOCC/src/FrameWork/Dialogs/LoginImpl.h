#pragma once
#include "ILogin.h"
#include "../FrameBase.hpp"
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
	STDMETHOD( Initialize(IModule* UpperFrame=NULL) );
	STDMETHOD( UnInitialize() );
	STDMETHOD( Run() );
	STDMETHOD( Terminate() );
	STDMETHOD( Show(BOOL bShow=TRUE) );
	STDMETHOD( SetAccount(LPCTSTR userName) );
	STDMETHOD( Login(LPCTSTR username,LPCTSTR pwd) );
	STDMETHOD( GetCurrentUser(CString& userName,CString& cookie) );
	STDMETHOD( Logout(LPCTSTR username=NULL) );

	STDMETHOD( OnNetMessage(const CMessage& msg));
protected:
	BOOL OnLogin(XMessage* pMsg);

private:
	IFrameWork* m_frame;
	CNetListenerHelper netHelper;
public:
	Begin_XMessage(CLoginImpl)
		OnXMessage(XMessageID_Login,OnLogin)
	End_XMessage()
};

