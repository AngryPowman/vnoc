#pragma once
#include "Error.hpp"

interface IModule
{
public:
	// UpperFrame�ṩ���ϲ�Ľӿ�
	virtual ResultCode Initialize(IModule* UpperFrame=NULL) = 0 ;
	virtual ResultCode UnInitialize() = 0;

	virtual ResultCode Run() = 0;
	virtual ResultCode Terminate() = 0 ;
	//! Terminate֮���ģ�飬Ӧ������ͨ��Run�ٴ����á�UnInitialize���ģ���򲻿�
};

// ����ת���ӿ�
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