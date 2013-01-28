#pragma once
#include "../../BKWin/wtlhelper/whwindow.h"
#include "../../net/INet.h"
#include "../FrameBase.hpp"

class CRoomListWnd : public CBkDialogImpl<CRoomListWnd>
	,public CFrameBase
{
public:
	CRoomListWnd::CRoomListWnd() : CBkDialogImpl<CRoomListWnd>(BKDlg_RoomListWin),CFrameBase(module_RoomListWin)
	{
	}


public:
	void	OnBkBtnClose();
	void	OnShowWnd(XMessage* pmsg);
	LRESULT OnInitDialog(HWND hWnd, LPARAM lparam);
	LRESULT OnListItemDblClick(int idRealWnd, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnGetRoomListResult(XMessage *pmsg);

private:
	CListViewCtrlEx m_wndListCtrl;

private:
	LRESULT	_ColumnInit();

public:
	BK_NOTIFY_MAP(IDC_RICHVIEW_WIN)
		BK_NOTIFY_ID_COMMAND(DlgControl_RoomListWin_ImgBtn_Close,	OnBkBtnClose)
	BK_NOTIFY_MAP_END()
	
	BEGIN_MSG_MAP_EX_DECLARE(CRoomListWnd)
		Begin_XMessage(CRoomListWnd)
		OnXMessage(XMessage_ShowRoomListWnd,OnShowWnd)
		OnXMessage(XMessage_GetRoomList_Result, OnGetRoomListResult)
	End_XMessage()
};