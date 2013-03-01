#ifndef VNOC_C_MESSAGE_?Name
#define VNOC_C_MESSAGE_?Name
#include "MessageDef.h"
#include "CMessage.h"
#include "MsgDataValue/StringData.h"
#include "MsgDataValue/NumData.hpp"

///> Generate by MessageProduse. DO NOT EDIT!

namespace VNOC
{
namespace Message
{

class ?Name : public CMessage
{
public:
    ?Name()
    {
        ?RegisterPort
        InitializeMessage(?Id);
    }

    ?Name(const CMessage& Msg)
    {
        ?RegisterPort
        CMessage::Copy(Msg, ?Id);
    }

    virtual ~?Name(){}

?SetParam

?GetParam
};

}// namespace Message
}// namespace VNOC

#endif