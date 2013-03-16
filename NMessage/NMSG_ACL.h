#ifndef VNOC_C_MESSAGE_NMSG_ACL
#define VNOC_C_MESSAGE_NMSG_ACL
#include "MessageDef.h"
#include "CMessage.h"
#include "MsgDataValue/StringData.h"
#include "MsgDataValue/NumData.hpp"
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
        InitializeMessage("NMSG_ACL");
    }

    NMSG_ACL(const CMessage& Msg)
    {
        CMessage::Copy(Msg, "NMSG_ACL");
    }

    virtual ~NMSG_ACL(){}

    MsgStatus SetRoomList(const std::vector<Define::uint32>& Value)
    {
        ArrayData* ValueArr = new ArrayData;
        ValueArr->Push<Define::uint32>(Value);
        return WriteArr("RoomList", ValueArr);
    }

    MsgStatus GetRoomList(std::vector<Define::uint32>& Value)
    {
        ArrayData* pReadValueArr = NULL;
        ReadArr("RoomList", pReadValueArr);
        pReadValueArr->GetArr_vec<Define::uint32>(Value);
        return MsgStatus_Ok;
    }
};

}// namespace Message
}// namespace VNOC

#endif