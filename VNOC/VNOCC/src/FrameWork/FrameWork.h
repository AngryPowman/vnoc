#pragma once
#include "IFrameWork.h"
#include "Dialogs/LoginImpl.h"
#include <atlsync.h>
#include <map>
#include <list>

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
	STDMETHOD( RegisterModule	(IModule* iModule,FrameModule module) );
	STDMETHOD( GetModule		(IModule** piModule,FrameModule module) );
	STDMETHOD( RemoveModule		(IModule* iModule) );
	STDMETHOD( SendXMessage		(XMessage* pMsg) );
	STDMETHOD( AddActor			(IFrameAdapter* actor));
	STDMETHOD( RemoveActor		(IFrameAdapter* actor));
private:
	IModule* _FindModule(FrameModule module);
private:
	typedef std::map<FrameModule,IModule*>	ModuleMap;
	ModuleMap m_map;
	ATL::CCriticalSection		m_mapcs;
	std::list<IFrameAdapter*>	m_actorList;
	ATL::CCriticalSection		m_listcs;

	CLoginImpl	m_loginModule;
};