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
	// ע��ģ�顣
	STDMETHOD( RegisterModule	(VNOCModule module,IModule* iModule) = 0 );
	// ��ȡģ��
	STDMETHOD( GetModule		(VNOCModule module,IModule** piModule) = 0 );
	// ɾ��ģ��
	STDMETHOD( ReleaseModule	(IModule* iModule) = 0);
	// �趨����ʱ��һ���򿪵�ģ��
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
