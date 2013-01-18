#ifndef VNOC_C_MESSAGE_?Name
#define VNOC_C_MESSAGE_?Name
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

class ?Name : public CMessage
{
public:
    ?Name()
    {
        CMessage("?Name");
    }

    virtual ~?Name(){}

?SetParam

?GetParam
};

}// namespace Message
}// namespace VNOC

#endif