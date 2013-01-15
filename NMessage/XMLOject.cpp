#include "XMLObject.h"

namespace VNOC
{
namespace Message
{
//XMLITEM
void XMLItem::SetMType(
    IN MsgDataMType strValue
    )
{
    m_strMType = strValue;
}

void XMLItem::SetType(
    IN MsgDataType strValue
    )
{
    m_strType = strValue;
}

void XMLItem::SetName(
    IN const std::string& strValue
    )
{
    m_strName = strValue;
}

MsgDataMType XMLItem::GetMType()
{
    return m_strMType;
}

std::string XMLItem::GetName()
{ 
    return m_strName;
}

MsgDataType XMLItem::GetType()
{
    return m_strType;
}
//XMLOBJECT
XMLObject::XMLObject(
    IN const char* pName, int nId
    )
    : m_strName(pName)
    , m_Id(nId)
{
}

XMLObject::~XMLObject()
{
}

XMLObject::XMLObject(
    IN const XMLObject& obj
    )
{
    m_strName = obj.m_strName;
    m_Id = obj.m_Id;
    m_mapParamList = obj.m_mapParamList;
}

XMLItem* XMLObject::GetItem(
    IN const std::string& strName
    )
{
    auto Itr = m_mapParamList.find(strName);
    if (Itr != m_mapParamList.end())
    {
        return &(Itr->second);
    }
    return 0;
}

std::string XMLObject::GetName()
{
    return m_strName;
}

int XMLObject::GetId()
{
    return m_Id;
}

MsgStatus XMLObject::SetItem(
    IN const std::string& strName,
    IN XMLItem itemObject
    )
{
    if (strName.empty())
    {
        return MsgStatus_Err;
    }
    m_mapParamList[strName] = itemObject;
    return MsgStatus_Ok;
}

}// namespace Message
}// namespace VNOC