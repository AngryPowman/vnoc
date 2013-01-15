#include "ParaserMessageXML.h"
#include <stdlib.h>

namespace VNOC
{
    namespace Message
    {

ParserMessageXML::ParserMessageXML()
{

}

ParserMessageXML::~ParserMessageXML()
{

}

MsgStatus ParserMessageXML::LoadFile(const char* strPath)
{
    if (strPath == NULL)
    {
        return MsgStatus_Err;
    }
    if(m_xmlTiny.LoadFile(strPath))
    {
        return _Parser() ? MsgStatus_Ok: MsgStatus_Err;
    }
    return MsgStatus_Err;
}

bool ParserMessageXML::_Parser()
{
    int MsgCount = 0;
    TiXmlElement* msgItem = NULL;
    TiXmlElement * msg = m_xmlTiny.RootElement();
    std::string sRootName = msg->Value();
    if (sRootName != "vnoc")
    {
        return false;
    }
    for (msg = m_xmlTiny.FirstChild("vnoc")->FirstChildElement();
        msg != NULL;
        msg = msg->NextSiblingElement())
    {
        if (strcmp(msg->Value(),"msg") == 0)
        {
            //msg
            const char* pName = msg->Attribute("name");
            const char* pId = msg->Attribute("id");
            if (!pName || !pId)
                return false;

            int nId = atoi(pId);
            XMLObject objXML(pName, nId);
            m_MsgIdList[std::string(pName)] = nId;

            //item
            for (msgItem = msg->FirstChild()->ToElement();
                msgItem != NULL;
                msgItem = msgItem->NextSiblingElement())
            {
                XMLItem ItemXML;
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
                objXML.SetItem(ItemXML.GetName(), ItemXML);
            }

            m_MsgObjectList.insert(std::make_pair(objXML.GetId(), objXML));
        }
    }
    return true;
}

XMLObject* ParserMessageXML::GetOjbect(const std::string& strName)
{
    auto itFind = m_MsgIdList.find(strName);
    if (itFind != m_MsgIdList.end())
    {
        auto Itr = m_MsgObjectList.find(itFind->second);
        if (Itr != m_MsgObjectList.end())
        {
            return &(Itr->second);
        }
    }

    return 0;
}

XMLObject* ParserMessageXML::GetOjbect(int nId)
{
    auto Itr = m_MsgObjectList.find(nId);
    if (Itr != m_MsgObjectList.end())
    {
        return &(Itr->second);
    }
    return 0;
}

}// namespace Message
}// namespace VNOC