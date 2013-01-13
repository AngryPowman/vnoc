#include "Student.h"
#include "GlobalDefinition.h"

Student::Student()
{
}

Student::~Student()
{

}

void Student::setUniqueID(GUID_t guid)
{
    _guid = guid;
}

GUID_t Student::getUniqueID()
{
    return _guid;
}

void Student::setStudentName(std::string studentName)
{
    if (studentName.length() <= MAX_STUDENT_NAME_LEN)
    {
        _studentName = studentName;
    }
}

void Student::heartbeat(int64 now)
{

}