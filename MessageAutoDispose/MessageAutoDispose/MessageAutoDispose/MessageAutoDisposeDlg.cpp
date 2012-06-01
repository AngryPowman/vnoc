// MessageAutoDisposeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Parser.h"
#include "MessageAutoDispose.h"
#include "MessageAutoDisposeDlg.h"
#include "Parser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

MessageAutoDispose m_mad;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMessageAutoDisposeDlg �Ի���




CMessageAutoDisposeDlg::CMessageAutoDisposeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMessageAutoDisposeDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMessageAutoDisposeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_Messagelist);
	DDX_Control(pDX, IDC_EDIT4, m_EditName);
	DDX_Control(pDX, IDC_ID, m_EditID);
	DDX_Control(pDX, IDC_LIST1, m_ParamList);
	DDX_Control(pDX, IDC_Note, m_EditNote);
}

BEGIN_MESSAGE_MAP(CMessageAutoDisposeDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CLICK, IDC_TREE1, &CMessageAutoDisposeDlg::OnNMClickTree1)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CMessageAutoDisposeDlg::OnTvnSelchangedTree1)
END_MESSAGE_MAP()


// CMessageAutoDisposeDlg ��Ϣ�������

BOOL CMessageAutoDisposeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// ��������...���˵�����ӵ�ϵͳ�˵��С�
	
	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//List
	m_ParamList.SetExtendedStyle(LVS_EX_GRIDLINES);
	m_ParamList.InsertColumn( 0, _T("��Ϣ����"), LVCFMT_LEFT, 70 );
	m_ParamList.InsertColumn( 1, _T("��Ϣ����"), LVCFMT_LEFT, 130);
	m_ParamList.InsertColumn( 2, _T("˵��"), LVCFMT_LEFT, 300);
	//Tree
	m_CTOS = m_Messagelist.InsertItem(_T("Client -> Server"));
	m_STOC = m_Messagelist.InsertItem(_T("Server -> Client"));
	m_CTODP= m_Messagelist.InsertItem(_T("Client -> DP"));
	m_DPTOC= m_Messagelist.InsertItem(_T("DP -> Client"));
	m_mad.Open(_T("MsgDef.h"));
	for(int index = 0; index < (int)m_mad.GetMessageList().size();index++)
	{
		if (m_mad.GetMessageList()[index].Find(_T("_A")) != -1)
		{
			m_Messagelist.InsertItem(m_mad.GetMessageList()[index],m_CTOS);
		}
		if (m_mad.GetMessageList()[index].Find(_T("_R")) != -1)
		{
			m_Messagelist.InsertItem(m_mad.GetMessageList()[index],m_STOC);
		}
		if (m_mad.GetMessageList()[index].Find(_T("_AQ")) != -1)
		{
			m_Messagelist.InsertItem(m_mad.GetMessageList()[index],m_CTODP);
		}
		if (m_mad.GetMessageList()[index].Find(_T("_RQ")) != -1)
		{
			m_Messagelist.InsertItem(m_mad.GetMessageList()[index],m_DPTOC);
		}
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMessageAutoDisposeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMessageAutoDisposeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMessageAutoDisposeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMessageAutoDisposeDlg::OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
// 	CString ctmp;
// 	ctmp = m_Messagelist.GetItemText(m_Messagelist.GetSelectedItem());
// 	if(((ctmp.Find(_T("Client -> Server")) == -1)&&(ctmp.Find(_T("Server -> Client")) == -1))&&((ctmp.Find(_T("Client -> DP")) == -1)&&(ctmp.Find(_T("DP -> Client")) == -1)))
// 	{
// 		if (!ctmp.IsEmpty())
// 		{
// 			m_EditName.SetWindowText(ctmp);
// 		}
// 	}
}

void CMessageAutoDisposeDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	CString ctmp;
	int nRow;
	//����б��
	m_ParamList.DeleteAllItems();
	ctmp = m_Messagelist.GetItemText(m_Messagelist.GetSelectedItem());
	if(((ctmp.Find(_T("Client -> Server")) == -1)&&(ctmp.Find(_T("Server -> Client")) == -1))&&((ctmp.Find(_T("Client -> DP")) == -1)&&(ctmp.Find(_T("DP -> Client")) == -1)))
	{
		if (!ctmp.IsEmpty())
		{
			m_EditName.SetWindowText(ctmp);
			m_EditID.SetWindowText(m_mad.GetID(ctmp));
			m_EditNote.SetWindowText(m_mad.GetNote(ctmp));
			m_mad.GetParam(ctmp);
			for (int index = 0; index < (int)m_mad.GetMessageParamType().size();index++)
			{
				nRow = m_ParamList.InsertItem(0,m_mad.GetMessageParamType()[index]);
				m_ParamList.SetItemText(nRow,1,m_mad.GetMessageParam()[index]);
				m_ParamList.SetItemText(nRow,2,m_mad.GetMessageParamNote()[index]);
			}
		}
	}else{
		m_EditName.SetWindowText(_T(""));
		m_EditID.SetWindowText(_T(""));
		m_EditNote.SetWindowText(_T(""));
	}
}
