#include "stdafx.h"
#include "BKWinLogin.h"

#define TimerID_LoginTimeout	0

BEGIN_MSG_MAP_EX_IMP(CLoginWnd)
	MSG_BK_NOTIFY(IDC_RICHVIEW_WIN)
	CHAIN_MSG_MAP(CBkDialogImpl<CLoginWnd>)
	MSG_WM_TIMER(OnTimer)
	REFLECT_NOTIFICATIONS_EX()
END_MSG_MAP_IMP()

void CLoginWnd::OnBkBtnClose()
{
	EndDialog(0);
}

void CLoginWnd::OnLoginClick()
{
	Disable();
	SetTimer(TimerID_LoginTimeout,5000);
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
		XMessage_Login msg;
		msg.username = userName;
		msg.pwd = pwd;
		SendXMessage(&msg);
	}
}

BOOL CLoginWnd::OnLoginResult( XMessage* pMsg )
{
	XMessage_Login_Result *pResult = dynamic_cast<XMessage_Login_Result*>(pMsg);
	if (pResult)
	{
		if (pResult->success == TRUE)
		{
			MessageBox(_T("³É¹¦µÇÂ½!"));
		}
		else
		{
			MessageBox(_T("µÇÂ½Ê§°Ü!"));
		}
	}
	return TRUE;
}

VOID CLoginWnd::Disable()
{
	CBkWindow* pUsernameEdit = NULL;
	pUsernameEdit = m_richView.FindChildByCmdID(DlgControl_LoginWin_Edit_UserName);
	if (pUsernameEdit)
	{
		pUsernameEdit->ModifyState(BkWndState_Disable,0);
	}
	CBkWindow* pPwdEdit = NULL;
	pPwdEdit = m_richView.FindChildByCmdID(DlgControl_LoginWin_Edit_PassWord);
	if (pPwdEdit)
	{
		pPwdEdit->ModifyState(BkWndState_Disable,0);
	}
	CBkWindow* pLoginBtn = NULL;
	pLoginBtn = m_richView.FindChildByCmdID(DlgControl_LoginWin_Button_Login);
	if (pLoginBtn)
	{
		pLoginBtn->ModifyState(BkWndState_Disable,0);
	}
}

VOID CLoginWnd::Enable()
{
	CBkWindow* pUsernameEdit = NULL;
	pUsernameEdit = m_richView.FindChildByCmdID(DlgControl_LoginWin_Edit_UserName);
	if (pUsernameEdit)
	{
		pUsernameEdit->ModifyState(0,BkWndState_Disable);
	}
	CBkWindow* pPwdEdit = NULL;
	pPwdEdit = m_richView.FindChildByCmdID(DlgControl_LoginWin_Edit_PassWord);
	if (pPwdEdit)
	{
		pPwdEdit->ModifyState(0,BkWndState_Disable);
	}
	CBkWindow* pLoginBtn = NULL;
	pLoginBtn = m_richView.FindChildByCmdID(DlgControl_LoginWin_Button_Login);
	if (pLoginBtn)
	{
		pLoginBtn->ModifyState(0,BkWndState_Disable);
		pLoginBtn->NotifyInvalidate();
	}
}

VOID CLoginWnd::OnTimer( UINT_PTR id )
{
	KillTimer(id);
	MessageBox(_T("µÇÂ½³¬Ê±"));
	Enable();
}
