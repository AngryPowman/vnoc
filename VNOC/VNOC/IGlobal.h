#pragma once
// 定义了全局管理器的接口
#include "IModule.h"
#include "IConfig.h"
#include "INet.h"
#include "GlobalDefine.h"

interface IGlobal:public IModule
{
public:
	//////////////////////////////////////////////////////////////////////////
	// 接口
	STDMETHOD( GetIConfig(IConfig** pConfig) = 0);
	STDMETHOD( GetINetCenter(INetCenter** pNetCenter) = 0);

	//////////////////////////////////////////////////////////////////////////
	// log接口
	// printInfo	输出时间、线程ID、前缀等
	// endLine	自动回车换行
	STDMETHOD( Log(CString file,CString str,BOOL printInfo=TRUE,BOOL endLine=TRUE) = 0 );
	
	// 强制不会自动回车换行,带时间等信息
	STDMETHOD( Logf(CString file,LPCTSTR str,...) = 0 );
	// 强制不会自动回车换行,不带时间等信息
	STDMETHOD( Lognf(CString file,LPCTSTR str,...) = 0 );
	
	// 设置log
	// bDebugView	是否可以通过dbgView查看
	// bConsole		是否输出到Console中显示
	STDMETHOD( SetLog(CString file,BOOL bDbgView=TRUE,BOOL bConsole=FALSE) = 0 );

	//////////////////////////////////////////////////////////////////////////
	// 线程
	// 设置线程名称，主要是log使用。
	// 每个线程创建后都应当调用这个接口。
	STDMETHOD( SetThreadName(CString name) = 0 );
	STDMETHOD( GetThreadName(DWORD threadID,CString& name) = 0 );

	//////////////////////////////////////////////////////////////////////////
	// 检查GetLastError,会将数据通过log输出。
	// helpStr	用于区分函数调用者
	STDMETHOD( CheckLastError(LPCTSTR helpStr=NULL) = 0 );
	// 检查指针是否为NULL，如果为NULL，将视为严重错误，并友好地退出程序。
	STDMETHOD( PtrAssert(void* p) = 0 );
	STDMETHOD( CriticalError(LPCTSTR message) = 0 );

	//////////////////////////////////////////////////////////////////////////
	// 消息
};

extern IGlobal* Global;		// 该指针假设在所有模块的生命期内都是合法的。

// 生命期内的log都将带上这个前缀
class CLogPrefix : private CGlobalUtilBase
{
public:
	CLogPrefix(CString file,CString prefix);
	~CLogPrefix();
};

// 生命期内的log将自动向右缩进
class CLogIndent : private CGlobalUtilBase
{
public:
	CLogIndent(CString file);
	~CLogIndent();
};
