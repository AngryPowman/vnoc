// RoomDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VNOC.h"
#include "RoomDlg.h"

// CRoomDlg �Ի���
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
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CRoomDlg ��Ϣ�������

int CRoomDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  �ڴ������ר�õĴ�������

	AfxInitRichEdit();
	return 0;
}

BOOL CRoomDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CEdit *pEdit = new CEdit;
	pEdit->Create(WS_VISIBLE|WS_CHILD,CRect(200,350,300,400),this,0);

	// ���SciEdit
	CString strCodeWndTitle;
	strCodeWndTitle.LoadString(AfxGetInstanceHandle(),IDS_Sci_WndTitle);
	CRect rec;
	CWnd* pSciPosControl = GetDlgItem(IDC_STATIC_SciPosition);
	if (pSciPosControl)
	{
		pSciPosControl->GetWindowRect(&rec);
		ScreenToClient(&rec);
	}
	BOOL result = m_codeEdit.Create(strCodeWndTitle,rec,this,0);
	m_codeEdit.ShowLineNumber();
	m_codeEdit.ShowBreakPointMargin();
	m_codeEdit.ShowFolderMargin();
	m_codeEdit.ShowIndentLine();
	//m_codeEdit.ShowInvisibleChars();
	ATLASSERT(result);
	if (result)
	{
		m_codeEdit.SetLexer(SCLEX_CPP);
	}
	m_codeEdit.EnableFolder();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CRoomDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	OnCancel();
}
