#pragma once
#include "../../TinyXML/tinyxml.h"
#include <map>
#include <list>
#include <vector>

typedef std::vector<std::string>			ConfigPath;

class CConfig
{
public:
	CConfig(void);
	~CConfig(void);
public:
	BOOL LoadConfigFromXML(LPCTSTR filePath);
	BOOL SaveConfigToXML(LPCTSTR filePath);
	BOOL RegisterConfigNodeOwner();
private:
	BOOL _ParseXML(TiXmlElement *root);
private:
	TiXmlDocument m_doc;
};
