//////////////////////////////////////////////////////////////////////////
//	����༭Edit�ؼ���
//	Ŀǰ������ֻ��һ���õ���������������ط��õ���ӵ�в�ͬ��UI���ֵĻ�����
//		�̳�һ���µ����࣬��Ҫ���Խ�����д����һ�������С�

#pragma once
#include "../../scintilla/include/Scintilla.h"
#include "../../scintilla/include/SciLexer.h"
#include "IConfig.h"

#define SCINTILLA_CLASS_NAME L"scintilla"
#define SCINTILLA_ADVANCED_DLL_NAME L"scilexer.dll"
#define SCINTILLA_DLL_NAME L"scintilla.dll"

class CSciEdit : 
	public CWnd,
	private IConfig
{
public:
	CSciEdit(HINSTANCE hInst = 0);
	virtual ~CSciEdit();
public:
	BOOL Create(LPCTSTR lpszWindowName, const RECT& rect, CWnd* pParentWnd, UINT nID);
	BOOL SetLexer(DWORD lexer = SCLEX_CPP);
	BOOL ShowLineNumber(BOOL show = TRUE);
	VOID FontConfigChanged();		// ֪ͨ�ؼ����������Ϣ�и��£�ˢ����ʾ��
private:
	VOID _AddRef();
	VOID _Release();
	VOID _InternalInitialize();
	sptr_t _SendSciMessage(UINT message,DWORD wParam=0,DWORD lParam=0);
	void _CalcLineNumberMarginWidth();
private:
private:
	HINSTANCE m_hinst;		// ָ����Դ�ļ�����λ��
	HMODULE m_dllHandle;
	LONG m_refCount;

	SciFnDirect m_pDirectFunc;
	sptr_t		m_pDirectPtr;
};