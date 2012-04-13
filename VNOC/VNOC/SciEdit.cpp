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
{
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
		//Global->CheckLastError(_T("Create SciContainer"));	// MFC会截获掉。。。
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
	if (InterlockedIncrement(&m_refCount))	// 范围内非线程安全
	{
		if (!m_dllHandle)
		{
			m_dllHandle = LoadLibraryW(SCINTILLA_ADVANCED_DLL_NAME);
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
	_SendSciMessage(SCI_SETMARGINTYPEN,0,SC_MARGIN_NUMBER);
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
			_SendSciMessage(SCI_SETMARGINWIDTHN, 0, pixelWidth);
		}
	}
}

VOID CSciEdit::_InternalInitialize()
{
	_SendSciMessage(SCI_SETCODEPAGE,SC_CP_UTF8);
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
{	// 暂不需要
	return TRUE;
}

