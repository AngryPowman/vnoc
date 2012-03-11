#include "StdAfx.h"
#include "LoginImpl.h"


CLoginImpl::CLoginImpl(void)
{
}


CLoginImpl::~CLoginImpl(void)
{
}

HRESULT CLoginImpl::Run()
{
	m_dlg = new CVNOCLoginDlg;
	ATLASSERT(m_dlg);
	INT_PTR ret = m_dlg->DoModal();
	if (ret == IDOK)
	{
		MessageBox(NULL,L"未实现剩下的模块",0,0);
	}
	return S_OK;
}

HRESULT CLoginImpl::Terminate()
{
	if (m_dlg)
	{
		delete m_dlg;
		m_dlg = NULL;
	}
	return S_OK;
}

HRESULT CLoginImpl::Show( BOOL bShow/*=TRUE*/ )
{
	return S_OK;
}

HRESULT CLoginImpl::SetAccount( LPCTSTR userName )
{
	return S_OK;
}

HRESULT CLoginImpl::Initialize( IUnknown* UpperFrame )
{
	return S_OK;
}

HRESULT CLoginImpl::UnInitialize()
{
	return S_OK;
}
