//////////////////////////////////////////////////////////////////////////
// ������Ӧ�ó���ȫ�����ݣ���������Ϣ�ȡ�
#pragma once
#include "IGlobal.h"
#include "../config/Config.h"
#include "../net/Net.h"
#include "../util/BLog.h"
#include "../FrameWork/FrameWork.h"

typedef std::map<CString,blog::CBLog*>	LogMap;
typedef std::map<DWORD,CString>	ThreadNameMap;

class CGlobalCenter : public IGlobal
{
public:
	CGlobalCenter(CString appName){m_appName = appName;};
	~CGlobalCenter(void);
private:
	CGlobalCenter(void);
	CGlobalCenter(const CGlobalCenter&){};
	CGlobalCenter& operator=(const CGlobalCenter&){};
public:
	virtual ResultCode Initialize(IModule* UpperFrame=NULL);
	virtual ResultCode UnInitialize();
	virtual ResultCode Run();
	virtual ResultCode Terminate();
	// ��ȡ���ýӿ�
	virtual ResultCode GetIConfig(IConfig** pConfig);

	virtual ResultCode Log(CString file,CString str,BOOL info=TRUE,BOOL endLine=TRUE);
	virtual ResultCode Logf(CString file,LPCTSTR str,...);
	virtual ResultCode Lognf(CString file,LPCTSTR str,...);
	virtual ResultCode SetLog(CString file,BOOL bDbgView=TRUE,BOOL bConsole=FALSE);
	virtual ResultCode LogPrefix(CString file,CString prefix,BOOL bAdd);
	virtual ResultCode LogIndent(CString file,BOOL bAdd);
	virtual ResultCode SetThreadName(CString name);
	virtual ResultCode GetThreadName(DWORD threadID,CString& name);
	virtual DWORD CheckLastError(LPCTSTR helpStr=NULL);
	virtual ResultCode PtrAssert(void* p);
	virtual ResultCode CriticalError(LPCTSTR message);

	virtual ResultCode GetINetCenter(INetCenter** pNetCenter);

	virtual ResultCode GetIFrameModule(IFrameWork** pFrame);
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

	void _InitializeFrameWork();
	void _UnInitializeFrameWork();
private:
	CString	m_appName;

	CConfigImpl		m_config;
	CNetCenter		m_netCenter;
	CFrameWork*		m_pFrameWork;
	CString		m_configFilePath;
	
	LogMap		m_log;
	blog::CLogDeviceDBGView m_logdbgView;
	blog::CLogDeviceConsole m_logConsole;
	CString		m_logPath;

	ThreadNameMap	m_threadName;
};
