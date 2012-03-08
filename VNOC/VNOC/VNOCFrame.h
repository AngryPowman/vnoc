#pragma once
#include "ivnocframe.h"

class CVNOCFrame :
	public IVNOCFrame
{
public:
	CVNOCFrame(void);
	~CVNOCFrame(void);
public:
	STDMETHOD( Initialize() );
	STDMETHOD( UnInitialize() );
	STDMETHOD( RegisterModule(VNOCModule module,IModule* iModule) );
	STDMETHOD( GetModule		(VNOCModule module,IModule** piModule) );
	STDMETHOD( ReleaseModule	(IModule* iModule) );
	STDMETHOD( SetStartupModule	(VNOCModule module) );
};

