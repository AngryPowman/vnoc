#ifndef VNOC_C_MESSAGE_MSG_RequestProfileSync
#define VNOC_C_MESSAGE_MSG_RequestProfileSync
#include "MessageDef.h"
#include "CMessage.h"
#include "MsgDataValue/StringData.h"
#include "MsgDataValue/NumData.hpp"

///> Generate by MessageProduse. DO NOT EDIT!

namespace VNOC
{
namespace Message
{

class MSG_RequestProfileSync : public CMessage
{
public:
    MSG_RequestProfileSync()
    {
        InitializeMessage("MSG_RequestProfileSync");
    }

    MSG_RequestProfileSync(IReadMessage& Msg)
    {
        CMessage::Copy(Msg, "MSG_RequestProfileSync");
    }

    virtual ~MSG_RequestProfileSync(){}

    MsgStatus SetAutograph(const std::string& Value)
    {
        return Write("Autograph", new StringData(Value));
    }

    MsgStatus SetHeadForm(const Define::uint8& Value)
    {
        return Write("HeadForm", new NumData<Define::uint8>(Value));
    }

    MsgStatus SetHeadPortrait(const std::string& Value)
    {
        return Write("HeadPortrait", new StringData(Value));
    }

    MsgStatus SetNickname(const std::string& Value)
    {
        return Write("Nickname", new StringData(Value));
    }

    MsgStatus SetRank(const Define::uint8& Value)
    {
        return Write("Rank", new NumData<Define::uint8>(Value));
    }

    MsgStatus GetAutograph(std::string& Value)
    {
        MsgDataValue* pReadValue = NULL;
        Read("Autograph", pReadValue);
        return pReadValue->ToStr(Value);
    }

    MsgStatus GetHeadForm(Define::uint8& Value)
    {
        MsgDataValue* pReadValue = NULL;
        Read("HeadForm", pReadValue);
        return pReadValue->ToUInt8(Value);
    }

    MsgStatus GetHeadPortrait(std::string& Value)
    {
        MsgDataValue* pReadValue = NULL;
        Read("HeadPortrait", pReadValue);
        return pReadValue->ToStr(Value);
    }

    MsgStatus GetNickname(std::string& Value)
    {
        MsgDataValue* pReadValue = NULL;
        Read("Nickname", pReadValue);
        return pReadValue->ToStr(Value);
    }

    MsgStatus GetRank(Define::uint8& Value)
    {
        MsgDataValue* pReadValue = NULL;
        Read("Rank", pReadValue);
        return pReadValue->ToUInt8(Value);
    }
};

}// namespace Message
}// namespace VNOC

#endif