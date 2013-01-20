#ifndef VNOC_ROOM_H_
#define VNOC_ROOM_H_

#include "Common.h"
#include "GlobalDefinition.h"

class VNOCUser;

typedef std::map<RoomID_t, VNOCUser*> VNOCUserMap;

class Room
{
public:
    Room();
	~Room();

public:
    void cleanup();

public:
	GUID_t       getRoomID() const;
	void         setRoomID(GUID_t roomId);
	void         setMaxUserCount(uint16 maxCount);
	uint16       getMaxUserCount() const;
	void         setRoomName(std::string roomName);
	std::string  getRoomName() const;
    uint16       getOnlineCount() const;
    bool         addUser(VNOCUser* user);
    bool         removeUser(GUID_t guid);
    bool         isUserExists(GUID_t guid) const;
    bool         isFull() const;
    VNOCUser*    getUser(GUID_t guid);
    const VNOCUserMap& getUserMap() const;
    void         setPoolObjId(uint32 poolObjId);
    uint32       getPoolObjId() const;
    void         setValid(bool valid);
    bool         getIsValid() const;

private:
	RoomID_t _roomId;
	VNOCUserMap _userMap;
    uint16 _maxUserCount;
    std::string _roomName;
    static Room _instance;
    uint32 _poolObjId;
    bool _isValid;
};

#endif