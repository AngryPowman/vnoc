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
	BOOL _ParseXML(TiXmlNode *root,ConfigNode& treeRoot);
	BOOL _ParseXMLNode(TiXmlNode *node,ConfigNode& cfg);

	BOOL _CreateXML(TiXmlNode& tree,const ConfigNode& root);
	BOOL _CreateXMLNode(TiXmlNode **node,const ConfigNode& root);

	BOOL _Find(ConfigPath path,ConfigPtrVec& vec,BOOL createIfNotExist=FALSE);
	VOID _Clear();
private:
	CString			m_filePath;
	ConfigNode	m_rootNode;
	ATL::CCriticalSection m_cs;
};
