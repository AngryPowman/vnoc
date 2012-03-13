#pragma once
#include "IModule.h"

enum VNOCModule
{
	module_userCenter,
	module_roomList,
	module_room,
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

template<class retType>
void GetiModule(VNOCModule module,IVNOCFrame* iFrame,retType** pRet)
{
	if (pRet && iFrame)
	{
		*pRet = NULL;
		IModule* pModule=NULL;
		iFrame->GetModule(module,&pModule);
		if (pModule)
		{
			GetiModule(pModule,pRet);
		}
	}
}
