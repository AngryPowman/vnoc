#ifndef  VNOC_C_MESSAGE_TAIL
#define  VNOC_C_MESSAGE_TAIL

#include "BaseMessage.h"

namespace VNOC
{
namespace Message
{

class MessageTail:public BaseMessage
{
public:
    MessageTail(){}
    virtual ~MessageTail(){}
public:



private:

    ByteArr  m_Tail;

};


}// namespace Message
}// namespace VNOC

#endif