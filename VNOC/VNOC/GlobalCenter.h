//////////////////////////////////////////////////////////////////////////
// ������Ӧ�ó���ȫ�����ݣ���������Ϣ�ȡ�
#pragma once
#include "IModule.h"
#include "IConfig.h"

class CGlobalCenter : public IModule
{
public:
	CGlobalCenter(void);
	~CGlobalCenter(void);
public:
	STDMETHOD( Initialize(IModule* UpperFrame=NULL) );
	STDMETHOD( UnInitialize() );
	STDMETHOD( Run() );
	STDMETHOD( Terminate() );
	// ��ȡ���ýӿ�
	STDMETHOD( GetIConfig(IConfig** ppConfig) );
};


extern CGlobalCenter Global;
