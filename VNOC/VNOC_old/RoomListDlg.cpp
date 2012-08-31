// MainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VNOC.h"
#include "RoomListDlg.h"
#include "VNOCLoginDlg.h"


// CRoomListDlg 对话框

IMPLEMENT_DYNAMIC(CRoomListDlg, CDialog)

CRoomListDlg::CRoomListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRoomListDlg::IDD, pParent)
{
	m_bIsMove = FALSE;
}

CRoomListDlg::~CRoomListDlg()
{
}

void CRoomListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SEACH, m_SearchEdit);
	DDX_Control(pDX, IDC_LIST_CLASS, m_ClassList);
}


BEGIN_MESSAGE_MAP(CRoomListDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CLASS, &CRoomListDlg::OnNMDblclkListClass)
END_MESSAGE_MAP()


// CRoomListDlg 消息处理程序

void CRoomListDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (m_bIsMove)
	{
		CRect EditRect,SplitRect,DlgRect,ListRect,NoticeRect;
		this->GetClientRect(DlgRect);
		GetDlgItem(IDC_EDIT_SEACH)->GetWindowRect(EditRect);
		ScreenToClient(EditRect);

		EditRect.left = DlgRect.left;
		GetDlgItem(IDC_EDIT_SEACH)->MoveWindow(EditRect);

		ListRect.bottom = DlgRect.bottom;
		ListRect.top =EditRect.bottom;
		ListRect.left = DlgRect.left;
		ListRect.right = DlgRect.Width()/3*2;
		GetDlgItem(IDC_LIST_CLASS)->MoveWindow(ListRect);
		m_ClassList.SetColumnWidth(0,ListRect.Width()/4);
		m_ClassList.SetColumnWidth(1,ListRect.Width()-ListRect.Width()/4);

		NoticeRect.bottom=DlgRect.bottom;
		NoticeRect.top =EditRect.bottom;
		NoticeRect.left = ListRect.right;
		NoticeRect.right  = DlgRect.right;
		GetDlgItem(IDC_STATIC_NOTICE)->MoveWindow(NoticeRect)	;

		SplitRect.top =NoticeRect.bottom/2; 
		SplitRect.bottom = SplitRect.top+1;
		SplitRect.left = NoticeRect.left;
		SplitRect.right = NoticeRect.right;
		GetDlgItem(IDC_STATIC_SPLIT)->MoveWindow(SplitRect);
	}
	// TODO: 在此处添加消息处理程序代码
}

BOOL CRoomListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CRect rect;
	m_SearchEdit.GetWindowRect(rect);
	CFont*  ptf=m_SearchEdit.GetFont();  
	LOGFONT  lf;  
	ptf->GetLogFont  (&lf);  
	lf.lfHeight=rect.Height()-5;//改变字体高度  
	m_EditFont.CreateFontIndirect  (&lf);  
	m_SearchEdit.SetFont  (&m_EditFont);
	m_bIsMove = TRUE;

	m_ClassList.SetExtendedStyle(m_ClassList.GetExtendedStyle()
		| LVS_EX_GRIDLINES
		| LVS_EX_FULLROWSELECT );
//	m_ClassList.ShowScrollBar(SB_VERT ,TRUE);
	m_ClassList.InsertColumn(0,_T("  "));
	m_ClassList.InsertColumn(1,_T(" "));
	GetDlgItem(IDC_STATIC_SPLIT)->EnableWindow(TRUE);
	int nItem;
	for (int i = 0; i < 10; i++)
	{
		CString strRoomName;
		strRoomName.Format(_T("教室-%d"), i);
		nItem = m_ClassList.InsertItem(0, strRoomName);	
		m_ClassList.SetItemText(nItem, 1, _T("C++"));
	}
	::SendMessage(this->m_hWnd, WM_SIZE, 0, 0);

	return TRUE;
}

void CRoomListDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialog::OnSizing(fwSide, pRect);
	
	// TODO: 在此处添加消息处理程序代码
}



void CRoomListDlg::OnNMDblclkListClass(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	OnOK();
	*pResult = 0;
}
