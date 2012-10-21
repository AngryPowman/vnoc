#include "stdafx.h"
#include "BKWinRoomList.h"

#define TimerID_LoginTimeout	0

BEGIN_MSG_MAP_EX_IMP(CRoomListWnd)
	MSG_BK_NOTIFY(IDC_RICHVIEW_WIN)
	MSG_WM_INITDIALOG(OnInitDialog)
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

VOID CRoomListWnd::OnShowWnd( XMessage* pmsg )
{
	DoModal();	
}

LRESULT CRoomListWnd::OnInitDialog(HWND hWnd, LPARAM lparam)
{
	CRect rec;
	rec.left = 10;
	rec.top = 40;
	rec.right = 200;
	rec.bottom = 750;
	m_ctlList.Create(m_hWnd, rec, NULL, WS_CLIPCHILDREN | WS_CHILD |
					 LVS_REPORT | LVS_SINGLESEL);
	m_ctlList.ShowWindow(SW_SHOW);

	m_ctlList.InsertColumn(0,_T("�����б�"),LVCFMT_LEFT, 100);
	//m_ctlList.InsertColumn(1,_T("test2"),LVCFMT_LEFT, 60); �Ժ�Ҫ���Ͼ�д��
	//m_ctlList.InsertColumn(2,_T("test3"),LVCFMT_LEFT, 70);
	int line = 0;
	m_ctlList.InsertItem(line++, _T("���� 1"));
	m_ctlList.InsertItem(line++, _T("���� 2"));
	m_ctlList.InsertItem(line++, _T("���� 3"));
	m_ctlList.InsertItem(line++, _T("���� 4"));
	m_ctlList.InsertItem(line++, _T("���� 5"));
	m_ctlList.InsertItem(line++, _T("���� 6"));
	m_ctlList.InsertItem(line++, _T("���� 7"));
	m_ctlList.InsertItem(line++, _T("���� 8"));
	m_ctlList.InsertItem(line++, _T("���� 9"));
	m_ctlList.InsertItem(line++, _T("���� 10"));
	m_ctlList.InsertItem(line++, _T("���� 11"));
	m_ctlList.InsertItem(line++, _T("���� 12"));

	return 0;
}