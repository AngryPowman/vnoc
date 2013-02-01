#ifndef VNOC_C_MESSAGE_?Name
#define VNOC_C_MESSAGE_?Name
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

class ?Name : public CMessage
{
public:
    ?Name()
    {
        InitializeMessage("?Name");
    }

    virtual ~?Name(){}

?SetParam

?GetParam
};

}// namespace Message
}// namespace VNOC

#endif