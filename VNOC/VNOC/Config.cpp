#include "StdAfx.h"
#include "Config.h"
#include "../util/util.h"

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
BOOL CConfig::_ParseXML(TiXmlElement *root)
{
	assert(root && "节点为空");
	return TRUE;
}
