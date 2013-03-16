#include "VnocRoomManager.hpp"
#include <vector>

using namespace std;
const std::vector<VnocRoom> *VnocRoomManager::getRoomList()
{
    return &roomList_;
}