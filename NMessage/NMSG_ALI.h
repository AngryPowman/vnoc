#ifndef VNOC_C_MESSAGE_NMSG_ALI
#define VNOC_C_MESSAGE_NMSG_ALI
#include "MessageDef.h"
#include "CMessage.h"
#include "MsgDataValue/StringData.h"
#include "MsgDataValue/UInt32Data.h"
#include "MsgDataValue/UInt16Data.h"
#include "MsgDataValue/UInt8Data.h"
#include "MsgDataValue/ArrayData.h"
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
        CMessage("NMSG_ALI");
    }

    virtual ~NMSG_ALI(){}

    MsgStatus SetATLGUID(ArrayData& Value)
    {
        return WriteArr("ATLGUID", &Value);
    }

    MsgStatus SetLoginResult(UInt8Data& Value)
    {
        return Write("LoginResult", &Value);
    }

    MsgStatus SetToken(UInt32Data& Value)
    {
        return Write("Token", &Value);
    }

    MsgStatus GetATLGUID(ArrayData*& Value)
    {
        return ReadArr("ATLGUID", Value);
    }

    MsgStatus GetLoginResult(UInt8Data*& Value)
    {
        return Read("LoginResult", (MsgDataValue*&)Value);
    }

    MsgStatus GetToken(UInt32Data*& Value)
    {
        return Read("Token", (MsgDataValue*&)Value);
    }
};

}// namespace Message
}// namespace VNOC

#endif