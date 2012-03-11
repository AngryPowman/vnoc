#pragma once
#include "IModule.h"

interface ILogin : public IModule
{
public:
	STDMETHOD( Show(BOOL bShow=TRUE) = 0 );
	STDMETHOD( SetAccount(LPCTSTR userName) = 0);
};