#include "CMessage.h"
#include "ParserMessageXML.h"

namespace VNOC
{
namespace Message
{

using namespace VNOC::Message::Define;

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

CMessage& CMessage::Copy(IReadMessage& lhs, int MessageId /* = 0 */)
{
    if (MessageId = 0)
    {
        InitializeMessage(lhs.MsgId());
    }
    else
    {
        InitializeMessage(MessageId);
    }
    MsgDataValue* lpReadValue = NULL;
    ArrayData* lpReadValueArr = NULL;
    m_mapMsgData.clear();
    m_mapMsgDataArr.clear();
    XMLObject* XmlObject = ParserMessageXML::Instance().GetMsgObject(lhs.MsgId());
    if (!XmlObject)
    {
        return (*this);
    }
    for (auto It = XmlObject->ParamBegin(); It != XmlObject->ParamEnd(); It++)
    {
        switch (It->second.GetMType())
        {
        case MsgDataMType_Data:
            lhs.Read(It->second.GetName(), lpReadValue);
            switch (It->second.GetType())
            {
            case MsgDataType_String:
                if (lpReadValue != NULL)
                {
                    std::string strValute;
                    lpReadValue->ToStr(strValute);
                    Write(It->second.GetName(), new StringData(strValute));
                }
                break;
            case MsgDataType_Uint8:
                if (lpReadValue != NULL)
                {
                    uint8 NumValute;
                    lpReadValue->ToUInt8(NumValute);
                    Write(It->second.GetName(), new NumData<uint8>(NumValute));
                }
                break;
            case MsgDataType_Uint16:
                if (lpReadValue != NULL)
                {
                    uint16 NumValute;
                    lpReadValue->ToUInt16(NumValute);
                    Write(It->second.GetName(), new NumData<uint16>(NumValute));
                }
                break;
            case MsgDataType_Uint32:
                if (lpReadValue != NULL)
                {
                    uint32 NumValute;
                    lpReadValue->ToUInt32(NumValute);
                    Write(It->second.GetName(), new NumData<uint32>(NumValute));
                }
                break;
            }
            break;
        case MsgDataMType_List:
            lhs.ReadArr(It->second.GetName(), lpReadValueArr);
            if (lpReadValueArr != NULL)
            {
                ArrayData* lpWriteArr = new ArrayData;
                for (auto ArrIt = lpReadValueArr->Begin();
                    ArrIt != lpReadValueArr->End();
                    ArrIt++
                    )
                {
                    lpWriteArr->Push((*ArrIt));
                }
                WriteArr(It->second.GetName(), lpWriteArr);
            }
            break;
        }
    }
    return (*this);
}

CMessage& CMessage::Copy(IReadMessage& lhs,const std::string MessageName)
{
    if (MessageName.empty())
    {
        InitializeMessage(lhs.MsgId());
    }
    else
    {
        InitializeMessage(MessageName);
    }
    MsgDataValue* lpReadValue = NULL;
    ArrayData* lpReadValueArr = NULL;
    m_mapMsgData.clear();
    m_mapMsgDataArr.clear();
    XMLObject* XmlObject = ParserMessageXML::Instance().GetMsgObject(lhs.MsgId());
    if (!XmlObject)
    {
        return (*this);
    }
    for (auto It = XmlObject->ParamBegin(); It != XmlObject->ParamEnd(); It++)
    {
        switch (It->second.GetMType())
        {
        case MsgDataMType_Data:
            lhs.Read(It->second.GetName(), lpReadValue);
            switch (It->second.GetType())
            {
            case MsgDataType_String:
                if (lpReadValue != NULL)
                {
                    std::string strValute;
                    lpReadValue->ToStr(strValute);
                    m_mapMsgData[It->second.GetName()] = new StringData(strValute);
                }
                break;
            case MsgDataType_Uint8:
                if (lpReadValue != NULL)
                {
                    uint8 NumValute;
                    lpReadValue->ToUInt8(NumValute);
                    m_mapMsgData[It->second.GetName()] = new NumData<uint8>(NumValute);
                }
                break;
            case MsgDataType_Uint16:
                if (lpReadValue != NULL)
                {
                    uint16 NumValute;
                    lpReadValue->ToUInt16(NumValute);
                    m_mapMsgData[It->second.GetName()] = new NumData<uint16>(NumValute);
                }
                break;
            case MsgDataType_Uint32:
                if (lpReadValue != NULL)
                {
                    uint32 NumValute;
                    lpReadValue->ToUInt32(NumValute);
                    m_mapMsgData[It->second.GetName()] = new NumData<uint32>(NumValute);
                }
                break;
            }
            break;
        case MsgDataMType_List:
            lhs.ReadArr(It->second.GetName(), lpReadValueArr);
            if (lpReadValueArr != NULL)
            {
                ArrayData* lpWriteArr = new ArrayData;
                for (auto ArrIt = lpReadValueArr->Begin();
                    ArrIt != lpReadValueArr->End();
                    ArrIt++
                    )
                {
                    lpWriteArr->Push((*ArrIt));
                }
                m_mapMsgDataArr[It->second.GetName()] = lpWriteArr;
            }
            break;
        }
    }
    return (*this);
}

}// namespace Message
}// namespace VNOC
