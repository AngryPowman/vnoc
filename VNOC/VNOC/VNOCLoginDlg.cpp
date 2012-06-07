// VNOCDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VNOC.h"
#include "VNOCLoginDlg.h"
#include "RoomDlg.h"
#include "Config.h"
#include "INet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVNOCDlg �Ի���




CVNOCLoginDlg::CVNOCLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVNOCLoginDlg::IDD, pParent)
    , m_strUsername(_T(""))
    , m_strPassword(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nlHelper.AddFilter(MSG_AVC_TYPE,this);
	m_nlHelper.Set();
}

CVNOCLoginDlg::~CVNOCLoginDlg()
{
	m_nlHelper.UnSet();
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


void CVNOCLoginDlg::OnBnClickedOk()
{
    UpdateData(TRUE);
	Global->Logf(LogFile_Net,L"��½�������û���:%s ����:%s\n", m_strUsername, m_strPassword);
	if (m_strUsername.IsEmpty())
	{
		OnOK();
	}

	INetCenter *pInet=NULL;
	Global->GetINetCenter(&pInet);
	ATLASSERT(pInet);
	if (pInet)
	{
		MSG_RVC mRVC;
		byte MAC[16]={0};
		mRVC.SetMachineAddress(MAC,16);
		pInet->SendServer(mRVC);
		_SetVerifyState(TRUE);
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
	if (msg.GetMessageType() == MSG_AVC_TYPE)
	{
		const MSG_AVC* ma = dynamic_cast<const MSG_AVC*>(&msg);
		if (ma->GetLoginTag() == 1)
		{
			_SetVerifyState(FALSE);
			OnOK();
		}
	}
	return S_OK;
}

void CVNOCLoginDlg::_SetVerifyState( BOOL bVerifying )
{
	CEdit* pEditun = (CEdit*)GetDlgItem(IDC_LoginDlg_EDIT_USERNAME);	ATLASSERT(pEditun);
	CEdit* pEditpw = (CEdit*)GetDlgItem(IDC_LoginDlg_EDIT_PWD);			ATLASSERT(pEditpw);
	CButton* pBtnrp = (CButton*)GetDlgItem(IDC_LoginDlg_CHECKBOX_R);		ATLASSERT(pBtnrp);
	CButton* pBtnLogin = (CButton*)GetDlgItem(IDOK);						ATLASSERT(pBtnLogin);

	pEditun->EnableWindow(!bVerifying);
	pEditpw->EnableWindow(!bVerifying);
	pBtnrp->EnableWindow(!bVerifying);
	pBtnLogin->EnableWindow(!bVerifying);
}
