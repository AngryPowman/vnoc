#ifndef  VNOC_I_MESSAGE_DATA_UINT16
#define  VNOC_I_MESSAGE_DATA_UINT16

#include "MsgDataValue.h"

namespace VNOC
{
namespace Message
{

class UInt16Data:public MsgDataValue
{
public:
    UInt16Data(){}
    virtual ~UInt16Data(){}

    UInt16Data(uint16 Value) { SetValue(Value);}

    virtual MsgStatus ToUInt16(uint16& Value);

    void SetValue(uint16 Value);
private:
    uint16      m_Value;
};

}// namespace Message
}// namespace VNOC

#endif