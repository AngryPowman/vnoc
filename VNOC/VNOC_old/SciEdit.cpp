#include "stdafx.h"
#include "../../scintilla/include/Platform.h"
#include "SciEdit.h"
#include "../util/util.h"

BEGIN_MESSAGE_MAP(CSciEdit, CWnd)
END_MESSAGE_MAP()

CSciEdit::CSciEdit(HINSTANCE hInst)
{
	m_dllHandle = 0;
	m_refCount = 0;
	if (hInst)
	{
		m_hinst = hInst;
	}
	else
	{
		m_hinst = AfxGetInstanceHandle();
	}
	ATLASSERT(m_hinst);
	_AddRef();
}

CSciEdit::~CSciEdit()
{
	_Release();
}

BOOL CSciEdit::Create( LPCTSTR lpszWindowName, const RECT& rect, CWnd* pParentWnd, UINT nID )
{	// ����һ����С��Scintilaһ���ĸ����ڣ����ڳ���scintila����װ��Ϣ��
	BOOL bResult = FALSE;
	if (_RegisterWndClass())
	{
		bResult = CWnd::Create(NULL
			,_T("SciContainerWnd")
			,WS_CHILD|WS_VISIBLE
			,rect
			,pParentWnd
			,nID
			,NULL);
		//Global->CheckLastError(_T("Create SciContainer"));	// MFC��ػ��������
		if (bResult && m_hWnd)
		{
			m_scihWnd = CreateWindow(SCINTILLA_CLASS_NAME
				,lpszWindowName
				,WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VISIBLE
				,0
				,0
				,rect.right - rect.left
				,rect.bottom - rect.top
				,m_hWnd
				,NULL
				,m_hinst
				,NULL);
			Global->CheckLastError(_T("CreateSci"));
			if (m_scihWnd)
			{
				m_pDirectFunc = (SciFnDirect)::SendMessage(m_scihWnd,SCI_GETDIRECTFUNCTION,0,0);
				if (m_pDirectFunc)
				{
					m_pDirectPtr = (sptr_t)::SendMessage(m_scihWnd,SCI_GETDIRECTPOINTER,0,0);
					if (m_pDirectPtr)
					{
						_InternalInitialize();
						return TRUE;
					}
				}
			}
		}
	}
	return FALSE;
}

VOID CSciEdit::_AddRef()
{
	if (InterlockedIncrement(&m_refCount))	// ��Χ�ڷ��̰߳�ȫ
	{
		if (!m_dllHandle)
		{
			m_dllHandle = LoadLibrary(SCINTILLA_ADVANCED_DLL_NAME);
			ATLASSERT(m_dllHandle);
			if (m_dllHandle == 0)
			{
				CString strErrDllMissingSci;
				strErrDllMissingSci.LoadString(AfxGetInstanceHandle(),IDS_Error_DllMissing);
				strErrDllMissingSci.Format(strErrDllMissingSci,SCINTILLA_DLL_USE);
				::MessageBox(0,strErrDllMissingSci,0,0);
			}
		}
	}
}

VOID CSciEdit::_Release()
{
	if (InterlockedDecrement(&m_refCount) <= 0)
	{
		if (m_dllHandle)
		{
			::FreeLibrary(m_dllHandle);
			m_dllHandle = 0;
		}
	}
}

sptr_t CSciEdit::_SendSciMessage( UINT message,DWORD wParam,DWORD lParam )
{
	if (m_pDirectFunc && m_pDirectPtr)
	{
		//Global->Logf(LogFile_Scintilla,_T("Message(%d,%d/%d)\n"),message,wParam,lParam);
		return m_pDirectFunc(m_pDirectPtr,message,wParam,lParam);
	}
	else
	{
		ATLASSERT(FALSE && "sci interface is not ready");
	}
	return 0;
}

BOOL CSciEdit::ShowLineNumber( BOOL show /*= TRUE*/ )
{
	_SendSciMessage(SCI_SETMARGINTYPEN,Scintilla_Margin_LineNumber,SC_MARGIN_NUMBER);
	_CalcLineNumberMarginWidth();
	return TRUE;
}

void CSciEdit::_CalcLineNumberMarginWidth()
{
	int linesVisible = (int) _SendSciMessage(SCI_LINESONSCREEN);
	if (linesVisible)
	{
		int firstVisibleLineVis = (int) _SendSciMessage(SCI_GETFIRSTVISIBLELINE);
		int lastVisibleLineVis = linesVisible + firstVisibleLineVis + 1;
		int i = 0;
		while (lastVisibleLineVis)
		{
			lastVisibleLineVis /= 10;
			i++;
		}
		i = max(i, Scintilla_LineNumber_Min);
		{
			int pixelWidth = int(8 + i * _SendSciMessage(SCI_TEXTWIDTH, STYLE_LINENUMBER, (LPARAM)"8"));
			_SendSciMessage(SCI_SETMARGINWIDTHN, Scintilla_Margin_LineNumber, pixelWidth);
		}
	}
}

VOID CSciEdit::_InternalInitialize()
{
	_SendSciMessage(SCI_SETCODEPAGE,SC_CP_UTF8);
	CConfig config;
	int tabWidth = 4;

	if (config.Get(_T("SciEdit/Language/Editor/TabWidth")))
	{
		config.GetNode().GetAttribute(_T("tabWidth"),tabWidth);
	}
	_SendSciMessage(SCI_SETTABWIDTH,tabWidth);
}

BOOL CSciEdit::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	SCNotification* pNotify = reinterpret_cast<SCNotification*>(lParam);
	if (!_OnSciNotify(pNotify))
	{
		return CWnd::OnNotify(wParam, lParam, pResult);
	}
	return TRUE;
}

BOOL CSciEdit::_RegisterWndClass()
{	// �ݲ���Ҫ
	return TRUE;
}

BOOL CSciEdit::ShowBreakPointMargin( BOOL bShow /*= TRUE*/ )
{
	_SendSciMessage(SCI_SETMARGINTYPEN,Scintilla_Margin_BreakPoint,SC_MARGIN_SYMBOL);
	_SendSciMessage(SCI_SETMARGINMASKN,Scintilla_Margin_BreakPoint,SC_CURSORARROW);
	_SendSciMessage(SCI_SETMARGINWIDTHN,Scintilla_Margin_BreakPoint,10);
	return TRUE;
}

BOOL CSciEdit::ShowFolderMargin( BOOL bShow /*= TRUE*/ )
{
	_SendSciMessage(SCI_SETMARGINTYPEN,Scintilla_Margin_BreakPoint,SC_MARGIN_SYMBOL);
	_SendSciMessage(SCI_SETMARGINWIDTHN,Scintilla_Margin_Folder,Scintilla_Margin_Folder_Width);
	return TRUE;
}

BOOL CSciEdit::ShowIndentLine( BOOL bShow /*= TRUE*/ )
{
	_SendSciMessage(SCI_SETINDENTATIONGUIDES, (WPARAM)bShow?(SC_IV_LOOKBOTH):(SC_IV_NONE));
	return TRUE;
}

BOOL CSciEdit::ShowInvisibleChars( BOOL bShow /*= TRUE*/ )
{
	_SendSciMessage(SCI_SETVIEWWS, bShow?SCWS_VISIBLEALWAYS:SCWS_INVISIBLE);
	_SendSciMessage(SCI_SETVIEWEOL, bShow);
	return TRUE;
}

BOOL CSciEdit::EnableFolder( BOOL bEnable /*= TRUE*/ )
{
	_SendSciMessage(SCI_SETPROPERTY,(DWORD)"fold",(DWORD)"1");
	_SendSciMessage(SCI_SETMARGINTYPEN,Scintilla_Margin_Folder,SC_MARGIN_SYMBOL);
	_SendSciMessage(SCI_SETMARGINMASKN,Scintilla_Margin_Folder,SC_MASK_FOLDERS);
	_SendSciMessage(SCI_SETMARGINSENSITIVEN,Scintilla_Margin_Folder,TRUE);
	
	_SetMarker(SC_MARKNUM_FOLDEREND,		13,	0x00ffffff,0x00808080,0xffffff);
	_SetMarker(SC_MARKNUM_FOLDEROPENMID,	15,	0x00ffffff,0x00808080,0xffffff);
	_SetMarker(SC_MARKNUM_FOLDERMIDTAIL,	11,	0x00ffffff,0x00808080,0xffffff);
	_SetMarker(SC_MARKNUM_FOLDERTAIL,		10,	0x00ffffff,0x00808080,0xffffff);
	_SetMarker(SC_MARKNUM_FOLDERSUB,		9,	0x00ffffff,0x00808080,0xffffff);
	_SetMarker(SC_MARKNUM_FOLDER,			12,	0x00ffffff,0x00808080,0xffffff);
	_SetMarker(SC_MARKNUM_FOLDEROPEN,		14,	0x00ffffff,0x00808080,0xffffff);

	_SendSciMessage(SCI_SETFOLDFLAGS, SC_FOLDFLAG_LINEAFTER_CONTRACTED); //����۵������۵��е����¸���һ������
	return TRUE;
}

BOOL CSciEdit::_SetMarker( int marker, int markerType, COLORREF fore, COLORREF back, COLORREF foreActive )
{
	_SendSciMessage(SCI_MARKERDEFINE, marker, markerType);
	_SendSciMessage(SCI_MARKERSETFORE, marker, fore);
	_SendSciMessage(SCI_MARKERSETBACK, marker, back);
	_SendSciMessage(SCI_MARKERSETBACKSELECTED, marker, foreActive);
	return TRUE;
}

