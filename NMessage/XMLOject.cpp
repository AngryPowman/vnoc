#include "XMLObject.h"


VNOC::Message::XMLItem* VNOC::Message::XMLObject::GetItem( 
    std::string strName
    )
{
    auto Itr = m_mapParamList.find(strName);
    if (Itr != m_mapParamList.end())
    {
        return &(Itr->second);
    }
    return 0;
}

VNOC::Message::MsgStatus VNOC::Message::XMLObject::SetItem(
    std::string strName,
    XMLItem itemObject
    )
{
    if (strName.empty())
    {
        return MsgStatus_Err;
    }
    m_mapParamList[strName] = itemObject;
    return MsgStatus_Ok;
}
