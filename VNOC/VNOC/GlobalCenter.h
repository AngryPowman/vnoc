//////////////////////////////////////////////////////////////////////////
// 管理了应用程序全局数据，如配置信息等。
#pragma once
#include "IModule.h"
#include "IConfig.h"

class CGlobalCenter : public IModule
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
	STDMETHOD( GetIConfig(IConfig** ppConfig) );
};


extern CGlobalCenter Global;
