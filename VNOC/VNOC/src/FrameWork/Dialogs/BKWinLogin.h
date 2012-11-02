#pragma once
#include "../../BKWin/wtlhelper/whwindow.h"
#include "../../net/INet.h"
#include "../FrameBase.hpp"

class CLoginWnd:public CBkDialogImpl<CLoginWnd>
	,public CFrameBase
{
public:
	CLoginWnd();
public:
	void	OnBkBtnClose();
	void	OnLoginClick();
	BOOL	OnLoginResult(XMessage* pMsg);

protected:
	VOID	Disable();
	VOID	Enable();
	VOID	OnTimer(UINT_PTR id);
	VOID	OnShowWnd(XMessage* pmsg);
protected:
	INT	m_loginState;

public:
	BK_NOTIFY_MAP(IDC_RICHVIEW_WIN)
		BK_NOTIFY_ID_COMMAND(DlgControl_LoginWin_ImgBtn_Close,	OnBkBtnClose)
		BK_NOTIFY_ID_COMMAND(DlgControl_LoginWin_Button_Login,	OnLoginClick)
	BK_NOTIFY_MAP_END()

	BEGIN_MSG_MAP_EX_DECLARE(CLoginWnd)

	Begin_XMessage(CLoginWnd)
		OnXMessage(XMessage_ShowLogin,OnShowWnd)
		OnXMessage(XMessage_Login_Result,OnLoginResult);
	End_XMessage()
};