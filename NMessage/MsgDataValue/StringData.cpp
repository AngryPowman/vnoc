#include "StringData.h"

void VNOC::Message::StringData::SetValue(std::string Value)
{
    m_Value = Value;
}

VNOC::Message::MsgStatus VNOC::Message::StringData::ToStr(std::string& Value)
{
    Value = m_Value;
    return MsgStatus_Ok;
}
