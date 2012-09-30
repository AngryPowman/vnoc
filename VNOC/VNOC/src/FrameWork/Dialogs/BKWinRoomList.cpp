#include "stdafx.h"
#include "BKWinRoomList.h"

#define TimerID_LoginTimeout	0

BEGIN_MSG_MAP_EX_IMP(CRoomListWnd)
	MSG_BK_NOTIFY(IDC_RICHVIEW_WIN)
	CHAIN_MSG_MAP(CBkDialogImpl<CRoomListWnd>)
	REFLECT_NOTIFICATIONS_EX()
END_MSG_MAP_IMP();

CRoomListWnd::CRoomListWnd() : CBkDialogImpl(BKDlg_RoomListWin),CFrameBase(module_RoomListWin)
{
}

void CRoomListWnd::OnBkBtnClose()
{
	EndDialog(0);
}
