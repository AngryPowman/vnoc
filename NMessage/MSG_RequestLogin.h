﻿#ifndef VNOC_C_MESSAGE_MSG_RequestLogin
#define VNOC_C_MESSAGE_MSG_RequestLogin
#include "MessageDef.h"
#include "CMessage.h"
#include "MsgDataValue/StringData.h"
#include "MsgDataValue/NumData.hpp"

///> Generate by MessageProduse. DO NOT EDIT!

namespace VNOC
{
namespace Message
{

class MSG_RequestLogin : public CMessage
{
public:
    MSG_RequestLogin()
    {
        InitializeMessage("MSG_RequestLogin");
    }

    MSG_RequestLogin(IReadMessage& Msg)
    {
        CMessage::Copy(Msg, "MSG_RequestLogin");
    }

    virtual ~MSG_RequestLogin(){}

    MsgStatus SetAccountNumber(const std::string& Value)
    {
        return Write("AccountNumber", new StringData(Value));
    }

    MsgStatus SetPassword(const std::string& Value)
    {
        return Write("Password", new StringData(Value));
    }

    MsgStatus SetVerificationCode(const std::string& Value)
    {
        return Write("VerificationCode", new StringData(Value));
    }

    MsgStatus GetAccountNumber(std::string& Value)
    {
        MsgDataValue* pReadValue = NULL;
        Read("AccountNumber", pReadValue);
        return pReadValue->ToStr(Value);
    }

    MsgStatus GetPassword(std::string& Value)
    {
        MsgDataValue* pReadValue = NULL;
        Read("Password", pReadValue);
        return pReadValue->ToStr(Value);
    }

    MsgStatus GetVerificationCode(std::string& Value)
    {
        MsgDataValue* pReadValue = NULL;
        Read("VerificationCode", pReadValue);
        return pReadValue->ToStr(Value);
    }
};

}// namespace Message
}// namespace VNOC

#endif