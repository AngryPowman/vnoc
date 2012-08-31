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
	netHelper.AddFilter(MSG_RLI_TYPE,this);
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
	return S_OK;
}

HRESULT CLoginImpl::OnNetMessage( const CMessage& msg )
{
	switch(msg.GetMessageType())
	{
	case MSG_RLI_TYPE:
		break;
	}
	return S_OK;
}

