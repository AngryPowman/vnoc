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
	GetCurrentDirectory(MAX_PATH,m_configFilePath.GetBuffer(MAX_PATH));
	m_configFilePath.ReleaseBuffer();
	m_configFilePath += PathSplit;
	m_configFilePath += CONFIGXMLFILE;
	m_config.LoadConfigFromXML(m_configFilePath);
}

void CGlobalCenter::_UnInitializeConfig()
{
	//m_config.SaveConfigToXML();
	m_config.UnInitialize();
}

HRESULT CGlobalCenter::CheckLastError(LPCTSTR helpStr)
{
	DWORD dw = GetLastError();
	CString str;
	str.Format(_T("���LastError : %d"),dw);
	if (helpStr)
	{
		str.AppendFormat(_T("(%s)"),helpStr);
	}
	Log(LogFile_General,str);
	return dw;
}

HRESULT CGlobalCenter::PtrAssert( void* p )
{
	ATLASSERT(p && "ָ��Ϊ��");
	if (!p)
	{
		MessageBox(0,_T("���������ش��󣬳����˳�ִ�С�"),_T("����"),0);
		ExitProcess(8);
	}
	return S_OK;
}

