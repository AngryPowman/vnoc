#pragma once
#include "IRefCountImpl.h"

// 尽管有引用计数，不过尽量避免使用
interface IModule : public IRefCountImpl
{
public:
	// UpperFrame提供了上层的接口，比如子模块会收到Frame模块的接口。请使用GetiModule而非QueryInterface来转换。
	STDMETHOD( Initialize(IModule* UpperFrame=NULL) = 0 );
	STDMETHOD( UnInitialize() = 0 );
	// 通知模块工作，模块内部可以使用独立线程，保证接口的线程安全即可。
	STDMETHOD( Run() = 0 );
	// 通知退出
	STDMETHOD( Terminate() = 0 );
	// 取子模块。设计好的模块应当是在绝大多数情况不需要暴漏子模块的。
};

// 方便转换接口
template<class retType>
void GetiModule(IModule* pBase,retType** pRet)
{
	ATLASSERT(pBase); ATLASSERT(pRet);
	if (pRet && pBase)
	{
		*pRet = dynamic_cast<retType*>(pBase);
		ATLASSERT(pRet);
	}
}