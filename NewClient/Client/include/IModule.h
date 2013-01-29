#pragma once
#include "Error.hpp"

interface IModule
{
public:
	// UpperFrame提供了上层的接口
	virtual ResultCode Initialize(IModule* UpperFrame=NULL) = 0 ;
	virtual ResultCode UnInitialize() = 0;

	virtual ResultCode Run() = 0;
	virtual ResultCode Terminate() = 0 ;
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