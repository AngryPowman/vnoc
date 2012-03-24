#include "StdAfx.h"
#include "GlobalCenter.h"

CGlobalCenter Global2;			// 实体
IGlobal* Global = &Global2;		// App全局访问接口

CGlobalCenter::CGlobalCenter(void)
{
}


CGlobalCenter::~CGlobalCenter(void)
{
}

HRESULT CGlobalCenter::Initialize( IModule* UpperFrame/*=NULL*/ )
{
	_InitializeLog();		// 所有地方都依赖log，所以当最先初始化
	_InitializeConfig();
	return S_OK;
}

HRESULT CGlobalCenter::UnInitialize()
{
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

HRESULT CGlobalCenter::GetIConfig( std::auto_ptr<IConfig>& pConfig )
{
	pConfig.reset(&m_config);
	return S_OK;
}

void CGlobalCenter::_InitializeConfig()
{
	m_config.Initialize(NULL);
}

void CGlobalCenter::_InitializeLog()
{
	CString params;
	Util::Filesys::GetSpecialPath(CSIDL_APPDATA,params);
	if (!params.IsEmpty())
	{
		params += PathSplit;
		params += PRODUCT_NAME;
		params += PathSplit;
		params += LogFileName;
		//m_log.AddDevice(BLOG_FILE,dynamic_cast<blog::CLogDeviceBase*>(logFile));
	}
	blog::CLogDeviceDBGView *logDbgView = new blog::CLogDeviceDBGView;
	if (logDbgView)
	{
		logDbgView->Open();
		m_log.AddDevice(BLOG_DBGVIEW,dynamic_cast<blog::CLogDeviceBase*>(logDbgView));
	}
}
