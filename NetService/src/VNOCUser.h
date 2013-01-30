#ifndef VNOC_VNOC_USER_H_
#define VNOC_VNOC_USER_H_

#include "Common.h"

struct AccountInfo
{
    std::string account;
    std::string email;
    uint8 gender;

    AccountInfo() : gender(0)
    {
    }
};

class VNOCUser
{
public:
    VNOCUser();
    virtual ~VNOCUser();

public:
    virtual void   setUniqueID(GUID_t guid);
    virtual GUID_t getUniqueID();
    virtual void   setNickName(std::string studentName);
    
public:
    virtual void heartbeat(int64 now);
    AccountInfo accountInfo;

private:
    GUID_t _guid;
    std::string _studentName;
};

#endif