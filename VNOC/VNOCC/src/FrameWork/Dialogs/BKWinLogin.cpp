#include "stdafx.h"
#include "BKWinLogin.h"


BEGIN_MSG_MAP_EX_IMP(CLoginWnd)
	MSG_BK_NOTIFY(IDC_RICHVIEW_WIN)
	CHAIN_MSG_MAP(CBkDialogImpl<CLoginWnd>)
	// 	CHAIN_MSG_MAP(CWHRoundRectFrameHelper<CMyWindow>)
	// 	CHAIN_MSG_MAP_MEMBER((*m_sysoptHandler))
	// 	MSG_WM_INITDIALOG(OnInitDialog)
	// 	MSG_WM_SYSCOMMAND(OnSysCommand)
	REFLECT_NOTIFICATIONS_EX()
END_MSG_MAP_IMP()

void CLoginWnd::OnBkBtnClose()
{
	EndDialog(0);
}

void CLoginWnd::OnBkBtnMax()
{
	int a=0;
}

void CLoginWnd::OnBkBtnMin()
{

}

void CLoginWnd::OnBkBtnMore()
{

}
