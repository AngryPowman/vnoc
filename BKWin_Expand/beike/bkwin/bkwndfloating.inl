inline CBkFloatingWindow::CBkFloatingWindow()
{
    m_hWndParent = NULL;
    m_bChild = FALSE;
    m_bCaptureSetted = FALSE;
    m_bActive = FALSE;
    m_bAutoClose = FALSE;
    m_bAutoHide = FALSE;
    m_tabDefault = 0;
    m_clrFocus = RGB(0,0,255);
    m_alpha = 256;
    m_bLayerTrans = FALSE;
    m_bLayerActive = FALSE;
    m_pTransRealLayer = NULL;
    m_clrTransRealLayer = RGB(255,255,255);
    m_dwPrePaintTime = 0;
    m_bInvalid = TRUE;
	m_pDragWnd = NULL;
}

inline CBkFloatingWindow::~CBkFloatingWindow()
{
	_Clear();

	if( m_wndToolTip )
    {
        if ( m_wndToolTip->IsWindow() )
        {
            m_wndToolTip->DestroyWindow();
            m_wndToolTip->m_hWnd = NULL;
        }

        delete m_wndToolTip;
        m_wndToolTip = NULL;
    }
}

inline IBkWnd* CBkFloatingWindow::GetItem(UINT uItemID) const
{
    return FindChildByCmdID(uItemID);
}

inline void CBkFloatingWindow::ShowWindow(BOOL bShow)
{
    if( ::IsWindow(m_hWnd) )
    {
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
        OnShown(bShow);
    }
}

inline HWND CBkFloatingWindow::Create(TiXmlElement* pTiXmlElem, HWND hWndParent /*= ::GetActiveWindow()*/)
{
    _Clear();

    do 
    {
        SetContainer(hWndParent);

        if( !m_tabControler )
        {
            m_tabControler = new CBkTaborder(this);
        }

        if (!Load(pTiXmlElem))
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

inline HWND CBkFloatingWindow::Create(LPCTSTR szXml, HWND hWndParent /* = ::GetActiveWindow */)
{
    _Clear();

    do 
    {
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

inline void CBkFloatingWindow::Show( LPCTSTR szXml, BOOL bShow /*= TRUE*/, HWND hOwner /* = ::GetActiveWindow */ )
{
    HWND hWnd = Create( szXml,hOwner );
    if( hWnd )
    {
        ShowWindow( bShow );
    }
}

inline void CBkFloatingWindow::Hide()
{
    if( IsWindow() )
    {
        SetWindowPos( NULL,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_HIDEWINDOW|SWP_NOACTIVATE );

        if( m_bLayerTrans && m_pTransRealLayer )
        {
            m_pTransRealLayer->SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE|SWP_HIDEWINDOW );
        }

        NotifyInvalidate();
        OnShown(FALSE);
    }
}

inline void CBkFloatingWindow::Move( POINT ptPos, BOOL bShow/* = TRUE*/ )
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

inline void CBkFloatingWindow::Close()
{
    if (IsWindow())
    {
        PostMessage(WM_CLOSE);
    }
}

inline void CBkFloatingWindow::_Clear()
{
    _ClearFocus( m_focusChild );
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
        m_hWnd = NULL;
    }
}

inline BOOL CBkFloatingWindow::_LoadXml(LPCTSTR szRes)
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

inline HWND CBkFloatingWindow::_CreateWindow(HWND hWndParent)
{
    if (NULL != m_hWnd)
        return m_hWnd;

    DWORD dwStyle = (m_bChild?WS_CHILD:WS_POPUP)|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;//NULL;
    DWORD dwExStyle  = WS_EX_NOACTIVATE;
    m_hWndParent = hWndParent;

    CRect rc = m_rcWindow;
    m_rcWindow.SetRect(0,0,0,0);
    HWND hWnd = __baseClass::Create(hWndParent, rc, m_strCaption, dwStyle, dwExStyle, 0U, NULL);
    if (hWnd != NULL)
    {
        if( m_wndToolTip )
        {
            if ( m_wndToolTip->IsWindow() )
            {
                m_wndToolTip->DestroyWindow();
                m_wndToolTip->m_hWnd = NULL;
            }

            delete m_wndToolTip;
            m_wndToolTip = NULL;
        }

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

inline void CBkFloatingWindow::_InvalidateRect(CRect& rcInvalidate)
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
            //SendMessage(WM_PAINT);
        }
    }
}

inline HBITMAP CBkFloatingWindow::_CreateCompatibleBitmap(CRect& rcClient)  
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

inline BOOL CBkFloatingWindow::PreTranslateMessage(MSG* pMsg)
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

inline void CBkFloatingWindow::CreateHandle( HWND hParent )
{
    HWND hWnd = _CreateWindow( hParent );
    SetContainer( hWnd );

    if( m_bLayerTrans )
    {
        m_pTransRealLayer = new CBkTransRealLayer( m_clrTransRealLayer, m_bLayerActive );
        HWND hWndRealContainer = m_pTransRealLayer->CreateTransLayer(hWnd);
        SetRealContainer( hWndRealContainer );
    }
}

inline void CBkFloatingWindow::SetFocus()
{
    if( m_focusChild )
    {
        m_focusChild->SetFocus();
    }
}

inline void CBkFloatingWindow::KillFocus()
{
    if( m_focusChild )
    {
        m_focusChild->KillFocus();
    }
}

inline CBkWnd* CBkFloatingWindow::OnSetFocus( CBkWnd* which )
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

inline void CBkFloatingWindow::OnChildLoad(TiXmlElement* pTiXmlElem, CBkWnd* pChildWindow)
{
}

inline BOOL CBkFloatingWindow::OnWindowPosChanged(LPWINDOWPOS lpWndPos)
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

inline void CBkFloatingWindow::OnInitDialog()
{
}

inline void CBkFloatingWindow::OnFreeDialog()
{
}

inline void CBkFloatingWindow::OnFinalMessage(HWND hWnd)
{
    OnFreeDialog();
}

inline void CBkFloatingWindow::OnShown(BOOL bShow)
{
}

inline void CBkFloatingWindow::_OnActivate(UINT nState, BOOL bMinimized, HWND wndOther)
{
    m_bActive ? SetFocus() : KillFocus();
}

inline int CBkFloatingWindow::_OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    ModifyStyle( WS_SIZEBOX|WS_MAXIMIZEBOX|WS_SYSMENU|WS_BORDER|WS_MAXIMIZE|WS_MINIMIZE,0,1 );
    return FALSE;
}

inline BOOL CBkFloatingWindow::_OnDestroy()
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

	if (CBkModule::GetInstance()->GetMessageLoop())
	{
		CBkModule::GetInstance()->GetMessageLoop()->RemoveMessageFilter( this );
	}

	TrackMouseEvent(FALSE);

    if( m_wndToolTip )
    {
        if( m_wndToolTip->IsWindow() )
        {
            m_wndToolTip->DestroyWindow();
            m_wndToolTip->m_hWnd = NULL;
        }
        delete m_wndToolTip;
        m_wndToolTip = NULL;
    }

    return FALSE;
}

inline BOOL CBkFloatingWindow::_OnEraseBkgnd(CDCHandle dc)
{
    return TRUE;
}

inline BOOL CBkFloatingWindow::_OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if( IsWindowEnabled() )
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

inline void CBkFloatingWindow::_OnKillFocus( HWND pNewWnd )
{
    if( m_bAutoHide )
    {
        Hide();
    }

    if( m_bAutoClose )
    {
        Close();
    }
}

inline BOOL CBkFloatingWindow::_OnLButtonDown(UINT nFlags, CPoint point)
{
    if (!m_bCaptureSetted)
    {
        __baseClass::SetCapture();
        m_bCaptureSetted = TRUE;
    }
    return __bkClass::OnLButtonDown(nFlags, point);
}

inline BOOL CBkFloatingWindow::_OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_bCaptureSetted)
    {
        ReleaseCapture();
        m_bCaptureSetted = FALSE;
    }
    return __bkClass::OnLButtonUp(nFlags, point);
}

inline BOOL CBkFloatingWindow::_OnRButtonDown(UINT nFlags, CPoint point)
{
    return __bkClass::OnRButtonDown(nFlags, point);
}

inline BOOL CBkFloatingWindow::_OnRButtonUp(UINT nFlags, CPoint point)
{
    return __bkClass::OnRButtonUp(nFlags, point);
}

inline BOOL CBkFloatingWindow::_OnLButtonDblClk(UINT nFlags, CPoint point)
{
    return __bkClass::OnLButtonDblClk(nFlags, point);
}

inline BOOL CBkFloatingWindow::_OnMouseMove(UINT nFlags, CPoint point)
{
    return __bkClass::OnMouseMove(nFlags, point);
}

inline BOOL CBkFloatingWindow::_OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    ScreenToClient(&pt);
    return __bkClass::OnMouseWheel(nFlags, zDelta, pt);
}

inline void CBkFloatingWindow::_OnMove(CPoint ptPos)
{
    SetMsgHandled( FALSE );
    if( m_bLayerTrans && m_pTransRealLayer )
    {
        ::SetWindowPos(m_hWndRealContainer, NULL, ptPos.x, ptPos.y, 0, 0, SWP_NOZORDER|SWP_NOSIZE );
    }
}

inline BOOL CBkFloatingWindow::_OnNcActivate(BOOL bActive)
{
    m_bActive = bActive;
    return TRUE;
}

inline BOOL CBkFloatingWindow::_OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
{
    if (bCalcValidRects)
    {
        CRect rcWindow;

        GetWindowRect(rcWindow);

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

inline UINT CBkFloatingWindow::_OnNcHitTest(CPoint point)
{
    return HTCLIENT;
}

inline BOOL CBkFloatingWindow::_OnPaint(HDC hdc)
{
    CPaintDC dc(m_hWnd);

    //CRect rc;
    //GetClientRect( &rc );

    //HDC hDCMem = ::CreateCompatibleDC( dc.m_hDC );
    //HBITMAP hBitmapMem = ::CreateCompatibleBitmap( dc.m_hDC, rc.Width(), rc.Height() );
    //HBITMAP hBitmapOld = (HBITMAP)::SelectObject( hDCMem, hBitmapMem );
    //::SetBkMode( hDCMem, TRANSPARENT );

    //if (m_bInvalid)
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
            DWORD dwExStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
            if ((dwExStyle & WS_EX_LAYERED) != WS_EX_LAYERED)
                ::SetWindowLong(m_hWnd, GWL_EXSTYLE, dwExStyle^WS_EX_LAYERED);

            CRect rectDlg;
            ::GetWindowRect(m_hWnd , &rectDlg);

            CPoint ptWindowScreenPosition(rectDlg.TopLeft());
            CSize szWindow(rectDlg.right - rectDlg.left, rectDlg.bottom - rectDlg.top);
            CPoint ptSrc(0,0); 
            BLENDFUNCTION blendPixelFunction= { AC_SRC_OVER, 0, m_alpha, AC_SRC_ALPHA };
            BOOL bRet= ::UpdateLayeredWindow(m_hWnd, dc, &ptWindowScreenPosition, &szWindow, hDCMem, &ptSrc, 0, &blendPixelFunction, ULW_ALPHA);
        }
        else
        {
            ::BitBlt(dc.m_hDC, 0, 0, m_rcWindow.Width(), m_rcWindow.Height(), m_dcDlg.m_hDC, 0, 0, SRCCOPY);
        }

        ::SelectObject( hDCMem, hBitmapOld );
        ::DeleteObject( hBitmapMem );
        ::DeleteDC( hDCMem );

        BkWinManager::LeavePaintLock();
        m_dwPrePaintTime = timeGetTime();
    }

    //if( m_alpha < 256 )
    //{
    //    DWORD dwExStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
    //    if ((dwExStyle & WS_EX_LAYERED) != WS_EX_LAYERED)
    //        ::SetWindowLong(m_hWnd, GWL_EXSTYLE, dwExStyle^WS_EX_LAYERED);

    //    CRect rectDlg;
    //    ::GetWindowRect(m_hWnd , &rectDlg);

    //    CPoint ptWindowScreenPosition(rectDlg.TopLeft());
    //    CSize szWindow(rectDlg.right - rectDlg.left, rectDlg.bottom - rectDlg.top);
    //    CPoint ptSrc(0,0); 
    //    BLENDFUNCTION blendPixelFunction= { AC_SRC_OVER, 0, m_alpha, AC_SRC_ALPHA };
    //    BOOL bRet= ::UpdateLayeredWindow(m_hWnd, dc, &ptWindowScreenPosition, &szWindow, hDCMem, &ptSrc, 0, &blendPixelFunction, ULW_ALPHA);
    //}
    //else
    //{
    //    ::BitBlt(dc.m_hDC, 0, 0, m_rcWindow.Width(), m_rcWindow.Height(), m_dcDlg.m_hDC, 0, 0, SRCCOPY);
    //}

    //::SelectObject( hDCMem, hBitmapOld );
    //::DeleteObject( hBitmapMem );
    //::DeleteDC( hDCMem );

    return FALSE;
}

inline BOOL CBkFloatingWindow::_OnSetCursor(CWindow wnd, UINT nHitTest, UINT message)
{
	if (nHitTest == 1&&m_lastHoverChild != NULL)
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

inline void CBkFloatingWindow::_OnShowWindow(BOOL bShow, UINT nStatus)
{
    SetMsgHandled(FALSE);

    if( m_bLayerTrans && m_pTransRealLayer )
    {
        DWORD dwStyle = bShow ? SWP_SHOWWINDOW : SWP_HIDEWINDOW;
        m_pTransRealLayer->SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE|dwStyle );
    }

    if( nStatus == 0 )
    {
        OnShown( bShow );
    }
}

inline void CBkFloatingWindow::_OnSize(UINT nType, CSize size)
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

            if (m_style.m_nRound > 0)
            {
                CRgn rgn;
                rgn.CreateRoundRectRgn(0, 0, m_rcWindow.Width() + 1, m_rcWindow.Height() + 1, m_style.m_nRound, m_style.m_nRound);
                SetWindowRgn(rgn, TRUE);
            }
        }
    }

}

inline LRESULT CBkFloatingWindow::_OnToolTipEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)
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

inline LRESULT CBkFloatingWindow::_OnBKINMInvalidateRect(LPNMHDR pnmh)
{
    BKINMINVALIDATERECT* pnms = (BKINMINVALIDATERECT*)pnmh;
    CRect rcWnd = pnms->rect;
    _InvalidateRect(rcWnd);
    return 0l;
}

inline LRESULT CBkFloatingWindow::_OnBKINMUpdateDragItem(LPNMHDR pnmh)
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

inline void CBkFloatingWindow::_OnSysCommand(UINT nID, CPoint point)
{
	if (nID == SC_CLOSE || nID == SC_MINIMIZE)
		return;

	SetMsgHandled(FALSE);
}