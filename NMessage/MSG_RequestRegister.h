#ifndef VNOC_C_MESSAGE_MSG_RequestRegister
#define VNOC_C_MESSAGE_MSG_RequestRegister
#include "MessageDef.h"
#include "CMessage.h"
#include "MsgDataValue/StringData.h"
#include "MsgDataValue/NumData.hpp"

///> Generate by MessageProduse. DO NOT EDIT!

namespace VNOC
{
namespace Message
{

class MSG_RequestRegister : public CMessage
{
public:
    MSG_RequestRegister()
    {
        RegisterPort("Autograph", MsgDataMType_Data, MsgDataType_String);

        RegisterPort("EmailAddress", MsgDataMType_Data, MsgDataType_String);

        RegisterPort("HeadForm", MsgDataMType_Data, MsgDataType_Uint8);

        RegisterPort("HeadPortrait", MsgDataMType_Data, MsgDataType_String);

        RegisterPort("Nickname", MsgDataMType_Data, MsgDataType_String);

        RegisterPort("Rank", MsgDataMType_Data, MsgDataType_Uint8);

        InitializeMessage(MSG_RequestRegister_Id);
    }

    MSG_RequestRegister(const CMessage& Msg)
    {
        RegisterPort("Autograph", MsgDataMType_Data, MsgDataType_String);

        RegisterPort("EmailAddress", MsgDataMType_Data, MsgDataType_String);

        RegisterPort("HeadForm", MsgDataMType_Data, MsgDataType_Uint8);

        RegisterPort("HeadPortrait", MsgDataMType_Data, MsgDataType_String);

        RegisterPort("Nickname", MsgDataMType_Data, MsgDataType_String);

        RegisterPort("Rank", MsgDataMType_Data, MsgDataType_Uint8);

        CMessage::Copy(Msg, MSG_RequestRegister_Id);
    }

    virtual ~MSG_RequestRegister(){}

    MsgStatus SetAutograph(const std::string& Value)
    {
        return Write("Autograph", new StringData(Value));
    }

    MsgStatus SetEmailAddress(const std::string& Value)
    {
        return Write("EmailAddress", new StringData(Value));
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

    MsgStatus GetAutograph(std::string& Value) const
    {
        MsgDataValue* pReadValue = NULL;
        Read("Autograph", pReadValue);
        return pReadValue->ToStr(Value);
    }

    MsgStatus GetEmailAddress(std::string& Value) const
    {
        MsgDataValue* pReadValue = NULL;
        Read("EmailAddress", pReadValue);
        return pReadValue->ToStr(Value);
    }

    MsgStatus GetHeadForm(Define::uint8& Value) const
    {
        MsgDataValue* pReadValue = NULL;
        Read("HeadForm", pReadValue);
        return pReadValue->ToUInt8(Value);
    }

    MsgStatus GetHeadPortrait(std::string& Value) const
    {
        MsgDataValue* pReadValue = NULL;
        Read("HeadPortrait", pReadValue);
        return pReadValue->ToStr(Value);
    }

    MsgStatus GetNickname(std::string& Value) const
    {
        MsgDataValue* pReadValue = NULL;
        Read("Nickname", pReadValue);
        return pReadValue->ToStr(Value);
    }

    MsgStatus GetRank(Define::uint8& Value) const
    {
        MsgDataValue* pReadValue = NULL;
        Read("Rank", pReadValue);
        return pReadValue->ToUInt8(Value);
    }
};

}// namespace Message
}// namespace VNOC

#endif