//////////////////////////////////////////////////////////////////////////
// 管理了应用程序全局数据，如配置信息等。
#pragma once
#include "IGlobal.h"
#include "Config.h"
#include "../util/BLog.h"
#include "GlobalDefine.h"

class CGlobalCenter : public IGlobal
{
public:
	CGlobalCenter(void);
	~CGlobalCenter(void);
public:
	STDMETHOD( Initialize(IModule* UpperFrame=NULL) );
	STDMETHOD( UnInitialize() );
	STDMETHOD( Run() );
	STDMETHOD( Terminate() );
	// 获取配置接口
	STDMETHOD( GetIConfig(std::auto_ptr<IConfig>& pConfig) );
private:
	void _InitializeConfig();
	void _InitializeLog();
private:
	CConfig		m_config;
	blog::CBLog m_log;
	CString		m_logPath;
};


extern CGlobalCenter Global2;
