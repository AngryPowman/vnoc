#include "RoomManager.h"
#include "Room.h"

RoomManager RoomManager::_instance;

RoomManager::RoomManager(void)
{
}


RoomManager::~RoomManager(void)
{
}

RoomManager& RoomManager::instance()
{
    return _instance;
}


void RoomManager::init()
{

}

void RoomManager::fini()
{

}

void RoomManager::release()
{
    
}

//manage
bool RoomManager::add(Room* room)
{
    assert(room != NULL);

    _roomMap[room->getRoomID()] = room;

    return true;
}

bool RoomManager::remove(Room* room)
{
    assert(room != NULL);

    return remove(room->getRoomID());
}

bool RoomManager::remove(RoomID_t roomId)
{
    auto iter = _roomMap.find(roomId);
    if (iter != _roomMap.end())
    {
        _roomMap.erase(iter);
        return true;
    }

    return false;
}

Room* RoomManager::getRoom(RoomID_t roomId)
{
    auto iter = _roomMap.find(roomId);
    if (iter != _roomMap.end())
    {
        return iter->second;
    }

    return NULL;
}