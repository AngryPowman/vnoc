#pragma once
#include "ivnocframe.h"
#include "../util/util.h"
#include "../util/TaskThread.h"
#include <map>

// 先放这里。。
#include "LoginImpl.h"
#include "RoomListImpl.h"
#include "RoomImpl.h"

typedef std::map<VNOCModule,IModule*> ModuleMap;

class CVNOCFrame :
	public IVNOCFrame
{
public:
	CVNOCFrame(void);
	~CVNOCFrame(void);
public:
	STDMETHOD( Initialize(IModule* UpperFrame=NULL) );
	STDMETHOD( UnInitialize() );
	STDMETHOD( RegisterModule	(VNOCModule module,IModule* iModule) );
	STDMETHOD( GetModule		(VNOCModule module,IModule** piModule) );
	STDMETHOD( ReleaseModule	(IModule* iModule) );
	STDMETHOD( SetStartupModule	(VNOCModule module) );
	STDMETHOD( Run() );
	STDMETHOD( Terminate() );
public:
private:
	VOID _Start();
private:
	//Thread::CTaskThread m_viewThread;
	VNOCModule m_startModule;
	ModuleMap m_moduleMap;
	Util::CTimedCriticalSection	m_mapCS;
	CLoginImpl m_loginModule;
	CRoomListImpl m_roomListModule;
	CRoomImpl m_roomModule;
};

