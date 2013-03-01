#ifndef VNOC_C_MESSAGE_MSG_RequestClassInfo
#define VNOC_C_MESSAGE_MSG_RequestClassInfo
#include "MessageDef.h"
#include "CMessage.h"
#include "MsgDataValue/StringData.h"
#include "MsgDataValue/NumData.hpp"

///> Generate by MessageProduse. DO NOT EDIT!

namespace VNOC
{
namespace Message
{

class MSG_RequestClassInfo : public CMessage
{
public:
    MSG_RequestClassInfo()
    {
        RegisterPort("RoomID", MsgDataMType_Data, MsgDataType_Uint32);

        InitializeMessage(MSG_RequestClassInfo_Id);
    }

    MSG_RequestClassInfo(const CMessage& Msg)
    {
        RegisterPort("RoomID", MsgDataMType_Data, MsgDataType_Uint32);

        CMessage::Copy(Msg, MSG_RequestClassInfo_Id);
    }

    virtual ~MSG_RequestClassInfo(){}

    MsgStatus SetRoomID(const Define::uint32& Value)
    {
        return Write("RoomID", new NumData<Define::uint32>(Value));
    }

    MsgStatus GetRoomID(Define::uint32& Value) const
    {
        MsgDataValue* pReadValue = NULL;
        Read("RoomID", pReadValue);
        return pReadValue->ToUInt32(Value);
    }
};

}// namespace Message
}// namespace VNOC

#endif