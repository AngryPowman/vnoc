
#ifndef  VNOC_I_MESSAGE_WRITE
#define  VNOC_I_MESSAGE_WRITE

namespace VNOC
{
namespace Message
{

class IWriteMessage
{
public:

    IWriteMessage(){}
    virtual ~IWriteMessage(){}

public:

    virtual int         Write() = 0;

};

}// namespace Message
}// namespace VNOC


#endif