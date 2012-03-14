// RoomDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VNOC.h"
#include "RoomDlg.h"

// CRoomDlg 对话框
const TCHAR _className[] = _T("Scintilla");
const TCHAR _appName[] = _T("");

IMPLEMENT_DYNAMIC(CRoomDlg, CDialog)

CRoomDlg::CRoomDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRoomDlg::IDD, pParent)
{
}

CRoomDlg::~CRoomDlg()
{
}

void CRoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRoomDlg, CDialog)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CRoomDlg 消息处理程序

int CRoomDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  在此添加您专用的创建代码

	AfxInitRichEdit();
	CString strCodeWndTitle;
	strCodeWndTitle.LoadString(AfxGetInstanceHandle(),IDS_Sci_WndTitle);
	CRect rec;
	BOOL result = m_codeEdit.Create(strCodeWndTitle,rec,this,0);
	ATLASSERT(result);
	if (result)
	{
		m_codeEdit.SetLexer(SCLEX_CPP);
	}
	return 0;
}

BOOL CRoomDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect  rectOnline,rectCodeOut,rectReport,rectThis,rectCodeCtrl;
	GetDlgItem(IDC_GROUP_ONLINE)->GetWindowRect(rectOnline);
	GetDlgItem(IDC_GROUP_CODE)->GetWindowRect(rectCodeOut);
	GetDlgItem(IDC_GROUP_SESSION)->GetWindowRect(rectReport);
	ScreenToClient(rectOnline);
	ScreenToClient(rectCodeOut);
	ScreenToClient(rectReport);
	rectCodeCtrl.left = rectOnline.right; 
    rectCodeCtrl.top = rectCodeOut.top; 
	rectCodeCtrl.bottom = rectReport.top; 
    rectCodeCtrl.right = rectCodeOut.left;

	rectCodeCtrl.left += 5;
	rectCodeCtrl.right -=5;
	rectCodeCtrl.top +=5;
	rectCodeCtrl.bottom -=5;
	m_codeEdit.MoveWindow(rectCodeCtrl);
	BOOL result = m_codeEdit.ShowWindow(SW_SHOW);
	if (!result)
	{
		DWORD errNo = GetLastError();
		errNo = errNo;
	}
	// TODO:  在此添加额外的初始化
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
