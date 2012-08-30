#pragma once
#include "IFrameWork.h"
#include "Dialogs/LoginImpl.h"
#include <atlsync.h>
#include <map>

typedef std::map<FrameModule,IModule*>	ModuleMap;

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
private:
	IModule* _FindModule(FrameModule module);
private:
	ModuleMap m_map;
	ATL::CCriticalSection m_cs;

	CLoginImpl	m_loginModule;
};