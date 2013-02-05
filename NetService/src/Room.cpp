#include "Room.h"
#include "VNOCUser.h"
#include "GlobalDefinition.h"

Room::Room()
{
    cleanup();
}


Room::~Room()
{
}

void Room::cleanup()
{
    _roomId = 0;
    _userMap.clear();
    _maxUserCount = 0;
    _roomName = "";
    _isValid = false;
}

GUID_t Room::getRoomID() const
{
    return _roomId;
}


void Room::setRoomID(GUID_t roomId)
{
    _roomId = roomId;
}

void Room::setMaxUserCount(uint16 maxCount)
{
    if (maxCount <= MAX_ROOM_USER_LIMIT)
    {
        _maxUserCount = maxCount;
    }
}

uint16 Room::getMaxUserCount() const
{
    return _maxUserCount;
}

void Room::setRoomName(std::string roomName)
{
    if (roomName.length() <= MAX_ROOM_NAME_LEN)
    {
        _roomName = roomName;
    }
}

std::string Room::getRoomName() const
{
    return _roomName;
}

uint16 Room::getOnlineCount() const
{
    return _userMap.size();
}

bool Room::addUser(VNOCUser* user)
{
    assert(user != NULL);
    GUID_t userId = user->getUniqueID();

    if (isFull() == false)
    {
        if (isUserExists(userId) == false)
        {
            _userMap.insert(std::make_pair(userId, user));
            return true;
        }
        else
        {
            //student exist
            return false;
        }
    }

    return false;
}

bool Room::removeUser(GUID_t guid)
{
    auto iter = _userMap.find(guid);
    if (iter != _userMap.end())
    {
        _userMap.erase(iter);
        return true;
    }

    return false;
}

bool Room::isUserExists(GUID_t studentId) const
{
    auto iter = _userMap.find(studentId);
    if (iter != _userMap.end())
    {
        return true;
    }

    return false;
}

bool Room::isFull() const
{
    if (getOnlineCount() >= getMaxUserCount())
    {
        return true;
    }

    return false;
}

VNOCUser* Room::getUser(GUID_t guid)
{
    if (isUserExists(guid) == true)
    {
        auto iter = _userMap.find(guid);
        return iter->second;
    }

    return NULL;
}

const VNOCUserMap& Room::getUserMap() const
{
    return _userMap;
}

void Room::setPoolObjId(uint32 poolObjId)
{
    _poolObjId = poolObjId;
}

uint32 Room::getPoolObjId() const
{
    return _poolObjId;
}

void Room::setIsValid(bool valid)
{
    _isValid = valid;
}


bool Room::getIsValid() const
{
    return _isValid;
}