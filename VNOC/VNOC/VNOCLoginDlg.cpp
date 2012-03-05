// VNOCDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VNOC.h"
#include "VNOCLoginDlg.h"
#include "RoomDlg.h"
#include "Config.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVNOCDlg 对话框




CVNOCLoginDlg::CVNOCLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVNOCLoginDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVNOCLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CVNOCLoginDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CVNOCLoginDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CVNOCLoginDlg::OnBnClickedCancel)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CVNOCDlg 消息处理程序

BOOL CVNOCLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	
	// TODO: 在此添加额外的初始化代码
	m_LinkRegist.Attach(GetDlgItem(IDC_REGIST)->GetSafeHwnd());
	m_FoundPassword.Attach(GetDlgItem(IDC_FOUND_PASSWORD)->GetSafeHwnd());

	m_LinkRegist.SetLink(_T("注册账号"),_T("google.com.hk"),FALSE);
	m_FoundPassword.SetLink(_T("找回密码"),_T("google.com.hk"),FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CVNOCLoginDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CVNOCLoginDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CVNOCLoginDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialog *pDlg = new CMainDlg();
//	pDlg->DoModal();
//	delete pDlg;

// 	CConfig config;
// 	config.LoadConfigFromXML(L"config.xml");
// 	CStringA text;
// 	ConfigPath path;
// 	path.push_back("CodeEditor");
// 	path.push_back("Language");
// 	config.GetConfigText()

// 	CDialog *pDlg = new CRoomDlg(this);
// 	int result = pDlg->DoModal();
// 	if (result == -1)
// 	{
// 		DWORD errNo = GetLastError();
// 		errNo = errNo;
// 	}
// 	delete pDlg;
	OnOK();
}

void CVNOCLoginDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

int CVNOCLoginDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	return 0;
}

BOOL CVNOCLoginDlg::LanguageChanged()
{
	return TRUE;
}

BOOL CVNOCLoginDlg::Refresh()
{
	return TRUE;
}
