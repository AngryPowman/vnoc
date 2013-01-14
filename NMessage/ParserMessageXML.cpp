#include "ParaserMessageXML.h"

VNOC::Message::MsgStatus VNOC::Message::ParserMessageXML::LoadFile(
    const char* strPath
    )
{
    if (strPath == NULL)
    {
        return MsgStatus_Err;
    }
    if(m_xmlTiny.LoadFile(strPath))
    {
        _Parser();
        return MsgStatus_Ok;
    }
    return MsgStatus_Err;
}

void VNOC::Message::ParserMessageXML::_Parser()
{
    int MsgCount = 0;
    TiXmlElement* msgItem = NULL;
    TiXmlElement * msg = m_xmlTiny.RootElement();
    m_RootName = msg->Value();
    if (m_RootName != "vnoc")
    {
        return;
    }
    for (
        msg = m_xmlTiny.FirstChild("vnoc")->FirstChildElement();
        msg != NULL;
        msg = msg->NextSiblingElement()
        )
    {
        VNOC::Message::XMLObject objXML;
        if (strcmp(msg->Value(),"msg") == 0)
        {
            //msg
            //name
            if (msg->Attribute("name")!= NULL)
            {
                m_NameList.push_back(msg->Attribute("name"));
                objXML.SetName(msg->Attribute("name"));
            }
            else
            {
                m_NameList.push_back("NULL");
            }
            //id
            if (msg->Attribute("id") != NULL)
            {
                m_IdList.push_back(msg->Attribute("id"));
                objXML.SetId(msg->Attribute("id"));
            }
            else
            {
                m_IdList.push_back("NULL");
            }
            //item
            for (msgItem = msg->FirstChild()->ToElement();
                msgItem != NULL;
                msgItem = msgItem->NextSiblingElement()
                )
            {
                VNOC::Message::XMLItem   ItemXML;
                if (msgItem->Attribute("mtype") != NULL)
                {
                    ItemXML.SetMType(msgItem->Attribute("mtype"));
                }
                //name
                if (msgItem->Attribute("name") != NULL)
                {
                    ItemXML.SetName(msgItem->Attribute("name"));
                }
                //type
                if (msgItem->Attribute("type") != NULL )
                {
                    ItemXML.SetType(msgItem->Attribute("type"));
                }
                objXML.SetItem(ItemXML.GetName(),ItemXML);
            }
        }
        m_MsgObjectList[objXML.GetName()] = objXML;
    }
    for (uint32 index = 0; index < m_NameList.size(); index++)
    {
        m_MsgIdList[m_NameList[index]] = m_IdList[index];
    }
}

VNOC::Message::XMLObject* VNOC::Message::ParserMessageXML::GetOjbect(
    std::string strName 
    )
{
    auto Itr = m_MsgObjectList.find(strName);
    if (Itr != m_MsgObjectList.end())
    {
        return &(Itr->second);
    }
    return 0;
}