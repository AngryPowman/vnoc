//////////////////////////////////////////////////////////////////////////
//	代码编辑Edit控件。
//	目前的需求只有一处用到。如果还有其它地方用到且拥有不同的UI表现的话，请
//		继承一个新的子类，不要尝试将代码写到这一个类体中。

#pragma once
#include "../../scintilla/include/Scintilla.h"
#include "../../scintilla/include/SciLexer.h"
#include "IConfig.h"
#include "SciDefine.h"

// 管理配置项。
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
