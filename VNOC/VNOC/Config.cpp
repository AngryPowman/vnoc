#include "StdAfx.h"
#include "Config.h"
#include "../public/util.h"

//////////////////////////////////////////////////////////////////////////
// CConfig

CConfig::CConfig(void)
{
}

CConfig::~CConfig(void)
{
}

BOOL CConfig::LoadConfigFromXML( LPCTSTR filePath )
{
	m_doc.Clear();
	BOOL bRet = m_doc.LoadFile(Util::String::CStringW2A(CString(filePath)));
	assert(bRet && "配置xml文件加载异常");
	if (bRet)
	{
		_ParseXML(m_doc.RootElement());
	}
	return bRet;
}

BOOL CConfig::SaveConfigToXML( LPCTSTR filePath )
{
	return m_doc.SaveFile(Util::String::CStringW2A(CString(filePath)));
}

ConfigNode* CConfig::_DeclareNode( LPCSTR name,const AttributeList& config,LPCSTR text/*=NULL*/ )
{
	ATLASSERT(name);
	if (name != NULL)
	{
		ConfigNode *node = new ConfigNode;
		node->value = name;
		if (text)
		{
			node->type = TiXmlNode::TINYXML_TEXT;
			node->value = text;
		}
		AttributeList::const_iterator i;
		i = config.begin();
		while (i != config.end())
		{
			node->attributeList.push_back(*i);
		}
		return node;
	}
	return NULL;
}

BOOL CConfig::_ParseXML(TiXmlElement *root)
{
	assert(root && "节点为空");
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//
// 
// CConfig::CConfig(void)
// {
// 
// }
// CConfig::~CConfig(void)
// {
// 
// }
// BOOL CConfig::LoadConfigFromXML( LPCTSTR filePath )
// {
// 	m_xml.LoadFile(filePath);
// 	if (m_xml.Error())
// 	{
// 		int nError = m_xml.ErrorId();
// 		std::cout<<m_xml.ErrorDesc();
// 		return false;
// 	}
// 	return true;
// }
// TiXmlElement* CConfig::GetRootElement()
// {
// 	return m_xml.RootElement();
// }
// TiXmlElement * CConfig::GetChildElement(TiXmlElement * pParentElement,const char * title)
// {
// 	if (pParentElement != NULL)
// 	{
// 		TiXmlNode * childNode = GetFirstChildElement(pParentElement);
// 		for (;childNode;childNode = GetNextChildElement(childNode->ToElement()))
// 		{
// 			if (!strcmp(title,childNode->Value()))
// 			{
// 				return childNode->ToElement();
// 			}
// 		}
// 	}
// 	return NULL;
// }
// TiXmlElement * CConfig::GetFirstChildElement(TiXmlElement * pParentElement)
// {
// 	if (pParentElement != NULL)
// 	{
// 		TiXmlElement * child = pParentElement->FirstChildElement();
// 		return child;
// 	}
// 	return NULL;
// }
// TiXmlElement * CConfig::GetNextChildElement(TiXmlElement * pElement)
// {
// 	if (pElement)
// 	{
// 		TiXmlElement * nextChild = pElement->NextSiblingElement();
// 		return nextChild;
// 	}
// 	return NULL;
// }
// std::string CConfig::GetElementValue(TiXmlElement * Element)
// {
// 	std::string reslut = "\0";
// 	if (Element != NULL)
// 	{
// 		reslut = Element->GetText();
// 	}
// 	return reslut;
// }
// std::string CConfig::GetElementAttributeValue(TiXmlElement* Element,const char* AttributeName)
// {
// 	std::string reslut = "\0";
// 	if (Element != NULL)
// 	{
// 		reslut = Element->Attribute(AttributeName);
// 	}
// 	return reslut;
// }
// std::string CConfig::GetChildElementValue(TiXmlElement * pParentElement,const char * title)
// {
// 	std::string reslut = "\0";
// 	TiXmlElement * pElem = GetChildElement(pParentElement,title);
// 	reslut = GetElementValue(pElem);
// 	return reslut;
// }
// std::string CConfig::GetChildElementAttributeValue(TiXmlElement * pParentElement,const char * title,const char* AttributeName)
// {
// 	std::string reslut = "\0";
// 	TiXmlElement * pElem = GetChildElement(pParentElement,title);
// 	reslut = GetElementAttributeValue(pElem,AttributeName);
// 	return reslut;
// }
// void CConfig::Clear()
// {
// 	m_xml.Clear();
// }
// //////////////////////////////////////////////////////////////////////////
// TiXmlElement* CConfig::AddXmlRootElement(const char* title)
// {
// 	TiXmlElement* element =new TiXmlElement(title);
// 	m_xml.LinkEndChild(element);
// 	return element;
// }
// TiXmlElement* CConfig::AddXmlChildElement(TiXmlElement* pPareElement,const char* title,const char * value)
// {
// 	if(pPareElement)
// 	{
// 		TiXmlElement* childElem =new TiXmlElement(title);
// 		pPareElement->LinkEndChild(childElem);
// 		if (value != "\0")
// 		{
// 			AddElementValue(childElem,value);
// 		}
// 		return childElem;
// 	}
// 	return 0;
// }
// void CConfig::AddXmlAttribute(TiXmlElement* pElement,const char* name,const char* value)
// {
// 	if(pElement)
// 	{
// 		pElement->SetAttribute(name,value);
// 	}
// }
// void CConfig::AddXmlDeclaration(const char* vesion,const char* encoding,const char* standalone)
// {
// 	TiXmlDeclaration * decl =new TiXmlDeclaration(vesion,encoding,standalone);
// 	m_xml.LinkEndChild(decl);
// }
// 
// void CConfig::AddElementValue(TiXmlElement *pElement,const char* value)
// {
// 	if(pElement)
// 	{
// 		TiXmlText * text =new TiXmlText(value);
// 		pElement->LinkEndChild(text);
// 	}
// }
// 
// void CConfig::AddXmlComment(TiXmlElement* pElement,const char* Comment)
// {
// 	if(pElement)
// 	{
// 		TiXmlComment * comment =new TiXmlComment(Comment);
// 		pElement->LinkEndChild(comment);
// 	}
// }
// 
// bool CConfig::ReplaceElementValue(TiXmlElement* pElement,const char * newValue)
// {
// 	if (pElement != NULL)
// 	{
// 		TiXmlNode * oldNode = NULL;
// 		oldNode = pElement->FirstChild();
// 		if (oldNode != NULL)
// 		{
// 			TiXmlText newText(newValue);
// 			pElement->ReplaceChild(oldNode,newText);
// 			return true;
// 		}
// 	}
// 	return false;
// }
// 
// bool CConfig::ReplaceElementAttribute(TiXmlElement* pElement,const char * name,const char * newValue)
// {
// 	if (pElement != NULL)
// 	{
// 		TiXmlAttribute * oldAttribute = NULL;
// 		oldAttribute = pElement->FirstAttribute();
// 		for(;oldAttribute != NULL;oldAttribute = oldAttribute->Next())
// 		{
// 			if (!strcmp(name,oldAttribute->Name()))
// 			{
// 				oldAttribute->SetValue(newValue);
// 			}
// 		}
// 	}
// 	return false;
// }
// 
// BOOL CConfig::SaveConfigToXML( LPCTSTR filePath )
// {
// 	m_xml.SaveFile(filePath);
// }
// 
// TiXmlElement * CConfig::FindFirstElement(const char * title)
// {
// 	if(!title)
// 		return 0;
// 	TiXmlElement * pElem = NULL;
// 	pElem = GetRootElement();
// 	pElem = FindFirstElement(pElem,title);
// 	return pElem;
// }
// 
// TiXmlElement * CConfig::FindFirstElement(TiXmlElement* pcrElement,const char * title)
// {
// 	TiXmlNode * pNode = pcrElement;
// 	while(pNode)
// 	{
// 		if(strcmp(pNode->Value(),title)==0)
// 		{
// 			return pNode->ToElement();
// 		}
// 		else
// 		{
// 			TiXmlNode * nextElement= pNode->FirstChildElement();
// 			while(nextElement)
// 			{
// 				if(strcmp(nextElement->Value(),title)==0)
// 				{
// 					return nextElement->ToElement();
// 				}
// 				else
// 				{
// 					TiXmlElement* reElement = NULL;
// 					reElement = FindFirstElement(nextElement->ToElement(),title);
// 					if(reElement)
// 					{
// 						return reElement;
// 					}
// 				}
// 				nextElement = nextElement->NextSiblingElement();
// 			}
// 		}
// 		pNode = pNode->NextSiblingElement();
// 	}
// 	return NULL;
// }
