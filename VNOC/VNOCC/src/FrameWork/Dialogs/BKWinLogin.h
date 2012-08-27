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
		BK_NOTIFY_ID_COMMAND(DlgControl_LoginWin_ImgBtn_Close, OnBkBtnClose)
		BK_NOTIFY_ID_COMMAND(DlgControl_LoginWin_ImgBtn_Maximize,	OnBkBtnMax)
		BK_NOTIFY_ID_COMMAND(DlgControl_LoginWin_ImgBtn_Minimize,	OnBkBtnMin)
	BK_NOTIFY_MAP_END()

	BEGIN_MSG_MAP_EX_DECLARE(CLoginWnd)
};