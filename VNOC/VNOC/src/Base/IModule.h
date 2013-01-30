#pragma once
#include "IRefCountImpl.h"

interface IModule
{
public:
	// UpperFrame提供了上层的接口，比如子模块会收到Frame模块的接口。请使用GetiModule而非QueryInterface来转换。
	STDMETHOD( Initialize(IModule* UpperFrame=NULL) = 0 );
	STDMETHOD( UnInitialize() = 0 );

	// 通知模块工作，模块内部可以使用独立线程，保证接口的线程安全即可。
	// 对于不存在私有线程的模块，这两个接口可以不使用
	STDMETHOD( Run() = 0 );
	// 通知退出
	STDMETHOD( Terminate() = 0 );
	//! Terminate之后的模块，应当可以通过Run再次启用。UnInitialize后的模块则不可
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