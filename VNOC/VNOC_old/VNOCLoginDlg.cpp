// VNOCDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VNOC.h"
#include "VNOCLoginDlg.h"
#include "RoomDlg.h"
#include "Config.h"
#include "INet.h"

#include "../../sha1/sha1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVNOCDlg 对话框

#define  MAX_NAME_LEN     128
#define  MAX_PASSWORD_LEN 128

CVNOCLoginDlg::CVNOCLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVNOCLoginDlg::IDD, pParent)
    , m_strUsername(_T(""))
    , m_strPassword(_T(""))
{
	m_bVerifying = FALSE;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nlHelper.AddFilter(MSG_ALI_TYPE,this);
	m_nlHelper.StartListen();
}

CVNOCLoginDlg::~CVNOCLoginDlg()
{
	m_nlHelper.StopListen();
}

void CVNOCLoginDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_LoginDlg_EDIT_USERNAME, m_strUsername);
    DDX_Text(pDX, IDC_LoginDlg_EDIT_PWD, m_strPassword);
}

BEGIN_MESSAGE_MAP(CVNOCLoginDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CVNOCLoginDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CVNOCLoginDlg::OnBnClickedCancel)
	ON_WM_CREATE()
	ON_WM_TIMER()
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

	m_LinkRegist.SetLink(_T("注册账号"),_T("http://google.com.hk"),FALSE);
	m_FoundPassword.SetLink(_T("找回密码"),_T("http://google.com.hk"),FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

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
#include <atlconv.h>

void CVNOCLoginDlg::OnBnClickedOk()
{
    UpdateData(TRUE);
	Global->Logf(LogFile_Net,_T("登陆操作，用户名:%s 密码:%s\n"), m_strUsername, m_strPassword);
	if (m_strUsername.IsEmpty())
	{
		OnOK();
	}

	SHA1 shaer;
	shaer.Reset();

	CStringA pwdBuffer = CT2A(m_strPassword);
	shaer.Input(pwdBuffer,pwdBuffer.GetLength());
	UINT pResult[5];
	shaer.Result(pResult);
	pwdBuffer.Format("%08x%08x%08x%08x%08x"
		,pResult[0],pResult[1],pResult[2],pResult[3],pResult[4]);
	Global->Logf(LogFile_General,_T("SHA1后的密码为:%s\n"),CA2T(pwdBuffer));

	INetCenter *pInet=NULL;
	Global->GetINetCenter(&pInet);
	ATLASSERT(pInet);
	if (pInet)
	{
		MSG_RLI mRli;
		mRli.SetAccountNumber((byte*)(LPCTSTR)m_strUsername,m_strUsername.GetLength()*sizeof(TCHAR));
		mRli.SetPassword((byte*)(LPCSTR)pwdBuffer,pwdBuffer.GetLength()*sizeof(TCHAR));
		pInet->SendServer(mRli);

		_SetVerifyState(TRUE);
		SetTimer(0,5000,NULL);
	}
}

void CVNOCLoginDlg::OnBnClickedCancel()
{
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

HRESULT CVNOCLoginDlg::OnMessage( const CMessage& msg )
{
	Global->Log(LogFile_Net,_T("接收到网络包"));
	if (msg.GetMessageType() == MSG_ALI_TYPE)
	{
		const MSG_ALI* ma = dynamic_cast<const MSG_ALI*>(&msg);
		if (ma->GetLoginResult() == 0)
		{
			OnOK();
		}
		else
		{
			MessageBox(_T("登陆失败，服务器拒绝登陆 ."));
		}
		_SetVerifyState(FALSE);
	}
	return S_OK;
}

void CVNOCLoginDlg::_SetVerifyState( BOOL bVerifying )
{
	m_bVerifying = bVerifying;
	CEdit* pEditun = (CEdit*)GetDlgItem(IDC_LoginDlg_EDIT_USERNAME);	ATLASSERT(pEditun);
	CEdit* pEditpw = (CEdit*)GetDlgItem(IDC_LoginDlg_EDIT_PWD);			ATLASSERT(pEditpw);
	CButton* pBtnrp = (CButton*)GetDlgItem(IDC_LoginDlg_CHECKBOX_R);		ATLASSERT(pBtnrp);
	CButton* pBtnLogin = (CButton*)GetDlgItem(IDOK);						ATLASSERT(pBtnLogin);

	pEditun->EnableWindow(!bVerifying);
	pEditpw->EnableWindow(!bVerifying);
	pBtnrp->EnableWindow(!bVerifying);
	pBtnLogin->EnableWindow(!bVerifying);
}

void CVNOCLoginDlg::OnTimer( UINT nIDEvent )
{
	if(m_bVerifying)
	{
		_SetVerifyState(FALSE);
		MessageBox(_T("登录失败，请检查网络连接."));
	}
}
