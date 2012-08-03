#pragma once
#include "../../BKWin/wtlhelper/whwindow.h"

class CLoginWnd:public CBkDialogImpl<CLoginWnd>
{
public:
	void		OnBkBtnClose();;
	void		OnBkBtnMax();;
	void		OnBkBtnMin();;
	void		OnBkBtnMore();;

public:
	BK_NOTIFY_MAP(IDC_RICHVIEW_WIN)
		BK_NOTIFY_ID_COMMAND(IDC_BTN_SYS_CLOSE, OnBkBtnClose)
		BK_NOTIFY_ID_COMMAND(IDC_BTN_SYS_MAX,	OnBkBtnMax)
		BK_NOTIFY_ID_COMMAND(IDC_BTN_SYS_MIN,	OnBkBtnMin)
		BK_NOTIFY_ID_COMMAND(IDC_BTN_MORE_BKWIN,    OnBkBtnMore)
	BK_NOTIFY_MAP_END()

	BEGIN_MSG_MAP_EX_DECLARE(CLoginWnd)
};