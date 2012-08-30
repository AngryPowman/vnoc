#pragma once
#include "../Base/IModule.h"
#include "ResDefine.h"

typedef DWORD BkWin_UIID;

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

struct XMessage : public IRefCountImpl
{
	DWORD	msgID;
	BkWin_UIID uiID;
	FrameModule	module;

	HRESULT	result;
};

interface IFrameWork : public IModule
{
public:
	// 注册模块。
	STDMETHOD( RegisterModule	(IModule* iModule,FrameModule module) = 0 );
	// 获取模块
	STDMETHOD( GetModule		(IModule** piModule,FrameModule module) = 0 );
	// 删除模块
	STDMETHOD( RemoveModule		(IModule* iModule) = 0);
	//
	STDMETHOD( SendXMessage		(XMessage* msg) = 0);
};

template<class retType>
BOOL GetIFrameModule(){}
