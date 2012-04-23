//////////////////////////////////////////////////////////////////////////
//	����༭Edit�ؼ���
//	Ŀǰ������ֻ��һ���õ���������������ط��õ���ӵ�в�ͬ��UI���ֵĻ�����
//		�̳�һ���µ����࣬��Ҫ���Խ�����д����һ�������С�

#pragma once
#include "../../scintilla/include/Scintilla.h"
#include "../../scintilla/include/SciLexer.h"
#include "IConfig.h"
#include "SciDefine.h"

// ���������
class CSciEditConfigBase
	: public IConfigMonitor
{
public:
	CSciEditConfigBase();
	~CSciEditConfigBase();
public:
	VOID SetLexerID(SciLexerID id);
	VOID GetLexerID(SciLexerID& id);
	BOOL GetConfigNode(LPCTSTR path,ConfigNode& node);
	BOOL GetConfig(LPCTSTR path,LPCTSTR key,CString& value);
	BOOL GetConfig(LPCTSTR path,LPCTSTR key,int& value);
	BOOL GetConfig(LPCTSTR path,LPCTSTR key,double& value);
	VOID SetConfig(LPCTSTR path,LPCTSTR key,LPCTSTR value);
	VOID SetConfig(LPCTSTR path,LPCTSTR key,int value);
	VOID SetConfig(LPCTSTR path,LPCTSTR key,double value);

	STDMETHOD( ConfigChanged(const ConfigNode& node));
	STDMETHOD( LanguageChanged(CString language));
private:
	LanguageID m_langID;
	SciLexerID m_lexerID;
};

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
