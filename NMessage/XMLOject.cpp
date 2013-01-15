#include "XMLObject.h"

namespace VNOC
{
namespace Message
{

XMLObject::XMLObject(const char* pName, int nId)
    : m_strName(pName)
    , m_Id(nId)
{
}

XMLObject::XMLObject(const XMLObject& obj)
{
    m_strName = obj.m_strName;
    m_Id = obj.m_Id;
    m_mapParamList = obj.m_mapParamList;
}

XMLItem* XMLObject::GetItem( std::string strName)
{
    auto Itr = m_mapParamList.find(strName);
    if (Itr != m_mapParamList.end())
    {
        return &(Itr->second);
    }
    return 0;
}

MsgStatus XMLObject::SetItem(std::string strName,XMLItem itemObject)
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