#include "CMessage.h"
#include "ParaserMessageXML.h"

namespace VNOC
{
namespace Message
{

MsgStatus CMessage::Read(
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

MsgStatus CMessage::ReadArr(
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

MsgStatus CMessage::Write(
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

MsgStatus CMessage::WriteArr(
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

CMessage::CMessage() : m_xmlObject(NULL)
{

}

CMessage::CMessage( const std::string& strName )
{
    m_xmlObject = ParserMessageXML::Instance().GetMsgObject(strName);
}

CMessage::CMessage( int nId )
{
    m_xmlObject = ParserMessageXML::Instance().GetMsgObject(nId);
}

CMessage::~CMessage()
{

}

bool CMessage::IsValid()
{
    return m_xmlObject != NULL;
}

}// namespace Message
}// namespace VNOC
