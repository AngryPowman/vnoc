#pragma once
#include "IFrameWork.h"

class CFrameModuleFactory
{
public:
	static ResultCode CreateFrameModule(FrameModule module,IFrameModule** ppModule);
};
