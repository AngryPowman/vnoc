#include "StdAfx.h"
#include <cstdlib>
#include "LoginImpl.h"

CLoginImpl::CLoginImpl(void) : CFrameBase(module_LoginData)
{
	m_frame = NULL;
}

CLoginImpl::~CLoginImpl(void)
{
}

ResultCode CLoginImpl::Run()
{
	netHelper.AddFilter(MSG_AnswerLogin_Id,this);
	netHelper.StartListen();
	return Result_Success;
}

ResultCode CLoginImpl::Terminate()
{
	netHelper.StopListen();
	return Result_Success;
}

ResultCode CLoginImpl::SetAccount( LPCTSTR userName )
{
	return Result_Success;
}

ResultCode CLoginImpl::Initialize( IModule* UpperFrame )
{
	m_frame = dynamic_cast<IFrameWork*>(UpperFrame);
	ATLASSERT(m_frame);
	return Result_Success;
}

ResultCode CLoginImpl::UnInitialize()
{
	return Result_Success;
}

ResultCode CLoginImpl::Logout( LPCTSTR username/*=NULL*/ )
{
	return Result_Success;
}

ResultCode CLoginImpl::GetCurrentUser(CString& username,CString& cookie)
{
	return Result_NotImpl;
}

VOID CLoginImpl::OnLogin( XMessage_Login* pMsg )
{
	if (CheckLoginRequest(pMsg))
	{
		Login(pMsg->username,pMsg->pwd);
	}
}

BOOL CLoginImpl::CheckLoginRequest( XMessage_Login* pMsg )
{
	CString& userName = pMsg->username;
	CString& pwd = pMsg->pwd;
	XMessage_LoginError error;
	if (userName.GetLength() == 0)
	{
		error.result = Result_UserName_LengthError;
	}
	else if (userName.Find(_T("sparta")))
	{
		error.result = Result_UserName_InvalidChar;
	}
	else if (pwd.GetLength() == 0)
	{
		error.result = Result_Pwd_LengthError;
	}

	SendXMessage(&error);
	return ResultSucceeded(error.result);
}

ResultCode CLoginImpl::Login( LPCTSTR username,LPCTSTR pwd )
{
	INetCenter* pNetCenter = NULL;
	Global->GetINetCenter(&pNetCenter);
	if (pNetCenter)
	{
		MSG_RequestLogin netMsg;
        char buffer[30];
        wcstombs(buffer, username, sizeof(buffer));
		netMsg.SetAccountNumber(buffer);
        wcstombs(buffer, pwd, sizeof(buffer));
		netMsg.SetPassword(buffer);
		pNetCenter->SendServer(netMsg);
		return Result_Success;
	}
	return Result_Fail;
}

ResultCode CLoginImpl::OnNetMessage( const CMessage &msg )
{
	switch(msg.MsgId())
	{
	case MSG_AnswerLogin_Id:
		MSG_AnswerLogin msgReal(msg);
		XMessage_Login_Result loginResult;
        uint8 result;
        msgReal.GetLoginResult(result);
        loginResult.success = !result;
		msgReal.GetToken(loginResult.userToken);
		msgReal.GetATLGUID(loginResult.guid);
		SendXMessage(&loginResult);
		break;
	}
	return Result_Success;
}

