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
	Global->Log(LogFile_Config,_T("��ʼ��"));
	return S_OK;
}

HRESULT CConfig::UnInitialize()
{
	Global->Log(LogFile_Config,_T("����ʼ��"));
	return S_OK;
}

HRESULT CConfig::Run()
{
	Global->Log(LogFile_Config,_T("����"));
	return S_OK;
}

HRESULT CConfig::Terminate()
{
	Global->Log(LogFile_Config,_T("��ֹ"));
	return S_OK;
}

HRESULT CConfig::LoadConfigFromXML( LPCTSTR filePath )
{
	Global->Log(LogFile_Config,_T("��ʼ����XML"));
	m_doc.Clear();
	BOOL bRet = m_doc.LoadFile(CStringA(filePath));
	assert(bRet && "����xml�ļ������쳣");
	if (bRet)
	{
		Global->Log(LogFile_Config,_T("�ļ����سɹ���"));
		CLogIndent i(LogFile_Config);
		bRet = _ParseXML(m_doc.RootElement());
	}
	return bRet? S_OK:S_FALSE;
}

HRESULT CConfig::SaveConfigToXML( LPCTSTR filePath )
{
	return m_doc.SaveFile(CStringA(filePath));
}

BOOL CConfig::_ParseXML(TiXmlElement *root)
{
	assert(root && "�ڵ�Ϊ��");
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
