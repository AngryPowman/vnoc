#pragma once
// ������ȫ�ֹ������Ľӿ�
#include "IModule.h"
#include "IConfig.h"

interface IGlobal:public IModule
{
public:
	STDMETHOD( GetIConfig(std::auto_ptr<IConfig>& pConfig) = 0);
};

extern IGlobal* Global;		// ��ָ�����������ģ����������ڶ��ǺϷ��ġ�
