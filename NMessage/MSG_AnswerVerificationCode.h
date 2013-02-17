#ifndef VNOC_C_MESSAGE_MSG_AnswerVerificationCode
#define VNOC_C_MESSAGE_MSG_AnswerVerificationCode
#include "MessageDef.h"
#include "CMessage.h"
#include "MsgDataValue/StringData.h"
#include "MsgDataValue/NumData.hpp"

///> Generate by MessageProduse. DO NOT EDIT!

namespace VNOC
{
namespace Message
{

class MSG_AnswerVerificationCode : public CMessage
{
public:
    MSG_AnswerVerificationCode()
    {
        InitializeMessage("MSG_AnswerVerificationCode");
    }

    MSG_AnswerVerificationCode(IReadMessage& Msg)
    {
        CMessage::Copy(Msg, "MSG_AnswerVerificationCode");
    }

    virtual ~MSG_AnswerVerificationCode(){}

    MsgStatus SetCaptcha(const std::string& Value)
    {
        return Write("Captcha", new StringData(Value));
    }

    MsgStatus SetCaptchaType(const uint8& Value)
    {
        return Write("CaptchaType", new NumData<uint8>(Value));
    }

    MsgStatus SetLoginTag(const uint8& Value)
    {
        return Write("LoginTag", new NumData<uint8>(Value));
    }

    MsgStatus GetCaptcha(std::string& Value)
    {
        MsgDataValue* pReadValue = NULL;
        Read("Captcha", pReadValue);
        return pReadValue->ToStr(Value);
    }

    MsgStatus GetCaptchaType(uint8& Value)
    {
        MsgDataValue* pReadValue = NULL;
        Read("CaptchaType", pReadValue);
        return pReadValue->ToUInt8(Value);
    }

    MsgStatus GetLoginTag(uint8& Value)
    {
        MsgDataValue* pReadValue = NULL;
        Read("LoginTag", pReadValue);
        return pReadValue->ToUInt8(Value);
    }
};

}// namespace Message
}// namespace VNOC

#endif