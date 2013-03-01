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
        RegisterPort("MessageSynchro", MsgDataMType_Data, MsgDataType_String);

        InitializeMessage(MSG_AnswerProfileSync_Id);
    }

    MSG_AnswerProfileSync(const CMessage& Msg)
    {
        RegisterPort("MessageSynchro", MsgDataMType_Data, MsgDataType_String);

        CMessage::Copy(Msg, MSG_AnswerProfileSync_Id);
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