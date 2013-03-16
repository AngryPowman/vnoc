#ifndef VNOC_ROOM_MANAGER_H_
#define VNOC_ROOM_MANAGER_H_

#include "Common.h"
#include "GlobalDefinition.h"

class Room;
class RoomManager
{
	typedef std::map<RoomID_t, Room*> RoomMap;

public:
    static RoomManager& instance();
	~RoomManager();

private:
	RoomManager();

public:
	void init();
    void fini();
    void release();

    //obj
    Room* acquire();

    //manage
    bool  add(Room* room);
    bool  remove(Room* room);
    bool  remove(RoomID_t roomId);
    Room* getRoom(RoomID_t roomId);

private:
    RoomMap _roomMap;
    static RoomManager _instance;
};

#endif