#ifndef  VNOC_I_MESSAGE_DATA_UINT8
#define  VNOC_I_MESSAGE_DATA_UINT8

#include "MsgDataValue.h"

namespace VNOC
{
namespace Message
{

class UInt8Data : public MsgDataValue
{
public:
    UInt8Data(uint8 Value = 0);
    virtual ~UInt8Data();

    virtual MsgStatus ToUInt8(OUT uint8& Value);

    void SetValue(IN uint8 Value);

private:
    DISALLOW_ASSIGN(UInt8Data);
    uint8 m_Value;
};

}// namespace Message
}// namespace VNOC

#endif