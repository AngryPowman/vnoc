#ifndef  VNOC_I_MESSAGE_DATA
#define  VNOC_I_MESSAGE_DATA

#include "../MessageDef.h"

namespace VNOC
{
namespace Message
{

typedef     int     MessageName;
typedef     int     MessageType;

class MsgDataValue
{
public:
    MsgDataValue(){}
    virtual ~MsgDataValue(){}

    virtual MsgStatus ToStr(
        OUT std::string& Value
        )
    {
        return MsgStatus_Unk;
    }

    virtual MsgStatus ToUInt8(
        OUT uint8& Value
        )
    { 
        return MsgStatus_Unk;
    }

    virtual MsgStatus ToUInt16(
        OUT uint16& Value
        )
    { 
        return MsgStatus_Unk;
    }

    virtual MsgStatus ToUInt32(
        OUT uint32& Value
        )
    { 
        return MsgStatus_Unk;
    }
};

}// namespace Message
}// namespace VNOC

#endif