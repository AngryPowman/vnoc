#ifndef VNOC_C_MESSAGE_NMSG_ACL
#define VNOC_C_MESSAGE_NMSG_ACL
#include "MessageDef.h"
#include "CMessage.h"
#include "MsgDataValue/StringData.h"
#include "MsgDataValue/UInt32Data.h"
#include "MsgDataValue/UInt16Data.h"
#include "MsgDataValue/UInt8Data.h"
#include "MsgDataValue/ArrayData.h"
//测试中文
///> Generate by MessageProduse. DO NOT EDIT!

namespace VNOC
{
namespace Message
{

class NMSG_ACL : public CMessage
{
public:
    NMSG_ACL()
    {
        CMessage("NMSG_ACL");
    }

    virtual ~NMSG_ACL(){}

    MsgStatus SetRoomList(ArrayData& Value)
    {
        return WriteArr("RoomList", &Value);
    }

    MsgStatus GetRoomList(ArrayData*& Value)
    {
        return ReadArr("RoomList", Value);
    }
};

}// namespace Message
}// namespace VNOC

#endif