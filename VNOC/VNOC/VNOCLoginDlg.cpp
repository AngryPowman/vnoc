// VNOCDlg.cpp : ʵ���ļ�
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


// CVNOCDlg �Ի���

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


// CVNOCDlg ��Ϣ�������

BOOL CVNOCLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_LinkRegist.Attach(GetDlgItem(IDC_REGIST)->GetSafeHwnd());
	m_FoundPassword.Attach(GetDlgItem(IDC_FOUND_PASSWORD)->GetSafeHwnd());

	m_LinkRegist.SetLink(_T("ע���˺�"),_T("http://google.com.hk"),FALSE);
	m_FoundPassword.SetLink(_T("�һ�����"),_T("http://google.com.hk"),FALSE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CVNOCLoginDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CVNOCLoginDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
#include <atlconv.h>

void CVNOCLoginDlg::OnBnClickedOk()
{
    UpdateData(TRUE);
	Global->Logf(LogFile_Net,_T("��½�������û���:%s ����:%s\n"), m_strUsername, m_strPassword);
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
	Global->Logf(LogFile_General,_T("SHA1�������Ϊ:%s\n"),CA2T(pwdBuffer));

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

	// TODO:  �ڴ������ר�õĴ�������

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
	Global->Log(LogFile_Net,_T("���յ������"));
	if (msg.GetMessageType() == MSG_ALI_TYPE)
	{
		const MSG_ALI* ma = dynamic_cast<const MSG_ALI*>(&msg);
		if (ma->GetLoginResult() == 0)
		{
			OnOK();
		}
		else
		{
			MessageBox(_T("��½ʧ�ܣ��������ܾ���½ ."));
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
		MessageBox(_T("��¼ʧ�ܣ�������������."));
	}
}
