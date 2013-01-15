#include "Message.h"

namespace VNOC
{
namespace Message
{

Message::Message() : m_XML(NULL)
{
}

Message::~Message()
{
}

Message::Message(
    IN const char* strPath) : m_XML(NULL)
{
    if (strPath == NULL)
    {
        return;
    }
    m_ParserXML.LoadFile(strPath);
}

MsgStatus Message::LoadXML(
    IN const char* strPath)
{
    if (strPath == NULL)
    {
        return MsgStatus_Err;
    }
    return m_ParserXML.LoadFile(strPath);
}

bool Message::SetMessage(
    IN const std::string& strName
    )
{
    m_XML = m_ParserXML.GetObject(strName);
    if (m_XML != NULL)
    {
        return true;
    }
    return false;
}

bool Message::SetMessage(
    IN int nId
    )
{
    m_XML = m_ParserXML.GetObject(nId);
    if (m_XML != NULL)
    {
        return true;
    }
    return false;
}

MsgStatus Message::Write(
    IN const std::string& strName,
    IN const MsgDataValue& strValue
    )
{
    if (!m_XML)
    {
        return MsgStatus_Err;
    }
    XMLItem* Item = m_XML->GetItem(strName);
    if (Item != NULL)
    {
        return BaseMessage::Write(strName,strValue);
    }
    return MsgStatus_Err;
}

MsgStatus Message::Write(
    IN const std::string& strName,
    IN const ArrayData& strValue
    )
{
    if (!m_XML)
    {
        return MsgStatus_Err;
    }
    XMLItem* Item = m_XML->GetItem(strName);
    if (Item != NULL)
    {
        return BaseMessage::WriteArr(strName,strValue);
    }
    return MsgStatus_Err;
}

MsgStatus Message::Read(
    IN const std::string& strName,
    IN MsgDataValue*& strValue
    )
{
    if (!m_XML)
    {
        return MsgStatus_Err;
    }
    XMLItem* Item = m_XML->GetItem(strName);
    if (Item != NULL)
    {
        return BaseMessage::Read(strName,strValue);
    }
    return MsgStatus_Err;
}

MsgStatus Message::Read(
    IN const std::string& strName,
    IN ArrayData*& strValue
    )
{
    if (!m_XML)
    {
        return MsgStatus_Err;
    }
    XMLItem* Item = m_XML->GetItem(strName);
    if (Item != NULL)
    {
        return BaseMessage::ReadArr(strName,strValue);
    }
    return MsgStatus_Err;
}

}// namespace Message
}// namespace VNOC