#ifndef  VNOC_C_MESSAGE_BUFFER
#define  VNOC_C_MESSAGE_BUFFER

#include "Message.h"

namespace VNOC
{
namespace Message
{

class CMessageBuffer
{
public:
    CMessageBuffer(){}
    virtual ~CMessageBuffer(){}
public:
private:
    Message* m_Message_ptr;
};

}// namespace Message
}// namespace VNOC

#endif