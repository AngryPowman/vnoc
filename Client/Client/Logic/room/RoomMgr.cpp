#include "stdafx.h"
#include "RoomMgr.h"

CRoomMgr::CRoomMgr() : CFrameBase(module_RoomListData)
{
    m_frame = NULL;
}

CRoomMgr::~CRoomMgr()
{

}

ResultCode CRoomMgr::Initialize( IModule* UpperFrame/*=NULL*/ )
{
    m_frame = dynamic_cast<IFrameWork*>(UpperFrame);
    ATLASSERT(m_frame);
	return Result_Success;
}

ResultCode CRoomMgr::UnInitialize()
{
    return Result_Success;
}

ResultCode CRoomMgr::Run()
{
	netHelper.AddFilter(MSG_AnswerClassList_Id, this);
    netHelper.AddFilter(MSG_AnswerEnterClassroom_Id, this);
    netHelper.StartListen();
    return Result_Success;
}

ResultCode CRoomMgr::Terminate()
{
	netHelper.StopListen();
    return Result_Success;
}

ResultCode CRoomMgr::CreateRoom( RoomID& id )
{
	return Result_NotImpl;
}

ResultCode CRoomMgr::GetRoom( RoomID id,CRoomBase* room )
{
	return Result_NotImpl;
}

ResultCode CRoomMgr::FreeRoom( RoomID id )
{
	return Result_NotImpl;
}

ResultCode CRoomMgr::Show( BOOL bShow/*=TRUE*/ )
{
	return Result_Success;
}

ResultCode CRoomMgr::EnterRoom(RoomID id, std::string password, std::string verificationCode)
{
    INetCenter *pNetCenter = nullptr;
    Global->GetINetCenter(&pNetCenter);
    if(pNetCenter)
    {
        MSG_RequestEnterClassroom netMsg;
        netMsg.SetRoomID(id);
        netMsg.SetRoomPassword(password);
        netMsg.SetVerificationCode(verificationCode);
        pNetCenter->SendServer(netMsg);
        return Result_Success;
    }
    return Result_Fail;
}

ResultCode CRoomMgr::GetRoomList()
{
    INetCenter *pNetCenter = nullptr;
    Global->GetINetCenter(&pNetCenter);
    if(pNetCenter)
    {
        MSG_RequestClassList netMsg;
        pNetCenter->SendServer(netMsg);
        return Result_Success;
    }
	return Result_Fail;
}

VOID CRoomMgr::OnGetRoomList( XMessage *pMsg )
{
    GetRoomList();
}

VOID CRoomMgr::OnEnterRoom(XMessage_EnterRoom *pMsg)
{
    EnterRoom(pMsg->roomId, pMsg->password, pMsg->verificationCode);
}

ResultCode CRoomMgr::OnNetMessage( const CMessage &msg )
{
    switch(msg.MsgId())
    {
    case MSG_AnswerClassList_Id:
        {
            MSG_AnswerClassList msgReal(msg);
            XMessage_GetRoomList_Result result;
            msgReal.GetRoomIdList(result.roomIdList);
            msgReal.GetRoomNameList(result.roomNameList);
            msgReal.GetRoomStateList(result.roomStateList);
            SendXMessage(&result);
            break;
        }
    case MSG_AnswerEnterClassroom_Id:
        {
            MSG_AnswerEnterClassroom msgReal(msg);
            XMessage_EnterRoom_Result result;
            msgReal.GetRetTag(result.retTag);
            SendXMessage(&result);
        }
    }
    return Result_Success;
}

