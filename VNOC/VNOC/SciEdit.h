//////////////////////////////////////////////////////////////////////////
//	����༭Edit�ؼ���
//	Ŀǰ������ֻ��һ���õ���������������ط��õ���ӵ�в�ͬ��UI���ֵĻ�����
//		�̳�һ���µ����࣬��Ҫ���Խ�����д����һ�������С�

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
	BOOL ShowBreakPointMargin(BOOL bShow = TRUE);	// ��ʾ�ϵ����
	BOOL ShowFolderMargin(BOOL bShow = TRUE);		// ��ʾ����۵�����
	BOOL ShowIndentLine(BOOL bShow = TRUE);
	BOOL ShowInvisibleChars(BOOL bShow = TRUE);
	BOOL EnableFolder(BOOL bEnable = TRUE);			// ���ô���۵�
	VOID FontConfigChanged();		// ֪ͨ�ؼ����������Ϣ�и��£�ˢ����ʾ��
private:
	// ���ü�����Ҫ�����Ա�֤dll����������ж��
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
	HINSTANCE m_hinst;		// ָ����Դ�ļ�����λ��
	HWND m_scihWnd;
	HMODULE m_dllHandle;
	LONG m_refCount;

	SciFnDirect m_pDirectFunc;
	sptr_t		m_pDirectPtr;

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};