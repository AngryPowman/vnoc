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
		0, DlgControl_RoomListWin_Realwnd, NULL)  ==  NULL){
		return S_FALSE;
	}
	XMessage_GetRoomList msg;
    SendXMessage(&msg);
	_ColumnInit();

	return S_OK;
}


LRESULT CRoomListWnd::_ColumnInit()
{ 
	m_wndListCtrl.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 50);
	m_wndListCtrl.InsertColumn(1, _T("�����ǳ�"), LVCFMT_LEFT, 270);
    m_wndListCtrl.InsertColumn(2, _T("״̬"), LVCFMT_LEFT, 80);

	return S_OK;
}

LRESULT CRoomListWnd::OnListItemDblClick(int idRealWnd, LPNMHDR pnmh, BOOL& bHandled)
{
	LPNMITEMACTIVATE lpnmItem = (LPNMITEMACTIVATE)pnmh;

	if(1001 == idRealWnd){ // ��������б�
		CString strCheckBox;
		if(m_wndListCtrl.InSubItemCheckBox(lpnmItem->ptAction, lpnmItem->iItem)){
			strCheckBox.Format(_T("CHECKBOX CHANGED: %d"), !m_wndListCtrl.GetCheckState(lpnmItem->iItem));
			MessageBox(strCheckBox, NULL, MB_OK);
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
	return S_OK;
}

LRESULT CRoomListWnd::OnGetRoomListResult( XMessage *pmsg )
{
    XMessage_GetRoomList_Result *pResult = dynamic_cast<XMessage_GetRoomList_Result*>(pmsg);
    if(pResult)
    {
        int nItem = 0;
        CString buf;
        for(int i = 0; i < pResult->roomNameList.size(); i++)
        {
            buf.Format(_T("%d"), pResult->roomIdList[i]);
            nItem = m_wndListCtrl.Append(buf);
            m_wndListCtrl.AppendSubItem(nItem, CString(pResult->roomNameList[i].c_str()));
            buf.Format(_T("%d"), pResult->roomStateList[i]);
            m_wndListCtrl.AppendSubItem(nItem, buf);
        }
		return S_OK;
    }
	
	return S_FALSE;
}

