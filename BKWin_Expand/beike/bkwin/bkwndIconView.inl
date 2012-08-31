
#include <utility/ExecutetTime.h>

/////////////////////////////////
////  BkIconView
/////////////////////////////////
inline CBkIconView::CBkIconView()
{
    m_nScrollOffset = 0;
    m_pBody = NULL;
    m_pVScroll = NULL;
}

inline CBkIconView::~CBkIconView()
{
}

// IDropContainer members
inline HRESULT CBkIconView::OnDragEnter(IDataObject* pDataObj, POINT pt, DWORD *pdwEffect)
{
	return __super::OnDragEnter(pDataObj, pt, pdwEffect);
}

inline HRESULT CBkIconView::OnDragOver(POINT pt, DWORD *pdwEffect)
{
	return __super::OnDragOver(pt ,pdwEffect);
}

inline HRESULT CBkIconView::OnDrop(IDataObject *pDataObj, POINT pt, DWORD *pdwEffect)
{
	return __super::OnDrop(pDataObj,pt,pdwEffect);
}

inline HRESULT CBkIconView::OnDragLeave( void)
{
	return __super::OnDragLeave();
}
inline int CBkIconView::AddItem( BOOL isRedraw/* = TRUE*/, BOOL isReCalc/* = TRUE*/ )
{
    ATLASSERT( m_pBody != NULL );
    int iRet = m_pBody->AddItem( isRedraw, isReCalc );
    if( isReCalc )
    {
        _CalcSize();
    }
    return iRet;
}

inline int CBkIconView::AddItemFromTemplate( int templateIndex, BOOL isRedraw/* = TRUE*/, BOOL isCalc/* = TRUE*/ )
{
    ATLASSERT( m_pBody != NULL );
    return m_pBody->AddItemFromTemplate( templateIndex, isRedraw, isCalc );
}

inline int CBkIconView::InsertItem( int nItem, BOOL isReCalc/* = FALSE*/ )
{
    ATLASSERT( m_pBody != NULL );
    int iRet = m_pBody->InsertItem( nItem );
    if( isReCalc )
    {
        _CalcSize();
    }
    return iRet;
}

inline int	CBkIconView::DeleteItem(int nItem)
{
	ATLASSERT( m_pBody != NULL );

    int itemShowTop = m_pBody->GetShowTop();
    int itemRealHeight = m_pBody->GetRealHeight();
    double delta = 0.0;
    if( itemRealHeight > 0 )
    {
        delta = (double)itemShowTop/(double)itemRealHeight;
    }

	int iRet = m_pBody->DeleteItem(nItem);
	_CalcSize();

    itemRealHeight = m_pBody->GetRealHeight();
    itemShowTop = itemRealHeight * delta;

    CRect rcBody;
    m_pBody->GetRect( &rcBody );
    if( itemRealHeight <= rcBody.Height() )
    {
        itemShowTop = 0;
    }

    m_pBody->SetShowTop( itemShowTop );
    m_pVScroll->SetValue( itemShowTop );

	return iRet;
}

inline void CBkIconView::DeleteItems(int nItemBegin, int nItemEnd)
{
    ATLASSERT( m_pBody != NULL );
    m_pBody->DeleteItems(nItemBegin, nItemEnd);
    m_pVScroll->SetValue(0);
    _CalcSize();
    NotifyInvalidate();
}

inline void CBkIconView::DeleteAllItems()
{
	ATLASSERT( m_pBody != NULL );
	m_pBody->DeleteAllItems();
    m_pVScroll->SetValue(0);
	_CalcSize();
    NotifyInvalidate();
}

inline IBkWnd* CBkIconView::GetItemEx(int nIndex, int nSubItemID)
{
    ATLASSERT( m_pBody != NULL );
    return m_pBody->GetItemEx(nIndex, nSubItemID);
}

inline BOOL CBkIconView::SetItemData(int nItem, DWORD dwData)
{
    ATLASSERT( m_pBody != NULL );
    return m_pBody->SetItemData( nItem, dwData );
}

inline DWORD CBkIconView::GetItemData(int nItem)
{
    ATLASSERT( m_pBody != NULL );
    return m_pBody->GetItemData( nItem );
}

inline CRect CBkIconView::GetItemRect(int nItem)
{
    ATLASSERT( m_pBody != NULL );
    return m_pBody->GetItemRect( nItem );
}

inline BOOL CBkIconView::Load(TiXmlElement* pTiXmlElem)
{
    __super::Load( pTiXmlElem );

    _CreateScrollbar();

    return TRUE;
}

inline void CBkIconView::OnChildLoad(TiXmlElement* pTiXmlElem, CBkWnd* pChildWindow)
{
    if( pTiXmlElem )
    {
        if( strcmp(CLASSNAME_ICONBODY,pTiXmlElem->Value())  == 0 )
        {
            m_pBody = new CBkIconBody();
            m_pBody->RegisterNotify(this);
            m_pBody->SetParent(m_hBkWnd);
            m_pBody->SetContainer(m_hWndContainer);
			m_pBody->SetDropTarget(m_pDropTarget);
            m_pBody->SetRealContainer(m_hWndRealContainer);
            m_pBody->SetParentsVisible(IsVisible());
            m_pBody->SetTabControler(m_tabControler);
            m_pBody->SetTooltipCtrl(m_wndToolTip);
            m_pBody->Load(pTiXmlElem);
            m_lstWndChild.AddTail(m_pBody);
        }
    }
}

inline CBkWnd* CBkIconView::OnSetFocus( CBkWnd* which )
{
    m_bFocus = TRUE;
    __super::OnSetFocus( this );
    return this;
}

inline BOOL CBkIconView::OnWindowPosChanged(LPWINDOWPOS lpWndPos)
{
    __super::OnWindowPosChanged(lpWndPos);
    _CalcSize();
    return TRUE;
}

inline BOOL CBkIconView::Action(NotifyInfo* pInfo)
{
    if (pInfo != NULL && pInfo->pFrom != NULL)
    {
        if (pInfo->nAction == BKM_LIST_MOUSEWHEEL)
        {
            m_pVScroll->SetValue(pInfo->dwData);
        }
        else if (pInfo->nAction == BKM_LIST_SCROLL_V)
        {
            m_pBody->SetShowTop(pInfo->dwData);
        }

		if (m_pNotify != NULL)
        {
            pInfo->pFrom = this;
            m_pNotify->Action(pInfo);
        }
    }
    return TRUE;
}

inline void CBkIconView::_CreateScrollbar()
{
    m_pVScroll = new CBkScroll();
    m_pVScroll->RegisterNotify(this);
    m_pVScroll->SetParent(m_hBkWnd);
    m_pVScroll->SetContainer(m_hWndContainer);
	m_pVScroll->SetDropTarget(m_pDropTarget);
    m_pVScroll->SetRealContainer(m_hWndRealContainer);
    m_pVScroll->SetVisible(FALSE);
    m_pVScroll->SetSkin(m_style.m_strScrollV);
    m_lstWndChild.AddTail(m_pVScroll);
}

inline void CBkIconView::_CalcSize( BOOL isForcePosition/* = FALSE*/ )
{
    BOOL bRepositionAll = FALSE;

    CRect rcClient;
    m_pBody->GetRect( &rcClient );

    int iBodyHeight = rcClient.Height();
    int iRealHeight = m_pBody->GetRealHeight() + m_pBody->GetItemsOffsetV();
    if( iRealHeight > iBodyHeight || m_pVScroll->NeedShow() )
    {
        BOOL bChanged = TRUE;//(iRealHeight > iCtrlHeight) != m_pVScroll->NeedShow();
        m_pVScroll->SetMax(iRealHeight);
        m_pVScroll->SetPage(iBodyHeight);
		m_pVScroll->SetValue(m_pBody->GetShowTop());

        if (bChanged)
        {
            BKDLG_POSITION pos;
            m_pBody->GetPos(&pos);
            pos.Right.nPos = (iRealHeight > iBodyHeight) ? m_pVScroll->GetWidth() : 0;
            m_pBody->SetPos(&pos, FALSE);

            pos.Left.bMinus = TRUE;
            pos.Left.nPos = m_pVScroll->GetWidth();
            pos.Right.bMinus = TRUE;
            pos.Right.nPos = 0;
            pos.Top.bMinus = FALSE;
            pos.Top.nPos += m_nScrollOffset;
            m_pVScroll->SetPos(&pos, FALSE);
            m_pVScroll->SetVisible(TRUE);
            bRepositionAll = TRUE;
        }
        else
        {
            m_pVScroll->NotifyInvalidate();
        }
    }
    else
    {
        m_pVScroll->SetVisible( FALSE );
    }

    if (bRepositionAll || isForcePosition)
    {
        RepositionChilds();
        NotifyInvalidate();
    }
}

inline int CBkIconView::GetItemCount()
{
    ATLASSERT( m_pBody != NULL );
    return m_pBody->GetItemCount();
}

inline int CBkIconView::GetVisibleItemsHeight()
{
    ATLASSERT( m_pBody != NULL );
    return m_pBody->GetVisibleItemsHeight(); 
}

inline BOOL CBkIconView::GetSelectItems(std::vector<int>& nItems)
{
    ATLASSERT( m_pBody != NULL );
    return m_pBody->GetSelectItems( nItems );
}

inline int CBkIconView::GetSelectItemsCount()
{
    ATLASSERT( m_pBody != NULL );
    return m_pBody->GetSelectItemsCount();
}

inline BOOL CBkIconView::IsItemSelected(int nItem)
{
    ATLASSERT( m_pBody != NULL );
    return m_pBody->IsItemSelected(nItem);
}

inline BOOL CBkIconView::SelectItem(int nItem,BOOL bSelect)
{
    ATLASSERT( m_pBody != NULL );
    return m_pBody->SelectItem(nItem, bSelect);
}

inline void CBkIconView::ClearSelectItems()
{
    ATLASSERT( m_pBody != NULL );
    return m_pBody->ClearSelectItems();
}

inline void CBkIconView::SortItems( ISortCompare* compare )
{
    ATLASSERT( m_pBody != NULL );
    return m_pBody->SortChildren( compare );
}

inline int CBkIconView::HitTest(CPoint point)
{
	return m_pBody ? m_pBody->HitTest(point) : -1;
}

inline int CBkIconView::FindItem(LIST_FIND_TYPE emType, DWORD dwData)
{
	return m_pBody ? m_pBody->FindItem(emType, dwData) : -1;
}

inline void CBkIconView::UpdateView()
{
    NotifyInvalidate();
}

inline void CBkIconView::UpdateItemsPos()
{
    _CalcSize(TRUE);
}

inline void CBkIconView::ScrollToTop()
{
    if( m_pVScroll && m_pVScroll->IsVisible() )
    {
        m_pVScroll->Scroll( SB_TOP, 0 );
    }
}

inline void CBkIconView::ScrollToBottom()
{
    if( m_pVScroll && m_pVScroll->IsVisible() )
    {
        m_pVScroll->Scroll( SB_BOTTOM, 0 );
    }
}

inline void CBkIconView::ScrollToItem(int nItem)
{
    if( m_pVScroll && m_pVScroll->IsVisibleDeep(FALSE) && !m_pBody->IsItemVisible(nItem) )
    {
        //CRect rcItem = m_pBody->GetItemRect( nItem );
        //if( rcItem.Width() <= 0 || rcItem.Height() <= 0 ) { return; }

        int iScrollPos = m_pBody->GetRealItemTop(nItem);
        m_pVScroll->Scroll( SB_THUMBPOSITION, iScrollPos );
    }
}

inline void CBkIconView::SetIsInnerDrag(BOOL bIsInnerDgag)
{
	m_pBody->SetIsInnerDrag(bIsInnerDgag);
}

inline int CBkIconView::GetScrollPos()
{
    if( m_pVScroll )
    {
        return m_pVScroll->GetValue();
    }
    return -1;
}

inline CBkScroll* CBkIconView::GetScrollBar()
{
  return  m_pVScroll;
}
inline BOOL CBkIconView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	BOOL bRet = __super::OnKeyDown(nChar, nRepCnt, nFlags);

	if (nChar == VK_LEFT || nChar == VK_RIGHT || nChar == VK_UP || nChar == VK_DOWN)
	{
		std::vector<int> selList;
		GetSelectItems(selList);

		int nFirst = -1, nLast = -1, nBase = -1;
		for (std::vector<int>::iterator it = selList.begin(); it != selList.end(); it++)
		{
			if (nBase == -1)
				nBase = *it;
			if (nFirst == -1 || nFirst > *it)
				nFirst = *it;
			if (nLast == -1 || nLast < *it)
				nLast = *it;
		}

		int nCur = -1;
		BOOL bShift = (GetKeyState(VK_SHIFT) & 0x80000000) == 0x80000000;
		if (selList.size())
		{
			int nStep = 1;
			if (nChar == VK_UP || nChar == VK_DOWN)
				nStep = GetLineItemCount();

			int nBaseTmp = nBase;
			if (nChar == VK_RIGHT || nChar == VK_DOWN)
				nBaseTmp = nBase > nFirst ? nFirst : nLast;
			else
				nBaseTmp = nLast > nBase ? nLast : nFirst;

			if (nChar == VK_RIGHT || nChar == VK_DOWN)
				nCur = nBaseTmp + nStep;
			else
				nCur = nBaseTmp - nStep;
		}

		if (nCur >= GetItemCount())
			nCur = GetItemCount() - 1;

		if (nCur >= 0)
		{
			if (bShift)
			{
				selList.clear();
				selList.push_back(nBase);

				for (int i = nBase + 1; i <= nCur; i++)
					selList.push_back(i);
				for (int i = nCur; i < nBase; i++)
					selList.push_back(i);

				m_pBody->SetSelectItems(selList);
			}
			else
			{
				ClearSelectItems();
				SelectItem(nCur, TRUE);

				if (m_pNotify != NULL)
				{
					NotifyInfo info = {BKM_ICONVIEW_SELCHANGED, CPoint(0,0), this, GetSelectItem()};
					m_pNotify->Action(&info);
				}
			}

			ScrollToItem(nCur);
		}
	}
	else if (nChar == 'a' || nChar == 'A')
	{
		if ((GetKeyState(VK_CONTROL) & 0x80000000) == 0x80000000)
			SelectAll();
	}
	else if(nChar == VK_RETURN || nChar == VK_DELETE)
	{
		NotifyInfo info = {BKM_ICONVIEW_KEYDOWN, CPoint(0,0), this, nChar};
		m_pNotify->Action(&info);
	}

	return bRet;
}

inline BOOL CBkIconView::OnLButtonUp(UINT nFlags, CPoint point,BOOL bAct)
{
    CBkDiv::OnLButtonUp( nFlags, point,bAct );
    return FALSE;
}

inline BOOL CBkIconView::OnMouseLeave()
{
    BOOL bRet = CBkDiv::OnMouseLeave();
    if (m_pNotify != NULL)
    {
        NotifyInfo info = {BKM_ICONVIEW_LEAVE, CPoint(0,0), this};
        m_pNotify->Action(&info);
    }
    return bRet;
}

inline BOOL CBkIconView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    m_pBody->OnMouseWheel(nFlags,zDelta,pt);
    return FALSE;
}


inline int CBkIconView::GetLineItemCount()
{
	int nColumn = 0;
	int lineWidth = 0;

	int nCount = GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
		lineWidth += GetItemRect(i).Width();
		if( lineWidth > m_rcWindow.Width() )
			return nColumn;
		nColumn++;
	}

	return 0;
}

inline int CBkIconView::GetSelectItem()
{
	return m_pBody->GetSelectItem();
}

inline BOOL	CBkIconView::SelectAll()
{
	std::vector<int> selList;
	int nCount = GetItemCount();
	for (int i = 0; i < nCount; i++)
		selList.push_back(i);
	m_pBody->SetSelectItems(selList);
	if (m_pNotify != NULL)
	{
		NotifyInfo info = {BKM_ICONVIEW_SELCHANGED, CPoint(0,0), this, GetSelectItem()};
		m_pNotify->Action(&info);
	}
	return TRUE;
}

/////////////////////////////////
////  BkIconBody
/////////////////////////////////
inline CBkIconBody::CBkIconBody()
{
    m_pDefault = NULL;
    m_bMultSelect = FALSE;
    m_pItemsBgSkin = NULL;
    m_bDrawVisibles = FALSE;
    m_visibleHeight = 0;
    m_itemsBgGap = 0;
	m_bIsInnerDgag = TRUE;
	m_bOldChecked = FALSE;
}

inline CBkIconBody::~CBkIconBody()
{
    if( m_pDefault != NULL )
    {
        delete m_pDefault;
        m_pDefault = NULL;
    }
    m_iSelectItems.clear();
}

inline int CBkIconBody::AddItem( BOOL isRedraw/* = TRUE*/, BOOL isCalc/* = TRUE*/ )
{
    CBkIconItem* pNew = _NewItem();
    if (pNew != NULL)
    {
	    pNew->RegisterNotify(this);
        m_lstWndChild.AddTail(pNew);
        if( isCalc )
        {
            RepositionChild(pNew);
            m_visibleHeight = GetRealHeight();
        }
        if( isRedraw )
        {
            pNew->NotifyInvalidate();

            //CRect rcNew;
            //pNew->GetRect(&rcNew);
            //_UpdateItem(rcNew);
        }
    }
    return (int)m_lstWndChild.GetCount() - 1;
}

inline int CBkIconBody::AddItemFromTemplate( int templateIndex, BOOL isRedraw/* = TRUE*/, BOOL isCalc/* = TRUE*/ )
{
    if( templateIndex < 0 || templateIndex >= (int)m_lstTemplate.GetCount() )
        return NULL;

    TiXmlElement* tabTemplate = m_lstTemplate.GetAt( m_lstTemplate.FindIndex(templateIndex) );        
    if( tabTemplate )
    {
        CBkWnd* pNew = _NewItem(tabTemplate);
        if (pNew != NULL)
        {
            pNew->RegisterNotify(this);
            m_lstWndChild.AddTail(pNew);
            if( isCalc )
            {
                RepositionChild(pNew);
                m_visibleHeight = GetRealHeight();
            }
            if( isRedraw )
            {
                pNew->NotifyInvalidate();

                //CRect rcNew;
                //pNew->GetRect(&rcNew);
                //_UpdateItem(rcNew);
            }
        }
        return (int)m_lstWndChild.GetCount() - 1;
    }
    return -1;
}

inline int CBkIconBody::InsertItem(int nItem)
{
    CBkIconItem* pNew = _NewItem();
    if (pNew != NULL)
    {
        pNew->RegisterNotify(this);
        m_lstWndChild.InsertBefore(GetPosByIndex(nItem), pNew);
        RepositionChildsFrom(pNew);
        m_visibleHeight = GetRealHeight();
        pNew->NotifyInvalidate();

        //CRect rcNew;
        //pNew->GetRect(&rcNew);
        //_UpdateItem(rcNew);
    }

    return nItem;
}

inline BOOL CBkIconBody::DeleteItem(int nItem)
{
    BOOL bRet = FALSE;
    do 
    {
        POSITION pos = GetPosByIndex(nItem);
        if (pos == NULL)
        {
            break;
        }

        CBkWnd* pBkWndChild = m_lstWndChild.GetAt(pos);
        if (pBkWndChild == NULL)
        {
            break;
        }

        if (m_lastHoverChild == pBkWndChild)
        {
            m_lastHoverChild = NULL;
        }

        ClearSelectItems();
        _ClearFocus( m_focusChild );
        _ClearDrag( m_focusChild );
        m_iShowTop = 0;

        pBkWndChild->SetData(NULL);
        pBkWndChild->OnDestroy();
        delete pBkWndChild;
        pBkWndChild = NULL;
        m_lstWndChild.RemoveAt(pos);

        RepositionChilds();
        m_visibleHeight = GetRealHeight();
        NotifyInvalidate();
        bRet = TRUE;
    } while (FALSE);
    return bRet;
}

inline void CBkIconBody::DeleteItems(int nItemBegin, int nItemEnd)
{
    BOOL bRet = !m_lstWndChild.IsEmpty();

    ClearSelectItems();
    _ClearLastHover();
    _ClearFocus( m_focusChild );
    _ClearDrag( m_focusChild );
    m_iShowTop = 0;

    int index = nItemEnd;
    POSITION pos = GetPosByIndex(nItemEnd);
    while (index >= nItemBegin)
    {
        pos = m_lstWndChild.FindIndex(index--);
        CBkWnd* pBkWndChild = m_lstWndChild.GetAt( pos );
        if (pBkWndChild == NULL)
        {
            break;
        }
        pBkWndChild->SetData(NULL);
        pBkWndChild->OnDestroy();
        delete pBkWndChild;
        pBkWndChild = NULL;
        m_lstWndChild.RemoveAt(pos);
    }

    RepositionChilds();
    m_visibleHeight = GetRealHeight();
    NotifyInvalidate();
}

inline void CBkIconBody::DeleteAllItems()
{
    BOOL bRet = !m_lstWndChild.IsEmpty();

    ClearSelectItems();
    _ClearLastHover();
    _ClearFocus( m_focusChild );
    _ClearDrag( m_focusChild );
    m_iShowTop = 0;

    POSITION pos = m_lstWndChild.GetHeadPosition();
    while (pos != NULL)
    {
        CBkWnd* pBkWndChild = m_lstWndChild.GetNext(pos);
        if (pBkWndChild == NULL)
        {
            break;
        }
        pBkWndChild->SetData(NULL);
        pBkWndChild->OnDestroy();
        delete pBkWndChild;
        pBkWndChild = NULL;
    }

    m_lstWndChild.RemoveAll();
    m_visibleHeight = 0;
    NotifyInvalidate();
}

inline IBkWnd* CBkIconBody::GetItemEx(int nIndex, int nSubItemID)
{
    IBkWnd* pFind = NULL;
    do 
    {
        IBkWnd* pItem = GetItemByIndex(nIndex);
        if (pItem == NULL)
        {
            break;
        }

        if (nSubItemID <= 0)
        {
            pFind = pItem;
            break;
        }

        pFind = pItem->GetItem(nSubItemID);
    } while (FALSE);
    return pFind;
}

inline BOOL CBkIconBody::SetItemData(int nItem, DWORD dwData)
{
    CBkWnd* pBkItem = _GetItem(nItem, 0);
    if (pBkItem != NULL)
    {
        pBkItem->SetData(dwData);
    }
    return pBkItem != NULL;
}

inline DWORD CBkIconBody::GetItemData(int nItem)
{
    DWORD dwData = 0;
    CBkWnd* pBkItem = _GetItem(nItem, 0);
    if (pBkItem != NULL)
    {
        dwData = pBkItem->GetData();
    }
    return dwData;
}

inline CRect CBkIconBody::GetItemRect(int nItem)
{
    CRect rcItem;
    CBkIconItem* pBkItem = _GetItem(nItem);
    if (pBkItem != NULL)
    {
        pBkItem->GetRect( &rcItem );
    }
    return rcItem;
}

inline int CBkIconBody::GetItemCount()
{
    return (int)m_lstWndChild.GetCount();
}

inline int CBkIconBody::GetVisibleItemsHeight()
{
    return m_visibleHeight;
}

inline int CBkIconBody::HitTest(CPoint point)
{
    int index = -1;
    POSITION pos = m_lstWndChild.GetHeadPosition();
    while (pos)
    {
        index++;
        CBkWnd* pBkWndChild = m_lstWndChild.GetNext(pos);
        if( !pBkWndChild->IsVisible() )
            continue;

        if (pBkWndChild->PtInRect(point))
        {
            return index;
        }
    }

	return -1;
}

inline int CBkIconBody::FindItem(LIST_FIND_TYPE emType, DWORD dwData)
{
	for (int i = 0; ; i++)
	{
		CBkWnd* pItem = _GetItem(i, 0);
		if (!pItem)
			break;

		if (emType == LIST_FIND_TEXT)
		{
			if (wcscmp(pItem->GetText(), (LPCTSTR)dwData) == 0)
				return i;
		}
		else if (emType == LIST_FIND_DATA)
		{
			if (pItem->GetData() == dwData)
				return i;
		}
	}

	return -1;
}

inline void CBkIconBody::SetIsInnerDrag(BOOL bIsInnerDgag)
{
	m_bIsInnerDgag = bIsInnerDgag;
}

inline int CBkIconBody::GetRealItemTop(int nItem)
{
    int index = 0;
    int iScrollPos = 0;
    int lineWidth = 0;
    POSITION pos = m_lstWndChild.GetHeadPosition();
    while (pos != NULL)
    {
        CBkWnd *pBkWndChild = (CBkWnd*)m_lstWndChild.GetNext(pos);
        if (pBkWndChild)
        {
            lineWidth += pBkWndChild->GetWidth();
			if (index++ == nItem)
			{
				if (lineWidth > m_rcWindow.Width())
					iScrollPos += pBkWndChild->GetHeight();
				break;
			}

            if( iScrollPos < 0 )
            {
                iScrollPos += pBkWndChild->GetHeight();
            }
            else if( lineWidth > m_rcWindow.Width() )
            {
                iScrollPos += pBkWndChild->GetHeight();
                lineWidth = pBkWndChild->GetWidth();
            }
        }
    }
    return iScrollPos;
}

inline BOOL CBkIconBody::SetSelectItems(const std::vector<int>& nItems)
{
    _ClearSelectItems();
    for (size_t i = 0; i < nItems.size(); i++)
    {
        _AddSelectItem(nItems[i]);
    }
    return nItems.empty();
}

inline BOOL CBkIconBody::GetSelectItem()
{
	return m_iSelectItems.size() ? m_iSelectItems[0] : -1;
}

inline BOOL CBkIconBody::GetSelectItems(std::vector<int>& nItems)
{
    nItems.resize(m_iSelectItems.size());
    std::copy(m_iSelectItems.begin(), m_iSelectItems.end(), nItems.begin());
    return nItems.empty();
}

inline BOOL CBkIconBody::Load(TiXmlElement* pTiXmlElem)
{
    if (!CBkWnd::Load(pTiXmlElem))
        return FALSE;

    if (pTiXmlElem->FirstChildElement() != NULL)
    {
        m_pDefault = pTiXmlElem->FirstChildElement()->Clone();
    }

    TiXmlElement* pXmlTemplate = pTiXmlElem->NextSiblingElement();
    if( pXmlTemplate )
    {
		if ( strcmp(CLASSNAME_ICONTEMPLATE,pXmlTemplate->Value())  == 0 )
		{
			for (TiXmlElement* pXmlChild = pXmlTemplate->FirstChildElement(); NULL != pXmlChild; pXmlChild = pXmlChild->NextSiblingElement())
			{
					TiXmlElement* pTiElement = (TiXmlElement*)pXmlChild->Clone();
					m_lstTemplate.AddTail( pTiElement );    
			}
		}
    }
    return TRUE;
}

inline BOOL CBkIconBody::Action(NotifyInfo* pInfo)
{
    if ( pInfo->nAction == BKM_ICONVIEW_LDBCLICK || 
		 pInfo->nAction == BKM_ICONVIEW_LCLICK ||
         pInfo->nAction == BKM_ICONVIEW_RCLICK ||
         pInfo->nAction == BKM_ICONVIEW_HOVER ||
         pInfo->nAction == BKM_ICONVIEW_LEAVE ||
		 pInfo->nAction == BKM_ICONVIEW_CUSTOMDRAW)
    {
        NotifyInfo info = *pInfo;
        info.pFrom = this;
        info.dwData = info.dwData = GetIndexByItem( (CBkWnd*)pInfo->pFrom );
        m_pNotify->Action(&info);
    }

    return TRUE;
}

inline BOOL CBkIconBody::OnLButtonDown(UINT nFlags, CPoint point)
{
    SetFocus();
	int nItem = HitTest( point );
	CBkIconItem* pItem = dynamic_cast<CBkIconItem*>( GetItemByIndex(nItem) );
	if( pItem && pItem->_HitTest(point) >= 0 )
	{

		ModifyState(BkWndState_PushDown, NULL);
		if (pItem && pItem->IsChecked())
		{
			m_bOldChecked = TRUE;
		}
		else
		{
			SelectItem(nItem, TRUE);
			m_bOldChecked = FALSE;
		}
	}
	if (m_pNotify)
	{
		NotifyInfo info = {WM_LBUTTONDOWN, point, this, nItem};
		m_pNotify->Action(&info);
	}

    return __super::OnLButtonDown(nFlags, point);
}

inline BOOL CBkIconBody::OnLButtonUp(UINT nFlags, CPoint point,BOOL bAct)
{
	int nItem = HitTest( point );
	CBkIconItem* pItem = dynamic_cast<CBkIconItem*>( GetItemByIndex(nItem) );
	if( pItem && pItem->_HitTest(point) >= 0 )
	{
		if( IsMultSelect() && (nFlags & MK_CONTROL) )
		{
			SelectItem( nItem, !m_bOldChecked );
			m_bOldChecked = FALSE;
		}
		else if( IsMultSelect() && (nFlags & MK_SHIFT) )
		{
			SelectMultipleItems( nItem );
		}
		else
		{
			SelectSingleItem( nItem );

			if (bAct)
			{
				if (m_pNotify != NULL)
				{
					NotifyInfo info = {BKM_ICONVIEW_SELCHANGED, CPoint(0,0), this, GetSelectItem()};
					m_pNotify->Action(&info);
				}
			}
		}
	}	
	else
	{
		if ((nFlags & MK_CONTROL)
			||(nFlags & MK_SHIFT))
		{
		}	
		else
		{
			ClearSelectItems();
		}			
	}

	if (bAct)
	{
		if (m_pNotify != NULL)
		{
			NotifyInfo info = {BKM_ICONVIEW_LCLICK, point, this, nItem};
			m_pNotify->Action(&info);
		}
	}
    __super::OnLButtonUp(nFlags, point,bAct);

    return FALSE;
}

inline BOOL CBkIconBody::OnRButtonDown(UINT nFlags, CPoint point)
{
    int nItem = HitTest( point );

    CBkIconItem* pItem = dynamic_cast<CBkIconItem*>( GetItemByIndex(nItem) );
    if( pItem && pItem->_HitTest(point) >= 0 )
    {
        if( !IsItemSelected(nItem) )
        {
            SelectSingleItem( nItem );
        }
        else if( nItem == -1 )
        {
            ClearSelectItems();
        }
    }
    return __super::OnRButtonDown( nFlags, point );
}

inline BOOL CBkIconBody::OnRButtonUp(UINT nFlags, CPoint point,BOOL bAct)
{
    int nItem = HitTest( point );
    __super::OnRButtonUp(nFlags, point,bAct);

	if (bAct)
	{
		if (m_pNotify != NULL)
		{
			NotifyInfo info = {BKM_ICONVIEW_RCLICK, point, this, nItem};
			m_pNotify->Action(&info);
		}
	}

    return FALSE;
}

inline BOOL CBkIconBody::OnMouseMove(UINT nFlags, CPoint point)
{
	if (IsDragInner()&&m_rcWindow.PtInRect(point)&&m_pDropTarget
		&&m_rcWindow.PtInRect(m_pDropTarget->GetLBDownPoint()))
	{
		int nItem = HitTest( m_pDropTarget->GetLBDownPoint() );
		CBkIconItem* pItem = dynamic_cast<CBkIconItem*>( GetItemByIndex(nItem) );
		if ( pItem && pItem->_HitTest(m_pDropTarget->GetLBDownPoint()) >= 0 )
		{
			if (m_bIsInnerDgag)
			{
				BkDragData DropItem;
				DropItem.bkItem = this;
				DropItem.dwCmdID = GetCmdID();
				DropItem.ptSrc = point;
				DropItem.dwDragOpt = DRAGOPT_FORM_MYSELF;

				m_pDropTarget->DoDragDrop(&DropItem);
			}
		}
	}

	return __super::OnMouseMove(nFlags, point);
}

inline BOOL CBkIconBody::OnMouseLeave()
{
    BOOL bRet = CBkDivZ::OnMouseLeave();
    if (m_pNotify != NULL)
    {
        NotifyInfo info = {BKM_ICONVIEW_LEAVE, CPoint(0,0), this};
        m_pNotify->Action(&info);
    }
    return bRet;
}

inline BOOL CBkIconBody::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    int iRealHeight = GetRealHeight() + GetItemsOffsetV();
    CRect rcWin(m_rcWindow);
    if (iRealHeight < rcWin.Height())
    {
        return FALSE;
    }

    int iPer = (m_rcWindow.bottom - m_rcWindow.top) / 8;

    int iShowTop = m_iShowTop;
    if (zDelta < 0)
    {
        if (iShowTop + iPer + rcWin.Height() < iRealHeight)
        {
            iShowTop += iPer;
        }
        else
        {
            iShowTop = iRealHeight - rcWin.Height();
        }
    }
    else
    {
        if (iShowTop > iPer)
        {
            iShowTop -= iPer;
        }
        else
        {
            iShowTop = 0;
        }
    }
    SetShowTop(iShowTop);
    if (m_pNotify != NULL)
    {
        NotifyInfo info = {BKM_LIST_MOUSEWHEEL, CPoint(0, 0), this, m_iShowTop};
        m_pNotify->Action(&info);
    }
    return FALSE;
}

inline BOOL CBkIconBody::OnPaint(CDCHandle dc)
{
    if (m_pItemsBgSkin != NULL)
    {
        BOOL bDraw = TRUE;

        CRect rcDraw = m_rcWindow;
        rcDraw.top = rcDraw.top /*+ m_itemsOffsetV*/ - m_iShowTop;
        if( m_bDrawVisibles )
        {
            if( m_lstWndChild.GetCount() > 0 )
            {
                rcDraw.top -= m_itemsBgGap;
                rcDraw.bottom = rcDraw.top + m_visibleHeight;
                //rcDraw.bottom = rcDraw.top + m_iShowTop + ((m_visibleHeight>m_rcWindow.Height())?m_rcWindow.Height():m_visibleHeight);
            }
            else
            {
                bDraw = FALSE;
            }
        }

        if( bDraw )
        {
            m_pItemsBgSkin->Draw( dc, rcDraw, m_dwState );
        }
    }

    return TRUE;
}

inline CBkIconItem* CBkIconBody::_NewItem()
{
    CBkIconItem* pNew = CBkIconItem::CheckAndNew(m_pDefault->Value());
    if (pNew != NULL)
    {
        pNew->SetParent(m_hBkWnd);
        pNew->SetContainer(m_hWndContainer);
		pNew->SetDropTarget(m_pDropTarget);
        pNew->SetRealContainer(m_hWndRealContainer);
        pNew->SetParentsVisible(IsVisible());
        pNew->SetTabControler(m_tabControler);
        pNew->SetTooltipCtrl(m_wndToolTip);
        pNew->Load(m_pDefault->ToElement());
    }
    return pNew;
}

inline CBkWnd* CBkIconBody::_NewItem(TiXmlElement* pTiXmlElem)
{
    CBkDiv* pNew = CBkIconItem::CheckAndNew(CLASSNAME_ICONITEM);
    if (pNew != NULL)
    {
        pNew->SetParent(m_hBkWnd);
        pNew->SetContainer(m_hWndContainer);
		pNew->SetDropTarget(m_pDropTarget);
        pNew->SetRealContainer(m_hWndRealContainer);
        pNew->SetParentsVisible(IsVisible());
        pNew->SetTabControler(m_tabControler);
        pNew->SetTooltipCtrl(m_wndToolTip);
        pNew->Load(pTiXmlElem->ToElement());
    }
    return pNew;
}

inline CBkWnd* CBkIconBody::_GetItem(int nItem, int nIndex)
{
    CBkWnd* pFind = NULL;
    do 
    {
        CBkIconItem* pItem = dynamic_cast<CBkIconItem*>(GetItemByIndex(nItem));
        if (pItem == NULL)
        {
            break;
        }

        if (nIndex < 0)
        {
            pFind = pItem;
            break;
        }

        pFind = dynamic_cast<CBkWnd*>(pItem->GetItemByIndex(nIndex));
    } while (FALSE);
    return pFind;
}

inline CBkIconItem* CBkIconBody::_GetItem(int nItem)
{
    CBkIconItem* pBkItem = NULL;
    do 
    {
        POSITION pos = GetPosByIndex(nItem);
        if (pos == NULL)
        {
            break;
        }

        pBkItem = dynamic_cast<CBkIconItem*>(m_lstWndChild.GetAt(pos));
    } while (FALSE);
    return pBkItem;
}

inline void CBkIconBody::_UpdateItem(const CRect& rcItem)
{
    CRect rcRedraw = rcItem;
    if( m_pItemsBgSkin )
    {
        rcRedraw.left = m_rcWindow.left;
        rcRedraw.right = m_rcWindow.right;
    }
    
    NotifyInvalidateRect(rcRedraw);
}

inline void CBkIconBody::_ClearSelectItems()
{
    for (size_t i = 0; i < m_iSelectItems.size(); i++)
    {
        CBkIconItem* pItem = dynamic_cast<CBkIconItem*>(GetItemByIndex(m_iSelectItems[i]));
        if (pItem != NULL)
        {
            pItem->SetChecked(FALSE);
        }
    }
    m_iSelectItems.clear();
}

inline void CBkIconBody::_AddSelectItem(int nItem)
{
    CBkIconItem* pItem = dynamic_cast<CBkIconItem*>(GetItemByIndex(nItem));
    if (pItem != NULL)
    {
        pItem->SetChecked(TRUE);
		m_iSelectItems.push_back(nItem);
    }
}

inline void CBkIconBody::SortChildren(ISortCompare* compare)
{
    CBkDivZ::SortChildren(compare);

    m_iSelectItems.clear();
    int index = 0;
    POSITION pos = m_lstWndChild.GetHeadPosition();
    while (pos != NULL)
    {
        CBkWnd* pBkWnd = m_lstWndChild.GetNext(pos);
        if (pBkWnd && pBkWnd->IsChecked())
        {
            m_iSelectItems.push_back(index);
        }
        index++;
    }
}

//////// Ñ¡Ôñ²Ù×÷
inline BOOL CBkIconBody::IsMultSelect()
{
    return m_bMultSelect;
}

inline BOOL CBkIconBody::SelectItem( int nItem, BOOL bSelect )
{
    CBkIconItem* pItem = dynamic_cast<CBkIconItem*>( GetItemByIndex(nItem) );
    if( pItem )
    {
        pItem->SetChecked(bSelect);
        if (bSelect)
        {
			 std::vector<int>::iterator it = find( m_iSelectItems.begin(), m_iSelectItems.end(), nItem );
			 if (m_iSelectItems.end() == it)
			 {
			   m_iSelectItems.push_back(nItem);
			 }
        }
        else
        {
            std::vector<int>::iterator it = find( m_iSelectItems.begin(), m_iSelectItems.end(), nItem );
            if (m_iSelectItems.end() != it)
            {
                m_iSelectItems.erase(it);
            }
        }
    }
    return pItem != NULL;
}

inline void CBkIconBody::ClearSelectItems()
{
    for (size_t i = 0; i < m_iSelectItems.size(); i++)
    {
        CBkIconItem* pItem = dynamic_cast<CBkIconItem*>(GetItemByIndex(m_iSelectItems[i]));
        if (pItem != NULL)
        {
            pItem->SetChecked(FALSE);
        }
    }
    m_iSelectItems.clear();
}

inline BOOL CBkIconBody::IsItemSelected(int nItem)
{
    for (size_t i=0; i< m_iSelectItems.size(); i++)
    {
        if (m_iSelectItems[i] == nItem)
        {
            return TRUE;
        }
    }
    return FALSE;
}

inline int CBkIconBody::GetSelectItemsCount()
{
    return (int)m_iSelectItems.size();
}

inline void CBkIconBody::SelectSingleItem( int nItem )
{
    ClearSelectItems();

    if( !GetItemByIndex(nItem) )
    {
        return;
    }
  
    SelectItem( nItem, TRUE );
}

inline void CBkIconBody::SelectMultipleItems( int nEndItem )
{
    if( !GetItemByIndex(nEndItem) )
    {
        return;
    }

    int nFocusItem = GetIndexByItem( m_focusChild );

    int nStartItem = -1;
    if( GetSelectItemsCount() <= 0 )
    {
        SelectItem( nEndItem, TRUE );
        return;
    }
    else
    {
        if( nFocusItem < 0 || IsItemSelected( nFocusItem ) )
            nStartItem = (*m_iSelectItems.begin());
        else
            nStartItem = nFocusItem;
    }

    if( nStartItem < 0 || nStartItem == nEndItem )
    {
        return;
    }

    ClearSelectItems();

    std::vector<int> selectItems;
    if( nStartItem < nEndItem )
    {
        for( int i = nStartItem; i <= nEndItem; ++i )
        {
            SelectItem( i, TRUE );
        }
    }
    else
    {
        for( int i = nStartItem; i >= nEndItem; --i )
        {
            SelectItem( i, TRUE );
        }
    }
}

inline BOOL CBkIconBody::IsItemVisible(int nItem)
{
	CBkIconItem* pItem = _GetItem(nItem);
	if (pItem)
		return pItem->IsVisible();
	return FALSE;
}

inline BOOL	CBkIconBody::OnWindowPosChanged(LPWINDOWPOS lpWndPos)
{
	BOOL bRet = __super::OnWindowPosChanged(lpWndPos);
	if (m_iShowTop > 0)
	{
		CBkWnd* pBkWndTail = m_lstWndChild.GetAt(m_lstWndChild.GetTailPosition());
		if (pBkWndTail != NULL)
		{
			CRect rcTail;
			pBkWndTail->GetRect(&rcTail);
			int nBottom = GetRealItemTop(m_lstWndChild.GetCount() - 1) + rcTail.Height();
			if (nBottom - m_iShowTop < m_rcWindow.Height())
				m_iShowTop -= m_rcWindow.Height() - (nBottom - m_iShowTop);
            if (m_iShowTop < 0)
            {
                m_iShowTop = 0;
            }
		}
	}
    m_visibleHeight = GetRealHeight();
	return bRet;
}

/////////////////////////////////
////  BkIconItem
/////////////////////////////////
inline CBkIconItem::CBkIconItem()
    : m_bCustomDraw(FALSE)
{

}

inline void CBkIconItem::SetChecked(BOOL bCheck)
{
    CBkDiv::SetChecked(bCheck);

    POSITION pos = m_lstWndChild.GetHeadPosition();
    while( pos )
    {
        CBkWnd* pBkWndChild = m_lstWndChild.GetNext(pos);

        if( pBkWndChild->IsAcceptParent() )
        {
            pBkWndChild->SetChecked(bCheck);
        }
    } 
}

inline BOOL CBkIconItem::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    if (m_pNotify && _HitTest(point) >= 0)
    {
	    NotifyInfo info = {BKM_ICONVIEW_LDBCLICK, point, this};
	    m_pNotify->Action(&info);
    }
    return __super::OnLButtonDblClk(nFlags, point);
}

inline void CBkIconItem::OnMouseHoverChild(CBkWnd* pChild, const CPoint& point)
{
    if (m_pNotify != NULL)
    {
        NotifyInfo info = {BKM_ICONVIEW_HOVER, CPoint(0,0), this};
        m_pNotify->Action(&info);
    }
}

inline void CBkIconItem::OnMouseLeaveChild(CBkWnd* pChild, const CPoint& point)
{
    if (m_pNotify != NULL)
    {
        NotifyInfo info = {BKM_ICONVIEW_LEAVE, CPoint(0,0), this};
        m_pNotify->Action(&info);
    }
}

inline BOOL CBkIconItem::OnPaint(CDCHandle dc)
{
    if (m_bCustomDraw)
    {
        if (m_pNotify != NULL)
        {
            NotifyInfo info = {BKM_ICONVIEW_CUSTOMDRAW, CPoint(0,0), this};
            m_pNotify->Action(&info);
        }
    }

    return CBkDiv::OnPaint(dc);
}

inline CBkWnd* CBkIconItem::OnSetFocus(CBkWnd* which)
{
     m_bFocus = TRUE;
     __super::OnSetFocus( this );
     return this;
}

inline HBITMAP CBkIconItem::GetBitmap()
{
    HDC hDCDesktop = ::GetDC(NULL);
    HBITMAP hBmpCreate = ::CreateCompatibleBitmap(hDCDesktop, m_rcWindow.Width(), m_rcWindow.Height());
    HDC hDCDraw = ::CreateCompatibleDC(hDCDesktop);
    HGDIOBJ hbmpOld = ::SelectObject(hDCDraw, hBmpCreate);
    RECT rcDraw = {0, 0, m_rcWindow.Width(), m_rcWindow.Height()};
    ::SetBkColor(hDCDraw, RGB(255, 0, 0));
    ::ExtTextOut(hDCDraw, 0, 0, ETO_OPAQUE, &rcDraw, NULL, 0, NULL);
    ::SelectObject(hDCDraw, hbmpOld);
    ::DeleteDC(hDCDraw);
    ::ReleaseDC(NULL, hDCDesktop);
    return hBmpCreate;
}
