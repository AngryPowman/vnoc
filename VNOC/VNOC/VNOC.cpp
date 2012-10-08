// VNOC.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "VNOC.h"
#include "src/FrameWork/IFrameWork.h"

#ifdef USE_VLD
#ifdef _DEBUG
#pragma comment(lib,"vld.lib")
#include "../../VisualLeakDetector/include/vld.h"
#endif
#endif

CAppModule _Module;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	_Module.Init(NULL,hInstance);
	IGlobal::GlobalDeliver();

	::CoInitializeEx(NULL,COINIT_APARTMENTTHREADED);
	Global->Initialize();

	Global->Run();

	Global->UnInitialize();
	::CoUninitialize();
	_Module.Term();
	return 0;
}
