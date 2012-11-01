#include "stdafx.h"
#include "RoomMgr.h"

CRoomMgr::CRoomMgr() : CFrameBase(module_RoomListData)
{

}

CRoomMgr::~CRoomMgr()
{

}

HRESULT CRoomMgr::Initialize( IModule* UpperFrame/*=NULL*/ )
{
	return E_NOTIMPL;
}

HRESULT CRoomMgr::UnInitialize()
{
	return E_NOTIMPL;
}

HRESULT CRoomMgr::Run()
{
	return E_NOTIMPL;
}

HRESULT CRoomMgr::Terminate()
{
	return E_NOTIMPL;
}

HRESULT CRoomMgr::CreateRoom( RoomID& id )
{
	return E_NOTIMPL;
}

HRESULT CRoomMgr::GetRoom( RoomID id,CRoomBase* room )
{
	return E_NOTIMPL;
}

HRESULT CRoomMgr::FreeRoom( RoomID id )
{
	return E_NOTIMPL;
}

HRESULT CRoomMgr::Show( BOOL bShow/*=TRUE*/ )
{
	return S_OK;
}

HRESULT CRoomMgr::GetRoomList( RoomList& list )
{
	return E_NOTIMPL;
}

