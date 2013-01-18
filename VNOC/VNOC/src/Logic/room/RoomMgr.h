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
	STDMETHOD( Initialize(IModule* UpperFrame=NULL) );
	STDMETHOD( UnInitialize() );
	STDMETHOD( Run() );
	STDMETHOD( Terminate() );

	STDMETHOD( Show(BOOL bShow=TRUE) );
	STDMETHOD( CreateRoom(RoomID& id) );
	STDMETHOD( GetRoom(RoomID id,CRoomBase* room) );
	STDMETHOD( GetRoomList() );
	STDMETHOD( FreeRoom(RoomID id) );

    STDMETHOD( OnNetMessage(const CMessage& msg));

    VOID OnGetRoomList(XMessage *pMsg);

	Begin_XMessage(CRoomMgr)
        OnXMessage(XMessage_ShowRoomList, OnGetRoomList)
	End_XMessage()

private:
    IFrameWork *m_frame;
    CNetListenerHelper netHelper;
};
