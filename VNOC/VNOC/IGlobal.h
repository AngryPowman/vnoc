#pragma once
// ������ȫ�ֹ������Ľӿ�
#include "IModule.h"
#include "IConfig.h"
#include "INet.h"
#include "GlobalDefine.h"

interface IGlobal:public IModule
{
public:
	//////////////////////////////////////////////////////////////////////////
	// �ӿ�
	STDMETHOD( GetIConfig(IConfig** pConfig) = 0);
	STDMETHOD( GetINetCenter(INetCenter** pNetCenter) = 0);

	//////////////////////////////////////////////////////////////////////////
	// log�ӿ�
	// printInfo	���ʱ�䡢�߳�ID��ǰ׺��
	// endLine	�Զ��س�����
	STDMETHOD( Log(CString file,CString str,BOOL printInfo=TRUE,BOOL endLine=TRUE) = 0 );
	
	// ǿ�Ʋ����Զ��س�����,��ʱ�����Ϣ
	STDMETHOD( Logf(CString file,LPCTSTR str,...) = 0 );
	// ǿ�Ʋ����Զ��س�����,����ʱ�����Ϣ
	STDMETHOD( Lognf(CString file,LPCTSTR str,...) = 0 );
	
	// ����log
	// bDebugView	�Ƿ����ͨ��dbgView�鿴
	// bConsole		�Ƿ������Console����ʾ
	STDMETHOD( SetLog(CString file,BOOL bDbgView=TRUE,BOOL bConsole=FALSE) = 0 );

	//////////////////////////////////////////////////////////////////////////
	// �߳�
	// �����߳����ƣ���Ҫ��logʹ�á�
	// ÿ���̴߳�����Ӧ����������ӿڡ�
	STDMETHOD( SetThreadName(CString name) = 0 );
	STDMETHOD( GetThreadName(DWORD threadID,CString& name) = 0 );

	//////////////////////////////////////////////////////////////////////////
	// ���GetLastError,�Ὣ����ͨ��log�����
	// helpStr	�������ֺ���������
	STDMETHOD( CheckLastError(LPCTSTR helpStr=NULL) = 0 );
	// ���ָ���Ƿ�ΪNULL�����ΪNULL������Ϊ���ش��󣬲��Ѻõ��˳�����
	STDMETHOD( PtrAssert(void* p) = 0 );
	STDMETHOD( CriticalError(LPCTSTR message) = 0 );

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
