#pragma once
#include "IRoomMgr.h"

class CRoomMgr : public IRoomMgr
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
};
