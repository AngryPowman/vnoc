#include "stdafx.h"
#include "GlobalCenter.h"

//////////////////////////////////////////////////////////////////////////
// 实现global的log接口

void CGlobalCenter::_InitializeLog()
{
	Util::Filesys::GetSpecialPath(CSIDL_APPDATA,m_logPath);
	if (!m_logPath.IsEmpty())
	{
		m_logPath += PathSplit;
		m_logPath += PRODUCT_NAME;
		m_logPath += PathSplit;
	}
}

void CGlobalCenter::_UnInitializeLog()
{
	return;
	LogMap::iterator i;
	i = m_log.begin();
	while (i != m_log.end())
	{
		delete i->second;
		++i;
	}
}

HRESULT CGlobalCenter::Log( CString file,CString str )
{
	blog::CBLog* log = _GetLogInstance(file);
	if (log)
	{
		log->Log(LogMask_ALL,str);
	}
	else
	{
		ASSERT(FALSE && L"无法获取到log对象");
	}
	return S_OK;
}

HRESULT CGlobalCenter::Logf( CString file,LPCTSTR str,... )
{
	blog::CBLog* log = _GetLogInstance(file);
	if (log)
	{
		log->Logf(LogMask_ALL,str);
	}
	return S_OK;
}

HRESULT CGlobalCenter::SetLog( CString file,BOOL bDbgView/*=TRUE*/,BOOL bConsole/*=FALSE*/ )
{
	LogMap::iterator i;
	i = m_log.begin();
	if (i != m_log.end())
	{
		if (i->second)
		{
			LogMap::iterator i;
			i = m_log.find(file);
			if (i != m_log.end())
			{
				if (bDbgView)
				{
					i->second->AddDevice(BLOG_DBGVIEW,&m_logdbgView);
				}
				else
				{
					i->second->RemoveDevice(BLOG_DBGVIEW);
				}
				if (bConsole)
				{
					i->second->AddDevice(BLOG_CONSOLE,&m_logConsole);
				}
				else
				{
					i->second->RemoveDevice(BLOG_CONSOLE);
				}
				return S_OK;
			}
		}
	}
	return S_OK;
}

HRESULT CGlobalCenter::LogPrefix( CString file,CString prefix,BOOL bAdd )
{
	blog::CBLog* log = _GetLogInstance(file);
	if (log)
	{
		if (bAdd)
		{
			log->AddPrefix(prefix);
		}
		else
		{
			log->RemoveLastPrefix();
		}
	}
	return S_OK;
}

HRESULT CGlobalCenter::LogIndent( CString file,BOOL bAdd )
{
	blog::CBLog* log = _GetLogInstance(file);
	if (log)
	{
		if (bAdd)
		{
			log->IncreaseIndent();
		}
		else
		{
			log->DecreaseIndent();
		}
	}
	return S_OK;
}

blog::CBLog* CGlobalCenter::_GetLogInstance(CString file)
{
	LogMap::iterator i;
	i = m_log.find(file);
	if (i != m_log.end())
	{
		return (i->second);
	}
	else
	{
		return _CreateLogInstance(file);
	}
	return NULL;
}

blog::CBLog* CGlobalCenter::_CreateLogInstance(CString file,BOOL bDbgView/* =TRUE */,BOOL bConsole/* =FALSE */)
{
	LogMap::iterator i;
	i = m_log.find(file);
	if (i != m_log.end())
	{
		return i->second;
	}
	else
	{
		blog::CBLog*log = new blog::CBLog;
		ASSERT(log);
		if (log)
		{
			if (bDbgView)
			{
				log->AddDevice(BLOG_DBGVIEW,&m_logdbgView);
			}
			if (bConsole)
			{
				log->AddDevice(BLOG_CONSOLE,&m_logConsole);
			}
			CString fileName = m_logPath;
			fileName += file;
			fileName += _LogFileExt;
			blog::CLogDeviceFile *logFile = new blog::CLogDeviceFile;
			if (logFile)
			{
				BOOL bResult = logFile->Open(fileName);
				//ATLASSERT(bResult && "log文件打开失败!");
				if (bResult)
				{
					log->AddDevice(BLOG_FILE,logFile);
				}
				else
				{
					DWORD dw = GetLastError();
					dw = dw;
				}
			}
			m_log.insert(std::make_pair(file,log));
			return log;
		}
	}
	ASSERT(FALSE);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////

CLogIndent::CLogIndent(CString file)
{
	m_file = file;
	Global2.LogIndent(m_file,TRUE);
}

CLogIndent::~CLogIndent()
{
	Global2.LogIndent(m_file,FALSE);
}

CLogPrefix::CLogPrefix(CString file,CString prefix)
{
	m_file = file;
	m_prefix = prefix;
	Global2.LogPrefix(m_file,m_prefix,TRUE);
}

CLogPrefix::~CLogPrefix()
{
	Global2.LogPrefix(m_file,m_prefix,FALSE);
}
