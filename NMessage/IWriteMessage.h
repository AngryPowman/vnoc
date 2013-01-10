#ifndef  VNOC_I_MESSAGE_WRITE
#define  VNOC_I_MESSAGE_WRITE

#include "MessageDef.h"

namespace VNOC
{
namespace Message
{

class IWriteMessage
{
public:
    virtual ~IWriteMessage(){}

public:
    virtual MsgStatus Write(
        IN const MsgDataName& name,
        IN const MsgDataValue& value) = 0;

    virtual MsgStatus WriteArr(
        IN const MsgDataName& name,
        IN const ArrayData& value) = 0;
};

}// namespace Message
}// namespace VNOC

#endif