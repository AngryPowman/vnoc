#include "stdafx.h"
#include "FrameWork.h"
#include "../util/util.h"

CFrameWork::CFrameWork()
{

}

CFrameWork::~CFrameWork()
{

}

HRESULT CFrameWork::Initialize( IModule* UpperFrame/*=NULL*/ )
{
	_LoadModule();

	BkFontPool::SetDefaultFont(_T("Courier New"), -12);
	BkSkin::LoadSkins(IDR_XML_SKIN_DEF);
	BkStyle::LoadStyles(IDR_BK_STYLE_DEF);
	CString strPath;
	GetModuleFileName(nullptr,strPath.GetBuffer(MAX_PATH),MAX_PATH);
	strPath.ReleaseBuffer();
	Util::Filesys::SplitPath(strPath,strPath,CString());
	strPath += BKWinResDir;
	Global->Logf(LogFile_FrameWork,_T("初始化资源文件夹: %s"),strPath);
	BkResManager::SetResourcePath(strPath);
	return S_OK;
}

HRESULT CFrameWork::UnInitialize()
{
	_ClearModule();
	return S_OK;
}

HRESULT CFrameWork::Run()
{
	XMessage msgFirstShow(XMessageID_ShowLogin);
	SendXMessage(&msgFirstShow);
	return S_OK;
}

HRESULT CFrameWork::Terminate()
{
	return S_OK;
}

HRESULT CFrameWork::RegisterModule( IFrameModule* iModule )
{
	FrameModule module;
	module = iModule->GetModuleType();
	ATLASSERT(module != module_Invalid);
	Util::CAutoCS ac(m_mapcs);
	m_map[module] = iModule;
	return S_OK;
}

HRESULT CFrameWork::GetModule( IFrameModule** piModule,FrameModule module )
{
	Util::CAutoCS ac(m_mapcs);
	if (piModule == NULL)
	{
		return E_INVALIDARG;
	}
	IFrameModule* var = _FindModule(module);
	if (var)
	{
		*piModule = var;
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CFrameWork::RemoveModule( IFrameModule* iModule )
{
	Util::CAutoCS ac(m_mapcs);

	for (auto i=m_map.begin(); i!=m_map.end();)
	{
		if (!iModule || (i->second==iModule))
		{
			if (i->second)
			{
				delete i->second;
			}
			i = m_map.erase(i);
		}
		else
		{
			++i;
		}
	}
	return S_OK;
}

IFrameModule* CFrameWork::_FindModule( FrameModule module )
{
	Util::CAutoCS ac(m_mapcs);
	auto i = m_map.find(module);
	if (i != m_map.end())
	{
		return i->second;
	}
	return NULL;
}

HRESULT CFrameWork::SendXMessage( XMessage* pMsg )
{
	Util::CAutoCS ac(m_mapcs);
	if (pMsg->destModule == module_Any)
	{ // broadcast
		for (auto i=m_map.begin(); i!=m_map.end() && !pMsg->IsProcessed(); ++i)
		{
			i->second->ProcessXMessage(pMsg);
		}
	}
	else
	{
		auto i = m_map.find(pMsg->destModule);
		if (i != m_map.end())
		{
			i->second->ProcessXMessage(pMsg);
		}
		else
		{
			return S_FALSE;
		}
	}
	return S_OK;
}

VOID CFrameWork::_ClearModule()
{
	RemoveModule(NULL);
}

VOID CFrameWork::_LoadModule()
{
	IFrameModule *pModule=NULL;
	CFrameModuleFactory::CreateFrameModule(module_LoginData,&pModule);
	if (pModule)
	{
		pModule->Initialize(this);
		pModule->Run();
		RegisterModule(pModule);
		pModule=NULL;
	}
	CFrameModuleFactory::CreateFrameModule(module_LoginWin,&pModule);
	if (pModule)
	{
		pModule->Initialize(this);
		pModule->Run();
		RegisterModule(pModule);
		pModule=NULL;
	}
}

