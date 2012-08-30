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

struct sa
{
};

struct sb : public sa
{
};

template <typename T>
struct T
{
};

class A
{
};

void CLoginWnd::OnLoginClick()
{
	CString userName;
	CString pwd;
	CBkWindow* pUsernameEdit = NULL;
	CBkWindow* pPwdEdit = NULL;
	pUsernameEdit = m_richView.FindChildByCmdID(DlgControl_LoginWin_Edit_UserName);
	pPwdEdit = m_richView.FindChildByCmdID(DlgControl_LoginWin_Edit_PassWord);
	if (pUsernameEdit && pPwdEdit)
	{
		userName = pUsernameEdit->GetInnerText();
		pwd = pPwdEdit->GetInnerText();
	}
}

HRESULT CLoginWnd::OnMessage( const CMessage& msg )
{
	return S_OK;
}
