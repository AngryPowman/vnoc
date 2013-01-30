#pragma once
#include "vector"
#include "map"
#include "time.h"
#include "atlstr.h"
#include "atlsync.h"
#include <atltime.h>

// 除了标准类型以外想加入新输出端(如多file输出)，自行定义一个数即可，不需要在这里添加定义，用static_cast强转类型
enum BLOG_TYPE
{
	BLOG_NOLOG = 0,
	BLOG_DBGVIEW = 1,
	BLOG_FILE = 2,
	BLOG_CONSOLE = 4
};

#define TIME_FORMAT_STRING _T("%Y-%m-%d %H:%M:%S")

namespace blog
{
	class CLogDeviceBase
	{
	public:
		virtual ~CLogDeviceBase(){};
		virtual BOOL Open(LPCTSTR param=NULL) = 0;
		virtual void Close() = 0;
		virtual void Write(LPCTSTR strLog) = 0;
		virtual void flush() = 0;
	};

	typedef std::map<DWORD,CLogDeviceBase*> LogDeviceListType;
	typedef std::vector<CString>			PrefixVectorType;
	typedef std::map<DWORD,CString>			ThreadNameMap;

	class CBLog
	{
	public:
		CBLog(void);
		~CBLog(void);
	public:
		void SetThreadName(DWORD threadID,LPCTSTR name);

		void Log(DWORD deviceMask,LPCTSTR strlog,BOOL info=TRUE,BOOL autoEndLine=TRUE);
		void Logf(DWORD deviceMask,LPCTSTR fmt,...);

		void AddDevice(BLOG_TYPE type,CLogDeviceBase* device);
		CLogDeviceBase* GetDevice(BLOG_TYPE type);
		CLogDeviceBase* RemoveDevice(BLOG_TYPE type);

		void AddPrefix(LPCTSTR strPrefix);
		void RemoveLastPrefix();

		void IncreaseIndent(){++m_indent;};
		void DecreaseIndent(){if(m_indent)--m_indent;};

	private:
		void _AddInfo(CString &strLog);
		void _AddPrefix(CString &strLog);
		void _AddIndent(CString &strlog);
	private:
		DWORD m_indent;
		LogDeviceListType m_logDevice;
		PrefixVectorType m_Prefix;
		ThreadNameMap m_threadNameMap;
		ATL::CCriticalSection	m_cs;
	};

	class CLogDeviceFile:public CLogDeviceBase
	{
	public:
		virtual BOOL Open(LPCTSTR param=NULL);
		virtual void Close();
		virtual void Write(LPCTSTR strLog);
		virtual void flush();
	private:
		CString m_file;
		FILE *m_fp;
	};

	class CLogDeviceDBGView:public CLogDeviceBase
	{
	public:
		virtual BOOL Open(LPCTSTR param=NULL){return TRUE;};
		virtual void Close(){};
		virtual void Write(LPCTSTR strLog)
		{
			::OutputDebugString(strLog);
		};
		virtual void flush(){};
	};

	class CLogDeviceMem : public CLogDeviceBase
	{
	public:
		virtual BOOL Open(LPCTSTR param=NULL){return TRUE;};
		virtual void Close(){};
		virtual void Write(LPCTSTR strLog)
		{
			m_log += strLog;
		};
		virtual void flush(){};

		void get_log(std::wstring& str){str = CStringW(m_log);};

	private:
		CString m_log;
	};


	class CLogDeviceConsole : public CLogDeviceBase
	{
	public:
		CLogDeviceConsole();

		virtual BOOL Open(LPCTSTR param=NULL);
		virtual void Close();
		virtual void flush(){}

		virtual void Write(LPCTSTR strLog);

	protected:
		HANDLE m_handle;
		WORD m_default_attr;
		bool m_free_console_on_close;
		bool padding[1];

	private:
		static bool is_handle_valid(HANDLE handle)
		{
			return (handle != NULL && handle != INVALID_HANDLE_VALUE);
		}
	};
}

namespace blog
{
	class CAutoBLogIndent
	{
	public:
		CAutoBLogIndent(CBLog *theLog) : m_theLog(NULL)
		{
			if (theLog)
			{
				m_theLog = theLog;
				theLog->IncreaseIndent();
			}
		};
		~CAutoBLogIndent()
		{
			if (m_theLog)
			{
				m_theLog->DecreaseIndent();
			}
		}
		CBLog *m_theLog;
	};

	class CAutoBLogPrefix
	{
	public:
		CAutoBLogPrefix(CBLog *theLog,LPCTSTR text){if(theLog){m_theLog=theLog;theLog->AddPrefix(text);}}
		~CAutoBLogPrefix(){m_theLog->RemoveLastPrefix();}
		CBLog *m_theLog;
	};
}
