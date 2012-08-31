#include "StdAfx.h"
#include "VNOCFrame.h"


CVNOCFrame::CVNOCFrame(void)
{
	m_loginModule.AddRef();
	m_roomListModule.AddRef();
	m_roomModule.AddRef();
	RegisterModule(module_userCenter,dynamic_cast<IModule*>(&m_loginModule));
	RegisterModule(module_roomList,dynamic_cast<IModule*>(&m_roomListModule));
	RegisterModule(module_room,dynamic_cast<IModule*>(&m_roomModule));
}

CVNOCFrame::~CVNOCFrame(void)
{
}

HRESULT CVNOCFrame::Initialize(IModule* UpperFrame)
{
	Util::CAutoTimedCS ac(m_mapCS);
	ModuleMap::iterator i;
	i = m_moduleMap.begin();
	while (i != m_moduleMap.end())
	{
		if (i->second)
		{
			i->second->Initialize(dynamic_cast<IModule*>(this));
		}
		++i;
	}
	return S_OK;
}

HRESULT CVNOCFrame::UnInitialize()
{
	Util::CAutoTimedCS ac(m_mapCS);
	ModuleMap::iterator i;
	i = m_moduleMap.begin();
	while (i != m_moduleMap.end())
	{
		if (i->second)
		{
			i->second->UnInitialize();
		}
		++i;
	}
	return S_OK;
}

HRESULT CVNOCFrame::RegisterModule( VNOCModule module,IModule* iModule )
{
	ATLASSERT(iModule);
	if (iModule)
	{
		Util::CAutoTimedCS ac(m_mapCS);
		m_moduleMap.insert(std::make_pair(module,iModule));
	}
	return S_OK;
}

HRESULT CVNOCFrame::GetModule( VNOCModule module,IModule** piModule )
{
	ATLASSERT(piModule);
	if (piModule)
	{
		Util::CAutoTimedCS ac(m_mapCS);
		ModuleMap::iterator i;
		i = m_moduleMap.find(module);
		if (i != m_moduleMap.end())
		{
			*piModule = i->second;
		}
	}
	return S_OK;
}

HRESULT CVNOCFrame::ReleaseModule( IModule* iModule )
{
	return S_OK;
}

HRESULT CVNOCFrame::SetStartupModule( VNOCModule module )
{
	m_startModule = module;
	return S_OK;
}

HRESULT CVNOCFrame::Run()
{
	//m_viewThread.PostTask(Thread::CreatePtrTask(this,&CVNOCFrame::_Start));
	_Start();
	return S_OK;
}

VOID CVNOCFrame::_Start()
{
	IModule *pStartupModule=NULL;
	GetModule(m_startModule,&pStartupModule);
	ATLASSERT(pStartupModule);
	if (pStartupModule)
	{
		pStartupModule->Run();
		ILogin* pLogin = NULL;
		GetiModule(pStartupModule,&pLogin);
		if (pLogin)
		{
			pLogin->Show();
		}
	}
}

HRESULT CVNOCFrame::Terminate()
{
	Util::CAutoTimedCS ac(m_mapCS);
	ModuleMap::iterator i;
	i = m_moduleMap.begin();
	while (i != m_moduleMap.end())
	{
		if (i->second)
		{
			i->second->Terminate();
		}
		++i;
	}
	return S_OK;
}
