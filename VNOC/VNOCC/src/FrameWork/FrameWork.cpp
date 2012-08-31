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
	m_loginModule = new CLoginImpl;
	m_loginModule->AddRef();
	m_loginModule->Initialize(this);
	m_loginModule->Run();
	RegisterModule(m_loginModule,module_LoginWin);

	BkString::Load(IDR_BK_STRING_DEF);
	BkFontPool::SetDefaultFont(BkString::Get(0), -12);
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
	m_loginModule->UnInitialize();
	m_loginModule->Release();
	return S_OK;
}

HRESULT CFrameWork::Run()
{
	m_loginModule->Show(SW_SHOW);
	return S_OK;
}

HRESULT CFrameWork::Terminate()
{
	return S_OK;
}

HRESULT CFrameWork::RegisterModule( IModule* iModule,FrameModule module )
{
	Util::CAutoCS ac(m_mapcs);
	m_map[module] = iModule;
	return S_OK;
}

HRESULT CFrameWork::GetModule( IModule** piModule,FrameModule module )
{
	Util::CAutoCS ac(m_mapcs);
	if (piModule == NULL)
	{
		//
		return E_INVALIDARG;
	}
	IModule* var = _FindModule(module);
	if (var)
	{
		*piModule = var;
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CFrameWork::RemoveModule( IModule* iModule )
{
	Util::CAutoCS ac(m_mapcs);

	for (auto i=m_map.begin(); i!=m_map.end(); ++i)
	{
		if (i->second == iModule)
		{
			m_map.erase(i);
			return S_OK;
		}
	}
	ATLASSERT(FALSE && "删除不存在的模块");
	return S_OK;
}

IModule* CFrameWork::_FindModule( FrameModule module )
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
	Util::CAutoCS ac(m_listcs);
	auto i = m_actorList.begin();
	while (i != m_actorList.end())
	{
		BOOL bRet = (*i)->ProcessXMessage(pMsg);
		if (bRet)
		{
			i = m_actorList.end();
		}
		else
		{
			++i;
		}
	}
	return S_OK;
}

HRESULT CFrameWork::AddActor( IFrameAdapter* actor )
{
	Util::CAutoCS ac(m_listcs);
	auto i = m_actorList.begin();
	while (i != m_actorList.end())
	{
		if (*i == actor)
		{
			return S_OK;
		}
		++i;
	}
	m_actorList.push_front(actor);
	return S_OK;
}

HRESULT CFrameWork::RemoveActor( IFrameAdapter* actor )
{
	Util::CAutoCS ac(m_listcs);
	m_actorList.remove(actor);
	return S_OK;
}
