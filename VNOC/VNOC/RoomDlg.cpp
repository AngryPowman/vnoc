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
	m_hDll= NULL;
	m_CodeCtrl = NULL;
}

CRoomDlg::~CRoomDlg()
{
	if (m_hDll){
		FreeLibrary(m_hDll);	
		m_hDll = NULL;
	}

}

void CRoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRoomDlg, CDialog)
	ON_EN_CHANGE(IDC_RICHEDIT_REPORT, &CRoomDlg::OnEnChangeRichedit21)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CRoomDlg 消息处理程序

void CRoomDlg::OnEnChangeRichedit21()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

int CRoomDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	AfxInitRichEdit();
	if (!m_hDll)
		m_hDll = LoadLibrary(_T("SciLexer.DLL"));
	if (m_hDll != NULL)
	{
		m_CodeCtrl= CreateWindowEx(0,
			_className,_appName, 
			WS_CHILD|WS_CHILDWINDOW | WS_VISIBLE | 
			WS_TABSTOP | WS_CLIPCHILDREN,
			0,0,0,0,this->GetSafeHwnd(),(HMENU)IDC_CODE_CTRL,AfxGetApp()->m_hInstance,NULL);
	}
	// TODO:  在此添加您专用的创建代码
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
	rectCodeCtrl.left = rectOnline.right; rectCodeCtrl.top =rectCodeOut.top; 
	rectCodeCtrl.bottom=rectReport.top; rectCodeCtrl.right = rectCodeOut.left;

	if (m_CodeCtrl)
	{
		GetDlgItem(IDC_CODE_CTRL)->MoveWindow(rectCodeCtrl);
	}
	// TODO:  在此添加额外的初始化
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
