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
	BOOL bResult = CWnd::Create(SCINTILLA_CLASS_NAME,lpszWindowName,WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VISIBLE,rect,pParentWnd,nID);
	if (!bResult)
	{
		DWORD errNo = GetLastError();
		errNo = errNo;
	}
	else
	{
		m_pDirectFunc = (SciFnDirect)SendMessage(SCI_GETDIRECTFUNCTION);
		if (m_pDirectFunc)
		{
			m_pDirectPtr = (sptr_t)SendMessage(SCI_GETDIRECTPOINTER);
			if (m_pDirectPtr)
			{
				_InternalInitialize();
				return TRUE;
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
	sptr_t ret;
	ret = _SendSciMessage(SCI_GETLINECOUNT,0,0);
	DWORD lineCount = static_cast<DWORD>(ret);
	CStringA strFmt;
	strFmt.Format("%d",lineCount);
	ret = _SendSciMessage(SCI_TEXTWIDTH,STYLE_LINENUMBER,(DWORD)strFmt.GetString());
	DWORD width = static_cast<DWORD>(ret) + 5;
	ret = _SendSciMessage(SCI_GETMARGINWIDTHN);
	DWORD oldWidth = static_cast<DWORD>(ret);
	Global->Logf(LogFile_Scintilla,_T("通知更新行号边栏宽度.行数:%s.旧宽度:%d,新宽度:%d\n"),CString(strFmt),oldWidth,width);
	if (oldWidth != width)
	{
		_SendSciMessage(SCI_SETMARGINWIDTHN,0, width);
	}
}

VOID CSciEdit::_InternalInitialize()
{
	_SendSciMessage(SCI_SETCODEPAGE,SC_CP_UTF8);
}

BOOL CSciEdit::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class

	return CWnd::OnNotify(wParam, lParam, pResult);
}
