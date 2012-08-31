
inline CBkTransRealLayer::CBkTransRealLayer()
{
    m_clrTrans = RGB(255,255,255);
    m_hWndParent = NULL;
    m_brush = NULL;
    m_bAllowActive = FALSE;
}

inline CBkTransRealLayer::CBkTransRealLayer(COLORREF clrTrans, BOOL bActive/* = FALSE*/)
{
    m_clrTrans = clrTrans;
    m_hWndParent = NULL;
    m_brush = NULL;
    m_bAllowActive = bActive;
}

inline CBkTransRealLayer::~CBkTransRealLayer()
{
}

inline HWND CBkTransRealLayer::CreateTransLayer(HWND hParent)
{
    return _CreateWindow(hParent);
}

inline int CBkTransRealLayer::_OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    ModifyStyle( WS_SIZEBOX|WS_DLGFRAME|WS_MAXIMIZEBOX|WS_SYSMENU|WS_BORDER|WS_MAXIMIZE|WS_MINIMIZE,0,1 );

    if( !m_brush )
    {
        m_brush = ::CreateSolidBrush(m_clrTrans);
    }
    
    ::SetLayeredWindowAttributes( m_hWnd, m_clrTrans, 0, LWA_COLORKEY );
    CBkModule::GetInstance()->GetMessageLoop()->AddMessageFilter( this );
    
    return FALSE;
}

inline HBRUSH CBkTransRealLayer::_OnCtlColorDlg(CDCHandle dc, CWindow wnd)
{
    return m_brush;
}

inline BOOL CBkTransRealLayer::_OnDestroy()
{
    CBkModule::GetInstance()->GetMessageLoop()->RemoveMessageFilter( this );

    if( m_brush )
    {
        ::DeleteObject(m_brush);
        m_brush = NULL;
    }
    return FALSE;
}

inline BOOL CBkTransRealLayer::_OnEraseBkgnd(CDCHandle dc)
{
    CRect rc;
    GetClientRect(&rc);
    ::FillRect( dc.m_hDC, rc, m_brush );
    return TRUE;
}

inline void CBkTransRealLayer::_OnKillFocus( HWND pNewWnd )
{
    if( m_pNotify )
    {
        NotifyInfo info = { WM_KILLFOCUS, CPoint(0, 0), this, 0 };
        m_pNotify->Action( &info );
    }
}

inline BOOL CBkTransRealLayer::_OnPaint(HDC hdc)
{
    CPaintDC dc(m_hWnd);

    CRect rc;
    GetClientRect(&rc);

    HBRUSH hBrush = ::CreateSolidBrush(RGB(255,0,0));
    ::FillRect( hdc, rc, hBrush );
    ::DeleteObject(hBrush);

    return FALSE;
}

inline LRESULT CBkTransRealLayer::_OnCtlColorDlg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    return (LRESULT)m_brush;
}

inline LRESULT CBkTransRealLayer::_OnSysKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled  = FALSE;
	//因为此窗口创建的时候没有WS_SYSMENU风格，所以应该禁用alt + F4
	if (wParam == VK_F4)
	{
		return FALSE;
	}
	return TRUE;
}
inline BOOL CBkTransRealLayer::PreTranslateMessage(MSG* pMsg)
{
    if( pMsg->message == WM_KEYDOWN )
    {
        if( ::IsChild(this->m_hWnd, pMsg->hwnd) && 
            !::SendMessage(pMsg->hwnd, WM_GETDLGCODE, pMsg->wParam, (LPARAM)pMsg) )
        {
            // 若控件不需要处理键盘消息，则转由窗口处理
            if( pMsg->wParam == VK_TAB || pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
            {
                pMsg->hwnd = m_hWndParent;
            }
        }
    }
    else if (pMsg->message == WM_KEYUP)
    {
        if( ::IsChild(this->m_hWnd, pMsg->hwnd) && 
            !::SendMessage(pMsg->hwnd, WM_GETDLGCODE, pMsg->wParam, (LPARAM)pMsg) )
        {
            if( pMsg->wParam == VK_RETURN )
            {
                // 若控件不需要处理键盘消息，则转由窗口处理
                pMsg->hwnd = m_hWndParent;
            }
        }
    }
    return FALSE;
}

inline HWND CBkTransRealLayer::_CreateWindow(HWND hWndParent)
{
    if (NULL != m_hWnd)
        return m_hWnd;

    DWORD dwActive = m_bAllowActive?NULL:WS_EX_NOACTIVATE;
    DWORD dwStyle = WS_POPUP|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
    DWORD dwExStyle  = WS_EX_LAYERED|dwActive;
    m_hWndParent = hWndParent;

    CRect rc(0,0,0,0);
    HWND hWnd = __baseClass::Create(hWndParent, rc, NULL, dwStyle, dwExStyle, 0U, NULL);
    return hWnd;
}

inline void CBkTransRealLayer::_OnSysCommand(UINT nID, CPoint point)
{
	if (nID == SC_CLOSE || nID == SC_MINIMIZE)
	{
		return;
	}

	SetMsgHandled(FALSE);
}