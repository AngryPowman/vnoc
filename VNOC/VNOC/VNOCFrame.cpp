#include "StdAfx.h"
#include "VNOCFrame.h"


CVNOCFrame::CVNOCFrame(void)
{
	//m_viewThread.Start();
	m_loginModule.AddRef();
	RegisterModule(module_userCenter,dynamic_cast<IModule*>(&m_loginModule));
}

CVNOCFrame::~CVNOCFrame(void)
{
}

HRESULT CVNOCFrame::Initialize(IUnknown* UpperFrame)
{
	return S_OK;
}

HRESULT CVNOCFrame::UnInitialize()
{
	Util::CAutoTimedCS ac(m_mapCS);
	ModuleMap::iterator i;
	i = m_moduleMap.begin();
	while (i != m_moduleMap.end())
	{
		i->second->Terminate();
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
	}
}

HRESULT CVNOCFrame::Terminate()
{
	return S_OK;
}
