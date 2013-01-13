#ifndef VNOC_ROOM_H_
#define VNOC_ROOM_H_

#include "Common.h"
#include "GlobalDefinition.h"

class Student;

typedef std::map<RoomID_t, Student*> StudentMap;

class Room
{
public:
    Room();
	~Room();

public:
	GUID_t       getRoomID() const;
	void         setRoomID(GUID_t roomId);
	void         setMaxStudent(uint16 maxCount);
	uint16       getMaxStudent() const;
	void         setRoomName(std::string roomName);
	std::string  getRoomName() const;
    uint16       getOnlineStudentCount() const;
    bool         addStudent(Student* student);
    bool         removeStudent(GUID_t studentId);
    bool         isStudentExists(GUID_t studentId) const;
    bool         isFull() const;
    Student*     getStudent(GUID_t studentId);
    const StudentMap& getStudentList() const; 

private:
	RoomID_t _roomId;
	StudentMap _studentMap;
    uint16 _maxStudentCount;
    std::string _roomName;
    static Room _instance;
};

#endif