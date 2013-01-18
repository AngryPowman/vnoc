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
		return S_FALSE;
	}

	ColumnData* pColData = new ColumnData;
	_ColumnInit(pColData);
	delete pColData;

	ListItemData* pItemData = new ListItemData;
	_SetListData(pItemData);
	delete pItemData;

	return S_OK;
}


LRESULT CRoomListWnd::_ColumnInit(ColumnData* pColData)
{ 
	m_wndListCtrl.InsertColumn(0, pColData->strColumn1, LVCFMT_LEFT, 80);
	m_wndListCtrl.InsertColumn(1, pColData->strColumn2, LVCFMT_LEFT, 80);
	m_wndListCtrl.InsertColumn(2, pColData->strColumn3, LVCFMT_LEFT, 80);
	m_wndListCtrl.InsertColumn(3, pColData->strColumn4, LVCFMT_LEFT, 80);

	return S_OK;
}


LRESULT CRoomListWnd::_SetListData(ListItemData* pItemData)
{
	int nItem = m_wndListCtrl.Append(pItemData->strItemData1, NULL, 0, SUBITEM_TEXT);
	m_wndListCtrl.AppendSubItem(nItem, pItemData->strItemData2);
	m_wndListCtrl.AppendSubItem(nItem, pItemData->strItemData3);
	m_wndListCtrl.AppendSubItem(nItem, pItemData->strItemData4);

	int nTestItem = m_wndListCtrl.Append(_T("VVV"), NULL, 0, LISTITEM_CHECKBOX); // nTestItem just be one sample, if CHECKBOX will have not used, it will delete.
	m_wndListCtrl.AppendSubItem(nTestItem, _T("NNN"));
	m_wndListCtrl.AppendSubItem(nTestItem, _T("OOO"));
	m_wndListCtrl.AppendSubItem(nTestItem, _T("CCC"));

	return nItem;
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

