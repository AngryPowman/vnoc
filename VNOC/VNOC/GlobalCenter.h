//////////////////////////////////////////////////////////////////////////
// ������Ӧ�ó���ȫ�����ݣ���������Ϣ�ȡ�
#pragma once
#include "IGlobal.h"
#include "Config.h"
#include "../util/BLog.h"

typedef std::map<CString,blog::CBLog*>	LogMap;

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
	// ��ȡ���ýӿ�
	STDMETHOD( GetIConfig(IConfig** pConfig) );
	STDMETHOD( Log(CString file,CString str));
	STDMETHOD( Logf(CString file,LPCTSTR str,...) );
	STDMETHOD( SetLog(CString file,BOOL bDbgView=TRUE,BOOL bConsole=FALSE));
	STDMETHOD( LogPrefix(CString file,CString prefix,BOOL bAdd));
	STDMETHOD( LogIndent(CString file,BOOL bAdd));
	STDMETHOD( SetThreadName(CString name));
private:
	void _InitializeConfig();
	void _UnInitializeConfig();
	void _InitializeLog();
	void _UnInitializeLog();
	blog::CBLog* _GetLogInstance(CString file);
	blog::CBLog* _CreateLogInstance(CString file,BOOL bDbgView=TRUE,BOOL bConsole=FALSE);
private:
	CConfig		m_config;
	
	LogMap		m_log;
	blog::CLogDeviceDBGView m_logdbgView;
	blog::CLogDeviceConsole m_logConsole;
	CString		m_logPath;
};


extern CGlobalCenter Global2;
