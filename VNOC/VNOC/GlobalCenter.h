//////////////////////////////////////////////////////////////////////////
// 管理了应用程序全局数据，如配置信息等。
#pragma once
#include "IGlobal.h"
#include "Config.h"
#include "Net.h"
#include "../util/BLog.h"

typedef std::map<CString,blog::CBLog*>	LogMap;
typedef std::map<DWORD,CString>	ThreadNameMap;

class CGlobalCenter : public IGlobal
{
public:
	CGlobalCenter(void);
	~CGlobalCenter(void);
public:
	STDMETHOD( Initialize(IModule* UpperFrame=NULL) );
	STDMETHOD( UnInitialize() );
	STDMETHOD( Run() );
	STDMETHOD( Terminate() );
	// 获取配置接口
	STDMETHOD( GetIConfig(IConfig** pConfig) );

	STDMETHOD( Log(CString file,CString str,BOOL info=TRUE,BOOL endLine=TRUE));
	STDMETHOD( Logf(CString file,LPCTSTR str,...) );
	STDMETHOD( Lognf(CString file,LPCTSTR str,...) );
	STDMETHOD( SetLog(CString file,BOOL bDbgView=TRUE,BOOL bConsole=FALSE));
	STDMETHOD( LogPrefix(CString file,CString prefix,BOOL bAdd));
	STDMETHOD( LogIndent(CString file,BOOL bAdd));
	STDMETHOD( SetThreadName(CString name));
	STDMETHOD( GetThreadName(DWORD threadID,CString& name));
	STDMETHOD( CheckLastError(LPCTSTR helpStr=NULL));
	STDMETHOD( PtrAssert(void* p));
	STDMETHOD( CriticalError(LPCTSTR message));

	STDMETHOD( GetINetCenter(INetCenter** pNetCenter) );
private:
	void _InitializeConfig();
	void _UnInitializeConfig();

	void _InitializeLog();
	void _UnInitializeLog();
	blog::CBLog* _GetLogInstance(CString file);
	blog::CBLog* _CreateLogInstance(CString file,BOOL bDbgView=TRUE,BOOL bConsole=FALSE);
	void _FillThreadNameToLogInstance(blog::CBLog* inst);

	void _InitializeNetCenter();
	void _UnInitializeNetCenter();
private:
	CConfigImpl		m_config;
	CNetCenter		m_netCenter;
	CString		m_configFilePath;
	
	LogMap		m_log;
	blog::CLogDeviceDBGView m_logdbgView;
	blog::CLogDeviceConsole m_logConsole;
	CString		m_logPath;

	ThreadNameMap	m_threadName;
};


extern CGlobalCenter Global2;
