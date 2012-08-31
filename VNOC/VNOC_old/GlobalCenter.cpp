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
	_InitializeNetCenter();
	return S_OK;
}

HRESULT CGlobalCenter::UnInitialize()
{
	_UnInitializeNetCenter();
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
	UINT buffSize = MAX_PATH * sizeof(TCHAR);
	GetModuleFileName(GetModuleHandle(NULL),m_configFilePath.GetBuffer(buffSize),buffSize);
	m_configFilePath.ReleaseBuffer();
	int nPos=0;
	Util::Filesys::GetDirFromPath(m_configFilePath,nPos);
	m_configFilePath = m_configFilePath.Left(nPos);

	m_config.Initialize(NULL);
	m_configFilePath += PathSplit;
	m_configFilePath += CONFIGXMLFILE;
	Global->Logf(LogFile_General,_T("����XML��·��Ϊ:%s\n"),m_configFilePath);
	m_config.LoadConfigFromXML(m_configFilePath);
}

void CGlobalCenter::_UnInitializeConfig()
{
	m_config.SaveConfigToXML();
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

void CGlobalCenter::_InitializeNetCenter()
{
	m_netCenter.Initialize(NULL);
	m_netCenter.Run();
}

void CGlobalCenter::_UnInitializeNetCenter()
{
	m_netCenter.Terminate();
	m_netCenter.UnInitialize();
}

HRESULT CGlobalCenter::GetINetCenter( INetCenter** pNetCenter )
{
	*pNetCenter = dynamic_cast<INetCenter*>(&m_netCenter);
	return S_OK;
}

HRESULT CGlobalCenter::CriticalError( LPCTSTR message )
{
	MessageBox(NULL,message,0,0);
	TerminateProcess(GetCurrentProcess(),1);
	return S_OK;
}

