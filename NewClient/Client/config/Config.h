#pragma once
#include "TinyXML/tinyxml.h"
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
	virtual ResultCode Initialize(IModule* UpperFrame=NULL);
	virtual ResultCode UnInitialize();
	virtual ResultCode Run();
	virtual ResultCode Terminate();
	virtual ResultCode LoadConfigFromXML(LPCTSTR filePath);
	virtual ResultCode SaveConfigToXML(LPCTSTR filePath=NULL);
	virtual ResultCode Get(const ConfigPath& path,ConfigPtrVec& node);
	virtual ResultCode AddMonitor(ConfigPath path,IConfigMonitor* pMonitor);
	virtual ResultCode RemoveMonitor(IConfigMonitor* pMonitor);
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
