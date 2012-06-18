// Room.cpp : 实现文件
//

#include "stdafx.h"
#include "VNOC.h"
#include "Room.h"

const TCHAR className[] = _T("CodeCtrl");
const TCHAR appName[] = _T("Code");

// CRoom 对话框

IMPLEMENT_DYNAMIC(CRoom, CDialog)

CRoom::CRoom(CWnd* pParent /*=NULL*/)
	: CDialog(CRoom::IDD, pParent)
{
	m_hDll = NULL;
	m_CodeCtrl = NULL;
}

CRoom::~CRoom()
{
	if (m_hDll){
		FreeLibrary(m_hDll);	
		m_hDll = NULL;
	}
}

void CRoom::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRoom, CDialog)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CRoom 消息处理程序

int CRoom::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (!m_hDll)
		m_hDll = LoadLibrary(_T("SciLexer.DLL"));
//	if (m_hDll)
//	{
//		m_CodeCtrl= CreateWindowEx(0,
//			className,appName, WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN,
//			10,10,500,400,this->GetSafeHwnd(),/*(HMENU)GuiID*/0,AfxGetApp()->m_hInstance,NULL);
//	}
//	::ShowWindow(m_CodeCtrl,SW_SHOW);	
	// TODO:  在此添加您专用的创建代码

	return 0;
}

BOOL CRoom::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
