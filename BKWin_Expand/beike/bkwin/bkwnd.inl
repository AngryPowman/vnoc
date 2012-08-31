#include "bkeffect/bkEffectRenderer.h"

//////////////////////////////////////////////////////////////////////////
inline CBkWnd::CBkWnd()
        : m_hBkWnd(BkWnds::NewWindow(this))
        , m_hBkWndParent(NULL)
        , m_hWndContainer(NULL)
        , m_hWndRealContainer(NULL)
        , m_uCmdID(NULL)
        , m_dwState(BkWndState_Normal)
        , m_bFocus(FALSE)
        , m_bAcceptParent(TRUE)
        , m_taborder(0)
        , m_bAccept(FALSE)
        , m_bHoverParent(FALSE)
        , m_bDrawFocus(TRUE)
        , m_tabControler(NULL)
        , m_wndToolTip(NULL)
        , m_pNotify(NULL)
        , m_dwData(0)
        , m_parentsVisible(TRUE)
        , m_bNeedRedraw(TRUE)
        , m_bResize(FALSE)
        , m_nDrawPosV(0)
        , m_nDrawHeight(0)
        , m_pEffect(NULL)
        , m_pEffectRender(NULL)
#ifdef _DEBUG
		, m_nMainThreadId( ::GetCurrentThreadId() ) // 初始化对象的线程不一定是主线程
#endif
        , m_bDragable(FALSE)
		,m_pDropTarget(NULL)
		,m_bDragInner(FALSE)
{
    memset(&m_dlgpos, 0, sizeof(m_dlgpos));
    if (!m_pEffectRender)
    {
        m_pEffectRender = new CBkEffectRenderer();
    }
}

inline CBkWnd::CBkWnd(const CBkWnd& bkWnd)
    : m_hBkWnd(BkWnds::NewWindow(this))
    , m_hBkWndParent(bkWnd.m_hBkWndParent)
    , m_hWndContainer(bkWnd.m_hWndContainer)
    , m_hWndRealContainer(bkWnd.m_hWndRealContainer)
    , m_uCmdID(bkWnd.m_uCmdID)
    , m_dwState(bkWnd.m_dwState)
    , m_bFocus(bkWnd.m_bFocus)
    , m_bAcceptParent(bkWnd.m_bAcceptParent)
    , m_bHoverParent(bkWnd.m_bHoverParent)
    , m_taborder(bkWnd.m_taborder)
    , m_bDrawFocus(bkWnd.m_bDrawFocus)
    , m_bAccept(bkWnd.m_bAccept)
    , m_tabControler(bkWnd.m_tabControler)
    , m_wndToolTip(bkWnd.m_wndToolTip)
    , m_pNotify(bkWnd.m_pNotify)
    , m_dwData(bkWnd.m_dwData)
    , m_parentsVisible(bkWnd.m_parentsVisible)
    , m_nDrawPosV(bkWnd.m_nDrawPosV)
    , m_nDrawHeight(bkWnd.m_nDrawHeight)
    , m_pEffect(bkWnd.m_pEffect)
    , m_pEffectRender(bkWnd.m_pEffectRender)
    #ifdef _DEBUG
    , m_nMainThreadId(bkWnd.m_nMainThreadId) // 初始化对象的线程不一定是主线程
    #endif
    , m_bDragable(bkWnd.m_bDragable)
    , m_dlgpos(bkWnd.m_dlgpos)
    , m_style(bkWnd.m_style)
    , m_tooltip(bkWnd.m_tooltip)
	,m_pDropTarget(bkWnd.m_pDropTarget)
	,m_bDragInner(bkWnd.m_bDragInner)
{
}

inline CBkWnd::~CBkWnd()
{
    if (m_pEffectRender)
    {
        delete m_pEffectRender;
        m_pEffectRender = NULL;
    }
    BkWnds::DestroyWindow(m_hBkWnd);
}

inline IBkWnd* CBkWnd::MakeCopy()
{
    _ASSERT(FALSE);
    return NULL;
}

inline IBkWnd* CBkWnd::GetParent() const
{
    return BkWnds::GetWindow(m_hBkWndParent);
}

inline IBkWnd* CBkWnd::GetItem(UINT uItemID) const
{
    return NULL;
}

inline IBkWnd* CBkWnd::GetItemByIndex(UINT nIndex)
{
    return NULL;
}

inline void CBkWnd::SetText(LPCTSTR szText)
{
}

inline LPCTSTR CBkWnd::GetText() const
{
    return NULL;
}

inline UINT CBkWnd::GetCmdID() const
{
    return m_uCmdID;
}

inline void CBkWnd::SetCmdID(UINT nCmdID)
{
    m_uCmdID = nCmdID;
}

inline void CBkWnd::GetPos(BKDLG_POSITION* pPos)
{
    if (pPos != NULL)
    {
        memcpy(pPos, &m_dlgpos, sizeof(BKDLG_POSITION));
    }
}

inline void CBkWnd::SetPos(BKDLG_POSITION* pPos, BOOL bRepaint)
{
    if (pPos != NULL)
    {
        TestMainThread();
        NotifyInvalidate();
        memcpy(&m_dlgpos, pPos, sizeof(BKDLG_POSITION));
        _RepositionSelf();
        if (bRepaint)
        {
            NotifyInvalidate();
        }
    }
}

// Get BkWindow rect(position in container)
inline void CBkWnd::GetRect(LPRECT lpRc)
{
    if (lpRc)
    {
        lpRc->left     = m_rcWindow.left;
        lpRc->right    = m_rcWindow.right;
        lpRc->top      = m_rcWindow.top;
        lpRc->bottom   = m_rcWindow.bottom;
    }
}

inline LPRECT CBkWnd::GetRect()
{
    return m_rcWindow;
}

inline void CBkWnd::Invalidate()
{
    NotifyInvalidate();
}

inline BOOL CBkWnd::IsChecked()
{
    return BkWndState_Check == (m_dwState & BkWndState_Check);
}

inline void CBkWnd::SetChecked(BOOL bCheck)
{
    if (bCheck)
    {
        ModifyState(BkWndState_Check, 0);
    }
    else
    {
        ModifyState(0, BkWndState_Check);
    }
    NotifyInvalidate();
}

inline BOOL CBkWnd::IsDisable()
{
    return IsDisableDeep(TRUE);
}

inline void CBkWnd::SetDisable(BOOL bDisable)
{
    if (bDisable)
    {
        ModifyState(BkWndState_Disable, BkWndState_Hover/*BkWndState_Normal|BkWndState_Hover|BkWndState_PushDown*/);
    }
    else
    {
        ModifyState(0, BkWndState_Disable);
    }
    //OnDisableChanged( IsDisable() );
    NotifyInvalidate();
}

inline BOOL CBkWnd::IsVisible()
{
    return IsVisibleDeep(TRUE);
}

inline BOOL CBkWnd::SetSkin(LPCTSTR szSkin)
{
    return FALSE;
}

inline BOOL CBkWnd::SetStyle(LPCSTR szStyle)
{
    m_style = BkStyle::GetStyle( szStyle );
    NotifyInvalidate();
    if (GetRealWnd())
    {
        CRect rc;
        ::GetClientRect(GetRealWnd(), &rc);
        ::RedrawWindow(GetRealWnd(), &rc, NULL, RDW_ERASE|RDW_FRAME|RDW_INTERNALPAINT|RDW_INVALIDATE|RDW_UPDATENOW|RDW_ALLCHILDREN);
    }
    return TRUE;
}

inline HWND CBkWnd::GetRealWnd()
{
    return NULL;
}

inline void CBkWnd::RegisterNotify(INotify* pNotify)
{
    m_pNotify = pNotify;
}

inline BOOL CBkWnd::OnMouseMove(UINT nFlags, CPoint point)
{
    return FALSE;
}

inline BOOL CBkWnd::OnMouseHover()
{
    if (!m_bHoverParent)
    {
        ModifyState(BkWndState_Hover, NULL);
        if (NeedRedrawWhenStateChange())
        {
            NotifyInvalidate();
        }
    }
    return FALSE;
}

inline BOOL CBkWnd::OnMouseLeave()
{
    if (!m_bHoverParent)
    {
        ModifyState(NULL, BkWndState_Hover);
        if (NeedRedrawWhenStateChange())
        {
            NotifyInvalidate();
        }
    }
    return FALSE;
}

inline BOOL CBkWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
    ModifyState(BkWndState_PushDown, NULL);
    if (NeedRedrawWhenStateChange())
    {
        NotifyInvalidate();
    }
    SetFocus();
    return FALSE;
}

inline BOOL CBkWnd::OnLButtonUp(UINT nFlags, CPoint point,BOOL bAct/* = TRUE*/)
{
    ModifyState(NULL, BkWndState_PushDown);
    if (NeedRedrawWhenStateChange())
    {
        NotifyInvalidate();
    }
    return FALSE;
}

inline BOOL CBkWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    return FALSE;
}

inline BOOL CBkWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
    return FALSE;
}

inline BOOL CBkWnd::OnRButtonUp(UINT nFlags, CPoint point,BOOL bAct/* = TRUE*/)
{
    return FALSE;
}

inline BOOL CBkWnd::OnRButtonDblClk(UINT nFlags, CPoint point)
{
    return FALSE;
}

inline BOOL CBkWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    return FALSE;
}

inline BOOL CBkWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    return TRUE;
}

inline BOOL CBkWnd::OnDestroy()
{
	_ClearFocus(this);
    _ClearDrag(this);

    RemoveTooltip();
    return TRUE;
}

inline BOOL CBkWnd::OnPaint(CDCHandle dc)
{
    return TRUE;
}

inline void CBkWnd::OnEffectChanged(BOOL bEffect)
{
}

inline BOOL CBkWnd::OnWindowPosChanged(LPWINDOWPOS lpWndPos)
{
    m_rcWindow.MoveToXY(lpWndPos->x, lpWndPos->y);
    SIZE sizeRet = {lpWndPos->cx, lpWndPos->cy};

    OnNcCalcSize(&sizeRet);

    m_rcWindow.SetRect(lpWndPos->x, lpWndPos->y, lpWndPos->x + sizeRet.cx, lpWndPos->y + sizeRet.cy);

    AddTooltip();

    m_bResize = TRUE;

    return TRUE;
}

// Calc BkWindow size
inline BOOL CBkWnd::OnNcCalcSize(LPSIZE pSize)
{
    if (4 == m_dlgpos.nType)
    {
        CRect rect;

        if (m_dlgpos.Left.bCenter)
            rect.left = pSize->cx / 2 + (m_dlgpos.Left.bMinus ? -1 : 1) *  m_dlgpos.Left.nPos;
        else if (m_dlgpos.Left.bMinus)
            rect.left = pSize->cx - m_dlgpos.Left.nPos;
        else
            rect.left = m_dlgpos.Left.nPos;

        if (m_dlgpos.Right.bCenter)
            rect.right = pSize->cx / 2 + (m_dlgpos.Right.bMinus ? -1 : 1) *  m_dlgpos.Right.nPos;
        else if (m_dlgpos.Right.bMinus)
            rect.right = pSize->cx - m_dlgpos.Right.nPos;
        else
            rect.right = m_dlgpos.Right.nPos;

        if (m_dlgpos.Top.bCenter)
            rect.top = pSize->cy / 2 + (m_dlgpos.Top.bMinus ? -1 : 1) *  m_dlgpos.Top.nPos;
        else if (m_dlgpos.Top.bMinus)
            rect.top = pSize->cy - m_dlgpos.Top.nPos;
        else
            rect.top = m_dlgpos.Top.nPos;

        if (m_dlgpos.Bottom.bCenter)
            rect.bottom = pSize->cy / 2 + (m_dlgpos.Bottom.bMinus ? -1 : 1) *  m_dlgpos.Bottom.nPos;
        else if (m_dlgpos.Bottom.bMinus)
            rect.bottom = pSize->cy - m_dlgpos.Bottom.nPos;
        else
            rect.bottom = m_dlgpos.Bottom.nPos;

        *pSize = rect.Size();
    }
    else if (2 == m_dlgpos.nType)
    {
        // text
    }
    else if (1 == m_dlgpos.nType)
    {
        if (!m_dlgpos.width.bMinus)
        {
            pSize->cx = m_dlgpos.width.nPos;
        }
        if (!m_dlgpos.height.bMinus)
        {
            pSize->cy = m_dlgpos.height.nPos;
        }
    }

    return TRUE;
}

inline BOOL CBkWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if( m_pNotify )
    {
        NotifyInfo info = { WM_KEYDOWN, CPoint(0, 0), this, 0 };
        m_pNotify->Action( &info );
    }
    return FALSE;
}

inline BOOL CBkWnd::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if( m_pNotify )
    {
        NotifyInfo info = { WM_KEYUP, CPoint(0, 0), this, 0 };
        m_pNotify->Action( &info );
    }
    return FALSE;
}

inline BOOL CBkWnd::PtInRect(CPoint point)
{
    return ::PtInRect(&m_rcWindow, point);
}

inline BOOL CBkWnd::IsDragable()
{
    return m_bDragable;
}

inline BOOL CBkWnd::IsDragInner()
{
	return m_bDragInner;
}

inline void CBkWnd::SetDragable(BOOL bDragable)
{
    m_bDragable = bDragable;
}

inline void CBkWnd::TestMainThread()
{
#ifdef DEBUG
    // 当你看到这个东西的时候，我不幸的告诉你，你的其他线程在刷界面
    // 这是一件很危险的事情
    // 为了卫士的稳定，请您检测更改您的代码
    DWORD dwCurThreadID = GetCurrentThreadId();

    BOOL bOK = (m_nMainThreadId == dwCurThreadID); // 当前线程和构造对象时的线程一致
    if ( !bOK && _pModule != NULL )
    {
        CAppModule* appModule = static_cast<CAppModule*>(_pModule);
        if ( appModule != NULL )
        {
            // 当前线程和主线程一致
            bOK = (appModule->m_dwMainThreadID == dwCurThreadID);
        }
    }
    ATLASSERT(bOK);
#endif
}

inline void CBkWnd::Move(LPRECT prect)
{
    TestMainThread();
    if (prect)
        m_rcWindow = prect;
}

inline void CBkWnd::OnSetCursor()
{
    HCURSOR hCur = ::LoadCursor(NULL, m_style.m_lpCursorName);
    ::SetCursor(hCur);
}

inline DWORD CBkWnd::GetState(void)
{
    return m_dwState;
}

inline DWORD CBkWnd::ModifyState(DWORD dwStateAdd, DWORD dwStateRemove)
{
    DWORD dwOldState = m_dwState;
    TestMainThread();
    m_dwState |= dwStateAdd;
    m_dwState &= ~dwStateRemove;
    return dwOldState;
}

inline HBKWND CBkWnd::GetBkHWnd()
{
    return m_hBkWnd;
}

inline HBKWND CBkWnd::GetParent()
{
    return m_hBkWndParent;
}

inline void CBkWnd::SetParent(HBKWND hBkWndParent)
{
    m_hBkWndParent = hBkWndParent;
}

inline BOOL CBkWnd::IsDisableDeep(BOOL bCheckParent /* = FALSE */)
{
    BOOL bDisable = (BkWndState_Disable == (m_dwState & BkWndState_Disable));
    if (bCheckParent && !bDisable)
    {
        CBkWnd *pWndParent = this;
        while (pWndParent = BkWnds::GetWindow(pWndParent->GetParent()))
        {
            if (pWndParent->IsDisableDeep())
                return TRUE;
        }
    }
    return bDisable;
}

inline BOOL CBkWnd::IsVisibleDeep(BOOL bCheckParent /* = FALSE */)
{
    BOOL bVisible = (0 == (m_dwState & BkWndState_Invisible));

    if (bCheckParent && bVisible)
    {
        CBkWnd *pWndParent = this;

        while (pWndParent = BkWnds::GetWindow(pWndParent->GetParent()))
        {
            if (!pWndParent->IsVisibleDeep())
                return FALSE;
        }
    }
    return bVisible;
}

inline void CBkWnd::SetParentsVisible(BOOL bParentsVisible)
{
    m_parentsVisible = bParentsVisible;
}

inline BOOL CBkWnd::IsAcceptParent()
{
    return m_bAcceptParent;
}

inline void CBkWnd::SetAcceptParent(BOOL bAcceptParent)
{
    m_bAcceptParent = bAcceptParent;
}

inline void CBkWnd::SetVisible(BOOL bVisible, BOOL bRedraw/* = TRUE*/)
{
    TestMainThread();
    if (bVisible)
        ModifyState(0, BkWndState_Invisible);
    else
        ModifyState(BkWndState_Invisible, 0);

    OnVisibleChanged( IsVisible() );
    if (bRedraw)
    {
        NotifyInvalidate();
    }

    //CRect rect;
    //GetRect(rect);

    //if (bVisible)
    //{
    //    NotifyInvalidateRect(rect);
    //}
    //else
    //{
    //    CBkWnd* parentBkWnd = BkWnds::GetWindow( GetParent() );
    //    if( parentBkWnd )
    //        parentBkWnd->NotifyInvalidateRect(rect);
    //}
}

inline int CBkWnd::GetTaborder()
{
    return m_taborder;
}

inline void CBkWnd::SetTaborder(int taborder)
{
    m_taborder = taborder;
}

inline void CBkWnd::SetTabControler(IBkTaborder* tabControler)
{
    m_tabControler = tabControler;
}

inline void CBkWnd::AddTaborder()
{
    if( m_tabControler )
    {
        m_tabControler->Add( this );
    }
}

inline BOOL CBkWnd::IsAccept()
{
    return m_bAccept;
}

inline void CBkWnd::SetAccept(BOOL bAccept)
{
    m_bAccept = bAccept;
}

inline void CBkWnd::SetEffect(IBkEffectObject* pEffect)
{
    m_pEffect = pEffect;
    OnEffectChanged( pEffect ? TRUE : FALSE );
}

inline void CBkWnd::AddEffect(IBkEffectObject* pEffect)
{
    if (m_pEffectRender)
    {
        m_pEffectRender->AddEffect(pEffect);
        OnEffectChanged(TRUE);
    }
}

inline void CBkWnd::RemoveEffect(IBkEffectObject* pEffect)
{
    if (m_pEffectRender)
    {
        m_pEffectRender->RemoveEffect(pEffect);
        OnEffectChanged(FALSE);
    }
}

inline void  CBkWnd::SetDropTarget(IBkDropTarget* pDropTarget)
{
	m_pDropTarget = pDropTarget;
}

inline void CBkWnd::SetTooltipCtrl( CToolTipCtrl* pTooltip )
{
    m_wndToolTip = pTooltip;
}

inline BOOL CBkWnd::PreAddTooltip( CBkWnd* pBkWnd )
{
    return TRUE;
}

inline BOOL CBkWnd::PreUpdateTooltip( CBkWnd* pBkWnd )
{
    if( m_tooltip.IsEmpty() )
    {
        return FALSE;
    }
    return TRUE;
}

inline BOOL CBkWnd::PreUpdateTooltipRect( CBkWnd* pBkWnd, CRect& rtTooltip )
{
    return TRUE;
}

inline void CBkWnd::AddTooltip()
{
    if( m_wndToolTip && !m_tooltip.IsEmpty() )
    {
        if( ::IsWindow(m_wndToolTip->m_hWnd) && IsVisibleDeep(FALSE) )
        {
            CRect rtTool;
            GetRect( &rtTool );

            TOOLINFO ti = { sizeof(ti), 0, GetContainer(), (WPARAM)this };
            BOOL hasTooltip = m_wndToolTip->GetToolInfo(&ti);
            if( !hasTooltip )
            {
                if( PreAddTooltip(this) )
                {
                    m_wndToolTip->SetFont( BkFontPool::GetFont(BKF_DEFAULTFONT) );
                    m_wndToolTip->AddTool( GetContainer(), (LPCWSTR)m_tooltip, rtTool, (UINT)(LONG_PTR)this );
                }
            }
            else
            {
                m_wndToolTip->SetToolRect( GetContainer(), (UINT)(LONG_PTR)this, rtTool );
            }
        }
    }
}

inline void CBkWnd::RemoveTooltip()
{
    if( m_wndToolTip && ::IsWindow(m_wndToolTip->m_hWnd) )
    {
        if( GetContainer() )
        {
            m_wndToolTip->DelTool( GetContainer(), (UINT)(LONG_PTR)this );
        }
    }
}

inline void CBkWnd::UpdateTooltipRect()
{
    if( m_wndToolTip && ::IsWindow(m_wndToolTip->m_hWnd) )
    {
        CRect rtTool;
        GetRect( &rtTool );
        if( PreUpdateTooltipRect(this, rtTool) )
        {
            m_wndToolTip->SetToolRect( GetContainer(), (UINT)(LONG_PTR)this, rtTool );
        }
    }
}

inline void CBkWnd::UpdateTooltipText( const CString& text )
{
    if( m_wndToolTip && ::IsWindow(m_wndToolTip->m_hWnd) )
    {
        m_tooltip = text;
 
        TOOLINFO ti = { sizeof(ti), 0, GetContainer(), (WPARAM)this };
        BOOL hasTooltip = m_wndToolTip->GetToolInfo(&ti);
        if( hasTooltip )
        {
            if( PreUpdateTooltip(this) )
            {
                m_wndToolTip->UpdateTipText((LPCWSTR)text, GetContainer(), (UINT)(LONG_PTR)this );
            }
            else
            {
                RemoveTooltip();
            }
        }
        else
        {
            AddTooltip();
        }
    }
}

inline BOOL CBkWnd::NeedRedrawParent()
{
    return (m_style.m_strSkinName.IsEmpty() && (m_style.m_crBg == CLR_INVALID));
}

inline void CBkWnd::NotifyInvalidate()
{
    if (!s_bLockView)
    {
        m_bNeedRedraw = TRUE;
        NotifyInvalidateRect(m_rcWindow);
    }
}

inline void CBkWnd::NotifyInvalidateRect(const CRect& rect)
{
    if (!s_bLockView && IsVisibleDeep(TRUE) && m_hWndContainer && ::IsWindow(m_hWndContainer))
    {
        BKINMINVALIDATERECT nms;
        nms.hdr.code        = BKINM_INVALIDATERECT;
        nms.hdr.hwndFrom    = NULL;
        nms.hdr.idFrom      = m_uCmdID;
        nms.hbkWnd          = m_hBkWnd;
        nms.rect            = rect;
        ::SendMessage(m_hWndContainer, WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);
    }
}

inline HWND CBkWnd::GetContainer()
{
    return m_hWndContainer;
}

inline HWND CBkWnd::GetRealContainer()
{
    return m_hWndRealContainer;
}

inline BOOL CBkWnd::Load(TiXmlElement* pTiXmlElem)
{
    if (!pTiXmlElem)
        return FALSE;
    __super::Load(pTiXmlElem);
    int nValue = 0;
    CStringA strValue;
    strValue = pTiXmlElem->Attribute("width", &nValue);
    if ("full" == strValue)
    {
        m_dlgpos.nType = 1;
        m_dlgpos.width.bMinus = TRUE;
    }
    else if (nValue > 0)
    {
        m_dlgpos.nType = 1;
        m_rcWindow.right = nValue;
        m_dlgpos.width.nPos = nValue;
    }

    strValue = pTiXmlElem->Attribute("height", &nValue);
    if ("full" == strValue)
    {
        m_dlgpos.nType = 1;
        m_dlgpos.height.bMinus = TRUE;
    }
    else if (nValue > 0)
    {
        m_dlgpos.nType = 1;
        m_rcWindow.bottom = nValue;
        m_dlgpos.height.nPos = nValue;
    }

    CreateHandle( GetContainer() );

    return TRUE;
}

// Create Control,maybe it is a REAL window
inline void CBkWnd::CreateHandle( HWND hParent )
{
    SetContainer( m_hWndContainer );
    SetRealContainer( m_hWndRealContainer );
    AddTaborder();
    OnCreate( NULL );
}

// Set container, container is a REAL window
inline void CBkWnd::SetContainer(HWND hWndContainer)
{
    m_hWndContainer = hWndContainer;
    //if (m_hWndContainer != NULL)
    //{
    //    CreateHandle(m_hWndContainer);
    //}
}

inline void CBkWnd::SetRealContainer(HWND hWndRealContainer)
{
    m_hWndRealContainer = hWndRealContainer;
}

inline BOOL CBkWnd::NeedRedrawWhenStateChange()
{
    if (!m_style.m_strSkinName.IsEmpty())
    {
        CBkSkinBase* pSkin = BkSkin::GetSkin(m_style.m_strSkinName);
        if (pSkin && !pSkin->IgnoreState())
            return TRUE;
    }

    return (CLR_INVALID != m_style.m_crHoverText) || (NULL != m_style.m_ftHover) || (CLR_INVALID != m_style.m_crBgHover);
}

inline BOOL CBkWnd::RedrawRegion(CDCHandle& dc)
{
    CRgn rgn;
    dc.GetClipRgn(rgn);
    if (rgn.IsNull() || rgn.RectInRegion(m_rcWindow))
    {
        if (m_bResize && m_style.m_bOwnDC)
        {
            m_bNeedRedraw |= m_MemDC.Create(dc, m_rcWindow);
        }

        if (!m_bResize && m_bNeedRedraw && m_style.m_bOwnDC != NULL)
        {
            m_MemDC.BitBlt(m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height()
                , dc, m_rcWindow.left, m_rcWindow.top, SRCCOPY);
        }

        CDCHandle dcDraw = m_style.m_bOwnDC ? m_MemDC.m_hDC : dc;
        if (m_bResize || m_bNeedRedraw || !m_style.m_bOwnDC)
        {
            if (m_style.m_nRound > 0)
            {
                CRgn rgnWnd;
                rgnWnd.CreateRoundRectRgn(m_rcWindow.left, m_rcWindow.top, m_rcWindow.right+1, m_rcWindow.bottom+1, m_style.m_nRound, m_style.m_nRound);
                dcDraw.SelectClipRgn(rgnWnd, RGN_AND);
            }
            else
            {
                dcDraw.IntersectClipRect(m_rcWindow);
            }

            //if (m_pEffect && m_pEffect->IsUpdate())
            //{
            //    m_pEffect->Update(GetContainer(), dcDraw);
            //}
            if (m_pEffectRender && m_pEffectRender->IsEffects())
            {
                m_pEffectRender->RenderEffectObjects(GetContainer(), dcDraw);
            }
            else
            {
                DrawBkgnd(dcDraw);
                OnPaint(dcDraw);
                DrawFocus(dcDraw);
            }
            //DrawBkgnd(dcDraw);
            //OnPaint(dcDraw);
            //DrawFocus(dcDraw);
        }

        if (m_style.m_bOwnDC)
        {
            dc.BitBlt(m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height()
                , dcDraw, m_rcWindow.left, m_rcWindow.top, SRCCOPY);
        }
        m_bResize = FALSE;
        m_bNeedRedraw = FALSE;
        return TRUE;
    }
    return FALSE;
}

// Draw background default
inline void CBkWnd::DrawBkgnd(CDCHandle& dc)
{
    if (m_style.m_strSkinName.IsEmpty())
    {
        COLORREF crBg = m_style.m_crBg;

        if (BkWndState_Hover == (GetState() & BkWndState_Hover) && CLR_INVALID != m_style.m_crBgHover)
        {
            crBg = m_style.m_crBgHover;
        }

        if (CLR_INVALID != crBg)
        {
            Gdiplus::Graphics g( dc.m_hDC );
            Gdiplus::SolidBrush b( Color(m_style.m_alpha, GetRValue(crBg), GetGValue(crBg), GetBValue(crBg) ));
            g.FillRectangle( &b, m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height() );
        }
    }
    else
    {
        CBkSkinBase* pSkin = BkSkin::GetSkin(m_style.m_strSkinName);
        if (pSkin)
        {
            int realHeight = (m_nDrawHeight?m_nDrawHeight:m_rcWindow.Height()) + m_nDrawPosV;
            CRect rcDraw = m_rcWindow;
            rcDraw.top -= m_nDrawPosV;
            rcDraw.bottom = rcDraw.top + realHeight;
            pSkin->Draw(dc, rcDraw, m_dwState);
        }
    }
}

inline void CBkWnd::DrawFocus(CDCHandle& dc)
{
    if( m_taborder > 0 && m_bFocus && m_bDrawFocus )
    {
        if( m_tabControler && m_tabControler->GetCurrent() == this )
        {
            CRect rcDraw = m_rcWindow;
            rcDraw.InflateRect(-1, -1);

            Gdiplus::Graphics g(dc.m_hDC);
            Gdiplus::Color crFocus;
            crFocus.SetFromCOLORREF(m_tabControler->GetFocusColor());
            if( m_style.m_crBgCheck != CLR_INVALID )
            {
                crFocus.SetFromCOLORREF( m_style.m_crBgCheck );
            }
            Gdiplus::Pen pen( crFocus );
            pen.SetDashStyle( Gdiplus::DashStyle::DashStyleDot );
            g.DrawRectangle( &pen, rcDraw.left, rcDraw.top, rcDraw.Width()-1, rcDraw.Height()-1 );
        }
    }
}

inline void CBkWnd::DrawDisableShade(CDCHandle& dc)
{
    HWND hWnd = GetRealWnd();
    if( hWnd && !::IsWindowEnabled(hWnd) )
    {
        CRect rcDraw;
        ::GetClientRect(hWnd, &rcDraw);

        CRgn rgnOld;
        dc.GetClipRgn(rgnOld);

        CRgn rgn;
        rgn.CreateRectRgnIndirect(rcDraw);
        dc.SelectClipRgn(rgn);

        Gdiplus::Graphics g( dc.m_hDC );
        Gdiplus::Color clrMask(128, 0, 0, 0);
        SolidBrush brush( clrMask );
        g.FillRectangle( &brush, rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height() );

        dc.SelectClipRgn(rgnOld);
    }
}

inline BOOL CBkWnd::IsBackground()
{
    return m_style.m_bBackground;
}

inline void CBkWnd::SetBackground(BOOL bIsBk)
{
    m_style.m_bBackground = bIsBk;
}

inline void CBkWnd::SetData(DWORD dwData)
{
    m_dwData = dwData;
}

inline DWORD CBkWnd::GetData()
{
    return m_dwData;
}

inline void CBkWnd::DeleteThis(HWND hWndAsyn/* = NULL*/)
{
    HWND hWnd = hWndAsyn ? hWndAsyn : m_hWndContainer;
    if( hWnd && ::IsWindow(hWnd) )
    {
        ::PostMessage( hWnd, BKINM_DELETE_THIS, (WPARAM)m_hBkWndParent, (LPARAM)m_hBkWnd );
    }
}

inline void CBkWnd::SetDrawPosV(int pos)
{
    m_nDrawPosV = pos;
    NotifyInvalidate();
}

inline void CBkWnd::SetDrawHeight(int height)
{
    m_nDrawHeight = height;
    NotifyInvalidate();
}

inline BOOL CBkWnd::IsFocus()
{
    return m_bFocus;
}

inline void CBkWnd::SetFocus()
{
    m_bFocus = TRUE;
    HWND hWnd = GetRealWnd() ? GetRealWnd() : GetContainer(); 
    if( hWnd )
    {
        ::SetFocus( hWnd );
    }
    if( m_tabControler )
    {
        CBkWnd* pBkWndLastAccept = (CBkWnd*)m_tabControler->GetAccept();
        if( pBkWndLastAccept )
        {
            if( pBkWndLastAccept->IsAccept() )
            {
                pBkWndLastAccept->ModifyState( NULL, BkWndState_Accept );
                pBkWndLastAccept->NotifyInvalidate();
            }
        }

        CBkWnd* pBkWndAccept = (CBkWnd*)m_tabControler->SetAccept( this );
        if( pBkWndAccept )
        {
            if( pBkWndAccept->IsAccept() )
            {
                pBkWndAccept->ModifyState( BkWndState_Accept, NULL );
                pBkWndAccept->NotifyInvalidate();
            }
        }
    }
    if( OnSetFocus( this ) != this )
    {
        m_bFocus = FALSE;
    }
    NotifyInvalidate();
}

inline void CBkWnd::KillFocus()
{
    if( m_bFocus )
    {
        m_bFocus = FALSE;
        NotifyInvalidate();
    }
}

inline CBkWnd* CBkWnd::OnSetFocus( CBkWnd* which )
{
    CBkWnd* parentBkWnd = BkWnds::GetWindow( GetParent() );
    if( parentBkWnd )
    {
        return parentBkWnd->OnSetFocus( which );
    }
    return which;
}

inline void CBkWnd::_ClearFocus( CBkWnd* focusChild )
{
	CBkWnd* parentBkWnd = BkWnds::GetWindow( GetParent() );
	if( parentBkWnd && parentBkWnd->IsContainer() )
	{
		parentBkWnd->_ClearFocus( focusChild );
	}
}

inline void CBkWnd::_ClearDrag(CBkWnd* dragChild)
{
    CBkWnd* parentBkWnd = BkWnds::GetWindow( GetParent() );
    if( parentBkWnd && parentBkWnd->IsContainer() )
    {
        parentBkWnd->_ClearDrag( dragChild );
    }
}

inline CString CBkWnd::GetTotalCmdID()
{
    CString cmdID;
    CString id;
    CBkWnd* parentBkWnd = BkWnds::GetWindow( GetParent() );
    if( parentBkWnd )
    {
        cmdID.Format( L"%s,",parentBkWnd->GetTotalCmdID() );
    }

    id.Format( L"%d",GetCmdID() );
    cmdID.Append( id );
    return cmdID;
}

inline HRESULT CBkWnd::OnAttributePosChange(CStringA& strValue, BOOL bLoading)
{
    if (!strValue.IsEmpty())
    {
        strValue.Remove(L' ');
        strValue.Remove(L'\t');
        LPCSTR lpszValue = strValue;
        int nPos = 0, nPosPrev = 0;

        for (m_dlgpos.nType = 0; m_dlgpos.nType < 4; m_dlgpos.nType++)
        {
            nPosPrev = nPos;
            nPos = strValue.Find(',', nPosPrev) + 1;

            m_dlgpos.Item[m_dlgpos.nType].bCenter = ('|' == lpszValue[nPosPrev]);
            if (m_dlgpos.Item[m_dlgpos.nType].bCenter)
                nPosPrev ++;
            m_dlgpos.Item[m_dlgpos.nType].bMinus = ('-' == lpszValue[nPosPrev]);
            if (m_dlgpos.Item[m_dlgpos.nType].bMinus)
                nPosPrev ++;

            if (0 == nPos)
            {
                m_dlgpos.Item[m_dlgpos.nType].nPos = ::StrToIntA(strValue.Mid(nPosPrev));
                m_dlgpos.nType++;
                break;
            }

            m_dlgpos.Item[m_dlgpos.nType].nPos = ::StrToIntA(strValue.Mid(nPosPrev, nPos - nPosPrev));
        }

        if (!bLoading)
        {
            _RepositionSelf();
        }
    }

    return S_FALSE;
}

inline HRESULT CBkWnd::OnAttributeVisible(CStringA& strValue, BOOL bLoading)
{
    BOOL bVisible = ::StrToIntA(strValue);
    //if( bVisible )
    //    ModifyState(0, BkWndState_Invisible);
    //else
    //    ModifyState(BkWndState_Invisible, 0);
    SetVisible(bVisible);
    return S_FALSE;
}

inline void CBkWnd::_RepositionSelf()
{
    if (!s_bLockView)
    {
        CBkWnd *pWndParent = BkWnds::GetWindow(GetParent());
        if (pWndParent && pWndParent->IsContainer())
        {
            pWndParent->RepositionChild(this);
        }
        else
        {
            m_rcWindow.left = m_dlgpos.Left.nPos;
            m_rcWindow.top = m_dlgpos.Top.nPos;
            m_rcWindow.right = m_dlgpos.Right.nPos;
            m_rcWindow.bottom = m_dlgpos.Bottom.nPos;
        }
    }
}

inline BOOL CBkWnd::IsContainer()
{
    return FALSE;
}

inline CBkWnd* CBkWnd::FindChildByCmdID(UINT uCmdID) const
{
    return NULL;
}

inline CBkWnd* CBkWnd::FindChildByPoint(const CPoint& pt, BOOL bBackground)
{
    return NULL;
}

inline void CBkWnd::RepositionChilds()
{

}

inline void CBkWnd::RepositionChild(CBkWnd *pBkWndChild)
{

}

inline POSITION CBkWnd::GetPosByIndex(UINT nIndex)
{
    return NULL;
}

inline POSITION CBkWnd::GetPosByItem(CBkWnd* pBkWnd)
{
    return NULL;
}

inline int CBkWnd::GetIndexByItem(CBkWnd* pBkWnd)
{
    return -1;
}

inline BkStyle* CBkWnd::GetStyle()
{
    return &m_style;
}

inline int CBkWnd::GetWidth()
{
    int nWidth = 0;
    if (m_dlgpos.nType == 1 && !m_dlgpos.width.bMinus)
    {
        nWidth = m_dlgpos.width.nPos;
    }
    else
    {
        nWidth = m_rcWindow.Width();
    }
    return nWidth;
}

inline int CBkWnd::GetHeight()
{
    int nHeight = 0;
    if (m_dlgpos.nType == 1 && !m_dlgpos.height.bMinus)
    {
        nHeight = m_dlgpos.height.nPos;
    }
    else
    {
        nHeight = m_rcWindow.Height();
    }
    return nHeight;
}

inline void CBkWnd::SetWidth(int nWidth)
{
    if (m_dlgpos.nType == 1 && !m_dlgpos.width.bMinus)
    {
        m_dlgpos.width.nPos = nWidth;
        _RepositionSelf();
    }
}

inline void CBkWnd::SetHeight(int nHeight)
{
    if (m_dlgpos.nType == 1 && !m_dlgpos.height.bMinus)
    {
        m_dlgpos.height.nPos = nHeight;
        _RepositionSelf();
    }
}

inline void CBkWnd::OnVisibleChanged(BOOL bVisible)
{
	if (IsVisible())
	{
		AddTooltip();
	}
	else
	{
		RemoveTooltip();
		CBkWnd* pWndParent = BkWnds::GetWindow(GetParent());
		if (pWndParent)
			pWndParent->NotifyInvalidateRect(m_rcWindow);
	}
}

inline void CBkWnd::OnDisableChanged(BOOL bDisable)
{
}

inline void CBkWnd::LockView(BOOL bLock)
{
    s_bLockView = bLock;
}

inline BOOL CBkWnd::IsLockView()
{
    return s_bLockView;
}

inline HRESULT CBkWnd::OnDragEnter(IDataObject *pDataObj, POINT pt, DWORD *pdwEffect)
{
	if (NULL == pdwEffect)
	{
		return E_INVALIDARG;
	}
	if ( IsDragable()&&m_rcWindow.PtInRect(pt))
	{
		*pdwEffect = DROPEFFECT_COPY;
		return S_OK;
	}
	return S_FALSE;
}

inline HRESULT CBkWnd::OnDragOver(POINT pt, DWORD *pdwEffect)
{
	if (NULL == pdwEffect)
	{
		return E_INVALIDARG;
	}
	if (IsDragable()&&m_rcWindow.PtInRect(pt))
	{
		*pdwEffect = DROPEFFECT_COPY;
		return S_OK;
	}
	return S_FALSE;
}

inline HRESULT CBkWnd::OnDrop(IDataObject *pDataObj, POINT pt, DWORD *pdwEffect)
{
	if (NULL == pdwEffect)
	{
		return E_INVALIDARG;
	}
	if (IsDragable()&&m_rcWindow.PtInRect(pt))
	{
		NotifyInfo info;
		if (m_pNotify != NULL)
		{

			if(m_pDropTarget)
			{	
				if (m_pDropTarget->GetDragInfo()->dwDragOpt == DRAGOPT_FROM_EXPLORER)
				{
					info.nAction    = BKM_DRAGDROP_ACCEPTDROP_FROM_EXPLORER;
				}
				else
				{
					info.nAction    = BKM_DRAGDROP_ACCEPTDROP_FROM_WINDOW_INNER;
				}
				info.dwData     = (DWORD)m_pDropTarget->GetDragInfo();
				info.pFrom      = this;
				info.pt         = pt;
				m_pNotify->Action(&info);	
			}
		}
		return S_OK;
	}
	return S_FALSE;
}

inline HRESULT CBkWnd::OnDragLeave(void)
{
	return S_FALSE;
}

inline IDropTarget* CBkWnd::GetIDropTargetPointer()
{
	return dynamic_cast<IDropTarget*>(m_pDropTarget);
}