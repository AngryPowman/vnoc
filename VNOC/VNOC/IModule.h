#pragma once
#include "IRefCountImpl.h"

interface IModule : public IRefCountImpl
{
public:
	STDMETHOD( Run() );
	// ֪ͨ�˳�
	STDMETHOD( Terminate() );
};