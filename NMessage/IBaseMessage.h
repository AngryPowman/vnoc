

#ifndef  VNOC_I_MESSAGE_BASE
#define  VNOC_I_MESSAGE_BASE

#include "IWriteMessage.h"
#include "IReadMessage.h"

namespace VNOC
{
namespace Message
{

class IMessage:public IReadMessage,IWriteMessage
{
public:

    IMessage(){}
    virtual ~IMessage(){}

public:

    virtual int         Read(){}

    virtual int         Write(){}

};

}// namespace Message
}// namespace VNOC
#endif