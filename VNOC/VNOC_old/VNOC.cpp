// VNOC.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "VNOC.h"

#ifdef USE_VLD
#ifdef _DEBUG
#include "vld.h"
#endif
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVNOCApp

BEGIN_MESSAGE_MAP(CVNOCApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CVNOCApp 构造

CVNOCApp::CVNOCApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CVNOCApp 对象

CVNOCApp theApp;


// CVNOCApp 初始化

BOOL CVNOCApp::InitInstance()
{
	setlocale(LC_ALL,"chs");
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	Global->SetThreadName(_T("App主线程"));
	Global->Initialize();
	m_frame.Initialize();
	//TODO: 这里没有设置m_pMainWnd

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return TRUE;
}


int CVNOCApp::Run()
{
	// TODO: Add your specialized code here and/or call the base class

	m_frame.Run();
	//ExitInstance();
	return CWinApp::Run();
}


int CVNOCApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
	m_frame.UnInitialize();
	Global->UnInitialize();
	return CWinApp::ExitInstance();
}
