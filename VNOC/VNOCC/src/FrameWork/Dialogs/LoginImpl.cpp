#include "StdAfx.h"
#include "LoginImpl.h"
#include "BKWinLogin.h"

CLoginImpl::CLoginImpl(void)
{
	m_frame = NULL;
}

CLoginImpl::~CLoginImpl(void)
{
}

HRESULT CLoginImpl::Run()
{
	netHelper.AddFilter(MSG_ALI_TYPE,this);
	netHelper.StartListen();
	return S_OK;
}

HRESULT CLoginImpl::Terminate()
{
	netHelper.StopListen();
	return S_OK;
}

HRESULT CLoginImpl::Show( BOOL bShow/*=TRUE*/ )
{
	if (bShow)
	{
		CLoginWnd wnd;
		wnd.Load(BKDlg_LoginWin);
		wnd.DoModal();
	}
	return S_OK;
}

HRESULT CLoginImpl::SetAccount( LPCTSTR userName )
{
	return S_OK;
}

HRESULT CLoginImpl::Initialize( IModule* UpperFrame )
{
	m_frame = dynamic_cast<IFrameWork*>(UpperFrame);
	ATLASSERT(m_frame);
	return S_OK;
}

HRESULT CLoginImpl::UnInitialize()
{
	return S_OK;
}

HRESULT CLoginImpl::Logout( LPCTSTR username/*=NULL*/ )
{
	return S_OK;
}

HRESULT CLoginImpl::GetCurrentUser(CString& username,CString& cookie)
{
	return E_NOTIMPL;
}

BOOL CLoginImpl::OnLogin( XMessage* pMsg )
{
	XMessage_Login* pData = dynamic_cast<XMessage_Login*>(pMsg);
	if (pData)
	{
		Login(pData->username,pData->pwd);
		return TRUE;
	}
	return FALSE;
}

HRESULT CLoginImpl::Login( LPCTSTR username,LPCTSTR pwd )
{
	INetCenter* pNetCenter = NULL;
	Global->GetINetCenter(&pNetCenter);
	if (pNetCenter)
	{
		MSG_RLI netMsg;
		netMsg.SetAccountNumber((const byte*)username,_tcslen(username)*sizeof(TCHAR));
		netMsg.SetPassword((const byte*)pwd,_tcslen(pwd)*sizeof(TCHAR));
		pNetCenter->SendServer(netMsg);
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CLoginImpl::OnNetMessage( const CMessage& msg )
{
	switch(msg.GetMessageType())
	{
	case MSG_ALI_TYPE:
		MessageBox(0,_T(""),0,0);
		break;
	}
	return S_OK;
}

