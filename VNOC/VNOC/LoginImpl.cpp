#include "StdAfx.h"
#include "LoginImpl.h"
#include "IRoomList.h"

CLoginImpl::CLoginImpl(void)
{
	m_dlg = NULL;
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
	if (m_dlg && bShow)
	{
		INT_PTR ret = m_dlg->DoModal();
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

	m_dlg = new CVNOCLoginDlg;
	ATLASSERT(m_dlg);
	return S_OK;
}

HRESULT CLoginImpl::UnInitialize()
{
	if (m_dlg)
	{
		delete m_dlg;
		m_dlg = NULL;
	}
	return S_OK;
}
