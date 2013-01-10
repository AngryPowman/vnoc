#ifndef  VNOC_I_MESSAGE_DATA_BYTE
#define  VNOC_I_MESSAGE_DATA_BYTE

#include "MsgDataValue.h"


namespace VNOC
{
namespace Message
{

class ByteData:public MsgDataValue
{
public:

    virtual MsgStatus ToByte(
        OUT Byte&  Value
        )
    { return MsgStatus_Ok;}

private:

    Byte      m_Value;

};



}// namespace Message
}// namespace VNOC

#endif