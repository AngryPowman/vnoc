//////////////////////////////////////////////////////////////////////////
// CBkMenuItem

#define TIME_SHOWSUBMENU_ID         1
#define TIME_HIDESUBMENU_ID         2
#define TIME_SHOWSUBMENU            300
#define TIME_HIDESUBMENU            300

inline CBkMenuItem::CBkMenuItem()
:m_nType(Menu_Item)
,m_pSubMenu(NULL)
,m_nTextMarginLeft(0)
,m_nTextMarginRight(0)
,m_nSubMenuGapX(2)
,m_nSubMenuGapY(0)
{
}

inline CBkMenuItem::CBkMenuItem(int type)
:m_nType(type)
,m_pSubMenu(NULL)
,m_nTextMarginLeft(0)
,m_nTextMarginRight(0)
,m_nSubMenuGapX(2)
,m_nSubMenuGapY(0)
{
}

inline CBkMenuItem::~CBkMenuItem()
{

}

inline int CBkMenuItem::GetType()
{
    return m_nType;
}

inline CBkMenu* CBkMenuItem::GetSubMenu() const
{
    return m_pSubMenu;
}

inline void	CBkMenuItem::ShowSubMenuStart(BOOL bShow)
{
	if (bShow)
	{
		::SetTimer( GetContainer(), (UINT_PTR)this+TIME_SHOWSUBMENU_ID, TIME_SHOWSUBMENU, OnTimer_ShowSubMenu );
	}
	else
	{
		::SetTimer( GetContainer(), (UINT_PTR)this+TIME_HIDESUBMENU_ID, TIME_HIDESUBMENU, OnTimer_HideSubMenu );
	}
}

inline void CBkMenuItem::ShowSubMenu(BOOL bShow)
{
    if (m_pSubMenu)
    {
        if (bShow)
        {
            CWindow wnd(m_hWndContainer);
            CRect rcItem = m_rcWindow;
            wnd.ClientToScreen( &rcItem );

            CPoint ptSubMenu( rcItem.right+m_nSubMenuGapX, rcItem.top+m_nSubMenuGapY );

            CRect rcWorkAera;
            SystemParametersInfo(SPI_GETWORKAREA, NULL, &rcWorkAera, 0);
            SIZE szMenu = m_pSubMenu->GetMenuSize();
            if (ptSubMenu.x+szMenu.cx >= rcWorkAera.right)
            {
                ptSubMenu.x = rcItem.left - m_nSubMenuGapX - szMenu.cx;
            }
            if (ptSubMenu.y+szMenu.cy >= rcWorkAera.bottom)
            {
                ptSubMenu.y = rcWorkAera.bottom - szMenu.cy - m_nSubMenuGapY;
            }

            m_pSubMenu->ShowSubMenu( ptSubMenu );
        }
        else
        {
            m_pSubMenu->Hide();
        }
    }
}

inline void CBkMenuItem::SetDisable(BOOL bDisable)
{
    if (bDisable)
    {
        UpdateState(BkWndState_Disable, 0);
    }
    else
    {
        UpdateState(0, BkWndState_Disable);
    }
    NotifyInvalidate();
}

inline void CBkMenuItem::SetChecked(BOOL bCheck)
{
    if (bCheck)
    {
        UpdateState(BkWndState_Check, 0);
    }
    else
    {
        UpdateState(0, BkWndState_Check);
    }
    NotifyInvalidate();
}

inline int CBkMenuItem::GetItemWidth()
{
    int nMargin = m_nTextMarginLeft + m_nTextMarginRight;
    CBkText* pText = dynamic_cast<CBkText*>(GetItemByIndex(0));
    if (pText)
    {
        SIZE szText = {};
        pText->MeasureDrawText(szText);
        return nMargin + szText.cx;
    }
    return nMargin;
}

inline void CBkMenuItem::OnChildLoad( TiXmlElement* pTiXmlElem,CBkWnd* pChildWindow )
{
    if( pTiXmlElem )
    {
        if( strcmp(CLASSNAME_MENU,pTiXmlElem->Value())  == 0 )
        {
            if( m_pSubMenu )
            {
                m_pSubMenu->OnDestroy();
                delete m_pSubMenu;
                m_pSubMenu = NULL;
            }

            m_pSubMenu = new CBkMenu();
            m_pSubMenu->RegisterNotify(this);
            m_pSubMenu->CBkWnd::SetParent(m_hBkWnd);
            m_pSubMenu->Create( pTiXmlElem, GetContainer() );
        }
    }
}

inline BOOL CBkMenuItem::OnDestroy()
{
    KillTimer(GetContainer(), TIME_SHOWSUBMENU_ID);
    KillTimer(GetContainer(), TIME_HIDESUBMENU_ID);

    __super::OnDestroy();

    if( m_pSubMenu )
    {
        m_pSubMenu->OnDestroy();
        delete m_pSubMenu;
        m_pSubMenu = NULL;
    }

    return TRUE;
}

inline BOOL CBkMenuItem::OnLButtonDown(UINT nFlags, CPoint point)
{
    return FALSE;
    if (Menu_Item == m_nType)
    {
        UpdateState(BkWndState_PushDown, NULL);
        CBkWnd::OnLButtonDown(nFlags, point);
        CBkDiv::OnLButtonDown(nFlags, point);
    }
    return FALSE;
}

inline BOOL CBkMenuItem::OnLButtonUp(UINT nFlags, CPoint point,BOOL bAct)
{
	ReleaseCapture();

    UpdateState(NULL, BkWndState_PushDown);
    CBkDiv::OnLButtonUp(nFlags, point,bAct);
	if (bAct)
	{
		if (m_pNotify && Menu_Item == m_nType)
		{
			NotifyInfo info = {BKM_MENU_CLICK, point, this, GetCmdID()};
			m_pNotify->Action(&info);
		}
	}
    return FALSE;
}

inline BOOL CBkMenuItem::OnMouseHover()
{
	::SetCapture(m_hWndContainer);
	CBkMenuBody* owningBody = (CBkMenuBody*)BkWnds::GetWindow(m_hBkWndParent);
	if (owningBody != NULL)
	{
		owningBody->HideOtherSubmenu(this);
	}

    if (Menu_Separator != m_nType)
    {
        UpdateState(BkWndState_Hover, NULL);
        CBkWnd::OnMouseHover();
        CBkDiv::OnMouseHover();

        if( Menu_Sub == m_nType && m_pSubMenu )
        {
            ShowSubMenuStart(TRUE);
        }
    }
    return TRUE;
}

inline BOOL CBkMenuItem::OnMouseLeave()
{
	ReleaseCapture();
    if (Menu_Separator != m_nType)
    {
        if( Menu_Sub == m_nType && m_pSubMenu )
        {
			ShowSubMenuStart(FALSE);
        }
		else
		{
			UpdateState(NULL, BkWndState_Hover | BkWndState_PushDown);
			CBkWnd::OnMouseLeave();
			CBkDiv::OnMouseLeave();
			NotifyInvalidate();
		}
    }

    return TRUE;
}

inline VOID CBkMenuItem::OnTimer_ShowSubMenu( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime )
{
    CBkMenuItem* pMenuItem = (CBkMenuItem*)(idEvent-TIME_SHOWSUBMENU_ID);
    if (pMenuItem && Menu_Sub == pMenuItem->GetType() )
    {
        KillTimer(hwnd, idEvent);

        CPoint pt;
        ::GetCursorPos(&pt);
        ::ScreenToClient(hwnd, &pt);

        CRect rcItem;
        pMenuItem->GetRect(&rcItem);
        if (rcItem.PtInRect(pt))
        {
            pMenuItem->ShowSubMenu(TRUE);
        }
    }
}

inline VOID CBkMenuItem::OnTimer_HideSubMenu( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime )
{
    CBkMenuItem* pMenuItem = (CBkMenuItem*)(idEvent-TIME_HIDESUBMENU_ID);
    if (pMenuItem && Menu_Sub == pMenuItem->GetType() )
    {
        KillTimer(hwnd, idEvent);

        CPoint pt;
        ::GetCursorPos(&pt);
       // ::ScreenToClient(hwnd, &pt);

        CRect rcItem;
        pMenuItem->GetSubMenu()->GetRect(&rcItem);
		pMenuItem->GetSubMenu()->MapWindowPoints(NULL, &rcItem);
		//::MapWindowRect(pMenuItem->GetSubMenu()->m_hWnd, NULL, &rcItem);
        if (!rcItem.PtInRect(pt))
        {
            pMenuItem->ShowSubMenu(FALSE);
        }
    }
}

inline BOOL CBkMenuItem::Action(NotifyInfo* pInfo)
{
    if (m_pNotify)
    {
        NotifyInfo info = *pInfo;
        m_pNotify->Action(&info);
    }
    return TRUE;
}

inline void CBkMenuItem::UpdateState(DWORD dwStateAdd, DWORD dwStateRemove)
{
    ModifyState( dwStateAdd, dwStateRemove );

    POSITION pos = m_lstWndChild.GetHeadPosition();
    while( pos )
    {
        CBkWnd* pBkWndChild = m_lstWndChild.GetNext(pos);

        if( pBkWndChild->IsAcceptParent() )
        {
            pBkWndChild->ModifyState( dwStateAdd, dwStateRemove );
        }
    } 
}

//////////////////////////////////////////////////////////////////////////
// CBkMenuBody
inline CBkMenuBody::CBkMenuBody()
{

}

inline CBkMenuBody::~CBkMenuBody()
{
    POSITION pos = m_lstTemplate.GetHeadPosition();
    while (pos != NULL)
    {
        TiXmlElement *pTemplate = m_lstTemplate.GetNext(pos);
        delete pTemplate;
    }
    m_lstTemplate.RemoveAll();
}

inline int CBkMenuBody::GetRealWidth()
{
    int realWidth = 0;

    POSITION pos = m_lstWndChild.GetHeadPosition();
    while (pos)
    {
        CBkMenuItem* pMenuItem = (CBkMenuItem*)m_lstWndChild.GetNext(pos);
        if( !pMenuItem->IsVisible() || pMenuItem->IsDisableDeep() )
            continue;

        int itemWidth = pMenuItem->GetItemWidth();
        realWidth = (itemWidth>realWidth)?itemWidth:realWidth;
    }
    return (realWidth<GetWidth())?GetWidth():realWidth;
}

inline void CBkMenuBody::HideSubMenus()
{
    POSITION pos = m_lstWndChild.GetHeadPosition();
    while (pos)
    {
        CBkMenuItem* pMenuItem = (CBkMenuItem*)m_lstWndChild.GetNext(pos);
        if (pMenuItem)
        {
            pMenuItem->ShowSubMenu(FALSE);
        }
    }
}

inline CBkMenu* CBkMenuBody::GetSubMenu(int nPos) const
{
    CBkMenuItem* pMenuItem = (CBkMenuItem*)m_lstWndChild.GetAt( m_lstWndChild.FindIndex(nPos) );  
    if (pMenuItem)
    {
        return pMenuItem->GetSubMenu();
    }
    return NULL;
}

inline int CBkMenuBody::AppendMenu(UINT nTemplateIndex, UINT_PTR nIDNewItem, int type)
{
    if ( nTemplateIndex < 0 || nTemplateIndex >= m_lstTemplate.GetCount())
    {
        return -1;
    }

    TiXmlElement* pTemplate = m_lstTemplate.GetAt( m_lstTemplate.FindIndex(nTemplateIndex) );        
    if( pTemplate )
    {
        CBkWnd* pNew = _NewItem(type, pTemplate);
        if (pNew)
        {
            pNew->SetCmdID((UINT)nIDNewItem);
            pNew->Load(pTemplate);
            m_lstWndChild.AddTail(pNew);
            return (int)m_lstWndChild.GetCount() - 1;;
        }
    }
    return -1;
}

inline int CBkMenuBody::DeleteMenu(UINT nIDItem, UINT nFlag)
{
	CBkMenuItem* pMenuItem = NULL;
	if (MF_BYPOSITION == (MF_BYPOSITION & nFlag))
	{
		pMenuItem = dynamic_cast<CBkMenuItem*>(GetItemByIndex(nIDItem));
	}
	else if (MF_BYCOMMAND == (MF_BYCOMMAND & nFlag))
	{
		pMenuItem = dynamic_cast<CBkMenuItem*>(GetItem(nIDItem));
	}

	if (pMenuItem)
	{
		m_lstWndChild.RemoveAt(m_lstWndChild.Find(pMenuItem));
		pMenuItem->OnDestroy();
		delete pMenuItem;
		return 1;
	}

	return 0;
}

inline BOOL CBkMenuBody::Action(NotifyInfo* pInfo)
{
    if (m_pNotify)
    {
        NotifyInfo info = {BKM_MENU_CLICK, CPoint(0,0), this, pInfo->dwData};
        m_pNotify->Action(&info);
    }
    return TRUE;
}

inline BOOL CBkMenuBody::LoadChilds(TiXmlElement* pTiXmlChildElem)
{
    for (TiXmlElement* pXmlChild = pTiXmlChildElem; NULL != pXmlChild; pXmlChild = pXmlChild->NextSiblingElement())
    {
        CString strNode = pXmlChild->Value();
        CBkWnd* pBkWnd = NULL;
        if (strNode == CLASSNAME_MENU_ITEM)
        {
            pBkWnd = _NewItem(Menu_Item, pXmlChild);
        }
        else if (strNode == CLASSNAME_MENU_SEPARATOR)
        {
            pBkWnd = _NewItem(Menu_Separator, pXmlChild);
        }
        else if (strNode == CLASSNAME_MENU_SUB)
        {
            pBkWnd = _NewItem(Menu_Sub, pXmlChild);
        }
        else if (strNode == CLASSNAME_MENU_TEMPLATE)
        {
            TiXmlElement* pTiElement = (TiXmlElement*)pXmlChild->Clone();
            m_lstTemplate.AddTail( pTiElement );
        }
        if (pBkWnd)
        {
            pBkWnd->Load(pXmlChild);
            m_lstWndChild.AddTail(pBkWnd);
        }
    }

    return TRUE;
}

inline void CBkMenuBody::HideOtherSubmenu(CBkMenuItem* currentItem)
{
	POSITION pos = m_lstWndChild.GetHeadPosition();
	while (pos != NULL)
	{
		CBkMenuItem *pBkWndChild = (CBkMenuItem*)m_lstWndChild.GetNext(pos);
		if (pBkWndChild && pBkWndChild != currentItem)
		{
			pBkWndChild->ShowSubMenuStart(FALSE);
			pBkWndChild->UpdateState(NULL, BkWndState_Hover | BkWndState_PushDown);
			pBkWndChild->NotifyInvalidate();
		}
	}
}

inline CBkMenuItem* CBkMenuBody::_NewItem(int type, TiXmlElement* pTiXmlChildElem)
{
    CBkMenuItem* pNewItem = new CBkMenuItem(type);
    if (pNewItem != NULL)
    {
        pNewItem->RegisterNotify(this);
        pNewItem->SetParent(m_hBkWnd);
        pNewItem->SetContainer(m_hWndContainer);
        pNewItem->SetRealContainer(m_hWndRealContainer);
        pNewItem->SetParentsVisible(IsVisible());
        pNewItem->SetTabControler(m_tabControler);
        pNewItem->SetTooltipCtrl(m_wndToolTip);
    }
    return pNewItem;
}

inline CBkMenuItem* CBkMenuBody::_NewItem(TiXmlElement* pTiXmlChildElem)
{
    CString strNode = pTiXmlChildElem->Value();
    int type = 0;
    if (strNode == CLASSNAME_MENU_ITEM)
    {
        type = Menu_Item;
    }
    else if (strNode == CLASSNAME_MENU_SEPARATOR)
    {
        type = Menu_Separator;
    }
    else if (strNode == CLASSNAME_MENU_SUB)
    {
        type = Menu_Sub;
    }
    return _NewItem(type, pTiXmlChildElem);
}

//////////////////////////////////////////////////////////////////////////
// CBkMenu
inline CBkMenu::CBkMenu()
:m_pBody(NULL)
,m_nMarginTop(0)
,m_nMarginBottom(0)
,m_nMarginLeft(0)
,m_nMarginRight(0)
,m_nMaxWidth(0)
{
    m_bAutoHide = TRUE;
}

inline CBkMenu::~CBkMenu()
{

}


inline void CBkMenu::ShowMenu(const CPoint& pt, LPCTSTR szXml)
{
    Show(szXml, FALSE);
    TrackPopupMenu(pt);
}

inline void CBkMenu::ShowSubMenu(const CPoint& pt)
{
    SIZE szMenu = GetMenuSize();
    __baseClass::SetWindowPos( NULL, pt.x, pt.y, szMenu.cx, szMenu.cy, SWP_NOZORDER|SWP_NOACTIVATE|SWP_SHOWWINDOW );
    ShowWindow(TRUE);
}

inline BOOL CBkMenu::CreateMenu(LPCTSTR szXml, HWND hParentWnd)
{
    HWND hWnd = Create( szXml,hParentWnd );
    if (hWnd)
    {
        return TRUE;
    }
    return FALSE;
}

inline BOOL CBkMenu::TrackPopupMenu(const CPoint& pt)
{
    if (IsWindow())
    {
        SIZE szMenu = GetMenuSize();

		CPoint menuPt = pt;
        CRect rcWorkAera;
        SystemParametersInfo(SPI_GETWORKAREA, NULL, &rcWorkAera, 0);
        if (menuPt.x + szMenu.cx >= rcWorkAera.right)
        {
            menuPt.x = rcWorkAera.right - szMenu.cx;
        }
        if (menuPt.y + szMenu.cy >= rcWorkAera.bottom)
        {
            menuPt.y = rcWorkAera.bottom - szMenu.cy;
        }

        __baseClass::SetWindowPos( NULL, menuPt.x, menuPt.y, szMenu.cx, szMenu.cy, SWP_NOZORDER/*|SWP_NOACTIVATE*/|SWP_SHOWWINDOW );
    }
    return FALSE;
}

inline CBkMenu* CBkMenu::GetSubMenu(int nPos) const
{
    if (m_pBody)
    {
        return m_pBody->GetSubMenu(nPos);
    }
    return NULL;
}

inline CBkMenu* CBkMenu::GetSubMenu(UINT nIDItem, UINT nFlags)
{
	CBkMenuItem* item = GetMenuItem(nIDItem, nFlags);
	if (item)
	{
		return item->GetSubMenu();
	}
    return NULL;
}

inline int CBkMenu::AppendMenu(UINT nTemplateIndex, UINT_PTR nIDNewItem /*= 0*/, int type/* = Menu_Item*/)
{
    if (m_pBody)
    {
        return m_pBody->AppendMenu(nTemplateIndex, nIDNewItem, type);
    }
    return -1;
}

inline int CBkMenu::DeleteMenu(UINT nIDItem, UINT nFlag)
{
	if (m_pBody)
	{
		return m_pBody->DeleteMenu(nIDItem, nFlag);
	}
	return -1;
}

inline UINT CBkMenu::CheckMenuItem(UINT nIDCheckItem, UINT nCheck)
{
    UINT lastCheck = 0;
    if (m_pBody)
    {
        CBkMenuItem* pMenuItem = NULL;
        if (MF_BYPOSITION == (MF_BYPOSITION & nCheck))
        {
            pMenuItem = dynamic_cast<CBkMenuItem*>(m_pBody->GetItemByIndex(nIDCheckItem));
        }
        else if (MF_BYCOMMAND == (MF_BYCOMMAND & nCheck))
        {
            pMenuItem = dynamic_cast<CBkMenuItem*>(m_pBody->GetItem(nIDCheckItem));
        } 

        if (pMenuItem)
        {
            DWORD dwState = pMenuItem->GetState();
            if (pMenuItem->IsChecked())
            {
                lastCheck |= MF_CHECKED;
            }
            else
            {
                lastCheck |= MF_UNCHECKED;
            }

            if (MF_CHECKED == (MF_CHECKED & nCheck))
            {
                pMenuItem->SetChecked( TRUE );
            }
            else if (MF_UNCHECKED == (MF_UNCHECKED & nCheck))
            {
                pMenuItem->SetChecked( FALSE );
            }
            return lastCheck;
        }
    }
    lastCheck = 0xFFFFFFFF;
    return lastCheck;
}

inline BOOL CBkMenu::CheckMenuRadioItem(UINT nIDFirst, UINT nIDLast, UINT nIDItem, UINT nFlags)
{
    if (m_pBody)
    {
        for (UINT i = nIDFirst; i < nIDLast; ++i )
        {
            BOOL bCheckItem = FALSE;

            CBkMenuItem* pMenuItem = NULL;
            if (MF_BYPOSITION == (MF_BYPOSITION & nFlags))
            {
                pMenuItem = dynamic_cast<CBkMenuItem*>(m_pBody->GetItemByIndex(i));
                bCheckItem = (i == nIDItem)?TRUE:FALSE;
            }
            else if (MF_BYCOMMAND == (MF_BYCOMMAND & nFlags))
            {
                pMenuItem = dynamic_cast<CBkMenuItem*>(m_pBody->GetItem(i));
                bCheckItem = (pMenuItem->GetCmdID() == nIDItem)?TRUE:FALSE;
            }

            pMenuItem->SetChecked( bCheckItem );
        }
        return TRUE;
    }

    return FALSE;
}

inline UINT CBkMenu::EnableMenuItem(UINT nIDEnableItem, UINT nEnable)
{
    UINT lastEnable = 0;
    if (m_pBody)
    {
        CBkMenuItem* pMenuItem = NULL;
        if (MF_BYPOSITION == (MF_BYPOSITION & nEnable))
        {
            pMenuItem = dynamic_cast<CBkMenuItem*>(m_pBody->GetItemByIndex(nIDEnableItem));
        }
        else if (MF_BYCOMMAND == (MF_BYCOMMAND & nEnable))
        {
            pMenuItem = dynamic_cast<CBkMenuItem*>(m_pBody->GetItem(nIDEnableItem));
        }

        if (pMenuItem)
        {
            DWORD dwState = pMenuItem->GetState();
            if (pMenuItem->IsDisable())
            {
                lastEnable |= MF_DISABLED;
            }
            else
            {
                lastEnable |= MF_ENABLED;
            }

            if (MF_DISABLED == (MF_DISABLED & nEnable))
            {
                pMenuItem->SetDisable( TRUE );
            }
            else if (MF_ENABLED == (MF_ENABLED & nEnable))
            {
                pMenuItem->SetDisable( FALSE );
            }
            return lastEnable;
        }
    }

    lastEnable = 0xFFFFFFFF;
    return lastEnable;
}

inline SIZE CBkMenu::GetMenuSize()
{
    CSize szMenu(m_rcWindow.Width(), m_rcWindow.Height());
    if (m_pBody)
    {
        szMenu.cx = _GetRealWidth();
        szMenu.cy = _GetRealHeight();
    }
    return szMenu;
}

inline int CBkMenu::GetMenuItemCount() const
{
    if (m_pBody)
    {
        return m_pBody->_GetItemCount();
    }
    return 0;
}

inline IBkWnd* CBkMenu::GetMenuItemChild(int nMenuIndex, int nChildIndex)
{
    if (m_pBody)
    {
        CBkMenuItem* pMenuItem = dynamic_cast<CBkMenuItem*>(m_pBody->GetItemByIndex(nMenuIndex));
        if (pMenuItem)
        {
            return pMenuItem->GetItemByIndex(nChildIndex);
        }
    }
    return NULL;
}

inline CBkMenuItem* CBkMenu::GetMenuItem(UINT nPosition, UINT nFlags)
{
	if (m_pBody)
	{
		if (MF_BYPOSITION == (MF_BYPOSITION & nFlags))
		{
			return dynamic_cast<CBkMenuItem*>(m_pBody->GetItemByIndex(nPosition));
		}
		else if (MF_BYCOMMAND == (MF_BYCOMMAND & nFlags))
		{
			return dynamic_cast<CBkMenuItem*>(m_pBody->GetItem(nPosition));
		}
	}

	return NULL;
}


inline BOOL CBkMenu::Action(NotifyInfo* pInfo)
{
    Hide();
    if (m_pNotify)
    {
        NotifyInfo info = *pInfo;
        info.pFrom = this;
        m_pNotify->Action(&info);
    }
    return TRUE;
}

inline BOOL CBkMenu::LoadChilds(TiXmlElement* pTiXmlChildElem)
{
    for (TiXmlElement* pXmlChild = pTiXmlChildElem; NULL != pXmlChild; pXmlChild = pXmlChild->NextSiblingElement())
    {
        CString strNode = pXmlChild->Value();
        if (strNode == CLASSNAME_MENU_BODY)
        {
            m_pBody = new CBkMenuBody();
            if (m_pBody)
            {
                m_pBody->RegisterNotify(this);
                m_pBody->SetParent(m_hBkWnd);
                m_pBody->SetContainer(m_hWndContainer);
                m_pBody->SetRealContainer(m_hWndRealContainer);
                m_pBody->SetParentsVisible(IsVisible());
                m_pBody->SetTabControler(m_tabControler);
                m_pBody->SetTooltipCtrl(m_wndToolTip);
				m_pBody->SetDropTarget(m_pDropTarget);
                m_pBody->Load(pXmlChild);
                m_lstWndChild.AddTail(m_pBody);
            }
        }
    }

    return TRUE;
}

inline void CBkMenu::OnShown(BOOL bShow)
{
    if (!bShow)
    {
        _HideSubMenus();
    }
}

inline int CBkMenu::_GetRealHeight()
{
    if (m_pBody)
    {
        int margin = 0;
        BkStyle* pStyle = m_pBody->GetStyle();
        if (pStyle)
        {
            margin += pStyle->m_nMarginY;
        }
        return m_pBody->GetRealHeight() + margin*2 + m_nMarginTop + m_nMarginBottom;
    }
    return 0;
}

inline int CBkMenu::_GetRealWidth()
{
    if (m_pBody)
    {
        int margin = 0;
        BkStyle* pStyle = m_pBody->GetStyle();
        if (pStyle)
        {
            margin += pStyle->m_nMarginX;
        }
        int realWidth = m_pBody->GetRealWidth() + margin*2 + m_nMarginLeft + m_nMarginRight;
        return m_nMaxWidth?((realWidth>m_nMaxWidth)?m_nMaxWidth:realWidth):realWidth;
    }
    return 0;
}

inline void CBkMenu::_HideSubMenus()
{
    if (m_pBody)
    {
        m_pBody->HideSubMenus();
    }
}

inline BOOL CBkMenu::_OnLButtonDown(UINT nFlags, CPoint point)
{
	

	return CBkPanel::OnLButtonDown(nFlags, point);
}

inline BOOL CBkMenu::_OnMouseMove(UINT dwKey, CPoint point)
{
	TRACKMOUSEEVENT csTME;
	csTME.cbSize = sizeof(csTME);
	csTME.dwFlags = TME_HOVER | TME_LEAVE;
	csTME.hwndTrack = m_hWnd;
	csTME.dwHoverTime = 1;
	::_TrackMouseEvent( &csTME );
	return CBkFloatingWindow::_OnMouseMove(dwKey, point);
}

inline BOOL CBkMenu::_OnMouseHover(UINT_PTR key, CPoint point)
{
	if (!m_bCaptureSetted)
	{
		__baseClass::SetCapture();
		m_bCaptureSetted = TRUE;
	}
	return TRUE;
}

inline BOOL CBkMenu::_OnMouseLeave()
{
	if (m_bCaptureSetted)
	{
		ReleaseCapture();
		m_bCaptureSetted = FALSE;
	}
	return TRUE;
}

