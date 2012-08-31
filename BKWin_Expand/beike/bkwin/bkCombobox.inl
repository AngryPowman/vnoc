//////////////////////////////////////////////////////////////////////////
// CBkComboboxDropDown
inline CBkComboboxDropDown::CBkComboboxDropDown()
:m_pList(NULL)
,m_nMaxHeight(0)
{
}

inline CBkComboboxDropDown::~CBkComboboxDropDown()
{

}

inline void CBkComboboxDropDown::ShowDropDown( const CPoint& ptShow, BOOL bShow ,BOOL bActiveFocus/*=TRUE*/)
{
    ShowWindow( bShow );

    if( bShow )
    {
        DWORD dwNoSize = SWP_NOSIZE;
        CSize szReal;
        if( m_nMaxHeight > 0 )
        {
            dwNoSize = 0;
            szReal.cx = GetWidth();
            szReal.cy = _GetRealHeight();
            if( szReal.cy > m_nMaxHeight )
            {
                szReal.cy = m_nMaxHeight;
            }
        }
		DWORD flag = dwNoSize|SWP_NOZORDER;
		if (!bActiveFocus)
		{
			flag |= SWP_NOACTIVATE;
		}
        __baseClass::SetWindowPos( NULL, ptShow.x, ptShow.y, szReal.cx, szReal.cy, flag );
    }
}

inline int CBkComboboxDropDown::AddString(LPCTSTR szText)
{
    int index = AddItem();
    SetItemText(index, szText );
    return index;
}

inline int CBkComboboxDropDown::FindString(LPCTSTR szText)
{
    if( m_pList )
    {
        //return m_pList->FindRow(LIST_FIND_TEXT,(DWORD)szText);
        for( int i = 0; i < m_pList->GetRowCount(); ++i )
        {
            if( wcscmp(m_pList->GetItemText( i, 0, 0 ), szText) == 0 )
            {
                return i;
            }
        }
    }
    return -1;
}

inline int CBkComboboxDropDown::SelectString(LPCTSTR szText)
{
    if( m_pList )
    {
		int index = m_pList->FindRow(LIST_FIND_TEXT,(DWORD)szText);
		if( index >= 0 && index < GetCount() )
		{
			SetCurSel( index );
			return index;
		}
    }
    return -1;
}

inline int CBkComboboxDropDown::AddItem()
{
    if( m_pList )
    {
        return m_pList->AddRow();
    }

    return -1;
}

inline int CBkComboboxDropDown::InsertItem(int nItem)
{
    if( m_pList )
    {
        return m_pList->InsertRow( nItem );
    }
    return -1;
}

inline BOOL CBkComboboxDropDown::DeleteItem(int nItem)
{
    if( m_pList )
    {
        return m_pList->DeleteRow(nItem);
    }
    return FALSE;
}

inline BOOL CBkComboboxDropDown::DeleteAllItems()
{
    if( m_pList )
    {
        return m_pList->DeleteAllRows();
    }
    return FALSE;
}

inline BOOL CBkComboboxDropDown::SetItemText(int nItem, LPCTSTR szText)
{
    if( m_pList )
    {
        BOOL bRet = m_pList->SetItemText( nItem, 0, 0, szText );
        CBkWnd* pText = dynamic_cast<CBkWnd*>(m_pList->GetItem( nItem, 0, 0 ));
        if( pText )
        {
            pText->UpdateTooltipText( szText );
        }
        return bRet;
    }
    return FALSE;
}

inline LPCWSTR CBkComboboxDropDown::GetItemText(int nItem )
{
    if( m_pList )
    {
        return m_pList->GetItemText( nItem, 0, 0 );
    }
    return NULL;
}

inline BOOL CBkComboboxDropDown::SetItemData(int nItem, DWORD dwData)
{
    if( m_pList )
    {
        return m_pList->SetRowData( nItem, dwData );
    }
    return FALSE;
}

inline DWORD CBkComboboxDropDown::GetItemData(int nItem)
{
    if( m_pList )
    {
        return m_pList->GetRowData( nItem );
    }
    return 0;
}

inline BOOL CBkComboboxDropDown::SetCurSel(int nItem)
{
    if( m_pList )
    {
        return m_pList->SelectRow( nItem, TRUE );
    }
    return FALSE;
}

inline int CBkComboboxDropDown::GetCurSel()
{
    if( m_pList )
    {
        return m_pList->GetSelectRow();
    }
    return FALSE;
}

inline int CBkComboboxDropDown::GetCount()
{
    if( m_pList )
    {
        return m_pList->GetRowCount();
    }
    return FALSE;
}

inline void CBkComboboxDropDown::SortItems( ISortCompare* compare )
{
    if( m_pList )
    {
        m_pList->SortRows(compare);
    }
}

inline IBkWnd* CBkComboboxDropDown::GetSubItem(int nItem, int subIndex)
{
    IBkWnd* pSubItem = NULL;
    if( m_pList )
    {
        pSubItem = m_pList->GetItem(nItem, 0, subIndex);
    }
    return pSubItem;
}

inline BOOL CBkComboboxDropDown::Load(TiXmlElement* pTiXmlElem)
{
    CBkFloatingWindow::Load( pTiXmlElem );

    WINDOWPOS WndPos = {0, 0, 0, 0, m_rcWindow.Width(), m_rcWindow.Height(), SWP_SHOWWINDOW};
    OnWindowPosChanged(&WndPos);

    return TRUE;
}

inline BOOL CBkComboboxDropDown::LoadChilds(TiXmlElement* pTiXmlChildElem)
{
    _CreateList();

    for (TiXmlElement* pXmlChild = pTiXmlChildElem; NULL != pXmlChild; pXmlChild = pXmlChild->NextSiblingElement())
    {
        CString strNode = pXmlChild->Value();
        CBkWnd* pBkWnd = NULL;
        if (strNode == CBkList::GetClassName())
        {
            m_pList->Load(pXmlChild);
        }
    }

    return TRUE;
}

inline BOOL CBkComboboxDropDown::Action(NotifyInfo* pInfo)
{
    if( pInfo->pFrom == m_pList )
    {
        switch( pInfo->nAction )
        {
        case BKM_LIST_SELCHANGED:
            ShowWindow( FALSE );
            if( m_pNotify )
            {
                int indexCurSel = m_pList->GetSelectRow();
                NotifyInfo info = { BKM_COMBOBOX_SELCHANGED, CPoint(0, 0), this, indexCurSel };
                m_pNotify->Action( &info );
            }
            break;
        }
    }
    return TRUE;
}

inline BOOL CBkComboboxDropDown::_OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    BOOL bRet = CBkFloatingWindow::_OnKeyDown(nChar, nRepCnt, nFlags);
    
    switch( nChar )
    {
    case VK_RETURN:
        ShowWindow( FALSE );
        break;
    }

    return bRet;
}

inline void CBkComboboxDropDown::_OnKillFocus( HWND pNewWnd )
{
    CBkFloatingWindow::_OnKillFocus(pNewWnd);
    ShowWindow( FALSE );
}

inline BOOL CBkComboboxDropDown::_OnLButtonUp(UINT nFlags, CPoint point)
{
    BOOL bRet = CBkFloatingWindow::_OnLButtonUp(nFlags,point);
    //ShowWindow( FALSE );
    return bRet;
}

inline BOOL CBkComboboxDropDown::_OnMouseMove(UINT nFlags, CPoint point)
{
    if( m_pList )
    {
        m_pList->ClearSelectRow();
    }
    return CBkFloatingWindow::OnMouseMove(nFlags, point);
}


inline void CBkComboboxDropDown::_CreateList()
{
    if( !m_pList )
    {
        m_pList = new CBkList();

        m_pList->RegisterNotify(this);
        m_pList->SetParent(m_hBkWnd);
        m_pList->SetContainer(m_hWndContainer);
        m_pList->SetRealContainer(m_hWndRealContainer);
        m_pList->SetParentsVisible(IsVisible());
        m_pList->SetTabControler(m_tabControler);
        m_pList->SetTooltipCtrl(m_wndToolTip);
		m_pList->SetDropTarget(m_pDropTarget);
        m_lstWndChild.AddTail(m_pList);
    }
}

inline int CBkComboboxDropDown::_GetRealHeight()
{
    if( m_pList )
    {
        int margin = 0;
        BkStyle* pStyle = m_pList->GetStyle();
        if( pStyle )
        {
            margin += pStyle->m_nMarginY;
        }
        return m_pList->GetRealHeight() + margin*2;
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////////
// CBkCombobox
inline CBkCombobox::CBkCombobox()
:m_pDropDown(NULL)
,m_pBtnDropDown(NULL)
,m_pEdit(NULL)
,m_pText(NULL)
,m_nType(DropDownList)
,m_nWidthBtnDropDown(24)
,m_nHeightDropDown(0)
{
    m_szMargin.cx = m_szMargin.cy = 0;
}

inline CBkCombobox::~CBkCombobox()
{

}

inline int CBkCombobox::AddString(LPCTSTR lpszString)
{
    if( m_pDropDown )
    {
        return m_pDropDown->AddString(lpszString);
    }
    return -1;
}

inline int CBkCombobox::FindString(LPCTSTR szText)
{
    if( m_pDropDown )
    {
        return m_pDropDown->FindString(szText);
    }
    return -1;
}

inline int CBkCombobox::SelectString(LPCTSTR szText)
{
	int nRes = -1;
	if( m_pDropDown )
	{
		nRes = m_pDropDown->SelectString(szText);
		if ( nRes >=0 )
		{
			m_pEdit->SetText(szText);
		}
	}
	return nRes;
}

inline int CBkCombobox::AddItem()
{
    if( m_pDropDown )
    {
        int index = m_pDropDown->AddItem();

        //switch( m_nType )
        //{
        //case DropDown:
        //    m_pEdit->SetText( GetItemText( index ) );
        //    break;
        //case DropDownList:
        //    m_pText->SetText( GetItemText( index ) );
        //    break;
        //}
        return index;
    }
    return -1;
}

inline int CBkCombobox::InsertItem(int nItem)
{
    if( m_pDropDown )
    {
        return m_pDropDown->InsertItem( nItem );
    }
    return -1;
}

inline BOOL CBkCombobox::DeleteItem(int nItem)
{
    if( m_pDropDown )
    {
        return m_pDropDown->DeleteItem(nItem);
    }
    return FALSE;
}

inline BOOL CBkCombobox::DeleteAllItems()
{
    if( m_pDropDown )
    {
        return m_pDropDown->DeleteAllItems();
    }
    return FALSE;
}

inline BOOL CBkCombobox::SetItemText(int nItem, LPCTSTR szText)
{
    if( m_pDropDown )
    {
        return m_pDropDown->SetItemText( nItem, szText );
    }
    return FALSE;
}

inline LPCWSTR CBkCombobox::GetItemText(int nItem )
{
    if( m_pDropDown )
    {
        return m_pDropDown->GetItemText( nItem );
    }
    return NULL;
}

inline BOOL CBkCombobox::SetItemData(int nItem, DWORD dwData)
{
    if( m_pDropDown )
    {
        return m_pDropDown->SetItemData( nItem, dwData );
    }
    return FALSE;
}

inline DWORD CBkCombobox::GetItemData(int nItem)
{
    if( m_pDropDown )
    {
        return m_pDropDown->GetItemData( nItem );
    }
    return 0;
}

inline BOOL CBkCombobox::SetCurSel(int nItem)
{
    if( m_pDropDown )
    {
        switch( m_nType )
        {
        case DropDown:
            m_pEdit->SetText( GetItemText( nItem ) );
            break;
        case DropDownList:
            m_pText->SetText( GetItemText( nItem ) );
            m_pText->UpdateTooltipText( GetItemText( nItem ) );
            break;
        }

        BOOL bRet = m_pDropDown->SetCurSel( nItem );
        if( m_pNotify )
        {
            NotifyInfo info = { BKM_COMBOBOX_SELCHANGED, CPoint(0, 0), this, nItem };
            m_pNotify->Action( &info );
        }
        return bRet;
    }
    return FALSE;
}

inline int CBkCombobox::GetCurSel()
{
    if( m_pDropDown )
    {
        return m_pDropDown->GetCurSel();
    }
    return FALSE;
}

inline int CBkCombobox::GetCount()
{
    if( m_pDropDown )
    {
        return m_pDropDown->GetCount();
    }
    return FALSE;
}

inline void CBkCombobox::SortItems( ISortCompare* compare )
{
    if( m_pDropDown )
    {
        m_pDropDown->SortItems(compare);
    }
}

inline LPCWSTR CBkCombobox::GetEditText()
{
    if( DropDown == m_nType && m_pEdit )
    {
        return m_pEdit->GetText();
    }
    return NULL;
}

inline BOOL CBkCombobox::LimitText(int nMaxChars)
{
    if( DropDown == m_nType && m_pEdit )
    {
        m_pEdit->LimitText(nMaxChars);
        return TRUE;
    }
    return FALSE;
}

inline BOOL CBkCombobox::SetEditSel(int nStartChar, int nEndChar)
{
    if( DropDown == m_nType && m_pEdit )
    {
        m_pEdit->SetSel(nStartChar, nEndChar);
        return TRUE;
    }
    return FALSE;
}

inline IBkWnd* CBkCombobox::GetSubItem(int nItem, int subIndex)
{
    if( m_pDropDown )
    {
        return m_pDropDown->GetSubItem(nItem, subIndex);
    }
    return NULL;
}

inline void CBkCombobox::SetDisable(BOOL bDisable)
{
	if (m_pEdit)
	{
		m_pEdit->SetDisable(bDisable);
	}
	CBkImageBtnWnd::SetDisable(bDisable);
}

inline void CBkCombobox::ShowList( BOOL bShow )
{
	CPoint pt;
	if (bShow)
	{
		pt.x = m_rcWindow.left;
		pt.y = m_rcWindow.bottom;
		::ClientToScreen( GetContainer(), &pt );
	}
	m_pDropDown->ShowDropDown(pt,bShow,FALSE);
}

inline void CBkCombobox::CreateHandle(HWND hParent)
{
    CBkImageBtnWnd::CreateHandle( hParent );
}

inline BOOL CBkCombobox::LoadChilds(TiXmlElement* pTiXmlChildElem)
{
    _CreateDropDownList();
    _CreateDropDownButton();
    _CreateEdit();
    _CreateText();

    for (TiXmlElement* pXmlChild = pTiXmlChildElem; NULL != pXmlChild; pXmlChild = pXmlChild->NextSiblingElement())
    {
        CString strNode = pXmlChild->Value();
        CBkWnd* pBkWnd = NULL;
        if (strNode == CBkComboboxDropDown::GetClassName())
        {
            m_pDropDown->Load(pXmlChild);
        }
    }

    switch( m_nType )
    {
    case DropDown:
        m_pEdit->SetVisible(TRUE);
        m_pText->SetVisible(FALSE);
        break;
    case DropDownList:
        m_pEdit->SetVisible(FALSE);
        m_pText->SetVisible(TRUE);
        break;
    }

    return TRUE;
}

inline BOOL CBkCombobox::RedrawRegion( CDCHandle& dc )
{
    return CBkImageBtnWnd::RedrawRegion( dc );
}

inline CBkWnd* CBkCombobox::OnSetFocus( CBkWnd* which )
{
    m_focusChild = which;
    CBkImageBtnWnd::OnSetFocus(this);
    return this;
}

inline BOOL CBkCombobox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    BOOL bRet = CBkImageBtnWnd::OnKeyDown(nChar, nRepCnt, nFlags);
    
    switch( nChar )
    {
    case VK_DOWN:
        if( m_pDropDown && !m_pDropDown->IsWindowVisible() )
        {
            CPoint pt;
            pt.x = m_rcWindow.left;
            pt.y = m_rcWindow.bottom;
            ::ClientToScreen( GetContainer(), &pt );
            m_pDropDown->ShowDropDown( pt, TRUE );
        }
        break;
    }
    return bRet;
}

inline BOOL CBkCombobox::Action(NotifyInfo* pInfo)
{
    if( pInfo->pFrom == m_pBtnDropDown )
    {
        switch( pInfo->nAction )
        {
        case WM_LBUTTONUP:
            if( m_pDropDown )
            {
                CPoint pt;
                pt.x = m_rcWindow.left;
                pt.y = m_rcWindow.bottom;
                ::ClientToScreen( GetContainer(), &pt );
                m_pDropDown->ShowDropDown( pt, TRUE );
            }
            break;
        }
    }
    else if( pInfo->pFrom == m_pDropDown )
    {
        switch( pInfo->nAction )
        {
        case BKM_COMBOBOX_SELCHANGED:
            {
                int indexCurSel = pInfo->dwData;
                SetCurSel( indexCurSel );
            }
            break;
        }
    }
    else if( DropDown == m_nType && pInfo->pFrom == m_pEdit )
    {
        if( m_pNotify )
        {
            BOOL bNotifyEdit = TRUE;
            NotifyInfo info = *pInfo;
            info.pFrom = this;
            switch( info.nAction )
            {
            case BKM_EDIT_PRESSENTER:
                info.nAction = BKM_COMBOBOX_EDITENTER;
                break;
            case BKM_EDIT_TEXTCHANGE:
                info.nAction = BKM_COMBOBOX_EDITCHANGED;
                break;
			case WM_KILLFOCUS:
				if (m_pDropDown)
				{
					CPoint pt;
					m_pDropDown->ShowDropDown(pt,FALSE);
				}
				break;
            default:
                bNotifyEdit = FALSE;
                break;
            }
            if( bNotifyEdit )
            {
                m_pNotify->Action( &info );
            }
        }
    }
    return TRUE;
}

inline void CBkCombobox::_CreateDropDownList()
{
    if( !m_pDropDown )
    {
        m_pDropDown = new CBkComboboxDropDown();
        m_pDropDown->RegisterNotify(this);
        m_pDropDown->SetContainer(m_hWndContainer);
		m_pDropDown->SetDropTarget(m_pDropTarget);
        m_pDropDown->SetRealContainer(m_hWndRealContainer);
    }
}

inline void CBkCombobox::_CreateDropDownButton()
{
    if( !m_pBtnDropDown )
    {
        m_pBtnDropDown = new CBkImageBtnWnd();

        BKDLG_POSITION pos = {};
        pos.nType = 4;
        pos.Left.bMinus = TRUE;
        pos.Left.nPos = m_nWidthBtnDropDown + m_szMargin.cx;
        pos.Right.bMinus = TRUE;
        pos.Right.nPos = m_szMargin.cx;
        pos.Top.nPos = m_szMargin.cy;
        pos.Bottom.bMinus = TRUE;
        pos.Bottom.nPos = m_szMargin.cy;
        m_pBtnDropDown->SetPos( &pos, FALSE );

        if( !m_strSkinBtnDropDown.IsEmpty() )
        {
            CBkSkinBase* pSkinDropDown = BkSkin::GetSkin(m_strSkinBtnDropDown);
            m_pBtnDropDown->SetSkin( pSkinDropDown );
        }

        m_pBtnDropDown->RegisterNotify(this);
        m_pBtnDropDown->SetParent(m_hBkWnd);
        m_pBtnDropDown->SetContainer(m_hWndContainer);
		m_pBtnDropDown->SetDropTarget(m_pDropTarget);
        m_pBtnDropDown->SetRealContainer(m_hWndRealContainer);
        m_pBtnDropDown->SetParentsVisible(IsVisible());
        m_pBtnDropDown->SetTabControler(m_tabControler);
        m_pBtnDropDown->SetTooltipCtrl(m_wndToolTip);
        m_pBtnDropDown->CreateHandle( m_hWndContainer );
        //m_pBtnDropDown->Load(pXmlChild);
        m_lstWndChild.AddTail(m_pBtnDropDown);
    }
}

inline void CBkCombobox::_CreateEdit()
{
    if( !m_pEdit )
    {
        m_pEdit = new CBkEdit();

        BKDLG_POSITION pos = {};
        pos.nType = 4;
        pos.Left.nPos = m_szMargin.cx;
        pos.Right.bMinus = TRUE;
        pos.Right.nPos = m_nWidthBtnDropDown + m_szMargin.cx;
        pos.Top.nPos = m_szMargin.cy;
        pos.Bottom.bMinus = TRUE;
        pos.Bottom.nPos = m_szMargin.cy;
        m_pEdit->SetPos( &pos, FALSE );

        m_pEdit->SetAttribute( "multiline", "0", FALSE );

        if( !m_strSkinEdit.IsEmpty() )
        {
            m_pEdit->SetStyle( m_strSkinEdit );
        }
      
        m_pEdit->RegisterNotify(this);
        m_pEdit->CBkWnd::SetParent(m_hBkWnd);
        m_pEdit->SetContainer(m_hWndContainer);
		m_pEdit->SetDropTarget(m_pDropTarget);
        m_pEdit->SetRealContainer(m_hWndRealContainer);
        m_pEdit->SetParentsVisible(IsVisible());
        m_pEdit->SetTabControler(m_tabControler);
        m_pEdit->SetTooltipCtrl(m_wndToolTip);
        m_pEdit->CreateHandle( m_hWndContainer );
        //m_pEdit->Load(pXmlChild);
        m_lstWndChild.AddTail(m_pEdit);
    }
}

inline void CBkCombobox::_CreateText()
{
    if( !m_pText )
    {
        m_pText = new CBkText();

        BKDLG_POSITION pos = {};
        pos.nType = 4;
        pos.Left.nPos = m_szMargin.cx;
        pos.Right.bMinus = TRUE;
        pos.Right.nPos = m_nWidthBtnDropDown + m_szMargin.cx;
        pos.Top.nPos = m_szMargin.cy;
        pos.Bottom.bMinus = TRUE;
        pos.Bottom.nPos = m_szMargin.cy;
        m_pText->SetPos( &pos, FALSE );

        if( !m_strSkinText.IsEmpty() )
        {
            m_pText->SetStyle( m_strSkinText );
        }

        m_pText->RegisterNotify(this);
        m_pText->SetParent(m_hBkWnd);
        m_pText->SetContainer(m_hWndContainer);
        m_pText->SetRealContainer(m_hWndRealContainer);
        m_pText->SetParentsVisible(IsVisible());
        m_pText->SetTabControler(m_tabControler);
        m_pText->SetTooltipCtrl(m_wndToolTip);
        m_pText->CreateHandle( m_hWndContainer );
        //m_pText->Load(pXmlChild);
        m_lstWndChild.AddTail(m_pText);
    }
}


