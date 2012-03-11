#pragma once
#include "IRefCountImpl.h"

interface IModule : public IRefCountImpl
{
public:
	// UpperFrame�ṩ���ϲ�Ľӿڣ�������ģ����յ�Frameģ��Ľӿڡ���ʹ��QueryInterface
	STDMETHOD( Initialize(IUnknown* UpperFrame=NULL) = 0 );
	STDMETHOD( UnInitialize() = 0 );
	// ֪ͨģ���ʼ����ģ���ڲ�����ʹ�ö����̣߳���֤�ӿڵ��̰߳�ȫ���ɡ�
	STDMETHOD( Run() = 0 );
	// ֪ͨ�˳�
	STDMETHOD( Terminate() = 0 );
};