#ifndef VNOC_C_MESSAGE_MSG_AnswerClassInfo
#define VNOC_C_MESSAGE_MSG_AnswerClassInfo
#include "MessageDef.h"
#include "CMessage.h"
#include "MsgDataValue/StringData.h"
#include "MsgDataValue/NumData.hpp"

///> Generate by MessageProduse. DO NOT EDIT!

namespace VNOC
{
namespace Message
{

class MSG_AnswerClassInfo : public CMessage
{
public:
    MSG_AnswerClassInfo()
    {
        InitializeMessage("MSG_AnswerClassInfo");
    }

    MSG_AnswerClassInfo(IReadMessage& Msg)
    {
        CMessage::Copy(Msg, "MSG_AnswerClassInfo");
    }

    virtual ~MSG_AnswerClassInfo(){}

    MsgStatus SetRoomID(const uint32& Value)
    {
        return Write("RoomID", new NumData<uint32>(Value));
    }

    MsgStatus SetRoomManageId(const uint32& Value)
    {
        return Write("RoomManageId", new NumData<uint32>(Value));
    }

    MsgStatus SetRoomName(const std::string& Value)
    {
        return Write("RoomName", new StringData(Value));
    }

    MsgStatus SetRoomPassword(const std::string& Value)
    {
        return Write("RoomPassword", new StringData(Value));
    }

    MsgStatus SetRoomPeopleListId(const std::vector<uint32>& Value)
    {
        ArrayData* ValueArr = new ArrayData;
        ValueArr->Push<uint32>(Value);
        return WriteArr("RoomPeopleListId", ValueArr);
    }

    MsgStatus SetRoomPeopleNumM(const uint32& Value)
    {
        return Write("RoomPeopleNumM", new NumData<uint32>(Value));
    }

    MsgStatus SetRoomPeopleNumMax(const uint32& Value)
    {
        return Write("RoomPeopleNumMax", new NumData<uint32>(Value));
    }

    MsgStatus SetRoomRank(const uint32& Value)
    {
        return Write("RoomRank", new NumData<uint32>(Value));
    }

    MsgStatus SetRoomState(const uint32& Value)
    {
        return Write("RoomState", new NumData<uint32>(Value));
    }

    MsgStatus SetRoomType(const uint32& Value)
    {
        return Write("RoomType", new NumData<uint32>(Value));
    }

    MsgStatus GetRoomID(uint32& Value)
    {
        MsgDataValue* pReadValue = NULL;
        Read("RoomID", pReadValue);
        return pReadValue->ToUInt32(Value);
    }

    MsgStatus GetRoomManageId(uint32& Value)
    {
        MsgDataValue* pReadValue = NULL;
        Read("RoomManageId", pReadValue);
        return pReadValue->ToUInt32(Value);
    }

    MsgStatus GetRoomName(std::string& Value)
    {
        MsgDataValue* pReadValue = NULL;
        Read("RoomName", pReadValue);
        return pReadValue->ToStr(Value);
    }

    MsgStatus GetRoomPassword(std::string& Value)
    {
        MsgDataValue* pReadValue = NULL;
        Read("RoomPassword", pReadValue);
        return pReadValue->ToStr(Value);
    }

    MsgStatus GetRoomPeopleListId(std::vector<uint32>& Value)
    {
        ArrayData* pReadValueArr = NULL;
        ReadArr("RoomPeopleListId", pReadValueArr);
        pReadValueArr->GetArr_vec<uint32>(Value);
        return MsgStatus_Ok;
    }

    MsgStatus GetRoomPeopleNumM(uint32& Value)
    {
        MsgDataValue* pReadValue = NULL;
        Read("RoomPeopleNumM", pReadValue);
        return pReadValue->ToUInt32(Value);
    }

    MsgStatus GetRoomPeopleNumMax(uint32& Value)
    {
        MsgDataValue* pReadValue = NULL;
        Read("RoomPeopleNumMax", pReadValue);
        return pReadValue->ToUInt32(Value);
    }

    MsgStatus GetRoomRank(uint32& Value)
    {
        MsgDataValue* pReadValue = NULL;
        Read("RoomRank", pReadValue);
        return pReadValue->ToUInt32(Value);
    }

    MsgStatus GetRoomState(uint32& Value)
    {
        MsgDataValue* pReadValue = NULL;
        Read("RoomState", pReadValue);
        return pReadValue->ToUInt32(Value);
    }

    MsgStatus GetRoomType(uint32& Value)
    {
        MsgDataValue* pReadValue = NULL;
        Read("RoomType", pReadValue);
        return pReadValue->ToUInt32(Value);
    }
};

}// namespace Message
}// namespace VNOC

#endif