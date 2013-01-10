#ifndef  VNOC_I_MESSAGE_DATA_UINT32
#define  VNOC_I_MESSAGE_DATA_UINT32

#include "MsgDataValue.h"

namespace VNOC
{
namespace Message
{

class UInt32Data:public MsgDataValue
{
public:
    UInt32Data(){}
    virtual ~UInt32Data(){}

    virtual MsgStatus ToUInt32(
        OUT uint32& Value
        )
    {
        Value = m_Value;
        return MsgStatus_Ok;
    }
private:
    uint32      m_Value;
};

}// namespace Message
}// namespace VNOC

#endif