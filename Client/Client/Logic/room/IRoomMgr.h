#pragma once
#include <list>
#include "IModule.h"
#include "Room.h"
#include "../../net/INet.h"


typedef unsigned int RoomID;
// 房间列表
typedef std::vector<RoomID> RoomList;

interface IRoomMgr : public IModule
{
public:
	virtual ResultCode Show(BOOL bShow=TRUE)=0;
	virtual ResultCode CreateRoom(RoomID& id)=0;
	virtual ResultCode GetRoom(RoomID id,CRoomBase* room)=0;
	virtual ResultCode GetRoomList()=0;
	virtual ResultCode FreeRoom(RoomID id)=0;
};
