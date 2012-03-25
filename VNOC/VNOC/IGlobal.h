#pragma once
// ������ȫ�ֹ������Ľӿ�
#include "IModule.h"
#include "IConfig.h"
#include "GlobalDefine.h"

interface IGlobal:public IModule
{
public:
	//////////////////////////////////////////////////////////////////////////
	// ���ýӿ�
	STDMETHOD( GetIConfig(IConfig** pConfig) = 0);
	//////////////////////////////////////////////////////////////////////////
	// log�ӿ�
	STDMETHOD( Log(CString file,CString str) = 0 );
	STDMETHOD( Logf(CString file,LPCTSTR str,...) = 0 );
	// ����log
	// bDebugView	�Ƿ����ͨ��dbgView�鿴
	// bConsole		�Ƿ������Console����ʾ
	STDMETHOD( SetLog(CString file,BOOL bDbgView=TRUE,BOOL bConsole=FALSE) = 0 );

	//////////////////////////////////////////////////////////////////////////
	//
	// �����߳����ƣ���Ҫ��logʹ�ã�Ҳ���Թ���ģ�����Էֱ浱ǰ�̡߳�
	// ÿ���̴߳�����Ӧ����������ӿڡ�
	STDMETHOD( SetThreadName(CString name) = 0 );
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
