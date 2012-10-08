#pragma once
#include "../../Base/IModule.h"
#include "room.h"
#include <list>


typedef CString RoomID;
// �����б�
typedef std::list<RoomID> RoomList;

interface IRoomMgr : public IModule
{
public:
	STDMETHOD( Show(BOOL bShow=TRUE) = 0 );
	// ��������
	STDMETHOD( CreateRoom(RoomID& id) = 0 );
	STDMETHOD( GetRoom(RoomID id,CRoomBase* room) = 0 );
	STDMETHOD( GetRoomList(RoomList& list) = 0 );
	// �رշ���
	STDMETHOD( FreeRoom(RoomID id) = 0);
};
