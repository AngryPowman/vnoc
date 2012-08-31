inline CBkReportViewBody::CBkReportViewBody()
{
    m_pDefault = NULL;
}

inline CBkReportViewBody::~CBkReportViewBody()
{
    if (m_pDefault != NULL)
    {
        delete m_pDefault;
        m_pDefault = NULL;
    }
}

inline int CBkReportViewBody::AddItem()
{
    CBkViewItem* pNew = _NewItem();
    if (pNew != NULL)
    {
        pNew->RegisterNotify(this);
        m_lstWndChild.AddTail(pNew);
        RepositionChild(pNew);
        pNew->NotifyInvalidate();
    }
    return (int)m_lstWndChild.GetCount();
}

inline int CBkReportViewBody::InsertItem(int nItem)
{
    CBkViewItem* pNew = _NewItem();
    if (pNew != NULL)
    {
        pNew->RegisterNotify(this);
        m_lstWndChild.InsertBefore(GetPosByIndex(nItem), pNew);
        RepositionChild(pNew);
        pNew->NotifyInvalidate();
    }
    return nItem;
}

inline IBkWnd* CBkReportViewBody::GetItem(int nItem, int nIndex)
{
    return _GetItem(nItem, nIndex);
}

inline BOOL CBkReportViewBody::SetItemData(int nItem, DWORD dwData)
{
    CBkWnd* pBkItem = _GetItem(nItem, 0);
    if (pBkItem != NULL)
    {
        pBkItem->SetData(dwData);
    }
    return pBkItem != NULL;
}

inline DWORD CBkReportViewBody::GetItemData(int nItem)
{
    DWORD dwData = 0;
    CBkWnd* pBkItem = _GetItem(nItem, -1);
    if (pBkItem != NULL)
    {
        dwData = pBkItem->GetData();
    }
    return dwData;
}

inline BOOL CBkReportViewBody::SetItemText(int nItem, int nIndex, LPCWSTR szText)
{
    CBkWnd* pBkItem = _GetItem(nItem, nIndex);
    if (pBkItem != NULL)
    {
        pBkItem->SetText(szText);
    }
    return pBkItem != NULL;
}

inline LPCWSTR CBkReportViewBody::GetItemText(int nItem, int nIndex)
{
    LPCWSTR szText = NULL;
    CBkWnd* pBkItem = _GetItem(nItem, nIndex);
    if (pBkItem != NULL)
    {
        szText = pBkItem->GetText();
    }
    return szText;
}

inline BOOL CBkReportViewBody::Load(TiXmlElement* pTiXmlElem)
{
    if (!CBkWnd::Load(pTiXmlElem))
        return FALSE;

    if (pTiXmlElem->FirstChildElement() != NULL)
    {
        m_pDefault = pTiXmlElem->FirstChildElement()->Clone();
    }
    return TRUE;
}

inline BOOL CBkReportViewBody::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    int iRealHeight = GetRealHeight();
    CRect rcWin(m_rcWindow);
    if (iRealHeight < rcWin.Height())
    {
        return FALSE;
    }

    int iPer = (m_rcWindow.bottom - m_rcWindow.top) / 10;

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
    return FALSE;
}

inline BOOL CBkReportViewBody::Action(NotifyInfo* pInfo)
{
    if (pInfo != NULL && pInfo->pFrom != NULL)
    {
        if (m_pNotify != NULL)
        {
            pInfo->pFrom = this;
            m_pNotify->Action(pInfo);
        }
    }
    return FALSE;
}

inline CBkViewItem* CBkReportViewBody::_NewItem()
{
    CBkViewItem* pNew = CBkViewItem::CheckAndNew(m_pDefault->Value());
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

inline CBkWnd* CBkReportViewBody::_GetItem(int nItem, int nIndex)
{
    CBkWnd* pFind = NULL;
    do 
    {
        CBkViewItem* pItem = dynamic_cast<CBkViewItem*>(GetItemByIndex(nItem));
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

inline int CBkReportViewBody::_GetRealHeight()
{
    int iHeight = 0;
    return iHeight;
}


//////////////////////////////////////////////////////////////////////////
// CBkViewItem
inline CBkViewItem::CBkViewItem()
{
    m_pCheck = NULL;
}

inline BOOL CBkViewItem::IsChecked()
{
    BOOL bChecked = FALSE;
    if (m_pCheck != NULL)
    {
        bChecked = m_pCheck->IsChecked();
    }
    return bChecked;
}

inline void CBkViewItem::SetChecked(BOOL bCheck)
{
    if (m_pCheck != NULL)
    {
        m_pCheck->SetChecked(bCheck);
    }
}

inline void CBkViewItem::OnChildLoad(TiXmlElement* pTiXmlElem, CBkWnd* pChildWindow)
{
    if (m_pCheck == NULL)
    {
        CString strNode = pTiXmlElem->Value();
        if (strNode == CBkCheckBox::GetClassName())
        {
            m_pCheck = dynamic_cast<CBkCheckBox*>(pChildWindow);
        }
    }
}

inline BOOL CBkViewItem::OnLButtonUp(UINT nFlags, CPoint point,BOOL bAct)
{
    __super::OnLButtonUp(nFlags, point,bAct);
    if (m_pCheck != NULL)
    {
        if (!m_pCheck->PtInRect(point))
        {
            m_pCheck->SetChecked(!m_pCheck->IsChecked());
        }
    }
	if (bAct)
	{
		if (m_pNotify != NULL)
		{
			NotifyInfo ni = {BKM_CHECK_CLICK, point, this};
			m_pNotify->Action(&ni);
		}
	}
    return TRUE;
}

inline BOOL CBkViewItem::NeedRedrawWhenStateChange()
{
    return TRUE;
}

inline BOOL CBkReportViewBody::SetCheck(int nItem, BOOL bCheck)
{
    BOOL bRet = FALSE;
    IBkWnd* pItem = GetItem(nItem, -1);
    if (pItem != NULL)
    {
        pItem->SetChecked(bCheck);
    }
    return bRet;
}

inline BOOL CBkReportViewBody::IsCheck(int nItem)
{
    BOOL bRet = FALSE;
    IBkWnd* pItem = GetItem(nItem, -1);
    if (pItem != NULL)
    {
        bRet = pItem->IsChecked();
    }
    return bRet;
}

//////////////////////////////////////////////////////////////////////////
// CBkReportView 
inline CBkReportView::CBkReportView()
{
    m_pBody = NULL;
    m_pVScroll = NULL;
}

inline int CBkReportView::AddItem()
{
    assert(m_pBody != NULL);
    int nRow = m_pBody->AddItem();
    _CalcSize();
    return nRow;
}

inline int CBkReportView::InsertItem(int nItem)
{
    assert(m_pBody != NULL);
    int nRow = m_pBody->InsertItem(nItem);
    _CalcSize();
    return nRow;
}

inline IBkWnd* CBkReportView::GetItem(int nItem, int nIndex)
{
    assert(m_pBody != NULL);
    return m_pBody->GetItem(nItem, nIndex);
}

inline BOOL CBkReportView::SetItemData(int nItem, DWORD dwData)
{
    assert(m_pBody != NULL);
    return m_pBody->SetItemData(nItem, dwData);
}

inline DWORD CBkReportView::GetItemData(int nItem)
{
    assert(m_pBody != NULL);
    return m_pBody->GetItemData(nItem);
}

inline BOOL CBkReportView::SetItemText(int nItem, int nIndex, LPCWSTR szText)
{
    assert(m_pBody != NULL);
    return m_pBody->SetItemText(nItem, nIndex, szText);
}

inline LPCWSTR CBkReportView::GetItemText(int nItem, int nIndex)
{
    assert(m_pBody != NULL);
    return m_pBody->GetItemText(nItem, nIndex);
}

inline BOOL CBkReportView::IsCheck(int nItem)
{
    assert(m_pBody != NULL);
    return m_pBody->IsCheck(nItem);
}

inline BOOL CBkReportView::SetCheck(int nItem, BOOL bCheck)
{
    assert(m_pBody != NULL);
    return m_pBody->SetCheck(nItem, bCheck);
}

inline BOOL CBkReportView::LoadChilds(TiXmlElement* pTiXmlChildElem)
{
    BOOL bVisible = IsVisibleDeep(TRUE);
    for (TiXmlElement* pXmlChild = pTiXmlChildElem; NULL != pXmlChild; pXmlChild = pXmlChild->NextSiblingElement())
    {
        CString strNode = pXmlChild->Value();
        CBkWnd* pBkWnd = NULL;
        if (strNode == CBkReportViewBody::GetClassName())
        {
            m_pBody = new CBkReportViewBody();
            pBkWnd = m_pBody;
        }

        if (pBkWnd != NULL)
        {
            pBkWnd->RegisterNotify(this);
            pBkWnd->SetParent(m_hBkWnd);
            pBkWnd->SetContainer(m_hWndContainer);
			pBkWnd->SetDropTarget(m_pDropTarget);
            pBkWnd->SetRealContainer(m_hWndRealContainer);
            pBkWnd->SetParentsVisible(IsVisible());
            pBkWnd->SetTabControler(m_tabControler);
            pBkWnd->SetTooltipCtrl(m_wndToolTip);
            pBkWnd->Load(pXmlChild);
            m_lstWndChild.AddTail(pBkWnd);
        }
    }
    m_pVScroll = new CBkScroll();
    m_pVScroll->RegisterNotify(this);
    m_pVScroll->SetParent(m_hBkWnd);
    m_pVScroll->SetContainer(m_hWndContainer);
	m_pVScroll->SetDropTarget(m_pDropTarget);
    m_pVScroll->SetRealContainer(m_hWndRealContainer);
    m_pVScroll->SetVisible(FALSE);
    m_pVScroll->SetSkin(m_style.m_strScrollV);
    m_lstWndChild.AddTail(m_pVScroll);

    assert(m_pBody != NULL);
    return TRUE;
}

inline BOOL CBkReportView::OnWindowPosChanged(LPWINDOWPOS lpWndPos)
{
    __super::OnWindowPosChanged(lpWndPos);
    _CalcSize();
    return FALSE;
}

inline BOOL CBkReportView::Action(NotifyInfo* pInfo)
{
    if ( pInfo->nAction == BKM_CHECK_CLICK)
    {
        if (m_pNotify != NULL)
        {
            NotifyInfo info = *pInfo;
            info.pFrom = this;
            m_pNotify->Action(&info);
        }
    }
    else if (pInfo->nAction == BKM_LIST_SCROLL_V)
    {
        m_pBody->SetShowTop(pInfo->dwData);
    }

    return FALSE;
}

inline void CBkReportView::_CalcSize()
{
    CRect rcBody;
    m_pBody->GetRect(&rcBody);
    int iBodyHeight = rcBody.Height();
    int iRealHeight = m_pBody->GetRealHeight();

    BOOL bRepositionAll = (iRealHeight > iBodyHeight) != m_pVScroll->NeedShow();
    if (iRealHeight > iBodyHeight || m_pVScroll->NeedShow())
    {
        m_pVScroll->SetMax(iRealHeight);
        m_pVScroll->SetPage(iBodyHeight);

        if (bRepositionAll)
        {
            BKDLG_POSITION pos;
            m_pBody->GetPos(&pos);
            pos.Right.nPos = (iRealHeight > iBodyHeight) ? m_pVScroll->GetWidth() : 0;
            m_pBody->SetPos(&pos, FALSE);

            pos.Left.bMinus = TRUE;
            pos.Left.nPos = m_pVScroll->GetWidth();
            pos.Right.bMinus = TRUE;
            pos.Right.nPos = 0;
            m_pVScroll->SetPos(&pos, FALSE);
            m_pVScroll->SetVisible(TRUE);
        }
        else
        {
            m_pVScroll->NotifyInvalidate();
        }
    }

    if (bRepositionAll)
    {
        RepositionChilds();
        NotifyInvalidate();
    }
}