#include "stdafx.h"
#include "../../scintilla/include/Platform.h"
#include "SciEdit.h"
#include "../public/util.h"

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
				return TRUE;
			}
		}
	}
	return FALSE;
}

VOID CSciEdit::_AddRef()
{
	if (InterlockedIncrement(&m_refCount))	// ·¶Î§ÄÚ·ÇÏß³Ì°²È«
	{
		if (!m_dllHandle)
		{
			m_dllHandle = LoadLibraryW(SCINTILLA_ADVANCED_DLL_NAME);
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

sptr_t CSciEdit::_SendSciMessage( UINT message,DWORD wParam,DWORD lParam )
{
	if (m_pDirectFunc && m_pDirectPtr)
	{
		return m_pDirectFunc(m_pDirectPtr,message,wParam,lParam);
	}
	else
	{
		ATLASSERT(FALSE && "sci interface is not ready");
	}
	return 0;
}

BOOL CSciEdit::SetLexer( DWORD lexer /*= SCLEX_CPP*/ )
{
	switch (lexer)
	{
	case SCLEX_CPP:
		{
			_SendSciMessage(SCI_STYLESETFONT,STYLE_DEFAULT,(DWORD)"Courier New");
			_SendSciMessage(SCI_STYLESETSIZE, STYLE_DEFAULT,10);
			_SendSciMessage(SCI_STYLECLEARALL);
			sptr_t temp = _SendSciMessage(SCI_GETLEXER,0,0);
			_SendSciMessage(SCI_SETLEXER,lexer);
			temp = _SendSciMessage(SCI_GETLEXER,0,0);
			CString strKeyWords;
			strKeyWords.LoadString(m_hinst,IDS_Sci_Lexer_Keywords_CPP);
			_SendSciMessage(SCI_SETKEYWORDS,0,(DWORD)(LPCSTR)Util::String::CStringW2A(strKeyWords));
			strKeyWords.LoadString(m_hinst,IDS_Sci_Lexer_VariableTypes_CPP);
			_SendSciMessage(SCI_SETKEYWORDS,1,(DWORD)(LPCSTR)Util::String::CStringW2A(strKeyWords));
			_SendSciMessage(SCI_STYLESETFORE, SCE_C_WORD, 0x00FF0000);   //¹Ø¼ü×Ö
			_SendSciMessage(SCI_STYLESETFORE, SCE_C_WORD2, 0x00800080);   //¹Ø¼ü×Ö
			_SendSciMessage(SCI_STYLESETBOLD, SCE_C_WORD2, TRUE);   //¹Ø¼ü×Ö
			_SendSciMessage(SCI_STYLESETFORE, SCE_C_STRING, 0x001515A3); //×Ö·û´®
			_SendSciMessage(SCI_STYLESETFORE, SCE_C_CHARACTER, 0x001515A3); //×Ö·û
			_SendSciMessage(SCI_STYLESETFORE, SCE_C_PREPROCESSOR, 0x00808080);//Ô¤±àÒë¿ª¹Ø
			_SendSciMessage(SCI_STYLESETFORE, SCE_C_COMMENT, 0x00008000);//¿é×¢ÊÍ
			_SendSciMessage(SCI_STYLESETFORE, SCE_C_COMMENTLINE, 0x00008000);//ÐÐ×¢ÊÍ
			_SendSciMessage(SCI_STYLESETFORE, SCE_C_COMMENTDOC, 0x00008000);//ÎÄµµ×¢ÊÍ£¨/**¿ªÍ·£©

			_SendSciMessage(SCI_SETCARETLINEVISIBLE, TRUE);
			_SendSciMessage(SCI_SETCARETLINEBACK, 0xb0ffff); 
		}
		break;
	}
	return TRUE;
}
