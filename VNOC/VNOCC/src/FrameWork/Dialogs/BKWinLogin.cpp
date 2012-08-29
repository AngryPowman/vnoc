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
	CString userName;
	CBkDialogImpl::GetDlgItemText(DlgControl_LoginWin_Edit_UserName,userName);
	MessageBox(userName);
}

HRESULT CLoginWnd::OnMessage( const CMessage& msg )
{
	return S_OK;
}
