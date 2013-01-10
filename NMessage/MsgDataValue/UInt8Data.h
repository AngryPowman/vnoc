#ifndef  VNOC_I_MESSAGE_DATA_UINT8
#define  VNOC_I_MESSAGE_DATA_UINT8

#include "MsgDataValue.h"


namespace VNOC
{
namespace Message
{

class UInt8Data:public MsgDataValue
{
public:
    UInt8Data(){}
    virtual ~UInt8Data(){};

    virtual MsgStatus ToByte(
        OUT uint8& Value
        )
    {
        Value = m_Value;
        return MsgStatus_Ok;
    }

private:
    uint8      m_Value;

};



}// namespace Message
}// namespace VNOC

#endif