#include "BLog.h"
#include "util.h"

namespace blog
{
	CBLog::CBLog(void)
	{
		m_indent = 0;
	}

	CBLog::~CBLog(void)
	{
		LogDeviceListType::iterator i;
		i = m_logDevice.begin();
		while (i != m_logDevice.end())
		{
			i->second->Close();
			delete i->second;
			++i;
		}
	}

	void CBLog::AddDevice( BLOG_TYPE type,CLogDeviceBase* device )
	{
		m_cs.Enter();
		if (m_logDevice.find(type) != m_logDevice.end())
		{
			delete m_logDevice[type];
		}
		m_logDevice.insert(std::make_pair(type,device));
		m_cs.Leave();
	}

	void CBLog::Log( DWORD deviceMask,LPCTSTR strlog,BOOL info/*=TRUE*/,BOOL autoEndLine/*=TRUE*/ )
	{
		CString logText;
		if (info)
		{
			_AddInfo(logText);
			_AddPrefix(logText);
			logText += _T('|');
			logText += _T('\t');
			_AddIndent(logText);
		}
		logText += strlog;
		if (autoEndLine)
		{
			logText += _T('\n');
		}

		m_cs.Enter();
		LogDeviceListType::iterator i;
		i = m_logDevice.begin();
		while (i != m_logDevice.end())
		{
			if (deviceMask & i->first)
			{
				i->second->Write(logText);
			}
			++i;
		}
		m_cs.Leave();
	}

	void CBLog::Logf( DWORD deviceMask,LPCTSTR fmt,... )
	{
		CString strFormat;
		va_list vlist;
		va_start(vlist,fmt);
		strFormat.Format(fmt,vlist);
		va_end(vlist);
		return Log(deviceMask,strFormat,TRUE,FALSE);
	}

	void CBLog::_AddIndent(CString &strLog)
	{
		if (m_indent)
		{
			strLog += _T(" ");
		}
		for (UINT n=0; n<m_indent; ++n)
		{
			strLog += _T('\t');
		}
	}

	void CBLog::_AddInfo( CString &strLog )
	{
		time_t ct = time(NULL);
		tm otm;
		localtime_s(&otm, &ct);
		SYSTEMTIME st;
		GetSystemTime(&st);
		strLog.Format(_T("%d-%d-%d %d:%d:%d.%d \t"),
			otm.tm_year+1900,otm.tm_mon+1,otm.tm_mday,
			otm.tm_hour,otm.tm_min,otm.tm_sec,st.wMilliseconds
			);
		DWORD threadID = GetCurrentThreadId();
		ThreadNameMap::iterator i;
		i = m_threadNameMap.find(threadID);
		if ( i != m_threadNameMap.end() )
		{
			strLog.AppendFormat(_T("%s(%d) \t"), i->second,threadID);
		}
		else
		{
			strLog.AppendFormat(_T("%d \t"), threadID);
		}
	}

	void CBLog::_AddPrefix( CString &strLog )
	{
		PrefixVectorType::iterator i;
		i = m_Prefix.begin();
		if (i == m_Prefix.end())
		{
			return;
		}
		while (i != m_Prefix.end())
		{
			strLog += *i++;
			if (i != m_Prefix.end())
			{
				strLog +=_T("::");
			}
		}
		//strLog += _T(" \t");
	}

	void CBLog::AddPrefix( LPCTSTR strPrefix )
	{
		ATLASSERT(strPrefix);
		if (strPrefix)
		{
			m_Prefix.push_back(strPrefix);
		}
	}

	void CBLog::RemoveLastPrefix()
	{
		if (!m_Prefix.empty())
		{
			m_Prefix.pop_back();
		}
	}

	CLogDeviceBase* CBLog::GetDevice( BLOG_TYPE type )
	{
		LogDeviceListType::iterator i;
		i = m_logDevice.find(type);
		if (i != m_logDevice.end())
		{
			return i->second;
		}
		return NULL;
	}

	CLogDeviceBase* CBLog::RemoveDevice( BLOG_TYPE type )
	{
		LogDeviceListType::iterator i;
		i = m_logDevice.find(type);
		if ( i != m_logDevice.end())
		{
			CLogDeviceBase* pBase = i->second;
			m_logDevice.erase(i);
			return pBase;
		}
		return NULL;
	}

	void CBLog::SetThreadName( DWORD threadID,LPCTSTR name )
	{
		ThreadNameMap::iterator i;
		i = m_threadNameMap.find(threadID);
		if ( i != m_threadNameMap.end())
		{
			i->second = name;
		}
		else
		{
			m_threadNameMap.insert(std::make_pair(threadID,name));
		}
	}


	BOOL CLogDeviceFile::Open(LPCTSTR param)
	{
		m_fp = NULL;
		CString logFile;
		if (param)
		{
			logFile = param;
			errno_t err = _tfopen_s(&m_fp,logFile,_T("at+"));
			if (m_fp)
			{
				CTime currTime = CTime::GetCurrentTime();
				_ftprintf_s(m_fp,_T("-----------------------------------\nLog Generated At %s\n"),currTime.Format(TIME_FORMAT_STRING));
			}
		}
		return m_fp!=NULL;
	}

	void CLogDeviceFile::Close()
	{
		if (m_fp)
		{
			fclose(m_fp); m_fp = NULL;
		}
	}

	void CLogDeviceFile::Write( LPCTSTR strLog )
	{
		if (strLog && m_fp)
		{
			_ftprintf_s(m_fp,_T("%s"),strLog);
		}
	}

	void CLogDeviceFile::flush()
	{
		if (m_fp)
		{
			fflush(m_fp);
		}
	}


	CLogDeviceConsole::CLogDeviceConsole() : m_free_console_on_close(false)
		, m_handle(NULL)
		, m_default_attr(0)
	{

	}

	BOOL CLogDeviceConsole::Open(LPCTSTR param)
	{
		if (::AllocConsole())
		{
			m_free_console_on_close = true;
		}
		m_handle = ::GetStdHandle(STD_OUTPUT_HANDLE);
		if (is_handle_valid(m_handle))
		{
			CONSOLE_SCREEN_BUFFER_INFO csbi;
			GetConsoleScreenBufferInfo(m_handle, &csbi);
			m_default_attr = csbi.wAttributes;
			return TRUE;
		}
		return FALSE;
	}

	void CLogDeviceConsole::Close()
	{
		if (m_free_console_on_close)
		{
			::FreeConsole();
			m_handle = NULL;
		}
	}

	void CLogDeviceConsole::Write( LPCTSTR strLog )
	{
		if (!is_handle_valid(m_handle)) return;
		DWORD wrote;
		::WriteConsole(m_handle, strLog, static_cast<DWORD>(_tcslen(strLog)), &wrote, NULL);
	}

}