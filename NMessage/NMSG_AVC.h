#ifndef VNOC_C_MESSAGE_NMSG_AVC
#define VNOC_C_MESSAGE_NMSG_AVC
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

class NMSG_AVC : public CMessage
{
public:
    NMSG_AVC()
    {
        CMessage("NMSG_AVC");
    }

    virtual ~NMSG_AVC(){}

    MsgStatus SetTest(ArrayData& Value)
    {
        return WriteArr("Test", &Value);
    }

    MsgStatus GetTest(ArrayData*& Value)
    {
        return ReadArr("Test", Value);
    }
};

}// namespace Message
}// namespace VNOC

#endif