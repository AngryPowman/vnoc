#include "UInt32Data.h"

void VNOC::Message::UInt32Data::SetValue(uint32 Value)
{
    m_Value = Value;
}

VNOC::Message::MsgStatus VNOC::Message::UInt32Data::ToUInt32(uint32& Value)
{
    Value = m_Value;
    return MsgStatus_Ok;
}
