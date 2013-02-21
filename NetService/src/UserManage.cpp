#include "UserManage.hpp"
#include <mutex>

std::mutex userSetMutex;
CUserManage CUserManage::_instance;