#include "StdAfx.h"
#include "VNOCFrame.h"


CVNOCFrame::CVNOCFrame(void)
{
}


CVNOCFrame::~CVNOCFrame(void)
{
}

HRESULT CVNOCFrame::Initialize()
{
	return S_OK;
}

HRESULT CVNOCFrame::UnInitialize()
{
	return S_OK;
}

HRESULT CVNOCFrame::RegisterModule( VNOCModule module,IModule* iModule )
{
	return S_OK;
}

HRESULT CVNOCFrame::GetModule( VNOCModule module,IModule** piModule )
{
	return S_OK;
}

HRESULT CVNOCFrame::ReleaseModule( IModule* iModule )
{
	return S_OK;
}

HRESULT CVNOCFrame::SetStartupModule( VNOCModule module )
{
	return S_OK;
}
