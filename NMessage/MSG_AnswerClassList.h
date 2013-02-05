#ifndef VNOC_C_MESSAGE_MSG_AnswerClassList
#define VNOC_C_MESSAGE_MSG_AnswerClassList
#include "MessageDef.h"
#include "CMessage.h"
#include "MsgDataValue/StringData.h"
#include "MsgDataValue/NumData.hpp"

///> Generate by MessageProduse. DO NOT EDIT!

namespace VNOC
{
namespace Message
{

class MSG_AnswerClassList : public CMessage
{
public:
    MSG_AnswerClassList()
    {
        InitializeMessage("MSG_AnswerClassList");
    }

    virtual ~MSG_AnswerClassList(){}

    MsgStatus SetRoomIdList(const std::vector<uint32>& Value)
    {
        ArrayData* ValueArr = new ArrayData;
        ValueArr->Push<uint32>(Value);
        return WriteArr("RoomIdList", ValueArr);
    }

    MsgStatus SetRoomNameList(const std::vector<std::string>& Value)
    {
        ArrayData* ValueArr = new ArrayData;
        ValueArr->Push<std::string>(Value);
        return WriteArr("RoomNameList", ValueArr);
    }

    MsgStatus SetRoomStateList(const std::vector<uint32>& Value)
    {
        ArrayData* ValueArr = new ArrayData;
        ValueArr->Push<uint32>(Value);
        return WriteArr("RoomStateList", ValueArr);
    }

    MsgStatus GetRoomIdList(std::vector<uint32>& Value)
    {
        ArrayData* pReadValueArr = NULL;
        ReadArr("RoomIdList", pReadValueArr);
        pReadValueArr->GetArr_vec<uint32>(Value);
        return MsgStatus_Ok;
    }

    MsgStatus GetRoomNameList(std::vector<std::string>& Value)
    {
        ArrayData* pReadValueArr = NULL;
        ReadArr("RoomNameList", pReadValueArr);
        pReadValueArr->GetArr_vec<std::string>(Value);
        return MsgStatus_Ok;
    }

    MsgStatus GetRoomStateList(std::vector<uint32>& Value)
    {
        ArrayData* pReadValueArr = NULL;
        ReadArr("RoomStateList", pReadValueArr);
        pReadValueArr->GetArr_vec<uint32>(Value);
        return MsgStatus_Ok;
    }
};

}// namespace Message
}// namespace VNOC

#endif