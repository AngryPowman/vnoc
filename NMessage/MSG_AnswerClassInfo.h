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
        RegisterPort("RoomID", MsgDataMType_Data, MsgDataType_Uint32);

        RegisterPort("RoomManageId", MsgDataMType_Data, MsgDataType_Uint32);

        RegisterPort("RoomName", MsgDataMType_Data, MsgDataType_String);

        RegisterPort("RoomPassword", MsgDataMType_Data, MsgDataType_String);

        RegisterPort("RoomPeopleListId", MsgDataMType_List, MsgDataType_Uint32);

        RegisterPort("RoomPeopleNumMax", MsgDataMType_Data, MsgDataType_Uint32);

        RegisterPort("RoomRank", MsgDataMType_Data, MsgDataType_Uint32);

        RegisterPort("RoomState", MsgDataMType_Data, MsgDataType_Uint32);

        RegisterPort("RoomType", MsgDataMType_Data, MsgDataType_Uint32);

        InitializeMessage(MSG_AnswerClassInfo_Id);
    }

    MSG_AnswerClassInfo(const CMessage& Msg)
    {
        RegisterPort("RoomID", MsgDataMType_Data, MsgDataType_Uint32);

        RegisterPort("RoomManageId", MsgDataMType_Data, MsgDataType_Uint32);

        RegisterPort("RoomName", MsgDataMType_Data, MsgDataType_String);

        RegisterPort("RoomPassword", MsgDataMType_Data, MsgDataType_String);

        RegisterPort("RoomPeopleListId", MsgDataMType_List, MsgDataType_Uint32);

        RegisterPort("RoomPeopleNumMax", MsgDataMType_Data, MsgDataType_Uint32);

        RegisterPort("RoomRank", MsgDataMType_Data, MsgDataType_Uint32);

        RegisterPort("RoomState", MsgDataMType_Data, MsgDataType_Uint32);

        RegisterPort("RoomType", MsgDataMType_Data, MsgDataType_Uint32);

        CMessage::Copy(Msg, MSG_AnswerClassInfo_Id);
    }

    virtual ~MSG_AnswerClassInfo(){}

    MsgStatus SetRoomID(const Define::uint32& Value)
    {
        return Write("RoomID", new NumData<Define::uint32>(Value));
    }

    MsgStatus SetRoomManageId(const Define::uint32& Value)
    {
        return Write("RoomManageId", new NumData<Define::uint32>(Value));
    }

    MsgStatus SetRoomName(const std::string& Value)
    {
        return Write("RoomName", new StringData(Value));
    }

    MsgStatus SetRoomPassword(const std::string& Value)
    {
        return Write("RoomPassword", new StringData(Value));
    }

    MsgStatus SetRoomPeopleListId(const std::vector<Define::uint32>& Value)
    {
        ArrayData* ValueArr = new ArrayData;
        ValueArr->Push<Define::uint32>(Value);
        return WriteArr("RoomPeopleListId", ValueArr);
    }

    MsgStatus SetRoomPeopleNumMax(const Define::uint32& Value)
    {
        return Write("RoomPeopleNumMax", new NumData<Define::uint32>(Value));
    }

    MsgStatus SetRoomRank(const Define::uint32& Value)
    {
        return Write("RoomRank", new NumData<Define::uint32>(Value));
    }

    MsgStatus SetRoomState(const Define::uint32& Value)
    {
        return Write("RoomState", new NumData<Define::uint32>(Value));
    }

    MsgStatus SetRoomType(const Define::uint32& Value)
    {
        return Write("RoomType", new NumData<Define::uint32>(Value));
    }

    MsgStatus GetRoomID(Define::uint32& Value) const
    {
        MsgDataValue* pReadValue = NULL;
        Read("RoomID", pReadValue);
        return pReadValue->ToUInt32(Value);
    }

    MsgStatus GetRoomManageId(Define::uint32& Value) const
    {
        MsgDataValue* pReadValue = NULL;
        Read("RoomManageId", pReadValue);
        return pReadValue->ToUInt32(Value);
    }

    MsgStatus GetRoomName(std::string& Value) const
    {
        MsgDataValue* pReadValue = NULL;
        Read("RoomName", pReadValue);
        return pReadValue->ToStr(Value);
    }

    MsgStatus GetRoomPassword(std::string& Value) const
    {
        MsgDataValue* pReadValue = NULL;
        Read("RoomPassword", pReadValue);
        return pReadValue->ToStr(Value);
    }

    MsgStatus GetRoomPeopleListId(std::vector<Define::uint32>& Value) const
    {
        ArrayData* pReadValueArr = NULL;
        ReadArr("RoomPeopleListId", pReadValueArr);
        pReadValueArr->GetArr_vec<Define::uint32>(Value);
        return MsgStatus_Ok;
    }

    MsgStatus GetRoomPeopleNumMax(Define::uint32& Value) const
    {
        MsgDataValue* pReadValue = NULL;
        Read("RoomPeopleNumMax", pReadValue);
        return pReadValue->ToUInt32(Value);
    }

    MsgStatus GetRoomRank(Define::uint32& Value) const
    {
        MsgDataValue* pReadValue = NULL;
        Read("RoomRank", pReadValue);
        return pReadValue->ToUInt32(Value);
    }

    MsgStatus GetRoomState(Define::uint32& Value) const
    {
        MsgDataValue* pReadValue = NULL;
        Read("RoomState", pReadValue);
        return pReadValue->ToUInt32(Value);
    }

    MsgStatus GetRoomType(Define::uint32& Value) const
    {
        MsgDataValue* pReadValue = NULL;
        Read("RoomType", pReadValue);
        return pReadValue->ToUInt32(Value);
    }
};

}// namespace Message
}// namespace VNOC

#endif