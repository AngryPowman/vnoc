#pragma once
#include "../../TinyXML/tinyxml.h"
#include <map>
#include <list>
#include <vector>
#include "IConfig.h"

class CConfigImpl : public IConfig
{
public:
	CConfigImpl(void);
	~CConfigImpl(void);
public:
	STDMETHOD( Initialize(IModule* UpperFrame=NULL) );
	STDMETHOD( UnInitialize() );
	STDMETHOD( Run() );
	STDMETHOD( Terminate() );
	STDMETHOD( LoadConfigFromXML(LPCTSTR filePath) );
	STDMETHOD( SaveConfigToXML(LPCTSTR filePath=NULL) );
	STDMETHOD( Get(const ConfigPath& path,ConfigPtrVec& node) );
	STDMETHOD( AddMonitor(ConfigPath path,IConfigMonitor* pMonitor));
	STDMETHOD( RemoveMonitor(IConfigMonitor* pMonitor) );
	BOOL RegisterConfigNodeOwner();
private:
	BOOL _ParseXMLTree(TiXmlNode *root,ConfigNode& treeRoot);
	BOOL _ParseXML(TiXmlNode *root,ConfigNode& treeRoot);
	BOOL _ParseNode(TiXmlNode *node,ConfigNode& cfg);
	ConfigNode*	_Find(ConfigPath path,BOOL createIfNotExist=FALSE);
	BOOL _CreateXMLTree(TiXmlNode& tree,const ConfigNode& root);
private:
	CString			m_filePath;
	TiXmlDocument	m_doc;
	ConfigNode	m_rootNode;
	ATL::CCriticalSection m_cs;
};
