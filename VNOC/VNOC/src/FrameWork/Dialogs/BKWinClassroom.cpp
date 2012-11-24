#include "stdafx.h"
#include "BKWinClassroom.h"

#define TimerID_LoginTimeout	0

BEGIN_MSG_MAP_EX_IMP(CClassroomWnd)
	MSG_BK_NOTIFY(IDC_RICHVIEW_WIN)
	MSG_WM_INITDIALOG(OnInitDialog)
	CHAIN_MSG_MAP(CBkDialogImpl<CClassroomWnd>)
	REFLECT_NOTIFICATIONS_EX()
END_MSG_MAP_IMP();

void CClassroomWnd::OnBkBtnClose()
{
	EndDialog(0);
}

VOID CClassroomWnd::OnShowWnd( XMessage* pmsg )
{
	DoModal();	
}

LRESULT CClassroomWnd::OnInitDialog(HWND hWnd, LPARAM lparam)
{
	MessageBox(_T("classroom"));

	return 0;
}