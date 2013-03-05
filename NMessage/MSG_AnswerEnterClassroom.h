#ifndef VNOC_C_MESSAGE_MSG_AnswerEnterClassroom
#define VNOC_C_MESSAGE_MSG_AnswerEnterClassroom
#include "MessageDef.h"
#include "CMessage.h"
#include "MsgDataValue/StringData.h"
#include "MsgDataValue/NumData.hpp"

///> Generate by MessageProduse. DO NOT EDIT!

namespace VNOC
{
namespace Message
{

class MSG_AnswerEnterClassroom : public CMessage
{
public:
    MSG_AnswerEnterClassroom()
    {
        RegisterPort("RetTag", MsgDataMType_Data, MsgDataType_Uint32);

        InitializeMessage(MSG_AnswerEnterClassroom_Id);
    }

    MSG_AnswerEnterClassroom(const CMessage& Msg)
    {
        RegisterPort("RetTag", MsgDataMType_Data, MsgDataType_Uint32);

        CMessage::Copy(Msg, MSG_AnswerEnterClassroom_Id);
    }

    virtual ~MSG_AnswerEnterClassroom(){}

    MsgStatus SetRetTag(const Define::uint32& Value)
    {
        return Write("RetTag", new NumData<Define::uint32>(Value));
    }

    MsgStatus GetRetTag(Define::uint32& Value) const
    {
        MsgDataValue* pReadValue = NULL;
        Read("RetTag", pReadValue);
        return pReadValue->ToUInt32(Value);
    }
};

}// namespace Message
}// namespace VNOC

#endif