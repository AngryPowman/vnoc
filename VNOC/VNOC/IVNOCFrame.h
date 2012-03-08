#pragma once
#include "IModule.h"

enum VNOCModule
{
	module_userCenter,
};

interface IVNOCFrame
{
public:
	STDMETHOD( Initialize() = 0 );
	STDMETHOD( UnInitialize() = 0 );

	// ע��ģ�顣
	STDMETHOD( RegisterModule	(VNOCModule module,IModule* iModule) = 0 );
	STDMETHOD( GetModule		(VNOCModule module,IModule** piModule) = 0 );
	STDMETHOD( ReleaseModule	(IModule* iModule) = 0);
	STDMETHOD( SetStartupModule	(VNOCModule module) = 0);
};

