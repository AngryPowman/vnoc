#pragma once
#include "../../TinyXML/tinyxml.h"
#include <map>
#include <list>
#include <vector>
#include "IConfig.h"

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
	STDMETHOD( Get(ConfigNode& node) );
	STDMETHOD( Set(const ConfigNode& node) );
	BOOL RegisterConfigNodeOwner();
private:
	BOOL _ParseXML(TiXmlElement *root);
private:
	TiXmlDocument m_doc;
};
