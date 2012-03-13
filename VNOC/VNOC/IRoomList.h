#pragma once
#include "IModule.h"

interface IRoomList : public IModule
{
public:
	STDMETHOD( Show(BOOL bShow=TRUE) = 0 );
};