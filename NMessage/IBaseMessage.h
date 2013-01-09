#ifndef  VNOC_I_MESSAGE_BASE
#define  VNOC_I_MESSAGE_BASE

#include "IWriteMessage.h"
#include "IReadMessage.h"

namespace VNOC
{
namespace Message
{

class IMessage
    : public IReadMessage
    , public IWriteMessage
{
public:
    virtual ~IMessage(){}

public:
    virtual MsgStatus Read(
        IN const MsgDataName& name,
        OUT MsgDataValue& value) = 0;
    virtual MsgStatus Write(
        IN const MsgDataName& name,
        IN const MsgDataValue& value) = 0;
};

}// namespace Message
}// namespace VNOC

#endif