#include "StdAfx.h"
#include "RoomListImpl.h"
#include "IVNOCFrame.h"
#include "IRoom.h"
#include "ILogin.h"

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
	return S_OK;
}

HRESULT CRoomListImpl::UnInitialize()
{
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
	if (bShow)
	{
		HRESULT hr = E_FAIL;
		while (FAILED(hr))
		{
			CRoomListDlg dlg;
			INT_PTR ret = dlg.DoModal();
			if (ret == IDOK)
			{
				IRoom* pRoom;
				GetiModule(module_room,m_frame,&pRoom);
				if (pRoom)
				{
					hr = pRoom->Show();
				}
			}
			else
			{
				break;
			}
		}
	}
	return S_OK;
}
