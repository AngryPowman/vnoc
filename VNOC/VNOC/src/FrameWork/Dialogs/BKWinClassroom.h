#pragma once
#include "../../BKWin/wtlhelper/whwindow.h"
#include "../../net/INet.h"
#include "../FrameBase.hpp"

class CClassroomWnd : public CBkDialogImpl<CClassroomWnd>,
					  public CFrameBase
{
public:	
	CClassroomWnd::CClassroomWnd() : CBkDialogImpl<CClassroomWnd>(BKDlg_ClassroomWin),
								   CFrameBase(module_ClassroomWin)
	{
	}

public:
	void	OnBkBtnClose();
	VOID	OnShowWnd(XMessage* pmsg);
	LRESULT OnInitDialog(HWND hWnd, LPARAM lparam);

protected:

protected:

public:
	BK_NOTIFY_MAP(IDC_RICHVIEW_WIN)
		BK_NOTIFY_ID_COMMAND(DlgControl_ClassroomWin_ImgBtn_Close,	OnBkBtnClose)
	BK_NOTIFY_MAP_END()


	BEGIN_MSG_MAP_EX_DECLARE(CClassroomWnd)
		Begin_XMessage(CClassroomWnd)
		OnXMessage(XMessage_ShowClassroom,OnShowWnd)
	End_XMessage()
};