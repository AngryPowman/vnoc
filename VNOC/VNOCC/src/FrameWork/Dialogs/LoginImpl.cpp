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
	return S_OK;
}

HRESULT CLoginImpl::Terminate()
{
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