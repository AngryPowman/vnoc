#pragma once
#include "../../TinyXML/tinyxml.h"
#include <map>
#include <list>
#include <vector>
#include "IConfig.h"

struct ConfigTreeNode;

typedef std::map<CString,ConfigTreeNode>	ConfigMap;

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
	STDMETHOD( SaveConfigToXML(LPCTSTR filePath=NULL) );
	STDMETHOD( Get(ConfigNode& node) );
	STDMETHOD( Set(const ConfigNode& node) );
	STDMETHOD( AddMonitor(ConfigPath path,IConfigMonitor* pMonitor));
	STDMETHOD( RemoveMonitor(IConfigMonitor* pMonitor) );
	BOOL RegisterConfigNodeOwner();
private:
	BOOL _ParseXMLTree(TiXmlNode *root,ConfigTreeNode& treeRoot);
	BOOL _ParseXML(TiXmlNode *root,ConfigTreeNode& treeRoot);
	BOOL _ParseNode(TiXmlNode *node,ConfigNode& cfg);
	ConfigNode*	_Find(ConfigPath path,BOOL createIfNotExist=FALSE);
	BOOL _CreateXMLTree(TiXmlNode& tree,const ConfigTreeNode& root);
private:
	CString			m_filePath;
	TiXmlDocument	m_doc;
	ConfigTreeNode	m_rootNode;
	ATL::CCriticalSection m_cs;
};
