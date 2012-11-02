#pragma once
#include "../../BKWin/wtlhelper/whwindow.h"
#include "../../net/INet.h"
#include "../FrameBase.hpp"

class CRoomListWnd:public CBkDialogImpl<CRoomListWnd>
	,public CFrameBase
{
public:
	CRoomListWnd();
public:
	void	OnBkBtnClose();
	VOID	OnShowWnd(XMessage* pmsg);
	LRESULT OnInitDialog(HWND hWnd, LPARAM lparam);
protected:
	CListViewCtrl m_ctlList;
	CListViewCtrlEx m_wndListCtrlVul;
private:

public:
	BK_NOTIFY_MAP(IDC_RICHVIEW_WIN)
		BK_NOTIFY_ID_COMMAND(DlgControl_RoomListWin_ImgBtn_Close,	OnBkBtnClose)
	BK_NOTIFY_MAP_END()

	BEGIN_MSG_MAP_EX_DECLARE(CRoomListWnd)

	Begin_XMessage(CRoomListWnd)
	OnXMessage(XMessage_ShowRoomList,OnShowWnd)
	End_XMessage()


};