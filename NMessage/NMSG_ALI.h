#ifndef VNOC_C_MESSAGE_NMSG_ALI
#define VNOC_C_MESSAGE_NMSG_ALI
#include "MessageDef.h"
#include "CMessage.h"
#include "MsgDataValue/StringData.h"
#include "MsgDataValue/NumData.hpp"
//测试中文
///> Generate by MessageProduse. DO NOT EDIT!

namespace VNOC
{
namespace Message
{

class NMSG_ALI : public CMessage
{
public:
    NMSG_ALI()
    {
        InitializeMessage("NMSG_ALI");
    }

    virtual ~NMSG_ALI(){}

    MsgStatus SetATLGUID(const std::string& Value)
    {
        return Write("ATLGUID", new StringData(Value));
    }

    MsgStatus SetLoginResult(const Define::uint8& Value)
    {
        return Write("LoginResult", new NumData<Define::uint8>(Value));
    }

    MsgStatus SetToken(const Define::uint32& Value)
    {
        return Write("Token", new NumData<Define::uint32>(Value));
    }

    MsgStatus GetATLGUID(std::string& Value)
    {
        MsgDataValue* pReadValue = NULL;
        Read("ATLGUID", pReadValue);
        return pReadValue->ToStr(Value);
    }

    MsgStatus GetLoginResult(Define::uint8& Value)
    {
        MsgDataValue* pReadValue = NULL;
        Read("LoginResult", pReadValue);
        return pReadValue->ToUInt8(Value);
    }

    MsgStatus GetToken(Define::uint32& Value)
    {
        MsgDataValue* pReadValue = NULL;
        Read("Token", pReadValue);
        return pReadValue->ToUInt32(Value);
    }
};

}// namespace Message
}// namespace VNOC

#endif