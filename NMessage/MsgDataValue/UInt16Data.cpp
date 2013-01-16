#include "UInt16Data.h"

namespace VNOC
{
namespace Message
{

void UInt16Data::SetValue(uint16 Value)
{
    m_Value = Value;
}

MsgStatus UInt16Data::ToUInt16(uint16& Value)
{
    Value = m_Value;
    return MsgStatus_Ok;
}

UInt16Data::UInt16Data(uint16 Value)
    : m_Value(Value)
{

}

UInt16Data::~UInt16Data()
{

}

}// namespace Message
}// namespace VNOC
