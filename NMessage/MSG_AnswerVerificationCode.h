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
        RegisterPort("Captcha", MsgDataMType_Data, MsgDataType_String);

        RegisterPort("CaptchaType", MsgDataMType_Data, MsgDataType_Uint8);

        RegisterPort("LoginTag", MsgDataMType_Data, MsgDataType_Uint8);

        InitializeMessage(MSG_AnswerVerificationCode_Id);
    }

    MSG_AnswerVerificationCode(const CMessage& Msg)
    {
        RegisterPort("Captcha", MsgDataMType_Data, MsgDataType_String);

        RegisterPort("CaptchaType", MsgDataMType_Data, MsgDataType_Uint8);

        RegisterPort("LoginTag", MsgDataMType_Data, MsgDataType_Uint8);

        CMessage::Copy(Msg, MSG_AnswerVerificationCode_Id);
    }

    virtual ~MSG_AnswerVerificationCode(){}

    MsgStatus SetCaptcha(const std::string& Value)
    {
        return Write("Captcha", new StringData(Value));
    }

    MsgStatus SetCaptchaType(const Define::uint8& Value)
    {
        return Write("CaptchaType", new NumData<Define::uint8>(Value));
    }

    MsgStatus SetLoginTag(const Define::uint8& Value)
    {
        return Write("LoginTag", new NumData<Define::uint8>(Value));
    }

    MsgStatus GetCaptcha(std::string& Value) const
    {
        MsgDataValue* pReadValue = NULL;
        Read("Captcha", pReadValue);
        return pReadValue->ToStr(Value);
    }

    MsgStatus GetCaptchaType(Define::uint8& Value) const
    {
        MsgDataValue* pReadValue = NULL;
        Read("CaptchaType", pReadValue);
        return pReadValue->ToUInt8(Value);
    }

    MsgStatus GetLoginTag(Define::uint8& Value) const
    {
        MsgDataValue* pReadValue = NULL;
        Read("LoginTag", pReadValue);
        return pReadValue->ToUInt8(Value);
    }
};

}// namespace Message
}// namespace VNOC

#endif