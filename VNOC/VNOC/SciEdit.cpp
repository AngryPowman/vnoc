#include "stdafx.h"
#include "../../scintilla/include/SciLexer.h"
#include "../../scintilla/include/Platform.h"
#include "../../scintilla/include/Scintilla.h"
#include "SciEdit.h"

HMODULE CSciEdit::m_dllHandle = 0;
LONG CSciEdit::m_refCount = 0;

CSciEdit::CSciEdit(HINSTANCE hInst)
{
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
	return bResult;
}

VOID CSciEdit::_AddRef()
{
	if (InterlockedIncrement(&m_refCount))	// 范围内非线程安全
	{
		if (!m_dllHandle)
		{
			m_dllHandle = LoadLibraryW(SCINTILLA_DLL_NAME);
			ATLASSERT(m_dllHandle);
			if (m_dllHandle == 0)
			{
				CString strErrDllMissingSci;
				strErrDllMissingSci.LoadString(AfxGetInstanceHandle(),IDS_Error_DllMissing);
				strErrDllMissingSci.Format(strErrDllMissingSci,SCINTILLA_ADVANCED_DLL_NAME);
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
