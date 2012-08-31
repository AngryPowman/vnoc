

inline CBkHotKeyCtrl::CBkHotKeyCtrl()
{
	m_wVirtualKeyCode = m_wModifiers = 0;
	m_bSelect = FALSE;

}

inline CBkHotKeyCtrl::~CBkHotKeyCtrl()
{

}


///////////消息处理//////////////

inline LRESULT CBkHotKeyCtrl::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{    
	
	UINT uVirtualCode = (UINT)wParam; 
	
	if (uVirtualCode == VK_CONTROL || uVirtualCode == VK_SHIFT || uVirtualCode == VK_MENU)
	{
		m_wModifiers = 0;
		m_wVirtualKeyCode = 0;
		this->SetWindowText(L"");
		this->SetWindowText(L"无");
		
	}
	else 
	{   
		UpdateModifiers(FALSE);
		m_wVirtualKeyCode = uVirtualCode;
		if (IsEqualKey() )
		{
			if (m_wVirtualKeyCode == VK_ADD)
			{
				m_wVirtualKeyCode = VK_OEM_PLUS;
			}
			else 
			{
				m_wVirtualKeyCode = VK_OEM_MINUS;
			}
		}
        if (IsUnsetableHotKey())
        {
			m_wModifiers = 0;
			m_wVirtualKeyCode = 0;
			this->SetWindowText(L"");
			this->SetWindowText(L"无");
			return TRUE;
        }
		
		SetWindowText(L"");
		SetWindowText(GetHotKeyName());
	}

 
	return TRUE;
}

inline LRESULT CBkHotKeyCtrl::OnChar( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{  

	return TRUE;

}

inline LRESULT CBkHotKeyCtrl::OnSysKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{  
	if(wParam == VK_MENU||wParam == VK_CONTROL || wParam == VK_SHIFT)
	{
		m_wModifiers = 0;
		m_wVirtualKeyCode = 0;
		this->SetWindowText(L"");
		this->SetWindowText(L"无");
		
	}
	else if (wParam == VK_F10)
	{
		m_wVirtualKeyCode = VK_F10;
		SetWindowText(L"");
		SetWindowText(GetHotKeyName());
	}
	else 
	{  
		UpdateModifiers(FALSE);
		m_wVirtualKeyCode = wParam;
		if (IsEqualKey() )
		{
			if (m_wVirtualKeyCode == VK_ADD)
			{
				m_wVirtualKeyCode = VK_OEM_PLUS;
			}
			else 
			{
				m_wVirtualKeyCode = VK_OEM_MINUS;
			}
		}
		if (IsUnsetableHotKey())
		{
			m_wModifiers = 0;
			m_wVirtualKeyCode = 0;
			this->SetWindowText(L"");
			this->SetWindowText(L"无");
			return TRUE;
		}
		SetWindowText(L"");
		SetWindowText(GetHotKeyName());
	}

	return TRUE;
}

inline LRESULT CBkHotKeyCtrl::OnSysChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{  

	return TRUE; 
}
inline LRESULT CBkHotKeyCtrl::OnGetDlgCode(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return DLGC_WANTARROWS | DLGC_WANTALLKEYS | DLGC_WANTCHARS |DLGC_WANTMESSAGE;
}

inline LRESULT CBkHotKeyCtrl::OnLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return TRUE;
}

inline LRESULT CBkHotKeyCtrl::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return TRUE;
}

inline LRESULT CBkHotKeyCtrl::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return TRUE;
}

inline LRESULT CBkHotKeyCtrl::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{  
	//处理WM_MOUSEMOVE消息不让串联到基类， 可以禁止选中
	return TRUE;
}

inline LRESULT CBkHotKeyCtrl::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//处理WM_SETFOCUS不让串联到基类，可以禁止caret
	return TRUE; 
}

///////////////////////////////////////////////////////////////////
inline void CBkHotKeyCtrl::CreateHandle( HWND hParent )
{
	if( m_hWnd )
	{
		::DestroyWindow(m_hWnd);
	}

    if( m_hWndRealContainer )
    {
        hParent = m_hWndRealContainer;
    }

	CBkRealCtrl::CreateHandle( hParent );
	DWORD dwStyle = WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
	m_hRealWnd = Create( hParent,m_rcWindow,NULL,dwStyle );
	SetFont( BkFontPool::GetFont(BKF_DEFAULTFONT) );
	LimitText(sizeof(wchar_t)*25);

}

inline void CBkHotKeyCtrl::SetHotKey(WORD wVirtualKeyCode,WORD wModifiers)
{
	m_wVirtualKeyCode = wVirtualKeyCode;
	m_wModifiers = wModifiers;
	CString strKeyName = GetHotKeyName();
	SetWindowText(strKeyName);
}

inline DWORD CBkHotKeyCtrl::GetHotKey() const
{
	return MAKEWORD( m_wVirtualKeyCode, m_wModifiers );
}
inline void CBkHotKeyCtrl::GetHotKey(WORD &wVirtualKeyCode,WORD &wModifiers) const
{
	wVirtualKeyCode = m_wVirtualKeyCode;
	wModifiers = m_wModifiers;
}

static const wchar_t szBkPlus[] = _T(" + ");
inline CString CBkHotKeyCtrl::GetHotKeyName() 
{
	ATLASSERT(::IsWindow(m_hWnd));
	CString strKeyName;
	if (m_wVirtualKeyCode != 0 || m_wModifiers != 0)
	{

		if (m_wModifiers & BK_HOTKEYF_CONTROL)
		{
			strKeyName += GetKeyName(VK_CONTROL, FALSE);
			strKeyName += szBkPlus;
		}
		if (m_wModifiers & BK_HOTKEYF_SHIFT)
		{
			strKeyName += GetKeyName(VK_SHIFT, FALSE);
			strKeyName += szBkPlus;
		}
		if (m_wModifiers & BK_HOTKEYF_ALT)
		{
			strKeyName += GetKeyName(VK_MENU, FALSE);
			strKeyName += szBkPlus;
		}
		if (m_wVirtualKeyCode == VK_OEM_PLUS)
		{
			strKeyName += L"+";
		}
		else if (m_wVirtualKeyCode == VK_OEM_MINUS)
		{
			strKeyName += L"-";
		}
		else
		{
			strKeyName += GetKeyName(m_wVirtualKeyCode, IsExtendKey(m_wVirtualKeyCode));
		}



	}
	if (strKeyName.IsEmpty())
	{
		strKeyName = L"无";
	}
	return strKeyName;
}

inline CString CBkHotKeyCtrl::GetKeyName(UINT vk,BOOL fExtended )
{

	LONG lScan = MapVirtualKey(vk, MAPVK_VK_TO_VSC);
	// if it's an extended key, add the extended flag
	if (fExtended)
		lScan |= 0x100; // Add extended bit
	CString str;


	::GetKeyNameText(lScan<<16, str.GetBuffer(20), 19);
	str.ReleaseBuffer();
	return str;
}
inline BOOL CBkHotKeyCtrl::IsExtendKey(UINT vk) 
{
	if (vk == VK_INSERT || vk == VK_DELETE || vk == VK_HOME || vk == VK_END || vk == VK_PRIOR || vk == VK_NEXT || vk == VK_LEFT || vk == VK_RIGHT || vk == VK_UP || vk == VK_DOWN )
	{
		return TRUE;
	}

	return FALSE;
}


inline void CBkHotKeyCtrl::UpdateModifiers(BOOL bExtended)
{
	m_wModifiers = 0;
	if ( GetKeyState(VK_CONTROL) & 0x8000 )
	{
		m_wModifiers |=BK_HOTKEYF_CONTROL;
	}
	if ( GetKeyState(VK_SHIFT) & 0x8000 )
	{
		m_wModifiers |= BK_HOTKEYF_SHIFT;
	}
	if ( GetKeyState(VK_MENU) & 0x8000 )
	{
		m_wModifiers |= BK_HOTKEYF_ALT;
	}
	if ( bExtended )
	{
		m_wModifiers |= BK_HOTKEYF_EXT;
	}
}


inline BOOL CBkHotKeyCtrl::IsUnsetableHotKey()
{
	for (int i = 0; i < _countof(m_unsetableHotKey); ++i)
	{
		WORD wModifiers = 0;
		if (m_unsetableHotKey[i].bControlKey)
		{
			wModifiers |= BK_HOTKEYF_CONTROL;
		}
		if (m_unsetableHotKey[i].bShiftKey)
		{
			wModifiers |= BK_HOTKEYF_SHIFT;
		}
		if (m_unsetableHotKey[i].bAltKey)
		{
			wModifiers |= BK_HOTKEYF_ALT;
		}

		if (m_wModifiers == wModifiers && m_wVirtualKeyCode == m_unsetableHotKey[i].nKey)
		{
			return TRUE;
		}

	}

	return FALSE;
}
inline BOOL CBkHotKeyCtrl::IsEqualKey()
{
	if (m_wVirtualKeyCode == VK_ADD || m_wVirtualKeyCode == VK_SUBTRACT)
		return TRUE;
	return FALSE;
	
}