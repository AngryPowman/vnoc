/////////////////////////////////
////// BKTabItemPanel
/////////////////////////////////
inline CBkTabItemPanel::CBkTabItemPanel()
: m_pTablItem(NULL)
{
}

inline CBkTabItemPanel::~CBkTabItemPanel()
{
}

// IDropContainer members
inline HRESULT CBkTabItemPanel::OnDragEnter(IDataObject*pDataObj, POINT pt, DWORD *pdwEffect)
{
	return __super::OnDragEnter(pDataObj,pt,pdwEffect);
}

inline HRESULT CBkTabItemPanel::OnDragOver(POINT pt, DWORD *pdwEffect)
{
	return __super::OnDragOver(pt,pdwEffect);
}

inline HRESULT CBkTabItemPanel::OnDrop(IDataObject *pDataObj, POINT pt, DWORD *pdwEffect)
{
	return __super::OnDrop(pDataObj,pt,pdwEffect);
}

inline HRESULT CBkTabItemPanel::OnDragLeave( void)
{
	return __super::OnDragLeave();
}

inline CBkDiv* CBkTabItemPanel::GetTabItem()
{
	return m_pTablItem;
}

inline BOOL CBkTabItemPanel::SetTabItem(CBkDiv* pTabItem)
{
	m_pTablItem = pTabItem;
	return TRUE;
}

inline CBkWnd* CBkTabItemPanel::OnSetFocus(CBkWnd* which)
{
	__super::OnSetFocus(which);
	if (m_pTablItem)
		m_pTablItem->OnSetFocus(which);
    return which;
}

inline BOOL CBkTabItemPanel::OnDestroy()
{
	BOOL bRet = __super::OnDestroy();
	m_pTablItem = NULL;
	return bRet;
}

/////////////////////////////////
////// BKTabItem
/////////////////////////////////

inline CBkTabItem::CBkTabItem()
:m_bFixed(FALSE)
,m_bPushExtraShow(FALSE)
,m_pSkin(NULL)
,m_panel(NULL)
{
}

inline CBkTabItem::~CBkTabItem()
{
    m_panel = NULL;
}

////////////////////////////////
//// BkTabItem Own Interface
inline CBkDiv* CBkTabItem::GetPanel()
{
    return m_panel;
}

inline void CBkTabItem::SetPanel( CBkDiv* pTabPanel )
{
    m_panel = pTabPanel;
}

inline void CBkTabItem::ShowTabPanel( BOOL isVisible )
{
    if( m_panel )
    {
        if( m_panel->IsVisibleDeep(FALSE) != isVisible )
        {
            m_panel->SetVisible( isVisible );
        }
    }
}

inline BOOL CBkTabItem::isFixed()
{
    return m_bFixed;
}

inline void CBkTabItem::UpdateState( DWORD dwStateAdd, DWORD dwStateRemove )
{
    ModifyState( dwStateAdd, dwStateRemove );

    //POSITION pos = m_lstWndChild.GetHeadPosition();
    //while( pos )
    //{
    //    CBkWnd* pBkWndChild = m_lstWndChild.GetNext(pos);

    //    if( pBkWndChild->IsAcceptParent() )
    //    {
    //        pBkWndChild->ModifyState( dwStateAdd, dwStateRemove );
    //    }
    //} 
}

//////////////////////////////////////
//// Base interface    

inline void CBkTabItem::SetDropTarget(IBkDropTarget* pDropTarget)
{
	__super::SetDropTarget( pDropTarget );

	if( m_panel )
	{
		m_panel->SetDropTarget( pDropTarget );
	}
}

inline void CBkTabItem::SetContainer(HWND hWndContainer)
{
    __super::SetContainer( hWndContainer );

    if( m_panel )
    {
        m_panel->SetContainer( hWndContainer );
    }
}

inline void CBkTabItem::SetRealContainer(HWND hWndRealContainer)
{
    __super::SetRealContainer( hWndRealContainer );

    if( m_panel )
    {
        m_panel->SetRealContainer( hWndRealContainer );
    }
}

inline CBkWnd* CBkTabItem::FindChildByPoint(const CPoint& pt, BOOL bBackground)
{
    CBkWnd* pBkChild = __super::FindChildByPoint( pt,bBackground );
    if( !pBkChild )
    {
        if( m_panel )
        {
            if( m_panel->IsVisible() && m_panel->PtInRect(pt) )
            {
                pBkChild = m_panel->FindChildByPoint( pt,bBackground );
            }
        }

        if( !pBkChild )
        {
            pBkChild = this;
        }
    }
    return pBkChild;
}

inline CBkWnd* CBkTabItem::FindChildByCmdID(UINT uCmdID) const
{
    CBkWnd* pBkChild = __super::FindChildByCmdID( uCmdID );
    if( !pBkChild )
    {
        if( m_panel )
        {
            pBkChild = m_panel->FindChildByCmdID( uCmdID );
        }
    }
    return pBkChild;
}

///////////////////////////////////////
//// interface Message Event Handle
inline BOOL CBkTabItem::OnLButtonDown( UINT nFlags, CPoint point )
{
    ////CBkWnd::OnLButtonDown( nFlags,point );
    //__super::OnLButtonDown( nFlags,point );

    //// 点击ITEM上的某些控件后，不接受再处理ITEM的点击操作
    //BOOL bNotify = TRUE;
    //if( m_focusChild )
    //{
    //    if( !m_focusChild->IsAcceptParent() )
    //    {
    //        bNotify = FALSE;
    //    }
    //}

    //if( m_pNotify != NULL && bNotify )
    //{
    //    NotifyInfo info = {WM_LBUTTONDOWN, point, this};
    //    m_pNotify->Action(&info);
    //}

    if (!IsVisibleDeep(FALSE) || IsDisableDeep(FALSE))
    {
        return FALSE;
    }

    POSITION pos = m_lstWndChild.GetHeadPosition();
    while (pos)
    {
        CBkWnd* pBkWndChild = m_lstWndChild.GetNext(pos);
        if (!pBkWndChild->IsVisibleDeep() || pBkWndChild->IsDisableDeep())
            continue;

        if (!pBkWndChild->IsClass("text") && pBkWndChild->PtInRect(point))
        {
            m_focusChild = pBkWndChild;
            if (!pBkWndChild->OnLButtonDown(nFlags, point))
            {
                if ( !pBkWndChild->IsBackground() &&
                     (!IsBackground() || (IsBackground() && !pBkWndChild->IsClass("text"))) )
                {
                    _SetDragWnd(pBkWndChild);
                }
                break;
            }
        }
    }

	if (_GetDragWnd() == NULL && IsBackground() && !::IsZoomed(m_hWndContainer))
	{
		if (m_pDropTarget)
			m_pDropTarget->UnPreDragDrop();
		::PostMessage(m_hWndContainer, WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
		return FALSE;
	}


    // 点击ITEM上的某些控件后，不接受再处理ITEM的点击操作
    BOOL bNotify = TRUE;
    if( m_focusChild )
    {
        if( !m_focusChild->IsAcceptParent() )
        {
            bNotify = FALSE;
        }
    }

    if( m_pNotify != NULL && bNotify )
    {
        NotifyInfo info = {WM_LBUTTONDOWN, point, this};
        m_pNotify->Action(&info);
    }


    return FALSE;
}

inline BOOL CBkTabItem::OnLButtonUp( UINT nFlags, CPoint point, BOOL bAct )
{
    //CBkDiv::OnLButtonUp( nFlags, point, bAct );

    if (!IsVisibleDeep(FALSE) || IsDisableDeep(FALSE))
    {
        return FALSE;
    }

    POSITION pos = m_lstWndChild.GetHeadPosition();
    BOOL bHandled = FALSE;
    while (pos)
    {
        CBkWnd* pBkWndChild = m_lstWndChild.GetNext(pos);
        if( !pBkWndChild->IsVisibleDeep() || pBkWndChild->IsDisableDeep() )
            continue;

        if (!pBkWndChild->IsClass("text") && pBkWndChild->PtInRect(point))
        {
            if (!pBkWndChild->OnLButtonUp(nFlags, point,bAct))
            {
                if (pBkWndChild == _GetDragWnd())
                {
                    _SetDragWnd(NULL);
                }
                bHandled = TRUE;
                break;
            }
        }
    }

    if (!bHandled && m_hBkWndParent == NULL && _GetDragWnd() != NULL)
    {
        _GetDragWnd()->OnLButtonUp(nFlags, point, bAct);
        _SetDragWnd(NULL);
    }

    return FALSE;
}

inline BOOL CBkTabItem::OnRButtonUp( UINT nFlags, CPoint point, BOOL bAct )
{
    //CBkWnd::OnLButtonDown( nFlags,point );
    __super::OnRButtonUp( nFlags,point,bAct );

	if (bAct)
	{
		if( m_pNotify != NULL )
		{
			NotifyInfo info = {WM_RBUTTONUP, point, this};
			return m_pNotify->Action(&info);
		}
	}

    return TRUE;
}

inline BOOL CBkTabItem::OnLButtonDblClk( UINT nFlags, CPoint point )
{
    //CBkWnd::OnLButtonDown( nFlags,point );
    __super::OnLButtonDblClk( nFlags,point );

    if( m_pNotify != NULL )
    {
        NotifyInfo info = {WM_LBUTTONDBLCLK, point, this};
        if (!m_pNotify->Action(&info))
            return FALSE;
    }

    return TRUE;
}

inline BOOL CBkTabItem::OnMouseMove(UINT nFlags, CPoint point)
{
    CBkWnd::OnMouseMove( nFlags,point );

    __super::OnMouseMove( nFlags,point );
    return TRUE;
}

inline BOOL CBkTabItem::OnMouseHover()
{
    if( m_bPushExtraShow )
    {
        CBkWnd::OnMouseHover();
        //__super::OnMouseHover();
    }
    else
    {
        if( BkWndState_PushDown != (GetState() & BkWndState_PushDown) )
        {
            CBkWnd::OnMouseHover();
            //__super::OnMouseHover();
        }
    }
     __super::OnMouseHover();
    return TRUE;
}

inline BOOL CBkTabItem::OnMouseLeave()
{
    CBkWnd::OnMouseLeave();
    __super::OnMouseLeave();
    return TRUE;
}

inline BOOL CBkTabItem::OnPaint( CDCHandle dc )
{
    if( m_bPushExtraShow && BkWndState_PushDown == (GetState() & BkWndState_PushDown) )
    {
        if( m_pSkin )
        {
            m_pSkin->Draw( dc, m_rcWindow,IIF_STATE4(GetState(),0,1,2,3) );
        }
    }

    __super::OnPaint( dc );
    return TRUE;
}

inline CBkWnd* CBkTabItem::OnSetFocus( CBkWnd* which )
{
    m_bFocus = TRUE;
    __super::OnSetFocus( this );
    return this;
}

inline BOOL CBkTabItem::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	BOOL bRet = CBkDiv::OnKeyDown(nChar, nRepCnt, nFlags);
	if( m_panel )
	{
		m_panel->OnKeyDown(nChar, nRepCnt, nFlags);
	}
	return bRet;
}

/////////////////////////////////////////////
////// CBkTabControl
/////////////////////////////////////////////
inline CBkTabControl::CBkTabControl()
:m_tabAlign(AlignTop)
,m_pageAlign(None)
,m_tabWidth(1)
,m_tabHeight(1)
,m_tabMinSize(1)
,m_tabMarginX(0)
,m_tabMarginY(0)
,m_tabPanelGap(0)
,m_tabStartPos(0)
,m_backendMargin(0)
,m_backendMinSize(0)
,m_pageWidth(0)
,m_pageHeight(0)
,m_curPage(-1)
,m_isBackendAutoHide(FALSE)
,m_pSkin(NULL)
,m_frontSize(0)
,m_backendSize(0)
,m_pDlgFront(NULL)
,m_pDlgBackend(NULL)
,m_pBtnPrev(NULL)
,m_pBtnNext(NULL)
,m_panelTab(NULL)
,m_bFullScreen(FALSE)
{
}

inline CBkTabControl::~CBkTabControl()
{
}

inline CBkTabItem* CBkTabControl::AppendItemByTemplate( int templateIndex )
{
    if( templateIndex < 0 || templateIndex >= (int)m_lstTemplate.GetCount() )
        return NULL;

    TiXmlElement* tabTemplate = m_lstTemplate.GetAt( m_lstTemplate.FindIndex(templateIndex) );        
    if( tabTemplate )
    {
        CBkTabItem *pTab = CBkTabItem::CheckAndNew(CLASSNAME_TABITEM);
        if( pTab )
        {
            pTab->RegisterNotify(this);
            pTab->SetParent(m_hBkWnd);
            pTab->SetContainer(m_hWndContainer);
			pTab->SetDropTarget(m_pDropTarget);
            pTab->SetRealContainer(m_hWndRealContainer);
            pTab->SetParentsVisible(IsVisible());
            pTab->SetTabControler(m_tabControler);
            pTab->SetTooltipCtrl(m_wndToolTip);
            if( pTab->Load(tabTemplate) )
            {
                m_lstTabs.AddTail( pTab );
                _LoadTabPanel( tabTemplate, pTab );
            }
            OnChildLoad( tabTemplate,pTab );
            m_lstWndChild.AddTail(pTab);

            _RepositionChilds();
            NotifyInvalidate();

            SetCurSel( m_lstTabs.GetCount()-1 );
            return pTab;
        }
    }
    return NULL;
}

inline CBkTabItem* CBkTabControl::InsertItemByTemplate( int templateIndex, int tabIndex  )
{
    if( templateIndex < 0 || templateIndex >= (int)m_lstTemplate.GetCount() )
        return NULL;

    if( tabIndex < 0 || tabIndex >= (int)m_lstTabs.GetCount() )
        return NULL;

    TiXmlElement* tabTemplate = m_lstTemplate.GetAt( m_lstTemplate.FindIndex(templateIndex) );        
    if( tabTemplate )
    {
        CBkTabItem *pTab = CBkTabItem::CheckAndNew(CLASSNAME_TABITEM);
        if( pTab )
        {
            pTab->RegisterNotify(this);
            pTab->SetParent(m_hBkWnd);
            pTab->SetContainer(m_hWndContainer);
			pTab->SetDropTarget(m_pDropTarget);
            pTab->SetRealContainer(m_hWndRealContainer);
            pTab->SetParentsVisible(IsVisible());
            pTab->SetTabControler(m_tabControler);
            pTab->SetTooltipCtrl(m_wndToolTip);
            if( pTab->Load(tabTemplate) )
            {
                POSITION pos = m_lstTabs.FindIndex( tabIndex );
                m_lstTabs.InsertAfter( pos, pTab );
                _LoadTabPanel( tabTemplate, pTab );
            }
            OnChildLoad( tabTemplate,pTab );
            m_lstWndChild.AddTail(pTab);

            _RepositionChilds();
            NotifyInvalidate();

            SetCurSel( m_lstTabs.GetCount()-1 );
            return pTab;
        }
    }
    return NULL;
}

inline void CBkTabControl::MoveHeadToTail(int count)
{
    if( count <= 0 )
    {
        return;
    }

    int lastSel = m_curPage;

    
    for( int index = 0; index < count; ++index )
    {
        POSITION pos = m_lstTabs.GetHeadPosition();
        CBkTabItem* pItem = (CBkTabItem*)m_lstTabs.GetAt(pos);
        if( pItem )
        {
            m_lstTabs.RemoveAt(pos);
            m_lstTabs.AddTail(pItem);
        }
    }
    
    if( lastSel < count )
    {
        m_curPage = m_lstTabs.GetCount() - count;
    }
    else
    {
        m_curPage = lastSel - count;
    }

    _ClearLastHover();
    _RepositionChilds();

    _SetCurSel( 0 );
}

inline void CBkTabControl::MoveIndexToHead(int index)
{
    if( index < 0 || index >= (int)m_lstTabs.GetCount() )
        return ;

    POSITION pos = m_lstTabs.FindIndex(index);
    CBkTabItem* pItem = (CBkTabItem*)m_lstTabs.GetAt(pos);
    m_lstTabs.RemoveAt(pos);
    m_lstTabs.AddHead(pItem);

    _ClearLastHover();
    _RepositionChilds();

    if( index > m_curPage )
    {
        m_curPage ++;
    }

    _SetCurSel( 0 );
}

inline CBkTabItem* CBkTabControl::GetTab( int index )
{
    if( index < 0 || index >= (int)m_lstTabs.GetCount() )
        return NULL;

    return (CBkTabItem*)m_lstTabs.GetAt( m_lstTabs.FindIndex(index) );
}

inline CBkDiv* CBkTabControl::GetTabPanel( UINT tabID )
{
    CBkTabItem* pTabItem = NULL;

    POSITION pos = m_lstTabs.GetHeadPosition();
    while (pos != NULL)
    {
        CBkWnd *pBkWndChild = m_lstTabs.GetNext(pos);

        if (pBkWndChild->GetCmdID() == tabID)
        {
            pTabItem = (CBkTabItem*)pBkWndChild;
            break;
        }
    }

    return pTabItem?pTabItem->GetPanel():NULL;
}

inline CBkTabItem* CBkTabControl::GetCurSelItem()
{
    if( m_curPage < 0 )
    {
        return NULL;
    }
    return GetTab( m_curPage );
}

inline int CBkTabControl::GetCurSel()
{
    return m_curPage;
}

inline int CBkTabControl::SetCurSel( int index )
{
    return _SetCurSel( index );
}

inline int CBkTabControl::SetCurSelByID( UINT uItemID )
{
    int index = -1;
    for( POSITION pos = m_lstTabs.GetHeadPosition(); pos != NULL; m_lstTabs.GetNext(pos))
    {
        index ++;
        if( uItemID == m_lstTabs.GetAt(pos)->GetCmdID() )
        {
            break;
        }
    }

    if( index >= 0 )
    {
        return _SetCurSel( index );
    }

    return -1;
}


inline int CBkTabControl::SetLastSel()
{
    return _SetCurSel( m_lstWndChild.GetCount()-1 );
}

inline int CBkTabControl::CloseTab( int index )
{
    CBkTabItem* pCloseTab = (CBkTabItem*)GetTab( index );
    if( pCloseTab )
    {
        int showIndex = -1;

        if( index < (int)m_lstTabs.GetCount()-1 )
        {
            showIndex = index;
        }

        // 保证能调用到SELCHANGE事件
        if (m_curPage > index)
            m_curPage--;
        else
            m_curPage++;

        CBkTabItemPanel* pTabPanel = (CBkTabItemPanel*)pCloseTab->GetPanel();
        m_lstTabs.RemoveAt( m_lstTabs.FindIndex(index) );
        m_lstWndChild.RemoveAt( m_lstWndChild.Find(pCloseTab) );
        m_lstWndChild.RemoveAt( m_lstWndChild.Find(pTabPanel) );

        if( showIndex < 0 && m_lstTabs.GetCount() >= 1 )
        {
            showIndex = (int)m_lstTabs.GetCount()-1;
        }

        if( showIndex >= 0 )
        {
            SetCurSel( showIndex );
        }
        else
        {
            m_curPage = showIndex;
            if( m_panelTab )
            {
                m_panelTab->SetVisible( FALSE );
                m_panelTab = NULL;
            }
        }

        pCloseTab->DeleteThis();
        pTabPanel->DeleteThis();

        _ClearFocus( m_focusChild );
        _ClearDrag( m_focusChild );
        _ClearLastHover();
        _RepositionChilds();
        NotifyInvalidate();
        return (int)m_lstTabs.GetCount();
    }
    return -1;
}

inline int CBkTabControl::CloseCurSel()
{
    if( m_curPage < 0 )
    {
        return -1;
    }
    return CloseTab( m_curPage );
}

inline int CBkTabControl::GetTabCount()
{
    return (int)m_lstTabs.GetCount();
}

inline int CBkTabControl::GetVisibleTabCount()
{
    int count = 0;
    for( int index = 0; index < (int)m_lstTabs.GetCount(); ++index )
    {
        if( IsVisibleTab(index) )
        {
            count ++;
        }
        else
        {
            break;
        }
    }
    return count;
}

inline BOOL CBkTabControl::IsVisibleTab(int index)
{
    CBkTabItem* pTab = GetTab( index );
    if( pTab )
    {
        if( !_IsHideTab(pTab) && pTab->IsVisibleDeep(FALSE) )
        {
            return TRUE;
        }
    }
    return FALSE;
}

inline BOOL CBkTabControl::IsFullScreen()
{
    return m_bFullScreen;
}

inline void CBkTabControl::FullScreen(BOOL bFullScreen)
{
    m_bFullScreen = bFullScreen;
    _RepositionChilds();
    NotifyInvalidate();
}

inline void CBkTabControl::ShowTab( int index, BOOL bShow )
{
    if( index >= 0 && index < m_lstTabs.GetCount() )
    {
        CBkTabItem* tab = GetTab( index );
        if( tab )
        {
            bShow ? _RemoveHideTab( tab ) : _AddHideTab( tab );
            _RepositionChilds();
        }
    }
}

////////////////////////////////
//// Base Interface
inline void CBkTabControl::CreateHandle(HWND hParent)
{
    __bkClass::CreateHandle( hParent );

    if( m_pDlgFront )
    {
        m_pDlgFront->CreateHandle( hParent );
        m_pDlgFront->SetContainer( m_hWndContainer );
        m_pDlgFront->SetRealContainer( m_hWndRealContainer );
		m_pDlgFront->SetDropTarget(m_pDropTarget);
    }

    if( m_pDlgBackend )
    {
        m_pDlgBackend->CreateHandle( hParent );
        m_pDlgBackend->SetContainer( m_hWndContainer );
        m_pDlgBackend->SetRealContainer( m_hWndRealContainer );
		m_pDlgBackend->SetDropTarget(m_pDropTarget);
    }

    if( m_pBtnPrev )
    {
        m_pBtnPrev->CreateHandle( hParent );
        m_pBtnPrev->SetContainer( m_hWndContainer );
        m_pBtnPrev->SetRealContainer( m_hWndRealContainer );
		m_pBtnPrev->SetDropTarget(m_pDropTarget);
    }

    if( m_pBtnNext )
    {
        m_pBtnNext->CreateHandle( hParent );
        m_pBtnNext->SetContainer( m_hWndContainer );
        m_pBtnNext->SetRealContainer( m_hWndRealContainer );
		m_pBtnNext->SetDropTarget(m_pDropTarget);
    }
}

inline BOOL CBkTabControl::Load(TiXmlElement* pTiXmlElem)
{
    if (!CBkWnd::Load(pTiXmlElem))
        return FALSE;

    //CreateHandle( GetContainer() );

    BOOL result = FALSE;
    result = LoadChilds(pTiXmlElem->FirstChildElement());

    //TiXmlElement* pTiElement = NULL;
    //pTiElement = pTiXmlElem->FirstChildElement("front");
    //if( pTiElement )
    //{
    //    if( !m_pDlgFront )
    //    {
    //        m_pDlgFront = new CBkDiv();
    //        m_pDlgFront->SetParent(m_hBkWnd);
    //        m_pDlgFront->SetContainer(m_hWndContainer);
    //        m_pDlgFront->SetTabControler(m_tabControler);
    //        m_pDlgFront->SetTooltipCtrl(m_wndToolTip);
    //        if( m_pDlgFront->Load( pTiElement ) )
    //        {
    //            CStringA strPos;
    //            int size = 0;
    //            pTiElement->Attribute("size", &size);
    //            switch( m_tabAlign )
    //            {
    //            case AlignTop:
    //                strPos.Format("0,0,%d,%d", size, m_tabHeight );
    //                break;
    //            case AlignLeft:
    //                strPos.Format("0,0,%d,%d", m_tabWidth, size );
    //                break;
    //            }

    //            m_pDlgFront->SetAttribute("pos", strPos, TRUE);
    //        }
    //    }
    //}

    //pTiElement = NULL;
    //pTiElement = pTiXmlElem->FirstChildElement("backend");
    //if( pTiElement )
    //{
    //    if( !m_pDlgBackend )
    //    {
    //        m_pDlgBackend = new CBkDiv();
    //        m_pDlgBackend->SetParent(m_hBkWnd);
    //        m_pDlgBackend->SetContainer(m_hWndContainer);
    //        m_pDlgBackend->SetTabControler(m_tabControler);
    //        m_pDlgBackend->SetTooltipCtrl(m_wndToolTip);
    //        if( m_pDlgBackend->Load( pTiElement ) )
    //        {
    //            CStringA strPos;
    //            pTiElement->Attribute("margin", &m_backendMargin);
    //            pTiElement->Attribute("minsize", &m_backendMinSize);
    //            pTiElement->Attribute("autohide", &m_isBackendAutoHide);
    //            int size = m_backendMinSize;
    //            switch( m_tabAlign )
    //            {
    //            case AlignTop:
    //                strPos.Format("-%d,0,-0,%d", size, m_tabHeight);
    //                break;
    //            case AlignLeft:
    //                strPos.Format("0,-%d,%d,-0", size, m_tabWidth);
    //                break;
    //            }

    //            m_pDlgBackend->SetAttribute("pos", strPos, TRUE);
    //        }
    //    }
    //}

    _AddPageButtons();

    SetCurSel( m_curPage );

    return result;
}

inline CBkWnd* CBkTabControl::FindChildByCmdID(UINT uCmdID) const
{
    CBkWnd* pBkChild = __super::FindChildByCmdID( uCmdID );
    if( !pBkChild )
    {
        if( m_pDlgFront )
        {
            pBkChild = m_pDlgFront->FindChildByCmdID( uCmdID );
        }

        if( !pBkChild && m_pDlgBackend )
        {
            pBkChild = m_pDlgBackend->FindChildByCmdID( uCmdID );
        }
    }
    return pBkChild;
}

inline CBkWnd* CBkTabControl::FindChildByPoint(const CPoint& pt, BOOL bBackground)
{
    CBkWnd* pBkChild = __super::FindChildByPoint( pt,bBackground );
    if( !pBkChild )
    {
        if( m_pDlgFront )
        {
            if( m_pDlgFront->IsVisible() && m_pDlgFront->PtInRect(pt) )
            {
                pBkChild = m_pDlgFront->FindChildByPoint( pt,m_style.m_bBackground );
            }
        }

        if( !pBkChild && m_pDlgBackend )
        {
            if( m_pDlgBackend->IsVisible() && m_pDlgBackend->PtInRect(pt) )
            {
                pBkChild = m_pDlgBackend->FindChildByPoint( pt,m_style.m_bBackground );
            }
        }

        if( !pBkChild && m_panelTab )
        {
            if( m_panelTab->IsVisible() && m_panelTab->PtInRect( pt ) )
            {
                pBkChild = m_panelTab->FindChildByPoint( pt,m_style.m_bBackground );
            }
        }

        if( !pBkChild && m_pBtnPrev )
        {
            if( m_pBtnPrev->IsVisible() && m_pBtnPrev->PtInRect( pt ) )
            {
                pBkChild = m_pBtnPrev;
            }
        }

        if( !pBkChild && m_pBtnNext )
        {
            if( m_pBtnNext->IsVisible() && m_pBtnNext->PtInRect( pt ) )
            {
                pBkChild = m_pBtnNext;
            }
        }
    }
    return pBkChild;
}

inline BOOL CBkTabControl::Action(NotifyInfo* pInfo)
{
    BOOL bUpdate = FALSE;
    if( pInfo )
    {
        if( pInfo->nAction == WM_LBUTTONDOWN )
        {
            if( pInfo->pFrom == m_pDlgFront ||
                pInfo->pFrom == m_pDlgBackend ||
                pInfo->pFrom == m_pBtnPrev ||
                pInfo->pFrom == m_pBtnNext )
            {

            }
            else if( pInfo->pFrom )
            {
                int index = 0;
                POSITION pos = m_lstTabs.GetHeadPosition();
                while (pos != NULL)
                {
                    CBkWnd* pBkWndChild = (CBkWnd*)m_lstTabs.GetNext(pos);
                    if (pBkWndChild)
                    {
                        if( pBkWndChild == pInfo->pFrom )
                        {
                            break;
                        }
                        index ++;
                    }
                }

                SetCurSel( index );
            }
        }
        else if (pInfo->nAction == WM_LBUTTONDBLCLK
            || pInfo->nAction == WM_RBUTTONUP)
        {
            if (m_pNotify != NULL)
            {
                int index = -1, i = 0;
                POSITION pos = m_lstTabs.GetHeadPosition();
                while (pos != NULL)
                {
                    CBkWnd* pBkWndChild = (CBkWnd*)m_lstTabs.GetNext(pos);
                    if (pBkWndChild)
                    {
                        if( pBkWndChild == pInfo->pFrom )
                        {
                            index = i;
                            break;
                        }
                        i ++;
                    }
                }

                if (index != -1)
                {
                    NotifyInfo info = {pInfo->nAction, CPoint(0, 0), this, index};
                    if (!m_pNotify->Action(&info))
                        return FALSE;
                }
            }
        }
    }
    return TRUE;
}

inline BOOL CBkTabControl::RedrawRegion( CDCHandle& dc )
{
    CRgn rgnPanel;
    dc.GetClipRgn(rgnPanel);
    if( __super::RedrawRegion(dc) )
    {
        CBkTabItem* pCurTab = GetCurSelItem();
        if( pCurTab && pCurTab->IsVisible() )
        {
            dc.SelectClipRgn(rgnPanel);
            pCurTab->RedrawRegion( dc );
        }
        return TRUE;
    }
 
    return FALSE;
}

////////////////////////////////
//// Own Virtual Function
inline void CBkTabControl::OnCancelTabSel( int index )
{

}

inline void CBkTabControl::OnTabSel( int index )
{

}

////////////////////////////////
//// Event Handle
inline BOOL CBkTabControl::OnDestroy()
{
    __super::OnDestroy();

    m_panelTab = NULL;
    _ClearHideTabs();

    m_pDlgBackend = NULL;
    m_pDlgFront = NULL;
    m_pBtnNext = NULL;
    m_pBtnPrev = NULL;


    //if( m_pDlgBackend )
    //{
    //    delete m_pDlgBackend;
    //    m_pDlgBackend = NULL;
    //}

    //if( m_pDlgFront )
    //{
    //    delete m_pDlgFront;
    //    m_pDlgFront = NULL;
    //}

    //if( m_pBtnNext )
    //{
    //    delete m_pBtnNext;
    //    m_pBtnNext = NULL;
    //}

    //if( m_pBtnPrev )
    //{
    //    delete m_pBtnPrev;
    //    m_pBtnPrev = NULL;
    //}
    return TRUE;
}

inline BOOL CBkTabControl::OnWindowPosChanged(LPWINDOWPOS lpWndPos)
{
    CBkWnd::OnWindowPosChanged(lpWndPos);
    _RepositionChilds();
    return TRUE;
}

inline BOOL CBkTabControl::LoadChilds(TiXmlElement* pTiXmlChildElem)
{
    OnDestroy();
    for( TiXmlElement* pXmlChild = pTiXmlChildElem; 
         NULL != pXmlChild; 
         pXmlChild = pXmlChild->NextSiblingElement() )
    {
        CString strNode = pXmlChild->Value();
        CBkWnd* pBkWnd = NULL;
        if( strNode == CLASSNAME_TABITEM )
        {
            pBkWnd = new CBkTabItem();
        }
        else if ( strNode == "front" || strNode == "backend" )
        {
            pBkWnd = new CBkDiv();
        }
        else if ( strNode == CLASSNAME_TABTEMPLATE )
        {
            TiXmlElement* pTiElement = (TiXmlElement*)pXmlChild->Clone();
            m_lstTemplate.AddTail( pTiElement );
        }

        if( pBkWnd )
        {
            pBkWnd->RegisterNotify(this);
            pBkWnd->SetParent(m_hBkWnd);
            pBkWnd->SetContainer(m_hWndContainer);
            pBkWnd->SetRealContainer(m_hWndRealContainer);
            pBkWnd->SetParentsVisible(IsVisible());
            pBkWnd->SetTabControler(m_tabControler);
            pBkWnd->SetTooltipCtrl(m_wndToolTip);
			pBkWnd->SetDropTarget(m_pDropTarget);
            if( pBkWnd->Load(pXmlChild) )
            {
                if( strNode == CLASSNAME_TABITEM )
                {
                    m_lstTabs.AddTail(pBkWnd);
                    _LoadTabPanel( pXmlChild, (CBkTabItem*)pBkWnd );
                }
                else if ( strNode == "front" )
                {
                    _LoadFront( pXmlChild, (CBkDiv*)pBkWnd );
                }
                else if ( strNode == "backend" )
                {
                    _LoadBackend( pXmlChild, (CBkDiv*)pBkWnd );
                }
            }
            OnChildLoad( pXmlChild,pBkWnd );
            m_lstWndChild.AddTail(pBkWnd);
        }
    }

    return TRUE;
}

inline void CBkTabControl::_ClearAllSel()
{
    POSITION pos = m_lstTabs.GetHeadPosition();
    while( pos != NULL )
    {
        CBkTabItem *pBkWndChild = (CBkTabItem*)m_lstTabs.GetNext(pos);
        if( pBkWndChild )
        {
            pBkWndChild->UpdateState( NULL,BkWndState_PushDown );
            pBkWndChild->ShowTabPanel( FALSE );
        }
    }

    m_curPage = -1;
    m_panelTab = NULL;
}

inline int CBkTabControl::_SetCurSel( int index )
{
    if( index < 0 || index >= (int)m_lstTabs.GetCount() )
        return -1;

    int lastSelIndex = m_curPage;

    if (m_pNotify != NULL)
    {
        DWORD dwData = (DWORD)MAKELONG( lastSelIndex, index );
        NotifyInfo info = {BKM_TABCTRL_PRESELCHANGE, CPoint(0, 0), this, dwData};
        m_pNotify->Action(&info);
    }

    _ClearAllSel();

    m_curPage = index;
    CBkTabItem* curSelTab = GetTab( index );
    if( curSelTab )
    {
        curSelTab->UpdateState( BkWndState_PushDown,/*BkWndState_Hover*/NULL );
        m_panelTab = curSelTab->GetPanel();
        curSelTab->ShowTabPanel( TRUE );
        OnTabSel( m_curPage );
    }

    if( lastSelIndex != index )
    {
        if (m_pNotify != NULL)
        {
            DWORD dwData = (DWORD)MAKELONG( lastSelIndex, m_curPage );
            NotifyInfo info = {BKM_TABCTRL_SELCHANGED, CPoint(0, 0), this, dwData};
            m_pNotify->Action(&info);
        }
    }

    NotifyInvalidate();

    return lastSelIndex;
}

inline int CBkTabControl::_GetTabDefaultSize()
{
    int size = 0;
    switch( m_tabAlign )
    {
    case AlignTop:
        size = m_tabWidth;
        break;
    case AlignLeft:
        size = m_tabHeight;
        break;
    }
    return size;
}

inline int CBkTabControl::_GetRectSize( const CRect& rc )
{
    int size = 0;
    switch( m_tabAlign )
    {
    case AlignTop:
        size = rc.Width();
        break;
    case AlignLeft:
        size = rc.Height();
        break;
    }
    return size;
}

inline int CBkTabControl::_CalculateTabsSize( int frontSize,int backendSize )
{
    int size = _GetRectSize( m_rcWindow );
    if( m_pDlgFront && m_pDlgFront->IsVisibleDeep(FALSE) )
    {
        size -= frontSize;
    }
    if( m_pDlgBackend && m_pDlgBackend->IsVisibleDeep(FALSE) )
    {
        size -= backendSize;
    }
    return size;
}

inline int CBkTabControl::_CaluculateOptimumTabSize( int tabsSize )
{
    int size = 0;
    int pageCount = (int)m_lstTabs.GetCount();
    if( pageCount > 0 )
    {
        int nextSize = 0;
        nextSize = size = (int)(tabsSize/pageCount);
        while( nextSize < m_tabMinSize && nextSize > 0 )
        {
            pageCount --;
            if( pageCount > 0 )
            {
                size = nextSize;
                nextSize = (int)(tabsSize/pageCount);
            }
        }
    }
    return size;
}

inline int CBkTabControl::_CaluculateTabsStartPositon( BOOL isCanContainAll )
{
    int position = 0;

    if( !isCanContainAll )
    {
        switch( m_pageAlign )
        {
        case None:
        case AllBackend:
            break;
        case AllFront:
            position = m_pageWidth*2;
            break;
        case BothSides:
            position = m_pageWidth;
            break;
        }
    }

    return position;
}

inline void CBkTabControl::_SetPagesVisible( BOOL isVisible )
{
    if( m_pBtnPrev && m_pBtnNext )
    {
        m_pBtnPrev->SetVisible( isVisible & !m_bFullScreen );
        m_pBtnNext->SetVisible( isVisible & !m_bFullScreen );
    }
}

inline void CBkTabControl::_AdjustPageButtonsPosition( int& tabssize )
{
    if( m_pBtnPrev && m_pBtnNext )
    {
        int left = (m_tabWidth - m_pageWidth)/2;
        int top = (m_tabHeight - m_pageHeight)/2;

        WINDOWPOS WndPosPrev = { 0, 0, m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height(), SWP_SHOWWINDOW };
        WINDOWPOS WndPosNext = { 0, 0, m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height(), SWP_SHOWWINDOW };

        CStringA strPosPrev,strPosNext;
        switch( m_pageAlign )
        {
        case None:
            m_pBtnPrev->SetVisible( FALSE );
            m_pBtnNext->SetVisible( FALSE );
            break;
        case AllFront:
            {
                CRect rc(0,0,0,0);
                switch( m_tabAlign )
                {
                case AlignTop:
                    rc.SetRect( m_frontSize,top,m_frontSize+m_pageWidth,top+m_pageHeight );
                    WndPosPrev.x = m_rcWindow.left + m_frontSize;
                    WndPosNext.x = m_rcWindow.left + m_frontSize + m_pageWidth;

                    tabssize -= (m_pageWidth*2);
                    break;
                case AlignLeft:                        
                    rc.SetRect( left,m_frontSize,left+m_pageWidth,m_frontSize+m_pageHeight );
                    WndPosPrev.y = m_rcWindow.top + m_frontSize;
                    WndPosNext.x = m_rcWindow.left + m_pageWidth;
                    WndPosNext.y = m_rcWindow.top + m_frontSize;

                    tabssize -= (m_pageHeight*2);
                    break;
                }
                strPosPrev.Format("%d,%d,%d,%d", rc.left, rc.top , rc.right, rc.bottom );
                rc.OffsetRect( m_pageWidth,0 );
                strPosNext.Format("%d,%d,%d,%d", rc.left, rc.top , rc.right, rc.bottom );

                m_pBtnPrev->SetAttribute( "pos",strPosPrev,TRUE );
                m_pBtnPrev->OnWindowPosChanged( &WndPosPrev );

                m_pBtnNext->SetAttribute( "pos",strPosNext,TRUE );
                m_pBtnNext->OnWindowPosChanged( &WndPosNext );
            }
            break;
        case AllBackend:
            {
                CRect rc(0,0,0,0);
                switch( m_tabAlign )
                {
                case AlignTop:
                    rc.SetRect( m_backendSize,top,m_backendSize+m_pageWidth,top+m_pageHeight );
                    strPosNext.Format("-%d,%d,-%d,%d", rc.right, rc.top , rc.left, rc.bottom );
                    rc.OffsetRect( m_pageWidth,0 );
                    strPosPrev.Format("-%d,%d,-%d,%d", rc.right, rc.top , rc.left, rc.bottom );

                    WndPosPrev.x = m_rcWindow.right - m_backendSize - m_pageWidth*2;
                    WndPosNext.x = m_rcWindow.right - m_backendSize - m_pageWidth;

                    tabssize -= (m_pageWidth*2);
                    break;
                case AlignLeft:                        
                    rc.SetRect( left,m_backendSize,left+m_pageWidth,m_backendSize+m_pageHeight );
                    strPosNext.Format("%d,-%d,%d,-%d", rc.left, rc.bottom , rc.right, rc.top );
                    rc.OffsetRect( m_pageWidth,0 );
                    strPosPrev.Format("%d,-%d,%d,-%d", rc.left, rc.bottom , rc.right, rc.top );

                    //WndPosPrev.y = m_rcWindow.top + m_frontSize + tabssize - m_pageHeight;
                    //WndPosNext.x = m_rcWindow.left + m_pageWidth;
                    //WndPosNext.y = m_rcWindow.top + m_frontSize + tabssize - m_pageHeight;

                    WndPosPrev.y = m_rcWindow.bottom - m_backendSize - m_pageHeight;
                    WndPosNext.x = m_rcWindow.left + m_pageWidth;
                    WndPosNext.y = m_rcWindow.bottom - m_backendSize - m_pageHeight;

                    tabssize -= (m_pageHeight*2);
                    break;
                }
                m_pBtnPrev->SetAttribute( "pos",strPosPrev,TRUE );
                m_pBtnPrev->OnWindowPosChanged( &WndPosPrev );

                m_pBtnNext->SetAttribute( "pos",strPosNext,TRUE );
                m_pBtnNext->OnWindowPosChanged( &WndPosNext );
            }
            break;
        case BothSides:
            {
                switch( m_tabAlign )
                {
                case AlignTop:
                    strPosPrev.Format("%d,%d,%d,%d", m_frontSize, top, m_frontSize+m_pageWidth, top+m_pageHeight );
                    strPosNext.Format("-%d,%d,-%d,%d", m_backendSize+m_pageWidth, top, m_backendSize, top+m_pageHeight );

                    WndPosPrev.x = m_rcWindow.left + m_frontSize;
                    WndPosNext.x = m_rcWindow.left + m_frontSize + tabssize - m_pageWidth;

                    tabssize -= (m_pageWidth*2);
                    break;
                case AlignLeft:                        
                    strPosPrev.Format("%d,%d,%d,%d", left, m_frontSize, left+m_pageWidth, m_frontSize + m_pageHeight );
                    strPosNext.Format("%d,-%d,%d,-%d", left, m_backendSize+m_pageHeight, left+m_pageWidth, m_backendSize );

                    WndPosPrev.y = m_rcWindow.top + m_frontSize;
                    WndPosNext.x = m_rcWindow.top + m_frontSize + tabssize - m_pageHeight;

                    tabssize -= (m_pageHeight*2);
                    break;
                }
                m_pBtnPrev->SetAttribute( "pos",strPosPrev,TRUE );
                m_pBtnPrev->OnWindowPosChanged( &WndPosPrev );

                m_pBtnNext->SetAttribute( "pos",strPosNext,TRUE );
                m_pBtnNext->OnWindowPosChanged( &WndPosNext );
            }
            break;
        }

        if( tabssize < 0 )
        {
            tabssize = 0;
        }
    }
}

inline void CBkTabControl::_AdjustTabPanelPosition( CBkDiv* tabPanel )
{
    if( tabPanel )
    {
        BKDLG_POSITION pos = {};
        tabPanel->GetPos( &pos );

        CStringA strPos;
        WINDOWPOS WndPos = { 0, 0, m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height(), SWP_SHOWWINDOW };
        switch( m_tabAlign )
        {
        case AlignTop:
            //strPos.Format("0,%d,-0,-0", (m_bFullScreen?0:m_tabHeight) );
            WndPos.y = m_rcWindow.top + (m_bFullScreen?0:(m_tabHeight+m_tabPanelGap));
            pos.Top.nPos = (m_bFullScreen?0:(m_tabHeight+m_tabPanelGap));
            break;
        case  AlignLeft:
            //strPos.Format("%d,0,-0,-0", (m_bFullScreen?0:m_tabWidth) );
            WndPos.x = m_rcWindow.left + (m_bFullScreen?0:(m_tabWidth+m_tabPanelGap));
            pos.Left.nPos = (m_bFullScreen?0:(m_tabWidth+m_tabPanelGap));
            break;
        }
        //tabPanel->SetAttribute( "pos",strPos,TRUE );
        //tabPanel->OnWindowPosChanged( &WndPos );
        tabPanel->SetPos( &pos, FALSE );
    }
}

inline void CBkTabControl::_RepositionChilds()
{
    m_frontSize = m_backendSize = 0;

    int tabDefaultSize = _GetTabDefaultSize();

    WINDOWPOS WndPos = { 0, 0, m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height(), SWP_SHOWWINDOW };
    if( m_pDlgFront )
    {
        m_pDlgFront->OnWindowPosChanged( &WndPos );
        CRect rcFront;
        m_pDlgFront->GetRect( rcFront );
        m_frontSize = _GetRectSize( rcFront );
        m_pDlgFront->SetVisible( !m_bFullScreen );
    }

    // 能容纳tabs的最大区域尺寸
    int tabssize = _CalculateTabsSize( m_frontSize,m_isBackendAutoHide?0:m_backendMinSize ) - m_tabStartPos;

    int tabcount = (int)m_lstTabs.GetCount();
    int tabsep = 0;
    switch( m_tabAlign )
    {
    case AlignTop:
        tabsep = m_tabMarginX;
        break;
    case AlignLeft:
        tabsep = m_tabMarginY;
        break;
    }

    BOOL isShowBackend = TRUE;
    BOOL isCanContainAll = TRUE;
    if( (int)(tabssize/(tabsep+tabDefaultSize)) >= tabcount )
    {
        // 可容纳下所有的tabs，使用标准尺寸
        m_tabAverageSize = tabDefaultSize;
        // 更新后端区域紧随最后的tab
        tabssize = (tabsep+tabDefaultSize)*tabcount;
        m_backendSize = _GetRectSize( m_rcWindow ) - m_frontSize - tabssize - m_tabStartPos;
        isShowBackend = FALSE;
    }
    else
    {
        m_backendSize = m_backendMinSize;
        if( m_isBackendAutoHide )
        {
            tabssize -= m_backendSize;
        }
        // 已不能容纳下所有的tabs，计算下平均尺寸
        if( tabcount > 0 )
        {
            m_tabAverageSize = (int)((tabssize-tabsep*tabcount)/tabcount);
        }
        else
        {
            m_tabAverageSize = m_tabMinSize;
        }

        if( m_tabAverageSize < m_tabMinSize )
        {
            isCanContainAll = FALSE;
            // 调整翻页按钮，更新一下显示翻页按钮后tabs的尺寸
            _AdjustPageButtonsPosition( tabssize );
            // 减去翻页按钮后再计算，恰好能容纳N个tabs后(但可不全部容纳下所有的tabs)且仅小于最小尺寸的最大尺寸
            //m_tabAverageSize = _CaluculateOptimumTabSize( tabssize-tabsep*tabcount );
            m_tabAverageSize = _CaluculateOptimumTabSize( tabssize );
            m_tabAverageSize -= tabsep;
        }
    }
    _SetPagesVisible( !isCanContainAll );

    // 调整后端跟随最后一个Tab
    switch( m_tabAlign )
    {
    case AlignTop:
        WndPos.x = m_rcWindow.right - m_backendSize - m_backendMargin;
        break;
    case AlignLeft:
        WndPos.y = m_rcWindow.bottom - m_backendSize - m_backendMargin;
        break;
    }
    if( m_pDlgBackend )
    {
        CStringA strPos;
        switch( m_tabAlign )
        {
        case AlignTop:
            strPos.Format("-%d,0,-0,%d", m_backendSize, m_tabHeight);
            break;
        case AlignLeft:
            strPos.Format("0,-%d,%d,-0", m_backendSize, m_tabWidth);
            break;
        }

        m_pDlgBackend->SetAttribute("pos", strPos, TRUE);
        m_pDlgBackend->OnWindowPosChanged( &WndPos );
        BOOL bVisibleBackend = TRUE;
        if( m_bFullScreen )
        {
            bVisibleBackend = FALSE;
        }
        else
        {
            if( m_isBackendAutoHide && !isShowBackend/*(m_backendSize>m_backendMinSize)*/ )
            {
                bVisibleBackend = FALSE;
            }
        }
        m_pDlgBackend->SetVisible( bVisibleBackend );
    }

    // 调整Tabs的位置和显示
    int visible_tabssize = _CaluculateTabsStartPositon( isCanContainAll ) + m_tabStartPos;
    POSITION pos = m_lstTabs.GetHeadPosition();
    while( pos != NULL )
    {
        CBkTabItem *pBkWndChild = (CBkTabItem*)m_lstTabs.GetNext(pos);
        if( pBkWndChild )
        {
            if( _IsHideTab(pBkWndChild) )
            {
                pBkWndChild->SetVisible( FALSE );
                continue;
            }

            BOOL bPosChild = TRUE;
            if( visible_tabssize - m_tabStartPos + (tabsep + m_tabAverageSize) <= tabssize && !m_bFullScreen )
            {
                pBkWndChild->SetVisible( TRUE );
            }
            else
            {
                pBkWndChild->SetVisible( FALSE );
                bPosChild = FALSE;
                //if( !m_bFullScreen )
                //{
                //    break;
                //}
            }

            if( bPosChild )
            {
                CStringA strPos;
                WINDOWPOS WndPos = {0};
                switch( m_tabAlign )
                {
                case AlignTop:
                    WndPos.x = m_rcWindow.left + m_frontSize + visible_tabssize + m_tabMarginX;
                    WndPos.y = m_rcWindow.top + m_tabMarginY;
                    WndPos.cx = m_rcWindow.Width() - m_frontSize - visible_tabssize;
                    WndPos.cy = m_rcWindow.Height();
                    strPos.Format("%d,%d,%d,%d", 0, 0, m_tabAverageSize, m_tabHeight-m_tabMarginY);
                    visible_tabssize += (m_tabAverageSize + m_tabMarginX);
                    break;
                case AlignLeft:
                    WndPos.x = m_rcWindow.left + m_tabMarginX;
                    WndPos.y = m_rcWindow.top + m_frontSize + visible_tabssize + m_tabMarginY;
                    WndPos.cx = m_rcWindow.Width();
                    WndPos.cy = m_rcWindow.Height() - m_frontSize - visible_tabssize;
                    strPos.Format("%d,%d,%d,%d", 0, 0, m_tabWidth-m_tabMarginX, m_tabAverageSize);
                    visible_tabssize += (m_tabAverageSize + m_tabMarginY);
                    break;
                }
                pBkWndChild->SetAttribute( "pos",strPos,TRUE );
                pBkWndChild->OnWindowPosChanged( &WndPos );

                // 调整TabPanel的位置
                _AdjustTabPanelPosition( pBkWndChild->GetPanel() );
            }
        }
    }

    //// 调整当前显示的TabPanel的位置
    //_AdjustTabPanelPosition( m_panelTab );
    if( m_bFullScreen && m_panelTab)
    {
        _AdjustTabPanelPosition( m_panelTab );
        m_panelTab->SetVisible( m_bFullScreen );
    }
}

inline void CBkTabControl::_AddPageButtons()
{
    if( m_pBtnPrev )
    {
        m_pBtnPrev->OnDestroy();
    }

    if( m_pBtnNext )
    {
        m_pBtnNext->OnDestroy();
    }

    m_pBtnPrev = CBkImageBtnWnd::CheckAndNew( "imgbtn" );
    m_pBtnPrev->SetParent(m_hBkWnd);
    m_pBtnPrev->SetContainer(m_hWndContainer);
	m_pBtnPrev->SetDropTarget(m_pDropTarget);
    m_pBtnPrev->SetRealContainer(m_hWndRealContainer);
    m_pBtnPrev->SetParentsVisible(IsVisible());
    m_pBtnPrev->SetTabControler(m_tabControler);
    m_pBtnPrev->SetTooltipCtrl(m_wndToolTip);
    m_pBtnPrev->RegisterNotify( this );
    m_pBtnPrev->SetAttribute( "skin",m_strSkinBtnPrev,TRUE );

    m_pBtnNext = CBkImageBtnWnd::CheckAndNew( "imgbtn" );
    m_pBtnNext->SetParent(m_hBkWnd);
    m_pBtnNext->SetContainer(m_hWndContainer);
    m_pBtnNext->SetRealContainer(m_hWndRealContainer);
    m_pBtnNext->SetParentsVisible(IsVisible());
    m_pBtnNext->SetTabControler(m_tabControler);
    m_pBtnNext->SetTooltipCtrl(m_wndToolTip);
    m_pBtnNext->RegisterNotify( this );
    m_pBtnNext->SetAttribute( "skin",m_strSkinBtnNext,TRUE );
}

inline void CBkTabControl::_LoadFront(TiXmlElement* pTiXmlElem, CBkDiv* front)
{
    if( pTiXmlElem && front )
    {
        m_pDlgFront = front;
        if( m_pDlgFront->Load( pTiXmlElem ) )
        {
            CStringA strPos;
            int size = 0;
            pTiXmlElem->Attribute("size", &size);
            switch( m_tabAlign )
            {
            case AlignTop:
                strPos.Format("0,0,%d,%d", size, m_tabHeight );
                break;
            case AlignLeft:
                strPos.Format("0,0,%d,%d", m_tabWidth, size );
                break;
            }

            m_pDlgFront->SetAttribute("pos", strPos, TRUE);
        }
    }
}

inline void CBkTabControl::_LoadBackend(TiXmlElement* pTiXmlElem, CBkDiv* backend)
{
    if( pTiXmlElem && backend )
    {
        m_pDlgBackend = backend;
        if( m_pDlgBackend->Load( pTiXmlElem ) )
        {
            CStringA strPos;
            pTiXmlElem->Attribute("margin", &m_backendMargin);
            pTiXmlElem->Attribute("minsize", &m_backendMinSize);
            pTiXmlElem->Attribute("autohide", &m_isBackendAutoHide);
            int size = m_backendMinSize;
            switch( m_tabAlign )
            {
            case AlignTop:
                strPos.Format("-%d,0,-0,%d", size, m_tabHeight);
                break;
            case AlignLeft:
                strPos.Format("0,-%d,%d,-0", size, m_tabWidth);
                break;
            }

            m_pDlgBackend->SetAttribute("pos", strPos, TRUE);
        }
    }
}

inline void CBkTabControl::_LoadTabPanel(TiXmlElement* pTiXmlElem,CBkTabItem* tab)
{
    if( pTiXmlElem && tab )
    {
        CStringA strSrc = pTiXmlElem->Attribute( "file" );
        if( !strSrc.IsEmpty() )
        {
            CStringA strXML;
            BOOL bRet = BkResManager::LoadResource(CA2T(strSrc), strXML);
            ATLASSERT(bRet);

            if (!bRet)
                return;

            TiXmlDocument xmlDoc;
            xmlDoc.Parse(strXML, NULL, TIXML_ENCODING_UTF8);
            if (xmlDoc.Error())
            {
                ATLASSERT(NULL);
                return ;
            }

            TiXmlElement *pSubTiElement = xmlDoc.RootElement();

            CBkTabItemPanel* pTabPanel = new CBkTabItemPanel();
            pTabPanel->SetParent(m_hBkWnd);
            pTabPanel->SetContainer(m_hWndContainer);
			pTabPanel->SetDropTarget(m_pDropTarget);
            pTabPanel->SetRealContainer(m_hWndRealContainer);
            pTabPanel->SetParentsVisible(IsVisible());
            pTabPanel->SetTabControler(m_tabControler);
            pTabPanel->SetTooltipCtrl(m_wndToolTip);
            pTabPanel->Load( pSubTiElement );
            pTabPanel->SetVisible( FALSE );

            pTabPanel->SetTabItem(this);
            tab->SetPanel( pTabPanel );
            m_lstWndChild.AddTail( pTabPanel );
        }
    }
}


inline void CBkTabControl::_AddHideTab( CBkWnd* tab )
{
    if( tab && !_IsHideTab(tab) )
    {
        m_lstHideTabs.AddTail(tab);
    }
}

inline void CBkTabControl::_RemoveHideTab( CBkWnd* tab )
{
    POSITION pos = m_lstHideTabs.Find( tab );
    if( pos )
    {
        m_lstHideTabs.RemoveAt( pos );
    }
}

inline void CBkTabControl::_ClearHideTabs()
{
    m_lstHideTabs.RemoveAll();
}

inline BOOL CBkTabControl::_IsHideTab( CBkWnd* tab )
{
    return m_lstHideTabs.Find( tab ) ? TRUE : FALSE;
}
