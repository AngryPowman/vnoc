#include "stdafx.h"
#include "BKWinRoomList.h"

#define TimerID_LoginTimeout	0

BEGIN_MSG_MAP_EX_IMP(CRoomListWnd)
	MSG_BK_NOTIFY(IDC_RICHVIEW_WIN)
	MSG_WM_INITDIALOG(OnInitDialog)
	CHAIN_MSG_MAP(CBkDialogImpl<CRoomListWnd>)
	NOTIFY_CODE_HANDLER(NM_CLICK, OnListItemClick)
	REFLECT_NOTIFICATIONS_EX()
END_MSG_MAP_IMP();



void CRoomListWnd::OnBkBtnClose()
{
	EndDialog(0);
}

VOID CRoomListWnd::OnShowWnd( XMessage* pmsg )
{
	DoModal();	
}

LRESULT CRoomListWnd::OnInitDialog(HWND hWnd, LPARAM lparam)
{	
	ColumnData ColData;
	if(ColumnInit(&ColData) == ERROR){
		MessageBox(_T("List create failed"));
		exit(0);
	}

	ListItemData ItemData;
	ItemData.strIRoomOrder = "1";
	ItemData.strITeacher = "C++教师";
	ItemData.strIPeople = "54 / 100"; // 54为在线人数, 100为教室人数上限
	ItemData.strITime = "15 / 60"; // 已授课15分钟, 一节课60分钟
	AppendItem(&ItemData);
	
	
	return 0;
}

BOOL CRoomListWnd::ColumnInit(ColumnData* pColData)
{
	if(m_wndListCtrl.Create( 
		GetViewHWND(), NULL, NULL, 
		WS_VISIBLE | WS_CHILD | LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL ,  
		0, 1001/*realwnd id = 1001*/, NULL)  ==  NULL){
		return ERROR;
	}
	{ 
		m_wndListCtrl.InsertColumn(0, pColData->strCRoomOrder, LVCFMT_LEFT, 80);
		m_wndListCtrl.InsertColumn(1, pColData->strCTeacher, LVCFMT_LEFT, 80);
		m_wndListCtrl.InsertColumn(2, pColData->strCPeople, LVCFMT_LEFT, 80);
		m_wndListCtrl.InsertColumn(3, pColData->strCTime, LVCFMT_LEFT, 80);
	}
	
	return TRUE;
}

int CRoomListWnd::AppendItem(ListItemData* pItemData)
{
	int nItem1 = m_wndListCtrl.Append(pItemData->strIRoomOrder, NULL, 0, LISTITEM_CHECKBOX);
	m_wndListCtrl.AppendSubItem(nItem1, pItemData->strITeacher);
	m_wndListCtrl.AppendSubItem(nItem1, pItemData->strIPeople);
	m_wndListCtrl.AppendSubItem(nItem1, pItemData->strITime);

	int nItem2 = m_wndListCtrl.Append(_T("2"), NULL, 0, SUBITEM_TEXT); // nItem2 just be one sample, it will delete next time.
	m_wndListCtrl.AppendSubItem(nItem2, _T("VVV"));
	m_wndListCtrl.AppendSubItem(nItem2, _T("NNN"));
	m_wndListCtrl.AppendSubItem(nItem2, _T("OOO"));
	return nItem1;
}

LRESULT CRoomListWnd::OnListItemClick(int idRealWnd, LPNMHDR pnmh, BOOL& bHandled)
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
			XMessage_ShowClassroom msg;
			SendXMessage(&msg);
		}
	
	}

	bHandled = FALSE;
	return 0;
}