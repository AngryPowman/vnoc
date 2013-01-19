#include "UInt8Data.h"

namespace VNOC
{
namespace Message
{

void UInt8Data::SetValue(uint8 Value)
{
    m_Value = Value;
}

MsgStatus UInt8Data::ToUInt8(uint8& Value)
{
    Value = m_Value;
    return MsgStatus_Ok;
}

UInt8Data::UInt8Data(uint8 Value)
    : m_Value(Value)
{

}

UInt8Data::~UInt8Data()
{

}

}// namespace Message
}// namespace VNOC