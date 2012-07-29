// VNOCC.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "VNOCC.h"
#include "src/BKWin/wtlhelper/whwindow.h"

#ifdef USE_VLD
#ifdef _DEBUG
#pragma comment(lib,"VldMemLeakCheck\\vld.lib")
#include "../../VisualLeakDetector/include/vld.h"
#endif
#endif

CAppModule _Module;

class CMyWindow:public CBkDialogImpl<CMyWindow>
{
public:
	void		OnBkBtnClose(){EndDialog(0);};
	void		OnBkBtnMax(){};
	void		OnBkBtnMin(){};
	void		OnBkBtnMore(){};
	
public:
	BK_NOTIFY_MAP(IDC_RICHVIEW_WIN)
		BK_NOTIFY_ID_COMMAND(IDC_BTN_SYS_CLOSE, OnBkBtnClose)
		BK_NOTIFY_ID_COMMAND(IDC_BTN_SYS_MAX,	OnBkBtnMax)
		BK_NOTIFY_ID_COMMAND(IDC_BTN_SYS_MIN,	OnBkBtnMin)
		BK_NOTIFY_ID_COMMAND(IDC_BTN_MORE_BKWIN,    OnBkBtnMore)
	BK_NOTIFY_MAP_END()

	BEGIN_MSG_MAP_EX_DECLARE(CMyWindow)
};

BEGIN_MSG_MAP_EX_IMP(CMyWindow)
	MSG_BK_NOTIFY(IDC_RICHVIEW_WIN)
	CHAIN_MSG_MAP(CBkDialogImpl<CMyWindow>)
// 	CHAIN_MSG_MAP(CWHRoundRectFrameHelper<CMyWindow>)
// 	CHAIN_MSG_MAP_MEMBER((*m_sysoptHandler))
// 	MSG_WM_INITDIALOG(OnInitDialog)
// 	MSG_WM_SYSCOMMAND(OnSysCommand)
	REFLECT_NOTIFICATIONS_EX()
END_MSG_MAP_IMP()

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	_Module.Init(NULL,hInstance);
	::CoInitializeEx(NULL,COINIT_APARTMENTTHREADED);
	Global->Initialize();
	IFrameWork* pFrameWork=NULL;
	Global->GetIFrameModule(&pFrameWork);
	if (pFrameWork)
	{
		pFrameWork->SetStartupModule(module_LoginWin);
	}
	Global->Run();

	BkString::Load(IDR_BK_STRING_DEF);
	BkFontPool::SetDefaultFont(BkString::Get(0), -12);
	BkSkin::LoadSkins(IDR_XML_SKIN_DEF);
	BkStyle::LoadStyles(IDR_BK_STYLE_DEF);

	CMyWindow wnd;
	wnd.Load(IDR_BK_MAIN_DIALOG);
	wnd.DoModal();

	Global->UnInitialize();
	::CoUninitialize();
	return 0;
}
