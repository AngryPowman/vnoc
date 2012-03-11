#pragma once
#include "IModule.h"

enum VNOCModule
{
	module_userCenter,
};

interface IVNOCFrame : public IModule
{
public:
	// ×¢²áÄ£¿é¡£
	STDMETHOD( RegisterModule	(VNOCModule module,IModule* iModule) = 0 );
	STDMETHOD( GetModule		(VNOCModule module,IModule** piModule) = 0 );
	STDMETHOD( ReleaseModule	(IModule* iModule) = 0);
	STDMETHOD( SetStartupModule	(VNOCModule module) = 0);
};

