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
	RegisterModule(&m_loginModule,module_LoginWin);

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
	return S_OK;
}

HRESULT CFrameWork::Run()
{
	m_loginModule.Run();
	m_loginModule.Show(SW_SHOW);
	return S_OK;
}

HRESULT CFrameWork::Terminate()
{
	return S_OK;
}

HRESULT CFrameWork::RegisterModule( IModule* iModule,FrameModule module )
{
	Util::CAutoCS ac(m_cs);
	m_map[module] = iModule;
	return S_OK;
}

HRESULT CFrameWork::GetModule( IModule** piModule,FrameModule module )
{
	Util::CAutoCS ac(m_cs);
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
	Util::CAutoCS ac(m_cs);

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
	Util::CAutoCS ac(m_cs);
	auto i = m_map.find(module);
	if (i != m_map.end())
	{
		return i->second;
	}
	return NULL;
}
