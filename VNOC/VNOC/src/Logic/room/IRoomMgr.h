#pragma once
#include <list>
#include "../../Base/IModule.h"
#include "Room.h"
#include "../../net/INet.h"


typedef int RoomID;
// 房间列表
typedef std::vector<RoomID> RoomList;

interface IRoomMgr : public IModule
{
public:
	STDMETHOD( Show(BOOL bShow=TRUE) = 0 );
	// 创建房间
	STDMETHOD( CreateRoom(RoomID& id) = 0 );
	STDMETHOD( GetRoom(RoomID id,CRoomBase* room) = 0 );
	STDMETHOD( GetRoomList() = 0 );
	// 关闭房间
	STDMETHOD( FreeRoom(RoomID id) = 0);
};
