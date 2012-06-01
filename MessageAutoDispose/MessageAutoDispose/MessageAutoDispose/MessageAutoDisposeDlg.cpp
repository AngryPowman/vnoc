// MessageAutoDisposeDlg.cpp : 实现文件
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

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CMessageAutoDisposeDlg 对话框




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


// CMessageAutoDisposeDlg 消息处理程序

BOOL CMessageAutoDisposeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// 将“关于...”菜单项添加到系统菜单中。
	
	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	
	// TODO: 在此添加额外的初始化代码
	//List
	m_ParamList.SetExtendedStyle(LVS_EX_GRIDLINES);
	m_ParamList.InsertColumn( 0, _T("消息类型"), LVCFMT_LEFT, 70 );
	m_ParamList.InsertColumn( 1, _T("消息名称"), LVCFMT_LEFT, 130);
	m_ParamList.InsertColumn( 2, _T("说明"), LVCFMT_LEFT, 300);
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMessageAutoDisposeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
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

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMessageAutoDisposeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMessageAutoDisposeDlg::OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	CString ctmp;
	int nRow;
	//清空列表框
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
