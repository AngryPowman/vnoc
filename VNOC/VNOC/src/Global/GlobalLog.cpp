#include "stdafx.h"
#include "GlobalCenter.h"
#include "../util/DbgHelper.hpp"

//////////////////////////////////////////////////////////////////////////
// 局部util函数

void _CheckFileSize(LPCTSTR file,long size)
{
	long fileSize = 0;
	FILE *fp;
	_tfopen_s(&fp,file,_T("rb"));
	if (fp)
	{
		fseek(fp,0,SEEK_END);
		fileSize = ftell(fp);
		fclose(fp);
	}
	if (fileSize > size)
	{
		DeleteFile(file);
	}
}

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
		Util::Filesys::ForceCreateDir(m_logPath);
	}
}

void CGlobalCenter::_UnInitializeLog()
{
	LogMap::iterator i;
	i = m_log.begin();
	while (i != m_log.end())
	{
		i->second->RemoveDevice(BLOG_DBGVIEW);
		i->second->RemoveDevice(BLOG_CONSOLE);
		delete i->second;
		++i;
	}
}

HRESULT CGlobalCenter::Log( CString file,CString str,BOOL info,BOOL endLine)
{
	blog::CBLog* log = _GetLogInstance(file);
	if (log)
	{
		log->Log(LogMask_ALL,str,info,endLine);
	}
	else
	{
		ATLASSERT(FALSE && L"无法获取到log对象");
	}
	return S_OK;
}

HRESULT CGlobalCenter::Logf( CString file,LPCTSTR str,... )
{
	static TCHAR buffer[_LogFormatBufferSize];
	buffer[_LogFormatBufferSize-1] = L'\0';
	va_list vl;
	va_start(vl,str);
	_vstprintf_s(buffer,str,vl);
	va_end(vl);
	return Log(file,buffer,TRUE,FALSE);
}

HRESULT CGlobalCenter::Lognf( CString file,LPCTSTR str,... )
{
	static TCHAR buffer[_LogFormatBufferSize];
	buffer[_LogFormatBufferSize-1] = L'\0';
	va_list vl;
	va_start(vl,str);
	_vstprintf_s(buffer,str,vl);
	va_end(vl);
	return Log(file,buffer,FALSE,FALSE);
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

HRESULT CGlobalCenter::SetThreadName( CString name )
{
	if (name.IsEmpty())
	{
		return S_FALSE;
	}
	SetThreadNameToDebugInfo(GetCurrentThreadId(),CStringA(name));
	m_threadName[GetCurrentThreadId()] = name;
	LogMap::iterator i;
	i = m_log.begin();
	while (i != m_log.end())
	{
		i->second->SetThreadName(GetCurrentThreadId(),name);
		++i;
	}
	return S_OK;
}

HRESULT CGlobalCenter::GetThreadName( DWORD threadID,CString& name )
{
	ThreadNameMap::iterator i;
	i = m_threadName.find(threadID);
	if (i != m_threadName.end())
	{
		name = i->second;
		return S_OK;
	}
	return S_FALSE;
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
		ATLASSERT(log);
		if (log)
		{
			_FillThreadNameToLogInstance(log);
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
				_CheckFileSize(fileName,_LogFileMaxSizeBytes);
				BOOL bResult = logFile->Open(fileName);
				ATLASSERT(bResult && "log文件打开失败!");
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
	ATLASSERT(FALSE);
	return NULL;
}

void CGlobalCenter::_FillThreadNameToLogInstance( blog::CBLog* inst )
{
	ATLASSERT(inst);
	if (inst)
	{
		ThreadNameMap::iterator i;
		for (i=m_threadName.begin(); i!=m_threadName.end(); ++i)
		{
			inst->SetThreadName(i->first,i->second);
		}
	}
}

//////////////////////////////////////////////////////////////////////////

CLogIndent::CLogIndent(CString file)
{
	m_file = file;
	Global->LogIndent(m_file,TRUE);
}

CLogIndent::~CLogIndent()
{
	Global->LogIndent(m_file,FALSE);
}

CLogPrefix::CLogPrefix(CString file,CString prefix)
{
	m_file = file;
	m_prefix = prefix;
	Global->LogPrefix(m_file,m_prefix,TRUE);
}

CLogPrefix::~CLogPrefix()
{
	Global->LogPrefix(m_file,m_prefix,FALSE);
}
