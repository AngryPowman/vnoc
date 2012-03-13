#include "StdAfx.h"
#include "RoomListImpl.h"


CRoomListImpl::CRoomListImpl(void)
{
}


CRoomListImpl::~CRoomListImpl(void)
{
}

HRESULT CRoomListImpl::Initialize( IModule* UpperFrame/*=NULL*/ )
{
	m_frame = dynamic_cast<IVNOCFrame*>(UpperFrame);
	ATLASSERT(m_frame);

	m_dlg = new CRoomListDlg;
	ATLASSERT(m_dlg);
	return S_OK;
}

HRESULT CRoomListImpl::UnInitialize()
{
	if (m_dlg)
	{
		delete m_dlg;
		m_dlg = NULL;
	}
	return S_OK;
}

HRESULT CRoomListImpl::Run()
{
	return S_OK;
}

HRESULT CRoomListImpl::Terminate()
{
	return S_OK;
}

HRESULT CRoomListImpl::Show( BOOL bShow/*=TRUE*/ )
{
	if (m_dlg && bShow)
	{
		INT_PTR ret = m_dlg->DoModal();
		if (ret == IDOK)
		{
		}
		else if(ret == IDCANCEL)
		{
			;
		}
	}
	return S_OK;
}
