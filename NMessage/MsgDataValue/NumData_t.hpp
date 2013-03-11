#ifndef VNOC_C_MESSAGE_UINT_T
#define VNOC_C_MESSAGE_UINT_T

#include "MsgDataValue.h"

namespace VNOC
{
namespace Message
{

template<typename TUIntType>
class NumData : public MsgDataValue
{
public:
    NumData(TUIntType Value) : m_Value(Value) {}
    virtual ~NumData() {}

    MsgStatus ToUInt8(Define::uint8& Value) {return MsgStatus_Err;}

    MsgStatus ToUInt16(Define::uint16& Value) {return MsgStatus_Err;}

    MsgStatus ToUInt32(Define::uint32& Value) {return MsgStatus_Err;}

    MsgStatus ToInt8(Define::int8& Value) {return MsgStatus_Err;}

    MsgStatus ToInt16(Define::int16& Value) {return MsgStatus_Err;}

    MsgStatus ToInt32(Define::int32& Value) {return MsgStatus_Err;}

    void SetValue(IN TUIntType& Value)
    {
        m_Value = Value;
    }

private:
    DISALLOW_ASSIGN(NumData);
    TUIntType m_Value;
};

}
}

#endif