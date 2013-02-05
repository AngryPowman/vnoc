#ifndef VNOC_C_MESSAGE_MSG_AnswerRegister
#define VNOC_C_MESSAGE_MSG_AnswerRegister
#include "MessageDef.h"
#include "CMessage.h"
#include "MsgDataValue/StringData.h"
#include "MsgDataValue/NumData.hpp"

///> Generate by MessageProduse. DO NOT EDIT!

namespace VNOC
{
namespace Message
{

class MSG_AnswerRegister : public CMessage
{
public:
    MSG_AnswerRegister()
    {
        InitializeMessage("MSG_AnswerRegister");
    }

    virtual ~MSG_AnswerRegister(){}

    MsgStatus SetRetTag(const uint32& Value)
    {
        return Write("RetTag", new NumData<uint32>(Value));
    }

    MsgStatus GetRetTag(uint32& Value)
    {
        MsgDataValue* pReadValue = NULL;
        Read("RetTag", pReadValue);
        return pReadValue->ToUInt32(Value);
    }
};

}// namespace Message
}// namespace VNOC

#endif