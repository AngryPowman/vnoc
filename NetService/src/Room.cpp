#include "Room.h"
#include "Student.h"
#include "GlobalDefinition.h"

Room::Room()
{
}


Room::~Room()
{
}


GUID_t Room::getRoomID() const
{
    return _roomId;
}


void Room::setRoomID(GUID_t roomId)
{
    _roomId = roomId;
}

void Room::setMaxStudent(uint16 maxCount)
{
    if (maxCount <= MAX_STUDENT_COUNT)
    {
        _maxStudentCount = maxCount;
    }
}

uint16 Room::getMaxStudent() const
{
    return _maxStudentCount;
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

uint16 Room::getOnlineStudentCount() const
{
    return _studentMap.size();
}

bool Room::addStudent(Student* student)
{
    assert(student != NULL);
    GUID_t studentId = student->getUniqueID();

    if (isFull() == false)
    {
        if (isStudentExists(studentId) == false)
        {
            _studentMap.insert(std::make_pair(studentId, student));
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

bool Room::removeStudent(GUID_t studentId)
{
    auto iter = _studentMap.find(studentId);
    if (iter != _studentMap.end())
    {
        _studentMap.erase(iter);
        return true;
    }

    return false;
}

bool Room::isStudentExists(GUID_t studentId) const
{
    auto iter = _studentMap.find(studentId);
    if (iter != _studentMap.end())
    {
        return true;
    }

    return false;
}

bool Room::isFull() const
{
    if (getOnlineStudentCount() >= getMaxStudent())
    {
        return true;
    }

    return false;
}

Student* Room::getStudent(GUID_t studentId)
{
    if (isStudentExists(studentId) == true)
    {
        auto iter = _studentMap.find(studentId);
        return iter->second;
    }

    return NULL;
}

const StudentMap& Room::getStudentList() const
{
    return _studentMap;
}