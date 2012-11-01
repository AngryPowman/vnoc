#include "stdafx.h"
#include "BKWinRoomList.h"

#define TimerID_LoginTimeout	0

BEGIN_MSG_MAP_EX_IMP(CRoomListWnd)
	MSG_BK_NOTIFY(IDC_RICHVIEW_WIN)
	MSG_WM_INITDIALOG(OnInitDialog)
	CHAIN_MSG_MAP(CBkListViewImpl<CRoomListWnd>)
	REFLECT_NOTIFICATIONS_EX()
END_MSG_MAP_IMP();

CRoomListWnd::CRoomListWnd() : CBkListViewImpl(),CFrameBase(module_RoomListWin)
{
	SetXml("RoomListWin.xml");
}

void CRoomListWnd::OnBkBtnClose()
{
}

VOID CRoomListWnd::OnShowWnd( XMessage* pmsg )
{
}

LRESULT CRoomListWnd::OnInitDialog( HWND hWnd,LPARAM lparam )
{
	CRect rec;
	rec.left = 0;
	rec.top = 50;
	rec.right = 500;
	rec.bottom = 500;


	return 0;
}
