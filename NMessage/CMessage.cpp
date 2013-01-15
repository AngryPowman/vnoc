#include "CMessage.h"

VNOC::Message::MsgStatus VNOC::Message::BaseMessage::Read(
    IN const MsgDataName& name,
    OUT MsgDataValue*& value
    )
{
    auto tmpItr = m_mapMsgData.find(name);
    if (tmpItr != m_mapMsgData.end())
    {
        value = tmpItr->second;
    }
    return MsgStatus_Ok;
}

VNOC::Message::MsgStatus VNOC::Message::BaseMessage::ReadArr(
    IN const MsgDataName& name,
    OUT ArrayData*& value
    )
{
    auto tmpItr = m_mapMsgDataArr.find(name);
    if (tmpItr != m_mapMsgDataArr.end())
    {
        value = tmpItr->second;
    }
    return MsgStatus_Ok;
}

VNOC::Message::MsgStatus VNOC::Message::BaseMessage::Write(
    IN const MsgDataName& name,
    IN const MsgDataValue& value
    )
{
    m_mapMsgData[name] = (MsgDataValue*)&value;
    return MsgStatus_Ok;
}

VNOC::Message::MsgStatus VNOC::Message::BaseMessage::WriteArr(
    IN const MsgDataName& name,
    IN const ArrayData& value
    )
{
    m_mapMsgDataArr[name] = (ArrayData*)&value;
    return MsgStatus_Ok;
}
