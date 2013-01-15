#include "CMessage.h"

VNOC::Message::MsgStatus VNOC::Message::CMessage::Read(
    IN const MsgDataName& name,
    OUT MsgDataValue*& value
    )
{
    if (!m_xmlObject)
    {
        return MsgStatus_Err;
    }
    XMLItem* Item = m_xmlObject->GetItem(name);
    if (Item != NULL)
    {
        auto tmpItr = m_mapMsgData.find(name);
        if (tmpItr != m_mapMsgData.end())
        {
            value = tmpItr->second;
        }
        return MsgStatus_Ok;
    }
    return MsgStatus_Err;
}

VNOC::Message::MsgStatus VNOC::Message::CMessage::ReadArr(
    IN const MsgDataName& name,
    OUT ArrayData*& value
    )
{
    if (!m_xmlObject)
    {
        return MsgStatus_Err;
    }
    XMLItem* Item = m_xmlObject->GetItem(name);
    if (Item != NULL)
    {
        auto tmpItr = m_mapMsgDataArr.find(name);
        if (tmpItr != m_mapMsgDataArr.end())
        {
            value = tmpItr->second;
        }
        return MsgStatus_Ok;
    }
    return MsgStatus_Err;
}

VNOC::Message::MsgStatus VNOC::Message::CMessage::Write(
    IN const MsgDataName& name,
    IN const MsgDataValue& value
    )
{
    if (!m_xmlObject)
    {
        return MsgStatus_Err;
    }
    XMLItem* Item = m_xmlObject->GetItem(name);
    if (Item != NULL)
    {
        m_mapMsgData[name] = (MsgDataValue*)&value;
        return MsgStatus_Ok;
    }
    return MsgStatus_Err;
}

VNOC::Message::MsgStatus VNOC::Message::CMessage::WriteArr(
    IN const MsgDataName& name,
    IN const ArrayData& value
    )
{
    if (!m_xmlObject)
    {
        return MsgStatus_Err;
    }
    XMLItem* Item = m_xmlObject->GetItem(name);
    if (Item != NULL)
    {
        m_mapMsgDataArr[name] = (ArrayData*)&value;
        return MsgStatus_Ok;
    }
    return MsgStatus_Err;
}

bool VNOC::Message::CMessage::SetMessage(
    IN const std::string& strName,
    IN ParserMessageXML& parserXML
    )
{
    m_xmlObject = parserXML.GetObject(strName);
    if (m_xmlObject != NULL)
    {
        return true;
    }
    return false;
}

bool VNOC::Message::CMessage::SetMessage(
    IN int nId,
    IN ParserMessageXML& parserXML )
{
    m_xmlObject = parserXML.GetObject(nId);
    if (m_xmlObject != NULL)
    {
        return true;
    }
    return false;
}
