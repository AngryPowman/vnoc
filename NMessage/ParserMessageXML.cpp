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
        if (strcmp(msg->Value(),"msg") == 0)
        {
            //msg
            //name
            if (msg->Attribute("name")!= NULL)
            {
                m_NameList.push_back(msg->Attribute("name"));
            }
            else
            {
                m_NameList.push_back("NULL");
            }
            //id
            if (msg->Attribute("id") != NULL)
            {
                m_IdList.push_back(msg->Attribute("id"));
            }
            else
            {
                m_IdList.push_back("NULL");
            }
        }
    }
    //item
    for (
        msgItem = m_xmlTiny.FirstChild("vnoc")->FirstChild("msg")->FirstChildElement();
        msgItem != NULL;
        msgItem = msgItem->NextSiblingElement()
        )
    {
        //mname
        if (msgItem->Attribute("mtype") != NULL)
        {
            m_ItmeTypeOList.push_back(msgItem->Attribute("mtype"));
        }
        else
        {
            m_ItmeTypeOList.push_back("NULL");
        }
        //name
        if (msgItem->Attribute("name") != NULL)
        {
            m_ItmeNameList.push_back(msgItem->Attribute("name"));
        }
        else
        {
            m_ItmeNameList.push_back("NULL");
        }
        //type
        if (msgItem->Attribute("type") != NULL )
        {
            m_ItmeTypeTList.push_back(msgItem->Attribute("type"));
        }
        else
        {
            m_ItmeTypeTList.push_back("NULL");
        }
    }
}