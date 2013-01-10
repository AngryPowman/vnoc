#ifndef  VNOC_I_MESSAGE_READ
#define  VNOC_I_MESSAGE_READ

#include "MessageDef.h"

namespace VNOC
{
namespace Message
{

class ArrayData;

class IReadMessage
{
public:
    virtual ~IReadMessage(){}

public:
    virtual MsgStatus Read(
        IN const MsgDataName& name,
        OUT MsgDataValue& value) = 0;

    virtual MsgStatus ReadArr(
        IN  const MsgDataName& name,
        OUT ArrayData& value) = 0;
};

}// namespace Message
}// namespace VNOC

#endif