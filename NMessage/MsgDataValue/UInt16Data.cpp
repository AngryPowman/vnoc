#include "UInt16Data.h"

void VNOC::Message::UInt16Data::SetValue(uint16 Value)
{
    m_Value = Value;
}

VNOC::Message::MsgStatus VNOC::Message::UInt16Data::ToUInt16(uint16& Value)
{
    Value = m_Value;
    return MsgStatus_Ok;
}
