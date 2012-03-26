#pragma once
#include "../../TinyXML/tinyxml.h"
#include <map>
#include <list>
#include <vector>
#include "IConfig.h"

struct ConfigTreeNode;

typedef std::map<CStringA,ConfigTreeNode>	ConfigMap;

struct ConfigTreeNode
{
	ConfigNode	config;
	ConfigMap	branch;
};

class CConfig : public IConfig
{
public:
	CConfig(void);
	~CConfig(void);
public:
	STDMETHOD( Initialize(IModule* UpperFrame=NULL) );
	STDMETHOD( UnInitialize() );
	STDMETHOD( Run() );
	STDMETHOD( Terminate() );
	STDMETHOD( LoadConfigFromXML(LPCTSTR filePath) );
	STDMETHOD( SaveConfigToXML(LPCTSTR filePath) );
	STDMETHOD( Get(ConfigNode* node) );
	STDMETHOD( Set(const ConfigNode& node) );
	BOOL RegisterConfigNodeOwner();
private:
	BOOL _ParseXMLTree(TiXmlNode *root,ConfigTreeNode& treeRoot);
	BOOL _ParseXML(TiXmlNode *root,ConfigTreeNode& treeRoot);
	BOOL _ParseNode(TiXmlNode *node,ConfigNode& cfg);
private:
	TiXmlDocument	m_doc;
	ConfigTreeNode	m_rootNode;
};
