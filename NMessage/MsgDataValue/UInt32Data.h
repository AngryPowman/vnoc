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

    virtual MsgStatus ToUInt32(uint32& Value);

    void SetValue(uint32 Value);
private:
    uint32      m_Value;
};

}// namespace Message
}// namespace VNOC

#endif