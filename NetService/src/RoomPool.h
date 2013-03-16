#ifndef VNOC_ROOM_POOL_H_
#define VNOC_ROOM_POOL_H_

#include "Common.h"
#include "GlobalDefinition.h"

class Room;
class RoomPool
{
public:
    RoomPool();
    ~RoomPool();

public:
    bool  initPool(uint32 size = DEFAULT_ROOM_POOL_SIZE, uint32 incrSize = DEFAULT_ROOM_POOL_INCREASE_SIZE);
    void  destroy();
    Room* acquire();
    void  release(uint32 poolObjId);

private:
    std::vector<Room*> _roomObjPool;
    uint32 _poolSize;
    uint32 _incrPoolSize;
    uint32 _offset;

};


#endif