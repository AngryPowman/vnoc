#pragma once
#include "IFrameWork.h"
#include <atlsync.h>
#include <map>
#include <list>
#include <set>

#include "../FrameWork/FrameModuleFactory.h"

class CFrameWork : public IFrameWork
{
public:
	CFrameWork();
	~CFrameWork();
public:
	STDMETHOD( Initialize(IModule* UpperFrame=NULL) );
	STDMETHOD( UnInitialize() );
	STDMETHOD( Run() );
	STDMETHOD( Terminate() );
	STDMETHOD( RegisterModule	(IFrameModule* iModule) );
	STDMETHOD( GetModule		(IFrameModule** piModule,FrameModule module) );
	STDMETHOD( RemoveModule		(IFrameModule* iModule) );
	STDMETHOD( SendXMessage		(XMessage* pMsg) );
private:
	typedef std::map<FrameModule,IFrameModule*>	ModuleMap;
	typedef std::map<CString,std::set<FrameModule>> XMessageMap;

	IFrameModule* _FindModule(FrameModule module);
	VOID _LoadModule();
	VOID _LoadModule(FrameModule module);
	VOID _ClearModule();
	VOID _GetModulesListenList();
private:
	ModuleMap m_map;
	ATL::CCriticalSection	m_mapcs;
	XMessageMap m_msgMap;
	ATL::CCriticalSection	m_msgMapCS;
};