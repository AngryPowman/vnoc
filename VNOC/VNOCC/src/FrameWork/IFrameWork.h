#pragma once
#include "../Base/IModule.h"

enum FrameModule
{
	module_Invalid=-1,

	module_ViewBegin,
	module_LoginWin,
	module_ViewEnd,

	module_DataBegin,
	module_LoginData,
	module_DataEnd
};

interface IFrameWork : public IModule
{
public:
	// ע��ģ�顣
	STDMETHOD( RegisterModule	(IModule* iModule,FrameModule module) = 0 );
	// ��ȡģ��
	STDMETHOD( GetModule		(IModule** piModule,FrameModule module) = 0 );
	// ɾ��ģ��
	STDMETHOD( RemoveModule		(IModule* iModule) = 0);
};

template<class retType>
BOOL GetIFrameModule(){}
