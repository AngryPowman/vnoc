#include "RoomPool.h"
#include "Room.h"

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

bool RoomPool::initPool(uint32 size = DEFAULT_ROOM_POOL_SIZE, uint32 incrSize = DEFAULT_ROOM_POOL_INCREASE_SIZE)
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
    //如果最后索引位置等于或者大于池的尺寸，则扩充
    if (_offset >= _poolSize)
    {
        //扩充后的尺寸
        uint32 increasedSize = _poolSize + _incrPoolSize;

        //重新分配池尺寸
        _roomObjPool.resize(increasedSize);

        //分配内存
        for (uint32 i = _poolSize; i < increasedSize; ++i)
        {
            Room* room = new Room();
            room->cleanup();
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
    }
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
    Room* room = _roomObjPool[poolObjId];
    
    _roomObjPool[_offset] = room;

}