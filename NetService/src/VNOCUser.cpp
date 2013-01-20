#include "VNOCUser.h"
#include "GlobalDefinition.h"

VNOCUser::VNOCUser()
{
}

VNOCUser::~VNOCUser()
{
}

void VNOCUser::setUniqueID(GUID_t guid)
{
    _guid = guid;
}

GUID_t VNOCUser::getUniqueID()
{
    return _guid;
}

void VNOCUser::setNickName(std::string studentName)
{
    if (studentName.length() <= MAX_NICKNAME_LEN)
    {
        _studentName = studentName;
    }
}

void VNOCUser::heartbeat(int64 now)
{

}