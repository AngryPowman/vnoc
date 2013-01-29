#include "stdafx.h"
#include "FrameWork.h"

CFrameWork::CFrameWork()
{

}

CFrameWork::~CFrameWork()
{

}

ResultCode CFrameWork::Initialize( IModule* UpperFrame/*=NULL*/ )
{
#ifndef CPPTEST
	_InitUILib();
#else
	_InitTest();
#endif
    _LoadModule();
	return Result_Success;
}

ResultCode CFrameWork::UnInitialize()
{
	_ClearModule();
	return Result_Success;
}

ResultCode CFrameWork::Run()
{
#ifndef CPPTEST
	XMessage_FrameStart firstMessage;
#else
	XMessage_StartTest firstMessage;
#endif
	SendXMessage(&firstMessage);
	return Result_Success;
}

ResultCode CFrameWork::Terminate()
{
	return Result_Success;
}

ResultCode CFrameWork::RegisterModule( IFrameModule* iModule )
{
	FrameModule module;
	module = iModule->GetModuleType();
	ATLASSERT(module != module_Invalid);
	Util::CAutoCS ac(m_mapcs);
	m_map[module] = iModule;
	
	_GetModulesListenList(module);
	iModule->Initialize(this);
	iModule->Run();
	return Result_Success;
}

ResultCode CFrameWork::GetModule( IFrameModule** piModule,FrameModule module )
{
	Util::CAutoCS ac(m_mapcs);
	if (piModule == NULL)
	{
		return Result_InvalidParam;
	}
	IFrameModule* var = _FindModule(module);
	if (var)
	{
		*piModule = var;
		return Result_Success;
	}
	return Result_Fail;
}

ResultCode CFrameWork::RemoveModule( IFrameModule* iModule )
{
	Util::CAutoCS ac(m_mapcs);

	for (auto i=m_map.begin(); i!=m_map.end();)
	{
		if (!iModule || (i->second==iModule))
		{
			if (i->second)
			{
				i->second->Terminate();
				i->second->UnInitialize();
			}
			i = m_map.erase(i);
		}
		else
		{
			++i;
		}
	}
	return Result_Success;
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

ResultCode CFrameWork::SendXMessage( XMessage* pMsg )
{
	Util::CAutoCS ac(m_mapcs);
	if (pMsg->destModule == module_Any)
	{ // broadcast
		Util::CAutoCS ac(m_msgMapCS);

		CString strID;
		pMsg->GetID(strID);
		auto& moduleSet = m_msgMap[strID];
		for (auto i=moduleSet.begin(); i!=moduleSet.end() && !pMsg->IsProcessed(); ++i)
		{
			IFrameModule* pModule = m_map[*i];
			if (pModule)
			{
				pModule->ProcessXMessage(pMsg);
			}
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
			return Result_Fail;
		}
	}
	return Result_Success;
}

VOID CFrameWork::_ClearModule()
{
	RemoveModule(NULL);
}

VOID CFrameWork::_LoadModule()
{
    int i;
    for (i = module_DataBegin + 1; i < module_DataEnd; ++i)
    {
        _LoadModule(static_cast<FrameModule>(i));
    }

#ifndef CPPTEST

    for (i = module_ViewBegin + 1; i < module_ViewEnd; ++i)
    {
        _LoadModule(static_cast<FrameModule>(i));
    }

#else

    for (i = module_CppTestBegin + 1; i < module_CppTestEnd; ++i)
    {
        _LoadModule(static_cast<FrameModule>(i));
    }

#endif
}

VOID CFrameWork::_LoadModule( FrameModule module )
{
	IFrameModule *pModule=NULL;
	CFrameModuleFactory::CreateFrameModule(module,&pModule);
	if (pModule)
	{
		RegisterModule(pModule);
	}
}

VOID CFrameWork::_GetModulesListenList(FrameModule module)
{
	if (module == module_Any)
	{
		XMessage_GetListenList msg;
		Util::CAutoCS ac(m_mapcs);
		for (auto i=m_map.begin(); i!=m_map.end(); ++i)
		{
			msg.msgIDList.clear();
			i->second->ProcessXMessage(NULL,&msg);

			Util::CAutoCS ac(m_msgMapCS);
			for (auto j=msg.msgIDList.begin(); j!=msg.msgIDList.end(); ++j)
			{
				m_msgMap[*j].insert( i->first );
			}
		}
	}
	else
	{
		Util::CAutoCS ac(m_mapcs);
		auto i = m_map.find(module);
		if (i != m_map.end())
		{
			XMessage_GetListenList msg;
			i->second->ProcessXMessage(NULL,&msg);

			Util::CAutoCS ac(m_msgMapCS);
			for (auto j=msg.msgIDList.begin(); j!=msg.msgIDList.end(); ++j)
			{
				m_msgMap[*j].insert( i->first );
			}
		}
	}
}

VOID CFrameWork::_InitUILib()
{
/*	BkFontPool::SetDefaultFont(_T("Courier New"), -12);
	BkSkin::LoadSkins(IDR_XML_SKIN_DEF);
	BkStyle::LoadStyles(IDR_BK_STYLE_DEF);
	CString strPath;
	GetModuleFileName(nullptr,strPath.GetBuffer(MAX_PATH),MAX_PATH);
	strPath.ReleaseBuffer();
	Util::Filesys::SplitPath(strPath,strPath,CString());
	strPath += BKWinResDir;
	Global->Logf(LogFile_FrameWork,_T("初始化资源文件夹: %s"),strPath);
	BkResManager::SetResourcePath(strPath);*/
}

VOID CFrameWork::_InitTest()
{
}

