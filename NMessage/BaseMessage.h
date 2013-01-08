
#ifndef  VNOC_C_MESSAGE_BASE
#define  VNOC_C_MESSAGE_BASE

#include "IBaseMessage.h"
#include <string>

using std::string;

typedef unsigned int byte;
typedef unsigned int uint;
typedef std::basic_string<byte>     ByteArr;

namespace VNOC
{
namespace Message
{


class BaseMessage:public IMessage
{
public:

    BaseMessage(){}
    virtual ~BaseMessage(){}

    int       Read(ByteArr& out_Data)  const;
    
    int         Write(byte* in_Data_ptr,uint  in_len);

private:

    ByteArr         m_Data;

};


}// namespace Message
}// namespace VNOC
#endif
