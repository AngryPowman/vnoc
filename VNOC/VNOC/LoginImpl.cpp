#include "StdAfx.h"
#include "LoginImpl.h"
#include "IRoomList.h"

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
		CVNOCLoginDlg dlg;
		INT_PTR ret = dlg.DoModal();
		if (ret == IDOK)
		{
			IRoomList* pRoomList;
			GetiModule(module_roomList,m_frame,&pRoomList);
			if (pRoomList)
			{
				pRoomList->Show();
			}
		}
	}
	return S_OK;
}

HRESULT CLoginImpl::SetAccount( LPCTSTR userName )
{
	return S_OK;
}

HRESULT CLoginImpl::Initialize( IModule* UpperFrame )
{
	m_frame = dynamic_cast<IVNOCFrame*>(UpperFrame);
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