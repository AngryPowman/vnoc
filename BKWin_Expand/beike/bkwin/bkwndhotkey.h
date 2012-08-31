//////////////////////////////////////////////////////////////////////////
//  Class Name: CBkHotkey
// Description: 热键控件(窗口控件)
//     Creator: YangDa
//     Version: 2011.05.26 - 1.0 - YangDa - Create
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "bkwndrealctrl.h"
#include "bkmsg.h"

static const TCHAR szPlus[] = _T(" + ");
class CBkHotkey : public CWindowImpl< CBkHotkey, WTL::CHotKeyCtrl , CWinTraitsOR< ES_AUTOHSCROLL >  >
	, public CBkRealCtrl
{
	BKOBJ_DECLARE_CLASS_NAME(CBkHotkey, "hotkey")
		DECLARE_WND_SUPERCLASS(_T("BKHotKey") , HOTKEY_CLASS );

	typedef CWindowImpl< CBkHotkey, WTL::CHotKeyCtrl , CWinTraitsOR< ES_AUTOHSCROLL > > __baseClass;
	typedef CBkRealCtrl __bkClass;

public:
	CBkHotkey()
	{

	}

	virtual ~CBkHotkey()
	{

	}

	virtual void CreateHandle( HWND hParent )
	{
		if( m_hWnd )
		{
			::DestroyWindow(m_hWnd);
		}

		DWORD dwStyle = WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|ES_AUTOHSCROLL;
		m_hRealWnd = Create( hParent,m_rcWindow,NULL,dwStyle );

		SetFont( BkFontPool::GetFont(BKF_DEFAULTFONT) );
	}

	CString GetKeyName(UINT vk, BOOL fExtended)const
	{

	LONG lScan = MapVirtualKey(vk,0) << 16;
	// if it's an extended key, add the extended flag
	if (fExtended)
	lScan |= 0x01000000L;;

	CString str;
	int nBufferLen = 64;
	int nLen;
	do
	{
	nBufferLen *= 2;
	LPTSTR psz = str.GetBufferSetLength(nBufferLen);
	nLen = ::GetKeyNameText(lScan, psz, nBufferLen + 1);
	str.ReleaseBuffer(nLen);
	}
	while (nLen == nBufferLen);
	return str;
	}


CString GetHotKeyName() const
{
	ATLASSERT(::IsWindow(m_hWnd));

	CString strKeyName;
	WORD wCode;
	WORD wModifiers;

	GetHotKey(wCode, wModifiers);
	if (wCode != 0 || wModifiers != 0)
	{
		if (wModifiers & HOTKEYF_CONTROL)
		{
			strKeyName += GetKeyName(VK_CONTROL, FALSE);
			strKeyName += szPlus;
		}

		if (wModifiers & HOTKEYF_SHIFT)
		{
			strKeyName += GetKeyName(VK_SHIFT, FALSE);
			strKeyName += szPlus;
		}

		if (wModifiers & HOTKEYF_ALT)
		{
			strKeyName += GetKeyName(VK_MENU, FALSE);
			strKeyName += szPlus;
		}

		strKeyName += GetKeyName(wCode, wModifiers&HOTKEYF_EXT);
	
	}

	return strKeyName;

}


public:
	BEGIN_MSG_MAP( CBkHotkey )
		MESSAGE_HANDLER( WM_SETFOCUS, OnWndSetFocus )
		MESSAGE_HANDLER( WM_KILLFOCUS, OnWndKillFocus )	
	END_MSG_MAP()

	LRESULT OnWndSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
	{
		bHandled = FALSE;

		return 0;
	}

	LRESULT OnWndKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
	{
		bHandled = FALSE;

		if( m_pNotify )
		{
			NotifyInfo info = { BKM_HOTKEY_KILLFOCUS, CPoint(0, 0), this, GetHotKey() };
			m_pNotify->Action( &info );
		}

		return 0;
	}
};