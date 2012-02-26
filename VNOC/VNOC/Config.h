#pragma once
#include "../../TinyXML/tinyxml.h"
#include <map>
#include <list>
#include <vector>

typedef std::vector<CStringA> ConfigPath;
typedef std::pair<CStringA,CStringA> Attribute;
typedef std::list<Attribute> AttributeList;

typedef TiXmlNode::NodeType ConfigNodeType;

struct ConfigNode 
{
	TiXmlNode::NodeType type;
	CStringA value;
	AttributeList attributeList;
};

class CConfig
{
public:
	CConfig(void);
	~CConfig(void);
public:
	BOOL LoadConfigFromXML(LPCTSTR filePath);
	BOOL SaveConfigToXML(LPCTSTR filePath);
	BOOL RegisterConfigNodeOwner();
	BOOL GetConfigText(const ConfigPath& path);
	BOOL GetConfigAttribute(const ConfigPath& path,__out AttributeList* attribute);
private:
	BOOL _ParseXML(TiXmlElement *root);
	ConfigNode* _DeclareNode(LPCSTR name,const AttributeList& config,LPCSTR text=NULL);
private:
	TiXmlDocument m_doc;
	typedef std::map<CStringA,ConfigNode> ConfigMap;
	ConfigMap m_map;
};

// class CConfig
// {
// public:
// 	CConfig(void);
// 	~CConfig(void);
// 	BOOL LoadConfigFromXML(LPCTSTR filePath);
// 	VOID Clear();
// 	BOOL SaveConfigToXML(LPCTSTR filePath);
// 
// 	TiXmlElement * GetRootElement();
// 	TiXmlElement * GetChildElement(TiXmlElement * pParentElement,const char * title);
// 	TiXmlElement * GetFirstChildElement(TiXmlElement * pParentElement);
// 	TiXmlElement * GetNextChildElement(TiXmlElement * pElement);
// 
// 	TiXmlElement * FindFirstElement(const char * title);//µ›πÈ≤È’“
// 
// 	std::string GetElementValue(TiXmlElement * pElement);
// 	std::string GetElementAttributeValue(TiXmlElement* Element,const char* AttributeName);
// 	std::string GetChildElementValue(TiXmlElement * pParentElement,const char * title);
// 	std::string GetChildElementAttributeValue(TiXmlElement * pParentElement,const char * title,const char* AttributeName);
// 
// 	TiXmlElement* AddXmlRootElement(const char* title);
// 	TiXmlElement* AddXmlChildElement(TiXmlElement* pPareElement,const char* title,const char * value = "\0");
// 	void AddXmlAttribute(TiXmlElement* pElement,const char* name,const char* value);
// 	void AddXmlDeclaration(const char* vesion="1.0",const char* encoding="gb2312",const char* standalone="");
// 	void AddElementValue(TiXmlElement * pElement,const char* value);
// 	void AddXmlComment(TiXmlElement* pElement,const char* Comment);
// 
// 	bool ReplaceElementValue(TiXmlElement * pElement,const char * newValue);
// 	bool ReplaceElementAttribute(TiXmlElement* pElement,const char * name,const char * newValue);
// 
// private:
// 	TiXmlElement * FindFirstElement(TiXmlElement* pcrElement,const char * title);//µ›πÈ≤È’“
// 
// private:
// 	TiXmlDocument m_xml;
// };
