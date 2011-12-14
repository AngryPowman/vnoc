// Room.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VNOC.h"
#include "Room.h"

const TCHAR className[] = _T("CodeCtrl");
const TCHAR appName[] = _T("Code");

// CRoom �Ի���

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


// CRoom ��Ϣ�������

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
	// TODO:  �ڴ������ר�õĴ�������

	return 0;
}

BOOL CRoom::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
