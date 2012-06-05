#include "StdAfx.h"
#include "RoomImpl.h"
#include "IRoomList.h"

CRoomImpl::CRoomImpl(void)
{
}


CRoomImpl::~CRoomImpl(void)
{
}

HRESULT CRoomImpl::Initialize( IModule* UpperFrame/*=NULL*/ )
{
	m_frame = dynamic_cast<IVNOCFrame*>(UpperFrame);
	ATLASSERT(m_frame);
	return S_OK;
}

HRESULT CRoomImpl::UnInitialize()
{
	return S_OK;
}

HRESULT CRoomImpl::Run()
{
	return S_OK;
}

HRESULT CRoomImpl::Terminate()
{
	return S_OK;
}

HRESULT CRoomImpl::Show( BOOL bShow/*=TRUE*/ )
{
	if (bShow)
	{
		CRoomDlg dlg;
		INT_PTR ret = dlg.DoModal();
		if (ret == IDOK)
		{
			return S_OK;
		}
		else if(ret == IDCANCEL)
		{
			return S_FALSE;
		}
	}
	return S_OK;
}
