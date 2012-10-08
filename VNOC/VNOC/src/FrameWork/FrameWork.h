#pragma once
#include "IFrameWork.h"
#include <atlsync.h>
#include <map>
#include <list>

#include "../Logic/UserSession/LoginImpl.h"
#include "../Logic/room/RoomMgr.h"

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
	IFrameModule* _FindModule(FrameModule module);
	VOID _ClearModule();
private:
	typedef std::map<FrameModule,IFrameModule*>	ModuleMap;
	ModuleMap m_map;
	ATL::CCriticalSection		m_mapcs;

	CLoginImpl*	m_loginModule;
	CRoomMgr*	m_roomMgr;
};