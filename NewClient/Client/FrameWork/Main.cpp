// VNOC.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

#ifdef USE_VLD
#ifdef _DEBUG
#pragma comment(lib,"vld.lib")
#include "../../VisualLeakDetector/include/vld.h"
#endif
#endif

void VnocMain()
{
    IGlobal::GlobalDeliver();

    ::CoInitializeEx(NULL,COINIT_APARTMENTTHREADED);
    Global->Initialize();

    Global->Run();

    Global->UnInitialize();
    ::CoUninitialize();
}

#ifndef CPPTEST
#include "../GUI/BKWin/wtl/atlapp.h"

CAppModule _Module;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	_Module.Init(NULL,hInstance);

    VnocMain();

	_Module.Term();
	return 0;
}

#else

int _tmain(int argc, _TCHAR* argv[])
{
    VnocMain();
	return 0;
}

#endif
