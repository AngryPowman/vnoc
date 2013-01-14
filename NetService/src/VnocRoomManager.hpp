#pragma once
#ifndef ROOM_MANAGER_H
#define ROOM_MANAGER_H
#include "Config.hpp"
#include <vector>
class VnocRoom{
};
class VnocRoomManager {
public:
    const std::vector<VnocRoom> *getRoomList();
private:
    std::vector<VnocRoom> roomList_;
};
#endif //ROOM_MANAGER_H
