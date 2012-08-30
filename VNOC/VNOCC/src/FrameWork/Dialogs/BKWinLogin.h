#pragma once
#include "../../BKWin/wtlhelper/whwindow.h"
#include "../../net/INet.h"
#include "../ViewBase.hpp"

class CLoginWnd:public CBkDialogImpl<CLoginWnd>
	,public CViewBase
	,public INetListener
{
public:
	void		OnBkBtnClose();
	void		OnLoginClick();

	STDMETHOD( OnMessage(const CMessage& msg));

public:
	BK_NOTIFY_MAP(IDC_RICHVIEW_WIN)
		BK_NOTIFY_ID_COMMAND(DlgControl_LoginWin_ImgBtn_Close,	OnBkBtnClose)
		BK_NOTIFY_ID_COMMAND(DlgControl_LoginWin_Button_Login,	OnLoginClick)
	BK_NOTIFY_MAP_END()

	BEGIN_MSG_MAP_EX_DECLARE(CLoginWnd)
};