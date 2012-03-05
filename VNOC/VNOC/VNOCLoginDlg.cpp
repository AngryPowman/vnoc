// VNOCDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VNOC.h"
#include "VNOCLoginDlg.h"
#include "RoomDlg.h"
#include "Config.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVNOCDlg �Ի���




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

	m_LinkRegist.SetLink(_T("ע���˺�"),_T("google.com.hk"),FALSE);
	m_FoundPassword.SetLink(_T("�һ�����"),_T("google.com.hk"),FALSE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
