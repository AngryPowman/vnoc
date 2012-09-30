#pragma once
#include "IRefCountImpl.h"

interface IModule
{
public:
	// UpperFrame�ṩ���ϲ�Ľӿڣ�������ģ����յ�Frameģ��Ľӿڡ���ʹ��GetiModule����QueryInterface��ת����
	STDMETHOD( Initialize(IModule* UpperFrame=NULL) = 0 );
	STDMETHOD( UnInitialize() = 0 );

	// ֪ͨģ�鹤����ģ���ڲ�����ʹ�ö����̣߳���֤�ӿڵ��̰߳�ȫ���ɡ�
	// ���ڲ�����˽���̵߳�ģ�飬�������ӿڿ��Բ�ʹ��
	STDMETHOD( Run() = 0 );
	// ֪ͨ�˳�
	STDMETHOD( Terminate() = 0 );
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