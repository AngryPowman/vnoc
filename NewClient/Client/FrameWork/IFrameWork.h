#pragma once
#include "IModule.h"
#include "ResDefine.h"

enum FrameModule
{
	module_Invalid      = -1,
	module_FrameWork    = 0,
	module_Any,

	module_ViewBegin    = 1024,
	module_LoginWin,
	module_RoomListWin,
	module_ClassroomWin,
	module_ViewEnd,

	module_DataBegin    = 2048,
	module_LoginData,
	module_RoomListData,
	module_ClassroomWinData,
	module_DataEnd,

    module_CppTestBegin = 3072,
	module_CppTestMain,
	module_CppTestEnd,
	module_CppTest_PrivateModule_Begin=3584,
	module_CppTest_PrivateModule_End=4095,
};

struct XMessage;

interface IFrameModule : public IModule
{
public:
	virtual FrameModule GetModuleType()=0;
	virtual ResultCode SendXMessage(XMessage* msg)=0;
	virtual ResultCode ProcessXMessage(XMessage* pMsg,XMessage* pMsgGetList=NULL)=0;
};

interface IFrameWork : public IModule
{
public:
	virtual ResultCode RegisterModule	(IFrameModule* iModule)=0;
	virtual ResultCode GetModule		(IFrameModule** piModule,FrameModule module)=0;
	virtual ResultCode RemoveModule		(IFrameModule* iModule)=0;
	virtual ResultCode SendXMessage		(XMessage* msg)=0;
};

template<class retType>
BOOL GetIFrameModule(){}
