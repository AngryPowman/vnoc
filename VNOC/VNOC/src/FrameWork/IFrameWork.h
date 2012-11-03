#pragma once
#include "../Base/IModule.h"
#include "ResDefine.h"

enum FrameModule
{
	module_Invalid=-1,
	module_FrameWork=0,
	module_Any,

	module_ViewBegin=256,
	module_LoginWin,
	module_RoomListWin,
	module_ViewEnd,

	module_DataBegin=65536,
	module_LoginData,
	module_RoomListData,
	module_DataEnd
};

struct XMessage;

interface IFrameModule : public IModule
	, public IRefCountImpl
{
public:
	virtual FrameModule GetModuleType() = 0;
	virtual VOID SendXMessage(XMessage* msg) = 0;
	virtual VOID ProcessXMessage(XMessage* pMsg,XMessage* pMsgGetList=NULL) = 0;
};

interface IFrameWork : public IModule,public IRefCountImpl
{
public:
	// 注册模块。
	STDMETHOD( RegisterModule	(IFrameModule* iModule) = 0 );
	// 获取模块
	STDMETHOD( GetModule		(IFrameModule** piModule,FrameModule module) = 0 );
	// 删除模块
	STDMETHOD( RemoveModule		(IFrameModule* iModule) = 0);
	//
	STDMETHOD( SendXMessage		(XMessage* msg) = 0);
};

template<class retType>
BOOL GetIFrameModule(){}
