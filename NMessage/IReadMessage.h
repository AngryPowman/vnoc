
#ifndef  VNOC_I_MESSAGE_READ
#define  VNOC_I_MESSAGE_READ

namespace VNOC
{
namespace Message
{

class IReadMessage
{
public:

    IReadMessage(){}
    virtual ~IReadMessage(){}
public:

    virtual int         Read() = 0;

};


}// namespace Message
}// namespace VNOC

#endif