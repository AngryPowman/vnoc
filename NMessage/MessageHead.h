

#ifndef  VNOC_C_MESSAGE_HEAD
#define  VNOC_C_MESSAGE_HEAD

#include "BaseMessage.h"

namespace VNOC
{
namespace Message
{

class MessageHead:public BaseMessage
{
public:
    MessageHead(){}
    virtual ~MessageHead(){}
public:


private:

    ByteArr  m_Head;

};


}// namespace Message
}// namespace VNOC

#endif