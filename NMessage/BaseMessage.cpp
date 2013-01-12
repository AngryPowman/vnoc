#include "BaseMessage.h"

VNOC::Message::MsgStatus VNOC::Message::BaseMessage::Read(
    IN const MsgDataName name,
    OUT MsgDataValue** value
    )
{
    if (*value == NULL)
    {
        return MsgStatus_Err;
    }
    auto tmpItr = m_mapMsgData.find(name);
    if (tmpItr != m_mapMsgData.end())
    {
        *value = &(tmpItr->second);
    }
    return MsgStatus_Ok;
}

VNOC::Message::MsgStatus VNOC::Message::BaseMessage::ReadArr(
    IN const MsgDataName name,
    OUT ArrayData** value
    )
{
    if (value == NULL)
    {
        return MsgStatus_Err;
    }
    auto tmpItr = m_mapMsgDataArr.find(name);
    if (tmpItr != m_mapMsgDataArr.end())
    {
        *value = &(tmpItr->second);
    }
    return MsgStatus_Ok;
}

VNOC::Message::MsgStatus VNOC::Message::BaseMessage::Write(
    IN const MsgDataName name,
    IN const MsgDataValue value
    )
{
    m_mapMsgData[name] = value;
    return MsgStatus_Ok;
}

VNOC::Message::MsgStatus VNOC::Message::BaseMessage::WriteArr(
    IN const MsgDataName name,
    IN const ArrayData value
    )
{
    m_mapMsgDataArr[name] = value;
    return MsgStatus_Ok;
}

VNOC::Message::MsgStatus VNOC::Message::BaseMessage::WriteArr(
    IN const MsgDataName name,
    IN const ArrayData value
    )
{
    m_mapMsgDataArr[name] = value;
    return MsgStatus_Ok;
}
