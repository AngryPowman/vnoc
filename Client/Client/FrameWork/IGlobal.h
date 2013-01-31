#pragma once
// ������ȫ�ֹ������Ľӿ�
#include "IModule.h"
#include "../config/IConfig.h"
#include "../net/INet.h"
#include "../FrameWork/IFrameWork.h"
#include "GlobalDefine.h"
#include "Error.hpp"

interface IGlobal:public IModule
{
public:
	// ����Global���󡣳�����������Ҫ����
	static VOID GlobalDeliver();
	//////////////////////////////////////////////////////////////////////////
	// �ӿ�
	virtual ResultCode GetIConfig(IConfig** pConfig) = 0;
	virtual ResultCode GetINetCenter(INetCenter** pNetCenter) = 0;
	virtual ResultCode GetIFrameModule(IFrameWork** pFrame) = 0;

	//////////////////////////////////////////////////////////////////////////
	// log�ӿ�
	// printInfo	���ʱ�䡢�߳�ID��ǰ׺��
	// endLine	�Զ��س�����
	virtual ResultCode Log(CString file,CString str,BOOL printInfo=TRUE,BOOL endLine=TRUE) = 0;
	
	// ǿ�Ʋ����Զ��س�����,��ʱ�����Ϣ
	virtual ResultCode Logf(CString file,LPCTSTR str,...) = 0;
	// ǿ�Ʋ����Զ��س�����,����ʱ�����Ϣ
	virtual ResultCode Lognf(CString file,LPCTSTR str,...) = 0;

	virtual ResultCode LogPrefix(CString file,CString prefix,BOOL bAdd) = 0;
	virtual ResultCode LogIndent(CString file,BOOL bAdd) = 0;
	
	// ����log
	// bDebugView	�Ƿ����ͨ��dbgView�鿴
	// bConsole		�Ƿ������Console����ʾ
	virtual ResultCode SetLog(CString file,BOOL bDbgView=TRUE,BOOL bConsole=FALSE) = 0;

	//////////////////////////////////////////////////////////////////////////
	// �߳�
	// �����߳����ƣ���Ҫ��logʹ�á�
	// ÿ���̴߳�����Ӧ����������ӿڡ�
	virtual ResultCode SetThreadName(CString name) = 0;
	virtual ResultCode GetThreadName(DWORD threadID,CString& name) = 0;

	//////////////////////////////////////////////////////////////////////////
	// ���GetLastError,�Ὣ����ͨ��log�����
	// helpStr	�������ֺ���������
	virtual DWORD CheckLastError(LPCTSTR helpStr=NULL) = 0;
	// ���ָ���Ƿ�ΪNULL�����ΪNULL������Ϊ���ش��󣬲��Ѻõ��˳�����
	virtual ResultCode PtrAssert(void* p) = 0;
	virtual ResultCode CriticalError(LPCTSTR message) = 0;

	//////////////////////////////////////////////////////////////////////////
	// ��Ϣ
};

extern IGlobal* Global;		// ��ָ�����������ģ����������ڶ��ǺϷ��ġ�

// �������ڵ�log�����������ǰ׺
class CLogPrefix : private CGlobalUtilBase
{
public:
	CLogPrefix(CString file,CString prefix);
	~CLogPrefix();
};

// �������ڵ�log���Զ���������
class CLogIndent : private CGlobalUtilBase
{
public:
	CLogIndent(CString file);
	~CLogIndent();
};
