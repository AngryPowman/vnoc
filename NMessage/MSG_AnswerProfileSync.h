#ifndef VNOC_C_MESSAGE_MSG_AnswerProfileSync
#define VNOC_C_MESSAGE_MSG_AnswerProfileSync
#include "MessageDef.h"
#include "CMessage.h"
#include "MsgDataValue/StringData.h"
#include "MsgDataValue/NumData.hpp"

///> Generate by MessageProduse. DO NOT EDIT!

namespace VNOC
{
namespace Message
{

class MSG_AnswerProfileSync : public CMessage
{
public:
    MSG_AnswerProfileSync()
    {
        InitializeMessage("MSG_AnswerProfileSync");
    }

    MSG_AnswerProfileSync(IReadMessage& Msg)
    {
        CMessage::Copy(Msg, "MSG_AnswerProfileSync");
    }

    virtual ~MSG_AnswerProfileSync(){}

    MsgStatus SetMessageSynchro(const std::string& Value)
    {
        return Write("MessageSynchro", new StringData(Value));
    }

    MsgStatus GetMessageSynchro(std::string& Value) const
    {
        MsgDataValue* pReadValue = NULL;
        Read("MessageSynchro", pReadValue);
        return pReadValue->ToStr(Value);
    }
};

}// namespace Message
}// namespace VNOC

#endif