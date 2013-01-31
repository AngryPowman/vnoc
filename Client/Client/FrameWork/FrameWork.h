#pragma once
#include "IFrameWork.h"
#include <atlsync.h>
#include <map>
#include <list>
#include <set>

class CFrameWork : public IFrameWork
{
public:
	CFrameWork();
	~CFrameWork();
public:
	virtual ResultCode Initialize(IModule* UpperFrame=NULL);
	virtual ResultCode UnInitialize();
	virtual ResultCode Run();
	virtual ResultCode Terminate();
	virtual ResultCode RegisterModule	(IFrameModule* iModule);
	virtual ResultCode GetModule		(IFrameModule** piModule,FrameModule module);
	virtual ResultCode RemoveModule		(IFrameModule* iModule);
	virtual ResultCode SendXMessage		(XMessage* pMsg);
private:
	typedef std::map<FrameModule,IFrameModule*>	ModuleMap;
	typedef std::map<CString,std::set<FrameModule>> XMessageMap;

	IFrameModule* _FindModule(FrameModule module);
	VOID _InitUILib();
	VOID _InitTest();
	VOID _LoadModule();
	VOID _LoadModule(FrameModule module);
	VOID _ClearModule();
	VOID _GetModulesListenList(FrameModule module=module_Any);
private:
	ModuleMap m_map;
	ATL::CCriticalSection	m_mapcs;
	XMessageMap m_msgMap;
	ATL::CCriticalSection	m_msgMapCS;
};