#ifndef VNOC_C_MESSAGE_MSG_AnswerLogin
#define VNOC_C_MESSAGE_MSG_AnswerLogin
#include "MessageDef.h"
#include "CMessage.h"
#include "MsgDataValue/StringData.h"
#include "MsgDataValue/NumData.hpp"

///> Generate by MessageProduse. DO NOT EDIT!

namespace VNOC
{
namespace Message
{

class MSG_AnswerLogin : public CMessage
{
public:
    MSG_AnswerLogin()
    {
        InitializeMessage("MSG_AnswerLogin");
    }

    virtual ~MSG_AnswerLogin(){}

    MsgStatus SetATLGUID(const std::string& Value)
    {
        return Write("ATLGUID", new StringData(Value));
    }

    MsgStatus SetLoginResult(const uint8& Value)
    {
        return Write("LoginResult", new NumData<uint8>(Value));
    }

    MsgStatus SetToken(const uint32& Value)
    {
        return Write("Token", new NumData<uint32>(Value));
    }

    MsgStatus GetATLGUID(std::string& Value)
    {
        MsgDataValue* pReadValue = NULL;
        Read("ATLGUID", pReadValue);
        return pReadValue->ToStr(Value);
    }

    MsgStatus GetLoginResult(uint8& Value)
    {
        MsgDataValue* pReadValue = NULL;
        Read("LoginResult", pReadValue);
        return pReadValue->ToUInt8(Value);
    }

    MsgStatus GetToken(uint32& Value)
    {
        MsgDataValue* pReadValue = NULL;
        Read("Token", pReadValue);
        return pReadValue->ToUInt32(Value);
    }
};

}// namespace Message
}// namespace VNOC

#endif