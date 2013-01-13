#ifndef VNOC_STUDENT_H_
#define VNOC_STUDENT_H_

#include "Common.h"

class Student
{
public:
    Student();
    virtual ~Student();

public:
    void   setUniqueID(GUID_t guid);
    GUID_t getUniqueID();
    void   setStudentName(std::string studentName);
    
public:
    void heartbeat(int64 now);

private:
    GUID_t _guid;
    std::string _studentName;
};

#endif