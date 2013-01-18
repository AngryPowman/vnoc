#include "stdafx.h"
#include "RoomMgr.h"

CRoomMgr::CRoomMgr() : CFrameBase(module_RoomListData)
{
    m_frame = NULL;
}

CRoomMgr::~CRoomMgr()
{

}

HRESULT CRoomMgr::Initialize( IModule* UpperFrame/*=NULL*/ )
{
    m_frame = dynamic_cast<IFrameWork*>(UpperFrame);
    ATLASSERT(m_frame);
    m_frame->AddRef();
	return S_OK;
}

HRESULT CRoomMgr::UnInitialize()
{
	if(m_frame)
    {
        m_frame->Release();
    }
    return S_OK;
}

HRESULT CRoomMgr::Run()
{
	netHelper.AddFilter(MSG_ACL_TYPE, this);
    netHelper.StartListen();
    return S_OK;
}

HRESULT CRoomMgr::Terminate()
{
	netHelper.StopListen();
    return S_OK;
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

HRESULT CRoomMgr::GetRoomList()
{
    INetCenter *pNetCenter = NULL;
    Global->GetINetCenter(&pNetCenter);
    if(pNetCenter)
    {
        MSG_RCL netMsg;
        pNetCenter->SendServer(netMsg);
        return S_OK;
    }
	return E_FAIL;
}

VOID CRoomMgr::OnGetRoomList( XMessage *pMsg )
{
    GetRoomList();
}

HRESULT CRoomMgr::OnNetMessage( const CMessage& msg )
{
    switch(msg.GetMessageType())
    {
    case MSG_ACL_TYPE:
        const MSG_ACL* msgReal = dynamic_cast<const MSG_ACL*>(&msg);
        if(msgReal)
        {
            XMessage_ShowRoomList_Result result;
            msgReal->GetRoomList(result.roomID);
        }
    }
    return S_OK;
}

