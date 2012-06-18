#pragma once
#include "IModule.h"

interface IRoom : public IModule
{
public:
	STDMETHOD( Show(BOOL bShow=TRUE) = 0 );
};