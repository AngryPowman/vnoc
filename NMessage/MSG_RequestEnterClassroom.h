#ifndef VNOC_C_MESSAGE_MSG_RequestEnterClassroom
#define VNOC_C_MESSAGE_MSG_RequestEnterClassroom
#include "MessageDef.h"
#include "CMessage.h"
#include "MsgDataValue/StringData.h"
#include "MsgDataValue/NumData.hpp"

///> Generate by MessageProduse. DO NOT EDIT!

namespace VNOC
{
namespace Message
{

class MSG_RequestEnterClassroom : public CMessage
{
public:
    MSG_RequestEnterClassroom()
    {
        RegisterPort("RoomID", MsgDataMType_Data, MsgDataType_Uint32);

        RegisterPort("RoomPassword", MsgDataMType_Data, MsgDataType_String);

        RegisterPort("VerificationCode", MsgDataMType_Data, MsgDataType_String);

        InitializeMessage(MSG_RequestEnterClassroom_Id);
    }

    MSG_RequestEnterClassroom(const CMessage& Msg)
    {
        RegisterPort("RoomID", MsgDataMType_Data, MsgDataType_Uint32);

        RegisterPort("RoomPassword", MsgDataMType_Data, MsgDataType_String);

        RegisterPort("VerificationCode", MsgDataMType_Data, MsgDataType_String);

        CMessage::Copy(Msg, MSG_RequestEnterClassroom_Id);
    }

    virtual ~MSG_RequestEnterClassroom(){}

    MsgStatus SetRoomID(const Define::uint32& Value)
    {
        return Write("RoomID", new NumData<Define::uint32>(Value));
    }

    MsgStatus SetRoomPassword(const std::string& Value)
    {
        return Write("RoomPassword", new StringData(Value));
    }

    MsgStatus SetVerificationCode(const std::string& Value)
    {
        return Write("VerificationCode", new StringData(Value));
    }

    MsgStatus GetRoomID(Define::uint32& Value) const
    {
        MsgDataValue* pReadValue = NULL;
        Read("RoomID", pReadValue);
        return pReadValue->ToUInt32(Value);
    }

    MsgStatus GetRoomPassword(std::string& Value) const
    {
        MsgDataValue* pReadValue = NULL;
        Read("RoomPassword", pReadValue);
        return pReadValue->ToStr(Value);
    }

    MsgStatus GetVerificationCode(std::string& Value) const
    {
        MsgDataValue* pReadValue = NULL;
        Read("VerificationCode", pReadValue);
        return pReadValue->ToStr(Value);
    }
};

}// namespace Message
}// namespace VNOC

#endif