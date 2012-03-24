#pragma once
// 定义了全局管理器的接口
#include "IModule.h"
#include "IConfig.h"

interface IGlobal:public IModule
{
public:
	STDMETHOD( GetIConfig(std::auto_ptr<IConfig>& pConfig) = 0);
};

extern IGlobal* Global;		// 该指针假设在所有模块的生命期内都是合法的。
