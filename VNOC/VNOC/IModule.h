#pragma once
#include "IRefCountImpl.h"

interface IModule : public IRefCountImpl
{
public:
	STDMETHOD( Run() );
	// Í¨ÖªÍË³ö
	STDMETHOD( Terminate() );
};