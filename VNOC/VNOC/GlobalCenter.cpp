#include "StdAfx.h"
#include "GlobalCenter.h"

CGlobalCenter Global2;			// ʵ��
IGlobal* Global = &Global2;		// Appȫ�ַ��ʽӿ�

CGlobalCenter::CGlobalCenter(void)
{
}


CGlobalCenter::~CGlobalCenter(void)
{
}

HRESULT CGlobalCenter::Initialize( IModule* UpperFrame/*=NULL*/ )
{
	_InitializeLog();		// ���еط�������log�����Ե����ȳ�ʼ��
	_InitializeConfig();
	return S_OK;
}

HRESULT CGlobalCenter::UnInitialize()
{
	_UnInitializeConfig();
	_UnInitializeLog();
	return S_OK;
}

HRESULT CGlobalCenter::Run()
{
	return S_OK;
}

HRESULT CGlobalCenter::Terminate()
{
	return S_OK;
}

HRESULT CGlobalCenter::GetIConfig( IConfig** pConfig )
{
	*pConfig = dynamic_cast<IConfig*>(&m_config);
	return S_OK;
}

void CGlobalCenter::_InitializeConfig()
{
	m_config.Initialize(NULL);
	CString configFile;
	GetCurrentDirectory(MAX_PATH,configFile.GetBuffer(MAX_PATH));
	configFile.ReleaseBuffer();
	configFile += PathSplit;
	configFile += CONFIGXMLFILE;
	m_config.LoadConfigFromXML(configFile);
}

void CGlobalCenter::_UnInitializeConfig()
{
	m_config.UnInitialize();
}

