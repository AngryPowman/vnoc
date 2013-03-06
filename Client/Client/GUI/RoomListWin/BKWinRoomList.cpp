#include "stdafx.h"
#include "BKWinRoomList.h"

#define TimerID_LoginTimeout	0

BEGIN_MSG_MAP_EX_IMP(CRoomListWnd)
	MSG_BK_NOTIFY(IDC_RICHVIEW_WIN)
	MSG_WM_INITDIALOG(OnInitDialog)
	CHAIN_MSG_MAP(CBkDialogImpl<CRoomListWnd>)
	NOTIFY_HANDLER(DlgControl_RoomListWin_ListCtl_RoomList, NM_DBLCLK, OnListItemDblClick)
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
		0, DlgControl_RoomListWin_ListCtl_RoomList, NULL)  ==  NULL){
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
	m_wndListCtrl.InsertColumn(1, _T("ΩÃ “Í«≥∆"), LVCFMT_LEFT, 270);
    m_wndListCtrl.InsertColumn(2, _T("◊¥Ã¨"), LVCFMT_LEFT, 80);

	return S_OK;
}

LRESULT CRoomListWnd::OnListItemDblClick(int idRealWnd, LPNMHDR pnmh, BOOL& bHandled)
{
	LPNMITEMACTIVATE lpnmItem = (LPNMITEMACTIVATE)pnmh;
    XMessage_EnterRoom msg;
    msg.roomId = 1000;
    msg.password = "123";
    msg.verificationCode = "111";
    SendXMessage(&msg);
    CString roomId;
    LVCOLUMN column;
//    m_wndListCtrl.GetItemText(lpnmItem->iItem, lpnmItem->iSubItem, roomId);
	//char strListItem[30];
	if(m_wndListCtrl.GetSelectedIndex() == lpnmItem->iItem)
    {
		//sprintf(strListItem, "Clicked id: %d", lpnmItem->iItem);
		//MessageBoxA(NULL, strListItem, "", MB_OK);
		
		
	}

	bHandled = FALSE;
	return S_OK;
}

VOID CRoomListWnd::OnGetRoomListResult( XMessage_GetRoomList_Result *pMsg )
{
    if(pMsg)
    {
        int nItem = 0;
        CString buf;
        for(int i = 0; i < pMsg->roomNameList.size(); i++)
        {
            buf.Format(_T("%d"), pMsg->roomIdList[i]);
            nItem = m_wndListCtrl.Append(buf);
            m_wndListCtrl.AppendSubItem(nItem, CString(pMsg->roomNameList[i].c_str()));
            buf.Format(_T("%d"), pMsg->roomStateList[i]);
            m_wndListCtrl.AppendSubItem(nItem, buf);
        }
    }
}

VOID CRoomListWnd::OnEnterRoomResult(XMessage_EnterRoom_Result *pMsg)
{
    if(pMsg != nullptr)
    {
        if(pMsg->retTag == 0)
        {
            EndDialog(0);
            XMessage_ShowClassroom msg;
		    SendXMessage(&msg);
        }
        else
        {
            MessageBox(_T("Ω¯»Î ß∞‹"));
        }
    }
}