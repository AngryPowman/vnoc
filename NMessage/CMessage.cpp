#include "CMessage.h"
#include "ParserMessageXML.h"

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
            return MsgStatus_Ok;
        }
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
            return MsgStatus_Ok;
        }
    }
    return MsgStatus_Err;
}

MsgStatus CMessage::Write(
    IN const MsgDataName& name,
    IN MsgDataValue* pValue
    )
{
    if (!m_xmlObject)
    {
        return MsgStatus_Err;
    }
    XMLItem* Item = m_xmlObject->GetItem(name);
    if (Item != NULL)
    {
        auto itFind = m_mapMsgData.find(name);
        if (itFind == m_mapMsgData.end())
        {
            return MsgStatus_Err;
        }
        delete itFind->second;
        itFind->second = pValue;
        return MsgStatus_Ok;
    }
    return MsgStatus_Err;
}

MsgStatus CMessage::WriteArr(
    IN const MsgDataName& name,
    IN ArrayData* pValue
    )
{
    if (!m_xmlObject)
    {
        return MsgStatus_Err;
    }
    XMLItem* Item = m_xmlObject->GetItem(name);
    if (Item != NULL)
    {
        auto itFind = m_mapMsgDataArr.find(name);
        if (itFind == m_mapMsgDataArr.end())
        {
            return MsgStatus_Err;
        }
        delete itFind->second;
        itFind->second = pValue;
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
    _InitDataMap();
}

CMessage::CMessage( int nId )
{
    m_xmlObject = ParserMessageXML::Instance().GetMsgObject(nId);
    _InitDataMap();
}

void CMessage::InitializeMessage( const std::string& strName )
{
    m_xmlObject = ParserMessageXML::Instance().GetMsgObject(strName);
    _InitDataMap();
}

void CMessage::InitializeMessage( int nId )
{
    m_xmlObject = ParserMessageXML::Instance().GetMsgObject(nId);
    _InitDataMap();
}

CMessage::~CMessage()
{
    _ReleaseMap();
}

bool CMessage::IsValid()
{
    return m_xmlObject != NULL;
}

void CMessage::_InitDataMap()
{
    if (m_xmlObject)
    {
        auto it = m_xmlObject->ParamBegin();
        for (; it != m_xmlObject->ParamEnd(); ++it)
        {
            if (it->second.GetMType() == MsgDataMType_Data)
            {
                m_mapMsgData[it->first] = NULL;
            }
            if (it->second.GetMType() == MsgDataMType_List)
            {
                m_mapMsgDataArr[it->first] = NULL;
            }
        }
    }
}

void CMessage::_ReleaseMap()
{
    auto itData = m_mapMsgData.begin();
    for (; itData != m_mapMsgData.end(); ++ itData)
    {
        delete itData->second;
    }
    m_mapMsgData.clear();
    auto itList = m_mapMsgDataArr.begin();
    for (; itList != m_mapMsgDataArr.end(); ++ itList)
    {
        delete itList->second;
    }
    m_mapMsgDataArr.clear();
}

int CMessage::MsgId()
{
    int nId = 0;
    if (m_xmlObject)
    {
        nId = m_xmlObject->GetId();
    }
    return nId;
}

}// namespace Message
}// namespace VNOC
