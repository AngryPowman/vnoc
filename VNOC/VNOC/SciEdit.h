//////////////////////////////////////////////////////////////////////////
//	代码编辑Edit控件。
//	目前的需求只有一处用到。如果还有其它地方用到且拥有不同的UI表现的话，请
//		继承一个新的子类，不要尝试将代码写到这一个类体中。

#pragma once
#include "../../scintilla/include/Scintilla.h"
#include "../../scintilla/include/SciLexer.h"
#include "IConfig.h"

#define SCINTILLA_CLASS_NAME			_T("scintilla")
#define SCINTILLA_ADVANCED_DLL_NAME		_T("scilexer.dll")
#define SCINTILLA_DLL_NAME				_T("scintilla.dll")
#define SCINTILLA_DLL_USE				SCINTILLA_ADVANCED_DLL_NAME
#define Scintilla_LineNumber_Min		2
#define Scintilla_Margin_BreakPoint		0
#define Scintilla_Margin_LineNumber		1
#define Scintilla_Margin_Folder			2
#define Scintilla_Margin_Folder_Width	14

class CSciEdit : 
	public CWnd
{
public:
	CSciEdit(HINSTANCE hInst = 0);
	virtual ~CSciEdit();
public:
	BOOL Create(LPCTSTR lpszWindowName, const RECT& rect, CWnd* pParentWnd, UINT nID);
	BOOL SetLexer(DWORD lexer = SCLEX_CPP);
	BOOL ShowLineNumber(BOOL show = TRUE);
	BOOL ShowBreakPointMargin(BOOL bShow = TRUE);	// 显示断点边栏
	BOOL ShowFolderMargin(BOOL bShow = TRUE);		// 显示大纲折叠边栏
	BOOL ShowIndentLine(BOOL bShow = TRUE);
	BOOL ShowInvisibleChars(BOOL bShow = TRUE);
	BOOL EnableFolder(BOOL bEnable = TRUE);			// 启用大纲折叠
	VOID FontConfigChanged();		// 通知控件，字体等信息有更新，刷新显示。
private:
	// 引用计数主要是用以保证dll的正常加载卸载
	VOID _AddRef();
	VOID _Release();
	VOID _InternalInitialize();
	BOOL _RegisterWndClass();
private:
	sptr_t _SendSciMessage(UINT message,DWORD wParam=0,DWORD lParam=0);
	void _CalcLineNumberMarginWidth();
	BOOL _OnSciNotify(SCNotification* notify);
	BOOL _SetMarker(int marker, int markerType, COLORREF fore, COLORREF back, COLORREF foreActive);
private:
	HINSTANCE m_hinst;		// 指定资源文件所在位置
	HWND m_scihWnd;
	HMODULE m_dllHandle;
	LONG m_refCount;

	SciFnDirect m_pDirectFunc;
	sptr_t		m_pDirectPtr;

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};