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

HRESULT CConfig::Initialize( IModule* UpperFrame/*=NULL*/ )
{
	return S_OK;
}

HRESULT CConfig::UnInitialize()
{
	return S_OK;
}

HRESULT CConfig::Run()
{
	return S_OK;
}

HRESULT CConfig::Terminate()
{
	return S_OK;
}

HRESULT CConfig::LoadConfigFromXML( LPCTSTR filePath )
{
	m_doc.Clear();
	BOOL bRet = m_doc.LoadFile(CStringA(filePath));
	assert(bRet && "配置xml文件加载异常");
	if (bRet)
	{
		_ParseXML(m_doc.RootElement());
	}
	std::auto_ptr<IConfig> ptr;
	return bRet? S_OK:S_FALSE;
}

HRESULT CConfig::SaveConfigToXML( LPCTSTR filePath )
{
	return m_doc.SaveFile(CStringA(filePath));
}

BOOL CConfig::_ParseXML(TiXmlElement *root)
{
	assert(root && "节点为空");
	return TRUE;
}

HRESULT CConfig::Get( ConfigNode& node )
{
	return S_OK;
}

HRESULT CConfig::Set( const ConfigNode& node )
{
	return S_OK;
}
