#pragma once
#include "IRefCountImpl.h"

interface IModule : public IRefCountImpl
{
public:
	// UpperFrame提供了上层的接口，比如子模块会收到Frame模块的接口。请使用QueryInterface
	STDMETHOD( Initialize(IUnknown* UpperFrame=NULL) = 0 );
	STDMETHOD( UnInitialize() = 0 );
	// 通知模块初始化，模块内部可以使用独立线程，保证接口的线程安全即可。
	STDMETHOD( Run() = 0 );
	// 通知退出
	STDMETHOD( Terminate() = 0 );
};