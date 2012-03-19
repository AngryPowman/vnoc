#include "StdAfx.h"
#include "GlobalCenter.h"

CGlobalCenter Global;

CGlobalCenter::CGlobalCenter(void)
{
}


CGlobalCenter::~CGlobalCenter(void)
{
}

HRESULT CGlobalCenter::Initialize( IModule* UpperFrame/*=NULL*/ )
{
	return S_OK;
}

HRESULT CGlobalCenter::UnInitialize()
{
	return S_OK;
}

HRESULT CGlobalCenter::Run()
{
	return S_OK;
}

HRESULT CGlobalCenter::Terminate()
{
	return S_OK;
}

HRESULT CGlobalCenter::GetIConfig( IConfig** ppConfig )
{
	return S_OK;
}
