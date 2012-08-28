#include "stdafx.h"
#include "BKWinLogin.h"


BEGIN_MSG_MAP_EX_IMP(CLoginWnd)
	MSG_BK_NOTIFY(IDC_RICHVIEW_WIN)
	CHAIN_MSG_MAP(CBkDialogImpl<CLoginWnd>)
	// 	MSG_WM_INITDIALOG(OnInitDialog)
	// 	MSG_WM_SYSCOMMAND(OnSysCommand)
	REFLECT_NOTIFICATIONS_EX()
END_MSG_MAP_IMP()

void CLoginWnd::OnBkBtnClose()
{
	EndDialog(0);
}

void CLoginWnd::OnLoginClick()
{
	MessageBox(_T(""));
}
