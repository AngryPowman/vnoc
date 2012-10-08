#pragma once
#include "IRoomMgr.h"
#include "../../FrameWork/FrameBase.hpp"

class CRoomMgr : public IRoomMgr
	, public CFrameBase
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
	STDMETHOD( GetRoomList(RoomList& list) );
	STDMETHOD( FreeRoom(RoomID id) );

	Begin_XMessage(CRoomMgr)
	End_XMessage()
};
