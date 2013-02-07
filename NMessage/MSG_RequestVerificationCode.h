#ifndef VNOC_C_MESSAGE_MSG_RequestVerificationCode
#define VNOC_C_MESSAGE_MSG_RequestVerificationCode
#include "MessageDef.h"
#include "CMessage.h"
#include "MsgDataValue/StringData.h"
#include "MsgDataValue/NumData.hpp"

///> Generate by MessageProduse. DO NOT EDIT!

namespace VNOC
{
namespace Message
{

class MSG_RequestVerificationCode : public CMessage
{
public:
    MSG_RequestVerificationCode()
    {
        InitializeMessage("MSG_RequestVerificationCode");
    }

    MSG_RequestVerificationCode(IReadMessage& Msg)
    {
        CMessage::Copy(Msg, "MSG_RequestVerificationCode");
    }

    virtual ~MSG_RequestVerificationCode(){}

    MsgStatus SetMachineAddress(const std::string& Value)
    {
        return Write("MachineAddress", new StringData(Value));
    }

    MsgStatus GetMachineAddress(std::string& Value)
    {
        MsgDataValue* pReadValue = NULL;
        Read("MachineAddress", pReadValue);
        return pReadValue->ToStr(Value);
    }
};

}// namespace Message
}// namespace VNOC

#endif