#ifndef  VNOC_C_MESSAGE_MSG
#define  VNOC_C_MESSAGE_MSG

#include "BaseMessage.h"
#include "MessageHead.h"
#include "MessageTail.h"
#include "MessageBody.h"

namespace VNOC
{
namespace Message
{

class Message:public BaseMessage
{
public:
    Message(){}
    virtual ~Message(){}
public:


protected:

    MessageHead m_MessageHead;
    MessageTail m_MessageTail;
    MessageBody m_Body;
};


}// namespace Message
}// namespace VNOC

#endif