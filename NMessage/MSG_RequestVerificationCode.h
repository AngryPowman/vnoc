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
        RegisterPort("MachineAddress", MsgDataMType_Data, MsgDataType_String);

        InitializeMessage(MSG_RequestVerificationCode_Id);
    }

    MSG_RequestVerificationCode(const CMessage& Msg)
    {
        RegisterPort("MachineAddress", MsgDataMType_Data, MsgDataType_String);

        CMessage::Copy(Msg, MSG_RequestVerificationCode_Id);
    }

    virtual ~MSG_RequestVerificationCode(){}

    MsgStatus SetMachineAddress(const std::string& Value)
    {
        return Write("MachineAddress", new StringData(Value));
    }

    MsgStatus GetMachineAddress(std::string& Value) const
    {
        MsgDataValue* pReadValue = NULL;
        Read("MachineAddress", pReadValue);
        return pReadValue->ToStr(Value);
    }
};

}// namespace Message
}// namespace VNOC

#endif