#include "../bkeffect/bkEffectRenderer.h"
#include "../bkeffect/bkEffect.h"

inline CBkDialog::CBkDialog()
{
    m_bUpdateFocus = FALSE;
    m_hWndParent = NULL;
	m_hWndTop = NULL;
    m_bAppWin = FALSE;
    m_bCaptureSetted = FALSE;
    m_bExitModalLoop = FALSE;
    m_bActive = FALSE;
    m_bResizable = FALSE;
    m_minSize = CSize(0, 0);
    m_startPos = CSize(-1, -1);
    m_tabDefault = 0;
    m_clrFocus = RGB(0,0,255);
    m_pDragWnd = NULL;
    m_alpha = 256;
    m_bLayerTrans = FALSE;
    m_bLayerActive = FALSE;
    m_pTransRealLayer = NULL;
    m_clrTransRealLayer = RGB(255,255,255);
    m_dwPrePaintTime = 0;
    m_bInvalid = TRUE;
	m_dwWndStyle = 0;
}

inline CBkDialog::~CBkDialog()
{
	_Clear();

	if( m_wndToolTip )
    {
        if ( m_wndToolTip && ::IsWindow(m_wndToolTip->m_hWnd) )
        {
            m_wndToolTip->DestroyWindow();
            m_wndToolTip->m_hWnd = NULL;
        }

        delete m_wndToolTip;
        m_wndToolTip = NULL;
    }
}

inline IBkWnd* CBkDialog::GetItem(UINT uItemID) const
{
    return FindChildByCmdID(uItemID);
}

inline void CBkDialog::ShowWindow(BOOL bShow)
{
    if( ::IsWindow(m_hWnd) )
    {
        //__bkClass::ShowWindow( bShow );
        __baseClass::ShowWindow(bShow ? SW_SHOW : SW_HIDE);

        DWORD dwStyle = bShow ? SWP_SHOWWINDOW : SWP_HIDEWINDOW;
        __baseClass::SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE|dwStyle );

        if( m_bLayerTrans && m_pTransRealLayer )
        {
            m_pTransRealLayer->SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE|dwStyle );
        }

        if( m_tabControler && m_tabDefault > 0 )
        {
            m_tabControler->SetCurrent( m_tabDefault, TRUE );
        }
        NotifyInvalidate();
    }
}

inline HWND CBkDialog::Create(LPCTSTR szXml, HWND hWndParent /* = ::GetActiveWindow */)
{
    _Clear();

    do 
    {
		if (!m_pDropTarget)
			m_pDropTarget = new CIDropTarget(this);

		SetDropTarget(m_pDropTarget);

        SetContainer(hWndParent);

        if( !m_tabControler )
        {
            m_tabControler = new CBkTaborder(this);
        }

        if (!_LoadXml(szXml))
        {
            break;
        }

        if( m_tabControler )
        {
            m_tabControler->SetFocusColor(m_clrFocus);
        }

        CBkModule::GetInstance()->GetMessageLoop()->AddMessageFilter( this );

        WINDOWPOS WndPos = {0, 0, 0, 0, m_rcWindow.Width(), m_rcWindow.Height(), SWP_SHOWWINDOW};
        OnWindowPosChanged(&WndPos);

        BkWinThemeFunc::SetWindowTheme(m_hWnd, L"", L"");
        OnInitDialog();

    } while (FALSE);
    return m_hWnd;
}

inline void CBkDialog::Show( LPCTSTR szXml, HWND hOwner /* = ::GetActiveWindow */ )
{
    HWND hWnd = Create( szXml,hOwner );
    if( hWnd )
    {
        if( m_startPos.cx > 0 &&
            m_startPos.cy > 0 )
        {
            ::SetWindowPos(hWnd, HWND_TOP, m_startPos.cx, m_startPos.cy, 0, 0, SWP_NOSIZE );
        }
        else
        {
            __baseClass::CenterWindow();
        }

        ShowWindow( TRUE );
        SetForegroundWindow(hWnd);
        ::BringWindowToTop(hWnd);
        ::SetActiveWindow(hWnd);

        if( m_tabControler && m_tabDefault > 0 )
        {
            m_tabControler->SetCurrent( m_tabDefault, TRUE );
        }
    }
}

inline void CBkDialog::Hide()
{
    if( IsWindow() )
    {
        SetWindowPos( NULL,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_HIDEWINDOW );

        if( m_bLayerTrans && m_pTransRealLayer )
        {
            m_pTransRealLayer->SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE|SWP_HIDEWINDOW );
        }

        NotifyInvalidate();
    }
}

inline void CBkDialog::Move( POINT ptPos, BOOL bShow/* = TRUE*/ )
{
    if( IsWindow() )
    {
        DWORD dwStyle = bShow ? SWP_SHOWWINDOW : SWP_HIDEWINDOW;
        SetWindowPos( NULL, ptPos.x, ptPos.y, 0, 0, SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE|dwStyle );

        if( m_bLayerTrans && m_pTransRealLayer )
        {
            ::SetWindowPos(m_hWndRealContainer, NULL, ptPos.x, ptPos.y, 0, 0, SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE|dwStyle );
        }
        NotifyInvalidate();
    }
}

inline void CBkDialog::Close()
{
    if (IsWindow())
    {
        PostMessage(WM_CLOSE);
    }
}

inline HWND CBkDialog::GetSafeOwner(HWND hParent, HWND* pWndTop)
{
	// 获取起始窗口
	HWND hWnd = hParent;
	if (hWnd == NULL)
	{
		hWnd = GetActiveWindow();
	}

	// 向上查找直到第一个非子窗口为止
	while (hWnd != NULL && (::GetWindowLong(hWnd, GWL_STYLE) & WS_CHILD))
		hWnd = ::GetParent(hWnd);

	// 向上查找顶层窗口
	HWND hWndTop = hWnd, hWndTemp = hWnd;
	for (;;)
	{
		if (hWndTemp == NULL)
			break;
		else
			hWndTop = hWndTemp;
		hWndTemp = ::GetParent(hWndTop);
	}

	// 禁止并存储顶层窗口
	if (pWndTop != NULL)
	{
		if (hWndTop != NULL && ::IsWindowEnabled(hWndTop) && hWndTop != hWnd)
		{
			*pWndTop = hWndTop;
			::EnableWindow(hWndTop, FALSE);
		}
		else
			*pWndTop = NULL;
	}

	return hWnd; 
}


inline HWND CBkDialog::PreModal()
{
	// 查找父窗口和顶层窗口
	HWND hWnd = GetSafeOwner(m_hWndParent, &m_hWndTop);

	return hWnd;
}

inline void CBkDialog::PostModal()
{
	// 重置顶层窗口Enable状态
	if (::IsWindow(m_hWndTop))
		::EnableWindow(m_hWndTop, TRUE);
	m_hWndTop = NULL;
}

inline UINT CBkDialog::DoModal2(LPCTSTR szXml, HWND hWndParent /* = ::GetActiveWindow */)
{
	m_hWndParent = hWndParent;

	hWndParent = PreModal();
	UINT nRet = DoModal(szXml, hWndParent);
	PostModal();

	return nRet;
}

inline UINT CBkDialog::DoModal(LPCTSTR szXml, HWND hWndParent /* = ::GetActiveWindow */)
{
    BOOL bEnableParent = FALSE;

    if (NULL == hWndParent)
        hWndParent = BkWinManager::GetActive();

    if (hWndParent && hWndParent != ::GetDesktopWindow() && ::IsWindowEnabled(hWndParent))
    {
        ::EnableWindow(hWndParent, FALSE);
        bEnableParent = TRUE;
    }

    m_bExitModalLoop = FALSE;

    HWND hWnd = Create(szXml, hWndParent);
    if (!hWnd)
    {
        ::EnableWindow(hWndParent, TRUE);
        return 0;
    }

    HWND hWndLastActive = BkWinManager::SetActive(hWnd);
    CenterWindow();
    //::SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
    ShowWindow(TRUE);
    SetForegroundWindow(hWnd);
    ::BringWindowToTop(hWnd);
    ::SetActiveWindow(hWnd);
	::SetFocus(hWnd);

    if( m_tabControler && m_tabDefault > 0 )
    {
        m_tabControler->SetCurrent( m_tabDefault, TRUE );
    }

	GetAllModalHwnd().push_back(hWnd);
    _ModalMessageLoop();
	std::vector<HWND>::iterator it = std::find(GetAllModalHwnd().begin(), GetAllModalHwnd().end(), hWnd);
	if (it != GetAllModalHwnd().end())
		GetAllModalHwnd().erase(it);

    m_bExitModalLoop = FALSE;

    // From MFC
    // hide the window before enabling the parent, etc.

    if (IsWindow())
    {
        SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
    }

    if (bEnableParent)
    {
        ::EnableWindow(hWndParent, TRUE);
    }

	BOOL bActive = ::GetActiveWindow() == m_hWnd;
	if (IsWindow())
		DestroyWindow();

	if (hWndParent != NULL && bActive)
	{
        ::SetActiveWindow(hWndParent);
		::SetFocus(m_hWndParent);
	}

    BkWinManager::SetActive(hWndLastActive);

    return m_uRetCode;
}

inline void CBkDialog::EndModal(UINT uRetCode)
{
    m_uRetCode = uRetCode;
    m_bExitModalLoop = TRUE;
    ::PostThreadMessage(::GetCurrentThreadId(), WM_NULL, 0, 0);
}

inline void CBkDialog::_Clear()
{
    _ClearFocus( m_focusChild );
    _ClearDrag( m_pDragWnd );
    _ClearLastHover();

    if( m_tabControler )
    {
        m_tabControler->Clear();
        delete m_tabControler;
        m_tabControler = NULL;
    }

    if( IsWindow() )
    {
        DestroyWindow();
    }

    m_startPos.cx = m_startPos.cy = -1;

	if (m_pDropTarget)
	{
		delete m_pDropTarget;
		m_pDropTarget = NULL;
	}
}

inline BOOL CBkDialog::_LoadXml(LPCTSTR szRes)
{
    BOOL bRet = FALSE;
    do 
    {
        CStringA strXml;
        BOOL bRet2 = BkResManager::LoadResource(szRes, strXml);
        if (!bRet2)
            break;

        TiXmlDocument xmlDoc;
        xmlDoc.Parse(strXml, NULL, TIXML_ENCODING_UTF8);
        if (xmlDoc.Error())
        {
            break;
        }

        bRet = Load(xmlDoc.RootElement());
    } while (FALSE);
    return bRet;
}

inline void CBkDialog::_ModalMessageLoop()
{
    BOOL bRet;
    CMessageLoop* MsgLoop = CBkModule::GetInstance()->GetMessageLoop();
    for( ;; )
    {
        if (::PeekMessage(&MsgLoop->m_msg, NULL, 0, 0, PM_NOREMOVE))
        {
            if (WM_QUIT == MsgLoop->m_msg.message)
                break;
        }

        if (m_bExitModalLoop || NULL == m_hWnd || !::IsWindow(m_hWnd))
            break;

        bRet = ::GetMessage(&MsgLoop->m_msg, NULL, 0, 0);

        if (bRet == -1)
        {
            continue;   // error, don't process
        }
        else if (!bRet)
        {
            ATLTRACE(L"Why Receive WM_QUIT here?\r\n");
            break;   // WM_QUIT, exit message loop
        }

        if( !MsgLoop->PreTranslateMessage( &MsgLoop->m_msg ) )
        {
            ::TranslateMessage(&MsgLoop->m_msg);
            ::DispatchMessage(&MsgLoop->m_msg);
        }
    }
}

inline void CBkDialog::SetWndStyle(DWORD dwStyle)
{
	m_dwWndStyle = dwStyle;
}

inline HWND CBkDialog::_CreateWindow(HWND hWndParent)
{
    if (NULL != m_hWnd)
        return m_hWnd;

	// 外界指定创建样式则使用指定样式，否则用默认样式
	DWORD dwStyle = (m_dwWndStyle != 0) ? m_dwWndStyle : (WS_POPUP | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN);
    if (m_bResizable)
    {
        dwStyle |= WS_SIZEBOX;
    }
    DWORD dwExStyle  = m_bAppWin ? WS_EX_APPWINDOW : WS_EX_TOOLWINDOW;
    m_hWndParent = hWndParent;

    CRect rc = m_rcWindow;
    m_rcWindow.SetRect(0,0,0,0);
    HWND hWnd = __baseClass::Create(hWndParent, rc, m_strCaption, dwStyle, dwExStyle, 0U, NULL);
    if (hWnd != NULL)
    {
        if( !m_wndToolTip )
        {
            m_wndToolTip = new CToolTipCtrl;
        }

        m_wndToolTip->Create( hWnd, NULL, NULL, TTS_ALWAYSTIP|TTS_NOPREFIX );
        m_wndToolTip->SetMaxTipWidth( 200 );
        m_wndToolTip->Activate(TRUE);
    }

    return hWnd;
}

inline void CBkDialog::_ThrowFocusChild(LPPOINT pt)
{
    if (m_focusChild)
    {
        CRect rcFocusCtrl;
        m_focusChild->GetRect(&rcFocusCtrl);
        if (!rcFocusCtrl.PtInRect(*pt))
        {
            m_focusChild->KillFocus();
        }
    }
}

inline HBITMAP CBkDialog::_CreateCompatibleBitmap(CRect& rcClient)  
{  
    BYTE*                     pBits = NULL;   
    BITMAPINFOHEADER          bmih;   
    ZeroMemory( &bmih, sizeof(BITMAPINFOHEADER));   

    bmih.biSize                 = sizeof(BITMAPINFOHEADER) ;   
    bmih.biWidth                = rcClient.Width();   
    bmih.biHeight               = rcClient.Height();   
    bmih.biPlanes               = 1;   
    bmih.biBitCount             = 32;
    bmih.biCompression          = BI_RGB;   
    bmih.biSizeImage            = 0;   
    bmih.biXPelsPerMeter        = 0;   
    bmih.biYPelsPerMeter        = 0;   
    bmih.biClrUsed              = 0;   
    bmih.biClrImportant         = 0;   

    HBITMAP hBitMap = CreateDIBSection (NULL, (BITMAPINFO *)  &bmih, 0, (VOID**)&pBits, NULL, 0) ;  
    return hBitMap;  
}  

inline BOOL CBkDialog::RedrawRegion(CDCHandle& dc)
{
    return __bkClass::RedrawRegion(dc);

    //if(IsWindowEnabled())
    //{
    //    return __bkClass::RedrawRegion(dc);
    //}

    ////DrawDisableScreen(dc);
    //DrawDisableShade(dc);
    //return FALSE;
}

inline BOOL CBkDialog::PreTranslateMessage(MSG* pMsg)
{
    if( pMsg->message == WM_KEYDOWN )
    {
        if( ::IsChild(this->m_hWnd, pMsg->hwnd) && 
            !::SendMessage(pMsg->hwnd, WM_GETDLGCODE, pMsg->wParam, (LPARAM)pMsg) )
        {
            // 若控件不需要处理键盘消息，则转由窗口处理
            if( pMsg->wParam == VK_TAB || pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
            {
                pMsg->hwnd = this->m_hWnd;
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
                pMsg->hwnd = this->m_hWnd;
            }
        }
    }
    return FALSE;
}

inline void CBkDialog::_OnActivate(UINT nState, BOOL bMinimized, HWND wndOther)
{
    m_bActive ? SetFocus() : NULL;//KillFocus();
}

inline void CBkDialog::_OnClose()
{
	EndModal(-1);
    SetMsgHandled( FALSE );
}

inline BOOL CBkDialog::_OnDestroy()
{
    OnDestroy();

    if( m_bLayerTrans && m_pTransRealLayer )
    {
        if( m_pTransRealLayer->IsWindow() )
        {
            m_pTransRealLayer->DestroyWindow();
        }
        delete m_pTransRealLayer;
        m_pTransRealLayer = NULL;
    }

    CBkModule::GetInstance()->GetMessageLoop()->RemoveMessageFilter( this );

    TrackMouseEvent(FALSE);

    if( m_wndToolTip /*&& ::IsWindow(m_wndToolTip->m_hWnd)*/ )
    {
        //m_wndToolTip->DestroyWindow();
        //m_wndToolTip->m_hWnd = NULL;
        delete m_wndToolTip;
        m_wndToolTip = NULL;
    }

    return FALSE;
}

inline void CBkDialog::_OnEnable(BOOL bEnable)
{
    SetVisible(bEnable);

    if (m_pNotify)
    {
        NotifyInfo info = {WM_ENABLE, CPoint(0,0), this, bEnable};
        m_pNotify->Action(&info);
    }
    
    if (!bEnable)
        _InvalidateRect(m_rcWindow);
    else
        NotifyInvalidate();
}

inline BOOL CBkDialog::_OnEraseBkgnd(CDCHandle dc)
{
    return TRUE;
}

inline void CBkDialog::_OnGetMinMaxInfo(LPMINMAXINFO lpMMI)
{
    HMONITOR hMonitor = ::MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONULL);

    if (hMonitor)
    {
        MONITORINFO mi = {sizeof(MONITORINFO)};
        ::GetMonitorInfo(hMonitor, &mi);

        CRect rcWork = mi.rcWork, rcMonitor = mi.rcMonitor;
        lpMMI->ptMaxPosition.x = abs(rcWork.left - rcMonitor.left) - 3;
        lpMMI->ptMaxPosition.y = abs(rcWork.top - rcMonitor.top) - 3;
        lpMMI->ptMaxSize.x = abs(rcWork.Width()) + 6;
        lpMMI->ptMaxSize.y = abs(rcWork.Height()) + 6;
        lpMMI->ptMaxTrackSize.x = abs(rcWork.Width()) + 6;
        lpMMI->ptMaxTrackSize.y = abs(rcWork.Height()) + 6;
    }
}

inline BOOL CBkDialog::_OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (IsWindowEnabled())
    {
        if( nChar == VK_TAB )
        {
            if( GetKeyState(VK_SHIFT) < 0 )
            {
                if( m_tabControler )
                {
                    KillFocus();
                    m_tabControler->SetPrevious();
                }
            }
            else
            {
                if( m_tabControler )
                {
                    KillFocus();
                    m_tabControler->SetNext();
                }
            }
            return FALSE;
        }

        if( !__bkClass::OnKeyDown( nChar,nRepCnt,nFlags ) )
        {
            if( nChar == VK_RETURN )
            {
                if( m_tabControler )
                {
                    if( m_tabControler->GetAccept() && 
                        //m_tabControler->GetAccept()->GetRealWnd() == m_hWnd &&
                        m_tabControler->GetAccept() != m_focusChild )
                    {
                        CBkWnd* ctrl = dynamic_cast<CBkWnd*>( m_tabControler->GetAccept() );
                        if( ctrl )
                        {
                            ctrl->OnKeyDown( nChar, nRepCnt, nFlags );
                            return 0;
                        }
                    }
                }
            }
        }

        SetMsgHandled(FALSE);
    }
    return FALSE;
}


inline BOOL CBkDialog::_OnLButtonDown(UINT nFlags, CPoint point)
{
    _ThrowFocusChild(&point);

    __bkClass::OnLButtonDown(nFlags, point);
    if (m_pDragWnd != NULL && !m_bCaptureSetted)
    {
        __baseClass::SetCapture();
        m_bCaptureSetted = TRUE;
    }

	if (m_pDropTarget)
		m_pDropTarget->PreDragDrop(point);

    return TRUE;
}

inline BOOL CBkDialog::_OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_pDropTarget)
		m_pDropTarget->UnPreDragDrop();

    __bkClass::OnLButtonUp(nFlags, point,TRUE);
    if (m_bCaptureSetted)
    {
        ReleaseCapture();
        m_bCaptureSetted = FALSE;
    }
    return TRUE;
}

inline BOOL CBkDialog::_OnRButtonDown(UINT nFlags, CPoint point)
{
    _ThrowFocusChild(&point);
    return __bkClass::OnRButtonDown(nFlags, point);
}

inline BOOL CBkDialog::_OnRButtonUp(UINT nFlags, CPoint point)
{
    return __bkClass::OnRButtonUp(nFlags, point,TRUE);
}

inline BOOL CBkDialog::_OnLButtonDblClk(UINT nFlags, CPoint point)
{
    return __bkClass::OnLButtonDblClk(nFlags, point);
}

inline BOOL CBkDialog::_OnMouseMove(UINT nFlags, CPoint point)
{
    return __bkClass::OnMouseMove(nFlags, point);
}

inline BOOL CBkDialog::_OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    ScreenToClient(&pt);
    return __bkClass::OnMouseWheel(nFlags, zDelta, pt);
}

inline void CBkDialog::_OnMove(CPoint ptPos)
{
    SetMsgHandled( FALSE );
    if( m_bLayerTrans && m_pTransRealLayer )
    {
        ::SetWindowPos(m_hWndRealContainer, NULL, ptPos.x, ptPos.y, 0, 0, SWP_NOZORDER|SWP_NOSIZE );
    }
}

inline BOOL CBkDialog::_OnNcActivate(BOOL bActive)
{
    m_bActive = bActive;
    return TRUE;
}

inline BOOL CBkDialog::_OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
{
    if (bCalcValidRects)
    {
        CRect rcWindow;
		
		if(((DWORD)::GetWindowLong(m_hWnd, GWL_STYLE) & WS_CHILD) == 0)
		{
			GetWindowRect(rcWindow);
		}
		else
		{
			GetClientRect(&rcWindow);
		}

        LPNCCALCSIZE_PARAMS pParam = (LPNCCALCSIZE_PARAMS)lParam;

        if (SWP_NOSIZE == (SWP_NOSIZE & pParam->lppos->flags))
            return 0;

        if (0 == (SWP_NOMOVE & pParam->lppos->flags))
        {
            rcWindow.left = pParam->lppos->x;
            rcWindow.top = pParam->lppos->y;
        }

        rcWindow.right = rcWindow.left + pParam->lppos->cx;
        rcWindow.bottom = rcWindow.top + pParam->lppos->cy;
        pParam->rgrc[0] = rcWindow;
        pParam->rgrc[1] = pParam->rgrc[0];
    }

    return 0;
}

inline void CBkDialog::_OnNcDestroy()
{
    SetMsgHandled( FALSE );
    LRESULT lRet = DefWindowProc( WM_NCDESTROY , NULL , NULL );
}

inline UINT CBkDialog::_OnNcHitTest(CPoint point)
{
    ScreenToClient(&point);

    if (m_bResizable && !IsZoomed())
    {
        CRect rcLeft = m_rcWindow;
        rcLeft.top   += BORDER_WIDTH;
        rcLeft.bottom  -= BORDER_WIDTH;
        rcLeft.right = rcLeft.left + BORDER_WIDTH;
        if (rcLeft.PtInRect(point))
        {
            return HTLEFT;
        }
        CRect rcRight = m_rcWindow;
        rcRight.top   += BORDER_WIDTH;
        rcRight.bottom  -= BORDER_WIDTH;
        rcRight.left = rcRight.right - BORDER_WIDTH;
        if (rcRight.PtInRect(point))
        {
            return HTRIGHT;
        }
        CRect rcTop = m_rcWindow;
        rcTop.left   += BORDER_WIDTH;
        rcTop.right  -= BORDER_WIDTH;
        rcTop.bottom = rcTop.top + BORDER_WIDTH;
        if (rcTop.PtInRect(point))
        {
            return HTTOP;
        }
        CRect rcBottom = m_rcWindow;
        rcBottom.left   += BORDER_WIDTH;
        rcBottom.right  -= BORDER_WIDTH;
        rcBottom.top = rcBottom.bottom - BORDER_WIDTH;
        if (rcBottom.PtInRect(point))
        {
            return HTBOTTOM;
        }
        CRect rcTopLeft = m_rcWindow;
        rcTopLeft.right   = rcTopLeft.left + CORNER_WIDTH;
        rcTopLeft.bottom  = rcTopLeft.top + CORNER_WIDTH;
        if (rcTopLeft.PtInRect(point))
        {
            return HTTOPLEFT;
        }
        CRect rcTopRight = m_rcWindow;
        rcTopRight.left   = rcTopRight.right - CORNER_WIDTH;
        rcTopRight.bottom  = rcTopRight.top + CORNER_WIDTH;
        if (rcTopRight.PtInRect(point))
        {
            return HTTOPRIGHT;
        }
        CRect rcBottomLeft = m_rcWindow;
        rcBottomLeft.right   = rcBottomLeft.left + CORNER_WIDTH;
        rcBottomLeft.top  = rcBottomLeft.bottom - CORNER_WIDTH;
        if (rcBottomLeft.PtInRect(point))
        {
            return HTBOTTOMLEFT;
        }
        CRect rcRightLeft = m_rcWindow;
        rcRightLeft.left   = rcTopRight.right - CORNER_WIDTH;
        rcRightLeft.top  = rcRightLeft.bottom - CORNER_WIDTH;
        if (rcRightLeft.PtInRect(point))
        {
            return HTBOTTOMRIGHT;
        }
    }
    return HTCLIENT;
}

inline BOOL CBkDialog::_OnNcLButtonDblClk(UINT nFlags, CPoint point)
{
    if (m_bResizable && nFlags == HTCAPTION)
    {
        if (__baseClass::GetStyle() & WS_MAXIMIZE)
            PostMessage(WM_SYSCOMMAND, SC_RESTORE | HTCAPTION, 0);
        else
            PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE | HTCAPTION, 0);
    }

    return TRUE;
}

inline BOOL CBkDialog::_OnPaint(HDC)
{
    //DPT_FUN;
    CPaintDC dc(m_hWnd);
    //if (m_bInvalid)
    //{
    //    BkWinManager::EnterPaintLock();

    //    HFONT hftOld = m_dcDlg.SelectFont(BkFontPool::GetFont(BKF_DEFAULTFONT));

    //    RedrawRegion(CDCHandle(m_dcDlg));

    //    m_dcDlg.SelectFont(hftOld);
    //    m_bInvalid = FALSE;

    //    BkWinManager::LeavePaintLock();
    //    m_dwPrePaintTime = timeGetTime();
    //}
    //::BitBlt(dc.m_hDC, 0, 0, m_rcWindow.Width(), m_rcWindow.Height(), m_dcDlg.m_hDC, 0, 0, SRCCOPY);

    if (m_bInvalid || m_alpha < 256)
    {
        BkWinManager::EnterPaintLock();

        CRect rc;
        GetClientRect( &rc );

        HDC hDCMem = ::CreateCompatibleDC( dc.m_hDC );
        HBITMAP hBitmapMem = _CreateCompatibleBitmap(rc);//::CreateCompatibleBitmap( dc.m_hDC, rc.Width(), rc.Height() );
        HBITMAP hBitmapOld = (HBITMAP)::SelectObject( hDCMem, hBitmapMem );
        ::SetBkMode( hDCMem, TRANSPARENT );

        HFONT hftOld = m_dcDlg.SelectFont(BkFontPool::GetFont(BKF_DEFAULTFONT));
        RedrawRegion(CDCHandle( (m_alpha<256)?hDCMem:m_dcDlg) );
        m_dcDlg.SelectFont(hftOld);
        m_bInvalid = FALSE;

        if( m_alpha < 256 )
        {
            CRect rectDlg;
            ::GetWindowRect(m_hWnd , &rectDlg);

            CPoint ptWindowScreenPosition(rectDlg.TopLeft());
            CSize szWindow(rectDlg.right - rectDlg.left, rectDlg.bottom - rectDlg.top);
            CPoint ptSrc(0,0); 
            BLENDFUNCTION blendPixelFunction= { AC_SRC_OVER, 0, m_alpha, AC_SRC_ALPHA };
            BOOL bRet= ::UpdateLayeredWindow(m_hWnd, dc, &ptWindowScreenPosition, &szWindow, hDCMem, &ptSrc, 0, &blendPixelFunction, ULW_ALPHA);
        }
        //else
        //{
        //    ::BitBlt(dc.m_hDC, 0, 0, m_rcWindow.Width(), m_rcWindow.Height(), m_dcDlg.m_hDC, 0, 0, SRCCOPY);
        //}

        ::SelectObject( hDCMem, hBitmapOld );
        ::DeleteObject( hBitmapMem );
        ::DeleteDC( hDCMem );

        BkWinManager::LeavePaintLock();
        m_dwPrePaintTime = timeGetTime();
    }

    if(m_alpha >= 256)
    {
        ::BitBlt(dc.m_hDC, 0, 0, m_rcWindow.Width(), m_rcWindow.Height(), m_dcDlg.m_hDC, 0, 0, SRCCOPY);
    }

    return FALSE;
}

inline void CBkDialog::_InvalidateRect(CRect& rcInvalidate)
{
    //if (IsWindowVisible())
    {
        if (!m_bInvalid)
        {
            CRgn rgn;
            rgn.CreateRectRgnIndirect(rcInvalidate);
            m_dcDlg.SelectClipRgn(rgn);
        }
        else
        {
            CRgn rgn;
            rgn.CreateRectRgnIndirect(rcInvalidate);
            m_dcDlg.SelectClipRgn(rgn, RGN_OR);
        }

        m_bInvalid = TRUE;
        InvalidateRect(rcInvalidate, FALSE);

        DWORD dwCurTime = timeGetTime();
        if (dwCurTime > m_dwPrePaintTime + 100)
        {
            m_dwPrePaintTime = dwCurTime;
            PostMessage(WM_PAINT);
        }
    }
}

inline BOOL CBkDialog::_OnSetCursor(CWindow wnd, UINT nHitTest, UINT message)
{
    if (nHitTest == 1 && m_lastHoverChild != NULL)
    {
        m_lastHoverChild->OnSetCursor();
    }
    else
    {
        LPARAM lParam = nHitTest;
        lParam |= message<<16;
        DefWindowProc(WM_SETCURSOR, (WPARAM)(HWND)wnd, lParam);
    }
    return TRUE;
}

inline void CBkDialog::_OnShowWindow(BOOL bShow, UINT nStatus)
{
    SetMsgHandled(FALSE);

    if( m_bLayerTrans && m_pTransRealLayer )
    {
        DWORD dwStyle = bShow ? SWP_SHOWWINDOW : SWP_HIDEWINDOW;
        m_pTransRealLayer->SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE|dwStyle );
    }

    OnShown( bShow );
}

inline void CBkDialog::_OnSize(UINT nType, CSize size)
{
    if (0 == size.cx || 0 == size.cy)
        return;

    if (nType != SIZE_MINIMIZED)
    {
        if (m_rcWindow.Width() != size.cx || m_rcWindow.Height() != size.cy || m_MemDC.m_hDC == NULL)
        {
            m_bInvalid |= m_dcDlg.Create(NULL, 0, 0, size.cx, size.cy);

            WINDOWPOS WndPos = {0, 0, 0, 0, size.cx, size.cy, SWP_SHOWWINDOW};
            OnWindowPosChanged(&WndPos);
        }
    }

    if (m_style.m_nRound > 0)
    {
        CRgn rgn;
        rgn.CreateRoundRectRgn(0, 0, m_rcWindow.Width()+1, m_rcWindow.Height()+1, m_style.m_nRound, m_style.m_nRound);
        SetWindowRgn(rgn, TRUE);
    }
}

inline LRESULT CBkDialog::_OnSizing(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LPRECT lpRc = (LPRECT)lParam;
    if (lpRc->right - lpRc->left < m_minSize.cx)
    {
        if (wParam == WMSZ_LEFT || wParam == WMSZ_TOPLEFT || wParam == WMSZ_BOTTOMLEFT)
        {
            lpRc->left = lpRc->right - m_minSize.cx;
        }
        else
        {
            lpRc->right = lpRc->left + m_minSize.cx;
        }
    }
    if (lpRc->bottom - lpRc->top < m_minSize.cy)
    {
        if (wParam == WMSZ_TOP || wParam == WMSZ_TOPLEFT || wParam == WMSZ_TOPRIGHT)
        {
            lpRc->top = lpRc->bottom - m_minSize.cy;
        }
        else
        {
            lpRc->bottom = lpRc->top + m_minSize.cy;
        }
    }
    return 0L;
}

inline LRESULT CBkDialog::_OnToolTipEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if( uMsg == WM_MOUSEMOVE ||
        uMsg == WM_LBUTTONDOWN ||
        uMsg == WM_LBUTTONUP ||
        uMsg == WM_RBUTTONDOWN ||
        uMsg == WM_MBUTTONDOWN ||
        uMsg == WM_RBUTTONUP ||
        uMsg == WM_MBUTTONUP )
    {
        if( m_wndToolTip && m_wndToolTip->IsWindow())
        {
            MSG msg = { m_hWnd, uMsg, wParam, lParam };
            m_wndToolTip->RelayEvent(&msg);
        }
    }

    SetMsgHandled(FALSE);

    return 0;
}

inline LRESULT CBkDialog::_OnDeleteThis(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    HBKWND hBkWndParent = (HBKWND)wParam;
    HBKWND hBkWnd = (HBKWND)lParam;
    if( hBkWnd )
    {
        CBkWnd* pBkWndDel = BkWnds::GetWindow(hBkWnd);
        if( pBkWndDel )
        {
            pBkWndDel->OnDestroy();
            delete pBkWndDel;
            pBkWndDel = NULL;
        }
    }
    return 0;
}

inline LRESULT CBkDialog::_OnEffectAnimateTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    UINT nEffectID = (UINT)lParam;
    CBkEffectObject* pEffectObject = BkEffect::GetEffect(nEffectID);
    if (pEffectObject)
    {
        pEffectObject->UpdateFrame(m_hWnd, (UINT_PTR)pEffectObject);
    }
    return TRUE;
}

inline LRESULT CBkDialog::_OnBKINMInvalidateRect(LPNMHDR pnmh)
{
    BKINMINVALIDATERECT* pnms = (BKINMINVALIDATERECT*)pnmh;
    CRect rcWnd = pnms->rect;
    _InvalidateRect(rcWnd);
    return 0l;
}

inline LRESULT CBkDialog::_OnBKINMUpdateDragItem(LPNMHDR pnmh)
{
    BkInMesUpdateDragItem* pInfo = (BkInMesUpdateDragItem*)pnmh;
    if (pInfo->oper)
    {
        m_pDragWnd = pInfo->pBkWnd;
    }
    else
    {
        pInfo->pBkWnd = m_pDragWnd;
    }
    return 0l;
}

inline BOOL CBkDialog::_IsWinXPAndLater()
{
    DWORD dwVersion = GetVersion();
    DWORD dwMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
    DWORD dwMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));

    if (dwMajorVersion > 5)         // Vista and later
        return TRUE;
    else if (dwMajorVersion < 5)    // 98/NT4 and before
        return FALSE;
    else if (dwMinorVersion > 0)    // XP and 2003
        return TRUE;
    else                            // 2000
        return FALSE;
}

//inline ATL::CWndClassInfo& CBkDialog::GetWndClassInfo()
//{
//    static ATL::CWndClassInfo wc = {
//        { sizeof(WNDCLASSEX), 
//        CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS /*| (_IsWinXPAndLater() ? CS_DROPSHADOW : 0)*/, 
//        StartWindowProc, 0, 0, NULL, NULL, NULL, 
//        (HBRUSH)(COLOR_WINDOW + 1), NULL, NULL, NULL },
//        NULL, NULL, IDC_ARROW, TRUE, 0, NULL
//    };
//    return wc;
//}

inline void CBkDialog::CreateHandle( HWND hParent )
{
    HWND hWnd = _CreateWindow( hParent );
    SetContainer( hWnd );

    if( m_alpha < 256 && m_alpha >= 0 )
    {
        DWORD dwExStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
        if ((dwExStyle & WS_EX_LAYERED) != WS_EX_LAYERED)
            ::SetWindowLong(m_hWnd, GWL_EXSTYLE, dwExStyle^WS_EX_LAYERED);
    }

    if( m_bLayerTrans )
    {
        m_pTransRealLayer = new CBkTransRealLayer( m_clrTransRealLayer, m_bLayerActive );
        HWND hWndRealContainer = m_pTransRealLayer->CreateTransLayer(hWnd);
        SetRealContainer( hWndRealContainer );
    }
}

inline void CBkDialog::SetFocus()
{
    if( m_focusChild )
    {
        m_focusChild->SetFocus();
    }
}

inline void CBkDialog::KillFocus()
{
    if( m_focusChild )
    {
        m_focusChild->KillFocus();
		m_focusChild = NULL;
    }
}

inline CBkWnd* CBkDialog::OnSetFocus( CBkWnd* which )
{
    if( m_focusChild != which )
    {
        if( m_focusChild )
        {
            m_focusChild->KillFocus();
        }
        m_focusChild = which;

        if( m_tabControler )
        {
            m_tabControler->SetCurrent( m_focusChild );
        }
    }
    return m_focusChild;
}

inline BOOL CBkDialog::OnWindowPosChanged(LPWINDOWPOS lpWndPos)
{
    m_rcWindow.SetRect(lpWndPos->x, lpWndPos->y, lpWndPos->x + lpWndPos->cx, lpWndPos->y + lpWndPos->cy);
    m_bResize = TRUE;
    RepositionChilds();
    if( m_bLayerTrans && m_pTransRealLayer )
    {
        ::SetWindowPos(m_hWndRealContainer, NULL, 0, 0, lpWndPos->cx, lpWndPos->cy, SWP_NOZORDER|SWP_NOMOVE );
    }
    _InvalidateRect(m_rcWindow);
    return FALSE;
}

inline void CBkDialog::OnInitDialog()
{
	m_pDropTarget->Register(m_hWnd);
}

inline void CBkDialog::OnFreeDialog()
{
}

inline void CBkDialog::OnFinalMessage(HWND hWnd)
{
    OnFreeDialog();
}

inline void CBkDialog::OnShown(BOOL bShow)
{

}

inline HWND CBkDialog::GetRealWnd()
{
    return m_hWnd;
}

inline BOOL CBkDialog::CloseTopModalHwnd()
{
	std::vector<HWND> hwndSet = GetAllModalHwnd();
	if (hwndSet.size() > 0)
	{
		SendMessage(hwndSet.back(), WM_CLOSE, 0, 0);
		return TRUE;
	}

	return FALSE;
};

inline std::vector<HWND>& CBkDialog::GetAllModalHwnd()
{
	static std::vector<HWND> hwndSet;
	return hwndSet;
}

inline void CBkDialog::DrawDisableScreen(CDCHandle& dc)
{
    CRect rcWindow, rcClient;
    GetWindowRect(&rcWindow);
    GetClientRect(&rcClient);

    HDC hdc = ::GetDC(m_hWnd);
    //HDC hdc = CreateDC(L"DISPLAY", NULL, NULL, NULL);
    HDC hNewdc = CreateCompatibleDC(hdc);
    HBITMAP hBmp = CreateCompatibleBitmap(hdc, rcClient.Width(), rcClient.Height());
    HBITMAP hOld = (HBITMAP)SelectObject(hNewdc, hBmp);
    //::BitBlt(hNewdc, 0, 0, rcWindow.Width(), rcWindow.Height(), hdc, rcWindow.left, rcWindow.top, SRCCOPY);
    //::BitBlt(hNewdc, 0, 0, rcWindow.Width(), rcWindow.Height(), hdc, 0, 0, SRCCOPY);
    //::BitBlt(dc.m_hDC, rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), hdc, 0, 0, SRCCOPY);
    DrawDisableShade(dc);

    SelectObject(hNewdc, hOld);
    DeleteDC(hNewdc);
    //DeleteDC(hdc);
    ::ReleaseDC(m_hWnd, hdc);
}

inline HRESULT CBkDialog::OnDragEnter(IDataObject *pDataObj, POINT pt, DWORD *pdwEffect)
{
    ::ScreenToClient(m_hWnd, &pt);
	__bkClass::OnDragEnter(pDataObj,pt,pdwEffect);
	return S_OK;
}

inline HRESULT CBkDialog::OnDragOver(POINT pt, DWORD *pdwEffect)
{
    ::ScreenToClient(m_hWnd, &pt);
	__bkClass::OnMouseMove(0,CPoint(pt));
	__bkClass::OnDragOver(pt,pdwEffect);
	return S_OK;
}

inline HRESULT CBkDialog::OnDrop(IDataObject *pDataObj, POINT pt, DWORD *pdwEffect)
{
    ::ScreenToClient(m_hWnd, &pt);
	__bkClass::OnDrop(pDataObj,pt,pdwEffect);

	if (m_pDropTarget)
		__bkClass::OnLButtonUp(0,pt,FALSE);

	return S_OK;
}

inline HRESULT CBkDialog::OnDragLeave(void)
{
	if (m_pDropTarget)
		__bkClass::OnLButtonUp(0,m_pDropTarget->GetLeavePoint(),FALSE);

	__bkClass::OnDragLeave();
	return S_OK;
}