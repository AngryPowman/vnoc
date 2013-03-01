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

    ResultCode OnNetMessage(const CMessage &msg);

    VOID OnGetRoomList(XMessage *pMsg);

	Begin_XMessage(CRoomMgr)
        OnXMessage(XMessage_GetRoomList, OnGetRoomList)
	End_XMessage()

private:
    IFrameWork *m_frame;
    CNetListenerHelper netHelper;
};
