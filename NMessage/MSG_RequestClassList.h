#ifndef VNOC_C_MESSAGE_MSG_RequestClassList
#define VNOC_C_MESSAGE_MSG_RequestClassList
#include "MessageDef.h"
#include "CMessage.h"
#include "MsgDataValue/StringData.h"
#include "MsgDataValue/NumData.hpp"

///> Generate by MessageProduse. DO NOT EDIT!

namespace VNOC
{
namespace Message
{

class MSG_RequestClassList : public CMessage
{
public:
    MSG_RequestClassList()
    {
        RegisterPort("UserType", MsgDataMType_Data, MsgDataType_Uint32);

        InitializeMessage(MSG_RequestClassList_Id);
    }

    MSG_RequestClassList(const CMessage& Msg)
    {
        RegisterPort("UserType", MsgDataMType_Data, MsgDataType_Uint32);

        CMessage::Copy(Msg, MSG_RequestClassList_Id);
    }

    virtual ~MSG_RequestClassList(){}

    MsgStatus SetUserType(const Define::uint32& Value)
    {
        return Write("UserType", new NumData<Define::uint32>(Value));
    }

    MsgStatus GetUserType(Define::uint32& Value) const
    {
        MsgDataValue* pReadValue = NULL;
        Read("UserType", pReadValue);
        return pReadValue->ToUInt32(Value);
    }
};

}// namespace Message
}// namespace VNOC

#endif