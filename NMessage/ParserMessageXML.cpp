#include "ParaserMessageXML.h"
#include "../TinyXML/tinyxml.h"
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
    TiXmlDocument xmlTiny;
    if(xmlTiny.LoadFile(strPath))
    {
        return _Parser(xmlTiny) ? MsgStatus_Ok: MsgStatus_Err;
    }
    return MsgStatus_Err;
}

bool ParserMessageXML::_Parser(TiXmlDocument& xmlTiny)
{
    int MsgCount = 0;
    TiXmlElement* msgItem = NULL;
    TiXmlElement* msg = xmlTiny.RootElement();
    std::string sRootName = msg->Value();
    if (sRootName != MsgDataObject_XML_Root)
    {
        return false;
    }
    TiXmlNode* msgNode = xmlTiny.FirstChild(MsgDataObject_XML_Root);
    if (msgNode == NULL)
    {
        return false;
    }
    for (msg = msgNode->FirstChildElement();
        msg != NULL;
        msg = msg->NextSiblingElement())
    {
        if (strcmp(msg->Value(),MsgDataObject_XML_Msg) == 0)
        {
            const char* pName = msg->Attribute(MsgDataObject_XML_Name);
            const char* pId = msg->Attribute(MsgDataObject_XML_Id);
            if (!pName || !pId)
            {
                continue;
            }
            int nId = atoi(pId);
            XMLObject objXML(pName, nId);
            m_MsgIdList[std::string(pName)] = nId;
            TiXmlNode* tmpNode = msg->FirstChild();
            if (tmpNode == NULL)
            {
                continue;
            }
            for (msgItem = tmpNode->ToElement();
                msgItem != NULL;
                msgItem = msgItem->NextSiblingElement())
            {
                XMLItem ItemXML;
                if (msgItem->Attribute(MsgDataObject_XML_MType) != NULL)
                {
                    std::string strMType = msgItem->Attribute(MsgDataObject_XML_MType);
                    if (strMType == MsgDataMType_XML_Data)
                    {
                        ItemXML.SetMType(MsgDataMType_Data);
                    }
                    if (strMType == MsgDataMType_XML_List)
                    {
                        ItemXML.SetMType(MsgDataMType_List);
                    }
                }
                if (msgItem->Attribute(MsgDataObject_XML_Name) != NULL)
                {
                    ItemXML.SetName(msgItem->Attribute(MsgDataObject_XML_Name));
                }
                if (msgItem->Attribute(MsgDataObject_XML_Type) != NULL )
                {
                    std::string strType = msgItem->Attribute(MsgDataObject_XML_Type);
                    if (strType == MsgDataType_XML_Dword)
                    {
                        ItemXML.SetType(MsgDataType_Dword);
                    }
                    if (strType == MsgDataType_XML_String)
                    {
                        ItemXML.SetType(MsgDataType_String);
                    }
                    if (strType == MsgDataType_XML_Byte)
                    {
                        ItemXML.SetType(MsgDataType_Byte);
                    }
                }
                objXML.SetItem(ItemXML.GetName(), ItemXML);
            }
            m_MsgObjectList.insert(std::make_pair(objXML.GetId(), objXML));
        }
    }
    return true;
}

XMLObject* ParserMessageXML::GetMsgObject(const std::string& strName)
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

XMLObject* ParserMessageXML::GetMsgObject(int nId)
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