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


// CRoomDlg ��Ϣ�������

void CRoomDlg::OnEnChangeRichedit21()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  �ڴ������ר�õĴ�������
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
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
