#pragma once
#include "IRoomMgr.h"
#include "../../FrameWork/FrameBase.hpp"

class CRoomMgr : public IRoomMgr
	, public CFrameBase
    , INetListener
{
public:
	CRoomMgr();
	~CRoomMgr();
public:
	ResultCode Initialize(IModule* UpperFrame=NULL);
	ResultCode UnInitialize();
	ResultCode Run();
	ResultCode Terminate();

	ResultCode Show(BOOL bShow=TRUE);
	ResultCode CreateRoom(RoomID& id);
	ResultCode GetRoom(RoomID id,CRoomBase* room);
	ResultCode GetRoomList();
	ResultCode FreeRoom(RoomID id);
    ResultCode EnterRoom(RoomID id, std::string password, std::string verificationCode);

    ResultCode OnNetMessage(const CMessage &msg);

    VOID OnGetRoomList(XMessage *pMsg);
    VOID OnEnterRoom(XMessage_EnterRoom *pMsg);

	Begin_XMessage(CRoomMgr)
        OnXMessage(XMessage_GetRoomList, OnGetRoomList)
        OnXMessage(XMessage_EnterRoom, OnEnterRoom)
	End_XMessage()

private:
    IFrameWork *m_frame;
    CNetListenerHelper netHelper;
};
