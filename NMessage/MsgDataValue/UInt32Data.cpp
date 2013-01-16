#include "UInt32Data.h"

namespace VNOC
{
namespace Message
{

void UInt32Data::SetValue(uint32 Value)
{
    m_Value = Value;
}

MsgStatus UInt32Data::ToUInt32(uint32& Value)
{
    Value = m_Value;
    return MsgStatus_Ok;
}

UInt32Data::UInt32Data(uint32 Value)
    : m_Value(Value)
{

}

UInt32Data::~UInt32Data()
{

}

}// namespace Message
}// namespace VNOC
