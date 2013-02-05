#include "Room.h"
#include "RoomPool.h"

RoomPool::RoomPool()
{
    _poolSize = 0;
    _incrPoolSize = 0;
    _offset = 0;
}

RoomPool::~RoomPool()
{
    destroy();
}

bool RoomPool::initPool(uint32 size/* = DEFAULT_ROOM_POOL_SIZE*/, uint32 incrSize/* = DEFAULT_ROOM_POOL_INCREASE_SIZE*/)
{
    if (size == 0 || incrSize == 0)
    {
        return false;
    }

    _poolSize = size;
    _incrPoolSize = incrSize;
    _offset = 0;

    _roomObjPool.reserve(_poolSize);
    for (uint32 i = 0; i < _poolSize; ++i)
    {
        Room* room = new Room();
        room->cleanup();
        _roomObjPool.push_back(room);
    }

    return true;
}

void RoomPool::destroy()
{
    for (uint32 i = 0; i < _roomObjPool.size(); ++i)
    {
        SAFE_DELETE(_roomObjPool[i]);
    }

    _roomObjPool.clear();
}

Room* RoomPool::acquire()
{
    if (_offset >= _poolSize)
    {
        uint32 increasedSize = _poolSize + _incrPoolSize;
        _roomObjPool.reserve(increasedSize);
        for (uint32 i = _poolSize; i < increasedSize; ++i)
        {
            Room* room = new Room();
            _roomObjPool.push_back(room);
        }

        _poolSize = increasedSize;
    }

    Room* room = _roomObjPool[_offset];
    if (room != NULL && room->getIsValid() == false)
    {
        room->setPoolObjId(_offset);
        room->setRoomID(_offset);
        room->setIsValid(true);

        _offset++;

        return room;
    }

    return NULL;
}

void  RoomPool::release(uint32 poolObjId)
{
    if (_roomObjPool[poolObjId] == NULL)
    {
        return;
    }

    if (poolObjId >= _offset)
    {
        return;
    }

    _offset--;
    Room* releaseRoomPtr = _roomObjPool[poolObjId];
    if (poolObjId == _offset)
    {
        releaseRoomPtr->cleanup();
    }
    else
    {
        // exchange pointer
        _roomObjPool[poolObjId] = _roomObjPool[_offset];
        _roomObjPool[_offset] = releaseRoomPtr;
        _roomObjPool[poolObjId]->setPoolObjId(poolObjId);

        releaseRoomPtr->cleanup();
    }
}