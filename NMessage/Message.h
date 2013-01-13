#ifndef  VNOC_C_MESSAGE_MSG
#define  VNOC_C_MESSAGE_MSG

#include "NBaseMessage.h"

namespace VNOC
{
namespace Message
{

class Message:public BaseMessage
{
public:
    Message(){}
    virtual ~Message(){}
};

}// namespace Message
}// namespace VNOC

#endif