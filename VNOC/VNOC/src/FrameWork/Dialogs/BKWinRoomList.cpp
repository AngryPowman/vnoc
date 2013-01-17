#include "stdafx.h"
#include "BKWinRoomList.h"

#define TimerID_LoginTimeout	0

BEGIN_MSG_MAP_EX_IMP(CRoomListWnd)
	MSG_BK_NOTIFY(IDC_RICHVIEW_WIN)
	MSG_WM_INITDIALOG(OnInitDialog)
	CHAIN_MSG_MAP(CBkDialogImpl<CRoomListWnd>)
	NOTIFY_CODE_HANDLER(NM_DBLCLK, OnListItemDblClick)
	REFLECT_NOTIFICATIONS_EX()
END_MSG_MAP_IMP();



void CRoomListWnd::OnBkBtnClose()
{
	EndDialog(0);
}

void CRoomListWnd::OnShowWnd( XMessage* pmsg )
{
	DoModal();	
}


LRESULT CRoomListWnd::OnInitDialog(HWND hWnd, LPARAM lparam)
{
	if(m_wndListCtrl.Create( 
		GetViewHWND(), NULL, NULL, 
		WS_VISIBLE | WS_CHILD | LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL ,  
		0, 1001/*realwnd id = 1001*/, NULL)  ==  NULL){
		return ERROR;
	}
	ColumnInit();
	return TRUE;
}

void CRoomListWnd::ColumnInit()
{ 
    m_wndListCtrl.InsertColumn(0, L"ID", LVCFMT_LEFT, 40);
    m_wndListCtrl.InsertColumn(1, L"教室名字", LVCFMT_LEFT, 270);
}

LRESULT CRoomListWnd::OnListItemDblClick(int idRealWnd, LPNMHDR pnmh, BOOL& bHandled)
{
	LPNMITEMACTIVATE lpnmItem = (LPNMITEMACTIVATE)pnmh;

	if(1001 == idRealWnd){ // 点击教室列表
		char strCheckBox[30];
		if(m_wndListCtrl.InSubItemCheckBox(lpnmItem->ptAction, lpnmItem->iItem)){
			sprintf(strCheckBox, "CHECKBOX CHANGED: %d", !m_wndListCtrl.GetCheckState(lpnmItem->iItem));
			MessageBoxA(NULL, strCheckBox, "", MB_OK);
		}

		//char strListItem[30];
		if(m_wndListCtrl.GetSelectedIndex() == lpnmItem->iItem){
			//sprintf(strListItem, "Clicked id: %d", lpnmItem->iItem);
			//MessageBoxA(NULL, strListItem, "", MB_OK);
			EndDialog(0);
			XMessage_ShowClassroom msg;
			SendXMessage(&msg);
		}
	
	}

	bHandled = FALSE;
	return 0;
}