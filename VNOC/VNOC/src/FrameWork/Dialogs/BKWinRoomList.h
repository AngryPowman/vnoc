#pragma once
#include "../../BKWin/wtlhelper/whwindow.h"
#include "../../net/INet.h"
#include "../FrameBase.hpp"

struct ListItemData
{
	CString strIRoomOrder, strITeacher, strIPeople, strITime;
};

class CRoomListWnd : public CBkDialogImpl<CRoomListWnd>
	,public CFrameBase
{
public:
	CRoomListWnd::CRoomListWnd() : CBkDialogImpl<CRoomListWnd>(BKDlg_RoomListWin), CFrameBase(module_RoomListWin)
	{
	}


public:
	void	OnBkBtnClose();
	void	OnShowWnd(XMessage* pmsg);
	LRESULT OnInitDialog(HWND hWnd, LPARAM lparam);
	LRESULT OnListItemClick(int idRealWnd, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT SetListData();

protected:	
	LRESULT		AppendListItem(ListItemData* pItemData);

protected:
	CListViewCtrlEx m_wndListCtrl;

private:
    void ColumnInit();

public:
	BK_NOTIFY_MAP(IDC_RICHVIEW_WIN)
		BK_NOTIFY_ID_COMMAND(DlgControl_RoomListWin_ImgBtn_Close,	OnBkBtnClose)
	BK_NOTIFY_MAP_END()
	
	BEGIN_MSG_MAP_EX_DECLARE(CRoomListWnd)
		Begin_XMessage(CRoomListWnd)
		OnXMessage(XMessage_ShowRoomList,OnShowWnd)
	End_XMessage()
};