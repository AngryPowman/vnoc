#pragma once
#include "../../scintilla/include/Scintilla.h"
#include "../../scintilla/include/SciLexer.h"

#define SCINTILLA_CLASS_NAME L"scintilla"
#define SCINTILLA_ADVANCED_DLL_NAME L"scilexer.dll"
#define SCINTILLA_DLL_NAME L"scintilla.dll"

class CSciEdit : public CWnd
{
public:
	CSciEdit(HINSTANCE hInst = 0);
	virtual ~CSciEdit();
public:
	BOOL Create(LPCTSTR lpszWindowName, const RECT& rect, CWnd* pParentWnd, UINT nID);
	BOOL SetLexer(DWORD lexer = SCLEX_CPP);
private:
	sptr_t _SendSciMessage(UINT message,DWORD wParam=0,DWORD lParam=0);
	VOID _AddRef();
	VOID _Release();
private:
	HINSTANCE m_hinst;		// 指定资源文件所在位置
	HMODULE m_dllHandle;
	LONG m_refCount;

	SciFnDirect m_pDirectFunc;
	sptr_t		m_pDirectPtr; 
};