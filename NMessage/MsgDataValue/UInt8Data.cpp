#include "UInt8Data.h"

void VNOC::Message::UInt8Data::SetValue(uint8 Value)
{
    m_Value = Value;
}

VNOC::Message::MsgStatus VNOC::Message::UInt8Data::ToByte(uint8& Value)
{
    Value = m_Value;
    return MsgStatus_Ok;
}
