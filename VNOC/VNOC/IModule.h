#pragma once
#include "IRefCountImpl.h"

// ���������ü�����������������ʹ��
interface IModule : public IRefCountImpl
{
public:
	// UpperFrame�ṩ���ϲ�Ľӿڣ�������ģ����յ�Frameģ��Ľӿڡ���ʹ��GetiModule����QueryInterface��ת����
	STDMETHOD( Initialize(IModule* UpperFrame=NULL) = 0 );
	STDMETHOD( UnInitialize() = 0 );
	// ֪ͨģ���ʼ����ģ���ڲ�����ʹ�ö����̣߳���֤�ӿڵ��̰߳�ȫ���ɡ�
	STDMETHOD( Run() = 0 );
	// ֪ͨ�˳�
	STDMETHOD( Terminate() = 0 );
};

template<class retType>
void GetiModule(IModule* pBase,retType** pRet)
{
	if (pRet && pBase)
	{
		*pRet = dynamic_cast<retType*>(pBase);
	}
}