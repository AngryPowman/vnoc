#include "StdAfx.h"
#include "GlobalCenter.h"

IGlobal* Global = NULL;		// App全局访问接口

VOID IGlobal::GlobalDeliver()
{
	static CGlobalCenter globalCenter(_T("VNOC"));
	Global = &globalCenter;
}

CGlobalCenter::CGlobalCenter(void)
{
}

CGlobalCenter::~CGlobalCenter(void)
{
}

ResultCode CGlobalCenter::Initialize( IModule* UpperFrame/*=NULL*/ )
{
	_InitializeLog();		// 所有地方都依赖log，所以当最先初始化
	_InitializeConfig();
	_InitializeNetCenter();
	_InitializeFrameWork();
	return Result_Success;
}

ResultCode CGlobalCenter::UnInitialize()
{
	_UnInitializeFrameWork();
	_UnInitializeNetCenter();
	_UnInitializeConfig();
	_UnInitializeLog();
	return Result_Success;
}

ResultCode CGlobalCenter::Run()
{
	m_pFrameWork->Run();
	return Result_Success;
}

ResultCode CGlobalCenter::Terminate()
{
	return Result_Success;
}

ResultCode CGlobalCenter::GetIConfig( IConfig** pConfig )
{
	*pConfig = dynamic_cast<IConfig*>(&m_config);
	return Result_Success;
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
	m_config.Run();
	m_configFilePath += PathSplit;
	m_configFilePath += CONFIGXMLFILE;
	Global->Logf(LogFile_General,_T("配置XML的路径为:%s\n"),m_configFilePath);
	m_config.LoadConfigFromXML(m_configFilePath);
}

void CGlobalCenter::_UnInitializeConfig()
{
	m_config.SaveConfigToXML();
	m_config.UnInitialize();
}

DWORD CGlobalCenter::CheckLastError(LPCTSTR helpStr)
{
	DWORD dw = GetLastError();
	CString str;
	str.Format(_T("检查LastError : %d"),dw);
	if (helpStr)
	{
		str.AppendFormat(_T("(%s)"),helpStr);
	}
	Log(LogFile_General,str);
	return dw;
}

ResultCode CGlobalCenter::PtrAssert( void* p )
{
	ATLASSERT(p && "指针为空");
	if (!p)
	{
		MessageBox(0,_T("发生了严重错误，程序将退出执行。"),_T("警告"),0);
		ExitProcess(8);
	}
	return Result_Success;
}

void CGlobalCenter::_InitializeNetCenter()
{
	m_netCenter.Initialize(this);
	m_netCenter.Run();
}

void CGlobalCenter::_UnInitializeNetCenter()
{
	m_netCenter.Terminate();
	m_netCenter.UnInitialize();
}

ResultCode CGlobalCenter::GetINetCenter( INetCenter** pNetCenter )
{
	*pNetCenter = dynamic_cast<INetCenter*>(&m_netCenter);
	return Result_Success;
}

ResultCode CGlobalCenter::CriticalError( LPCTSTR message )
{
	MessageBox(NULL,message,0,0);
	TerminateProcess(GetCurrentProcess(),1);
	return Result_Success;
}

ResultCode CGlobalCenter::GetIFrameModule( IFrameWork** pFrame )
{
	if (m_pFrameWork)
	{
		*pFrame = m_pFrameWork;
		return Result_Success;
	}
	return Result_Fail;
}

void CGlobalCenter::_InitializeFrameWork()
{
	m_pFrameWork = new CFrameWork;
	PtrAssert(m_pFrameWork);
	m_pFrameWork->Initialize(this);
}

void CGlobalCenter::_UnInitializeFrameWork()
{
	if (m_pFrameWork)
	{
		m_pFrameWork->Terminate();
		m_pFrameWork->UnInitialize();
	}
}