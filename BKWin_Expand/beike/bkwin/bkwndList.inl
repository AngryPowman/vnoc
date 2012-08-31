#pragma once


//////////////////////////////////////////////////////////////////////////
// CBkListHead
inline CBkListHead::CBkListHead()
{
    m_pSortCol = NULL;
    m_pSkinArrow = NULL;
    m_nDragSrcWidth = 0;
    m_bDrag = FALSE;
    m_bAllowDrag = FALSE;
    m_nDragCol = -1;
    m_nDragMinWidth = 0;
    m_hCursor = ::LoadCursor(NULL, IDC_ARROW);
}

inline BOOL CBkListHead::LoadChilds(TiXmlElement* pTiXmlChildElem)
{
    for (TiXmlElement* pXmlChild = pTiXmlChildElem; NULL != pXmlChild; pXmlChild = pXmlChild->NextSiblingElement())
    {
        CString strNode = pXmlChild->Value();
        if (strNode == CBkListCol::GetClassName())
        {
            CBkListCol* pBkWnd = new CBkListCol();
            pBkWnd->SetParent(m_hBkWnd);
            pBkWnd->SetContainer(m_hWndContainer);
			pBkWnd->SetDropTarget(m_pDropTarget);
            pBkWnd->SetParentsVisible(IsVisible());
            pBkWnd->Load(pXmlChild);
            m_lstWndChild.AddTail(pBkWnd);
        }
    }
    return TRUE;
}

inline IBkWnd* CBkListHead::GetItem(int nCol, int nIndex)
{
    CBkWnd* pBkItem = NULL;
    do 
    {
        CBkDiv* pCol = dynamic_cast<CBkDiv*>(GetItemByIndex(nCol));
        if (pCol == NULL)
        {
            break;
        }

        pBkItem = pCol->_GetItemByIndex(nIndex);
    } while (FALSE);
    return pBkItem;
}

inline BOOL CBkListHead::SetColumnWidth(int nCol, int nWidth)
{
    CBkWnd* pBkItem = NULL;
    do 
    {
        if (nWidth < 0)
        {
            nWidth = 0;
        }

        CBkDiv* pCol = dynamic_cast<CBkDiv*>(GetItemByIndex(nCol));
        if (pCol == NULL)
        {
            break;
        }
        pCol->SetWidth(nWidth);
    } while (FALSE);
    RepositionArrow();
    return pBkItem != NULL;
}

inline void CBkListHead::OnSetCursor()
{
    ::SetCursor(m_hCursor);
}

inline BOOL CBkListHead::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (m_bCanDrag && m_bAllowDrag)
    {
        m_bDrag = TRUE;
        m_ptDown = point;
        CRect rc;
        CBkWnd* pBkWnd = _GetItemByIndex(m_nDragCol);
        if (pBkWnd != NULL)
        {
            pBkWnd->GetRect(&rc);
            m_nDragSrcWidth = rc.Width();

            CBkListCol* pBkCol = dynamic_cast<CBkListCol*>(pBkWnd);
            if (pBkCol != NULL)
            {
                m_nDragMinWidth = pBkCol->GetMinWidth();
            }
        }
    }
    else
    {
        __super::OnLButtonDown(nFlags, point);
    }
    return FALSE;
}

inline BOOL CBkListHead::OnLButtonUp(UINT nFlags, CPoint point,BOOL bAct)
{
    BOOL bDragEnd = m_bDrag;
    if (m_bDrag)
    {
        m_bDrag = FALSE;
        m_hCursor = ::LoadCursor(NULL, IDC_ARROW);
    }
    else
    {
        __super::OnLButtonUp(nFlags, point,bAct);
        int nCol = -1;
        int nIndex = -1;
        CBkWnd* pBkItem = NULL;
        do 
        {
            CBkWnd* pBkWndCol = FindChildByPoint(point, TRUE);
            if (pBkWndCol == NULL)
            {
                break;
            }
            nCol = GetIndexByItem(pBkWndCol);

            CBkWnd* pBkWndItem = pBkWndCol->FindChildByPoint(point, TRUE);
            if (pBkWndItem == NULL)
            {
                break;
            }
            nIndex = pBkWndCol->GetIndexByItem(pBkWndItem);

            if (m_pSortCol != NULL && m_pSortCol != pBkWndItem)
            {
                m_pSortCol->SetData(0);
                m_pSortCol->NotifyInvalidate();
            }
            DWORD dwData = pBkWndItem->GetData();   
            dwData = 1 + dwData % 2;
            m_pSortCol = pBkWndItem;
            m_pSortCol->SetData(dwData);
            RepositionArrow();

			if (bAct)
			{
				if (m_pNotify != NULL && !bDragEnd)
				{
					NotifyInfo info = {BKM_LIST_HIT_HEAD, point, this, MAKELONG(MAKEWORD(nCol, nIndex), MAKEWORD(dwData, 0))};
					m_pNotify->Action(&info);
				}
			}
        } while (FALSE);
    }

    return FALSE;
}

inline void CBkListHead::RepositionArrow()
{
    if (m_pSortCol != NULL && m_pSkinArrow != NULL)
    {
        CRect rcSort;
        m_pSortCol->GetRect(&rcSort);
        SIZE size = m_pSkinArrow->GetSkinSize();
        int nOffset = 4;
        if (rcSort.Width() > nOffset + size.cx)
        {
            m_rcArrow = rcSort;
            m_rcArrow.right -= nOffset;
            m_rcArrow.left = m_rcArrow.right - size.cx;
            m_rcArrow.top = (m_rcArrow.bottom + m_rcArrow.top - size.cy) / 2;
            m_rcArrow.bottom = m_rcArrow.top + size.cy;
        }
        else
        {
            SetRect(m_rcArrow, 0, 0, 0, 0);
        }
    }
}

inline BOOL CBkListHead::RedrawRegion(CDCHandle& dc)
{
    __super::RedrawRegion(dc);

    if (m_pSortCol != NULL && m_pSkinArrow != NULL)
    {
        m_pSkinArrow->Draw(dc, m_rcArrow, m_pSortCol->GetData());
    }
    return FALSE;
}

inline void CBkListHead::RepositionChilds()
{
    __super::RepositionChilds();
    RepositionArrow();
}

inline BOOL CBkListHead::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_bAllowDrag)
    {
        int nEdge = 5;
        if (m_ptPre.x != point.x)
        {
            m_ptPre = point;
            if (m_bDrag)
            {
                int nWidth = point.x - m_ptDown.x + m_nDragSrcWidth;
                if (nWidth <= m_nDragMinWidth)
                {
                    nWidth = m_nDragMinWidth;
                }
                if (m_pNotify != NULL)
                {
                    NotifyInfo info = {BKM_LIST_UPDATE_COL_WIDTH, point, this, MAKELONG(m_nDragCol, nWidth)};
                    m_pNotify->Action(&info);
                }
            }
            else
            {
                BOOL bOldStatus = m_bCanDrag;
                do 
                {
                    m_bCanDrag = FALSE;
                    CBkWnd* pBkWndCol = FindChildByPoint(point, TRUE);
                    if (pBkWndCol != NULL)
                    {
                        int nCol = GetIndexByItem(pBkWndCol);

                        RECT rc;
                        pBkWndCol->GetRect(&rc);
                        if (point.x < rc.left + nEdge && nCol >= 1)
                        {
                            nCol--;
                            m_nDragCol = nCol;
                            m_bCanDrag = TRUE;
                        }
                        else if (point.x > rc.right - nEdge)
                        {
                            m_nDragCol = nCol;
                            m_bCanDrag = TRUE;
                        }
                    }
                    else
                    {
                        if (point.x < m_rcWindow.left + nEdge)
                        {
                            m_nDragCol = _GetItemCount() - 1;
                            m_bCanDrag = TRUE;
                        }
                    }
                } while (FALSE);
                if (bOldStatus != m_bCanDrag)
                {
                    m_hCursor = ::LoadCursor(NULL, m_bCanDrag ? IDC_SIZEWE : IDC_ARROW);
                }
            }
        }
        if (m_bDrag || m_bCanDrag)
        {
            point.x -= nEdge;
        }
    }
    __super::OnMouseMove(nFlags, point);
    return FALSE;
}


//////////////////////////////////////////////////////////////////////////
// CBkListBody
inline CBkListBody::CBkListBody()
{
    m_bClickDnoeSub = TRUE;

    m_pDefaultRow = NULL;
    m_nDownRow = -1;
    m_bDownRowChecked = FALSE;
    m_bMultSelect = FALSE;
	m_bNavigator = FALSE;
}

inline CBkListBody::~CBkListBody()
{
    if (m_pDefaultRow != NULL)
    {
        delete m_pDefaultRow;
        m_pDefaultRow = NULL;
    }
    m_iSelectRows.clear();
}

inline int CBkListBody::AddRow()
{
    CBkListRow* pNewRow = _NewRow();
    if (pNewRow != NULL)
    {
        m_lstWndChild.AddTail(pNewRow);
        RepositionChild(pNewRow);
        pNewRow->NotifyInvalidate();
    }
    return (int)m_lstWndChild.GetCount() - 1;
}

inline int CBkListBody::InsertRow(int nRow)
{
    CBkListRow* pNewRow = _NewRow();
    if (pNewRow != NULL)
    {
        m_lstWndChild.InsertBefore(GetPosByIndex(nRow), pNewRow);
        _UpdateSelectRow(nRow, TRUE);
        RepositionChilds();
        NotifyInvalidate();
    }
    return nRow;
}

inline BOOL CBkListBody::SetItemText(int nRow, int nCol, int nIndex, LPCTSTR szText)
{
    CBkWnd* pItem = _GetItem(nRow, nCol, nIndex);
    if (pItem != NULL)
    {
        pItem->SetText(szText);
    }
    return pItem != NULL;
}

inline LPCWSTR CBkListBody::GetItemText(int nRow, int nCol, int nIndex)
{
    LPCWSTR szText = NULL;
    CBkWnd* pItem = _GetItem(nRow, nCol, nIndex);
    if (pItem != NULL)
    {
        szText = pItem->GetText();
    }
    return szText;
}

inline BOOL CBkListBody::SetRowData(int nRow, DWORD dwData)
{
    CBkListRow* pBkRow = _GetRow(nRow);
    if (pBkRow != NULL)
    {
        pBkRow->SetData(dwData);
    }
    return pBkRow != NULL;
}

inline DWORD CBkListBody::GetRowData(int nRow)
{
    DWORD dwData = 0;
    CBkListRow* pBkRow = _GetRow(nRow);
    if (pBkRow != NULL)
    {
        dwData = pBkRow->GetData();
    }
    return dwData;
}

inline CRect CBkListBody::GetRowRect(int nRow)
{
    CRect rcRow;
    CBkListRow* pBkRow = _GetRow(nRow);
    if (pBkRow != NULL)
    {
        pBkRow->GetRect( &rcRow );
    }
    return rcRow;
}

inline int CBkListBody::GetSelectRow()
{
    int nRow = -1;
    if (!m_iSelectRows.empty())
    {
        nRow = m_iSelectRows[m_iSelectRows.size() - 1];
    }
    return nRow;
}

inline BOOL CBkListBody::SetSelectRows(const std::vector<int>& nRows)
{
    ClearSelectRow();
    for (size_t i = 0; i < nRows.size(); i++)
    {
        SelectRow(nRows[i], TRUE);
    }
    return nRows.empty();
}

inline BOOL CBkListBody::GetSelectRows(std::vector<int>& nRows)
{
    nRows.resize(m_iSelectRows.size());
    std::copy(m_iSelectRows.begin(), m_iSelectRows.end(), nRows.begin());
    return nRows.empty();
}

inline void CBkListBody::SelectAll()
{
	if (IsMultSelect())
	{
		m_iSelectRows.clear();
		UINT nRow = 0;
		POSITION pos = m_lstWndChild.GetHeadPosition();
		while (pos != NULL)
		{
			CBkWnd *pBkWndChild = m_lstWndChild.GetNext(pos);
			pBkWndChild->SetChecked(TRUE);
			m_iSelectRows.push_back(nRow++);
		}
		if (m_pNotify != NULL)
		{
			NotifyInfo info = {BKM_LIST_SELCHANGED, CPoint(0,0), this, GetSelectRow()};
			m_pNotify->Action(&info);
		}
	}
}

inline BOOL CBkListBody::DeleteRow(int nRow)
{
    BOOL bRet = FALSE;
    do 
    {
        POSITION pos = GetPosByIndex(nRow);
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

        if (m_iShowTop > 0)
        {
            CBkWnd* pBkWndTail = m_lstWndChild.GetAt(m_lstWndChild.GetTailPosition());
            if (pBkWndTail != NULL)
            {
                CRect rcTail;
                pBkWndTail->GetRect(&rcTail);
                if (rcTail.bottom >= m_rcWindow.bottom)
                {
                    CRect rcDelRow;
                    pBkWndChild->GetRect(&rcDelRow);
                    m_iShowTop -= rcDelRow.Height();
                    if (m_iShowTop < 0)
                    {
                        m_iShowTop = 0;
                    }
                }
            }
        }

        pBkWndChild->OnDestroy();
        delete pBkWndChild;
        m_lstWndChild.RemoveAt(pos);
        _UpdateSelectRow(nRow, FALSE);
        RepositionChilds();
        NotifyInvalidate();
        bRet = TRUE;
    } while (FALSE);
    return bRet;
}

inline BOOL CBkListBody::DeleteAllRows()
{
    BOOL bRet = !m_lstWndChild.IsEmpty();
    POSITION pos = m_lstWndChild.GetHeadPosition();
    while (pos != NULL)
    {
        CBkWnd* pBkWnd = m_lstWndChild.GetNext(pos);
        pBkWnd->OnDestroy();
        delete pBkWnd;
    }
    if (m_lastHoverChild)
    {
        m_lastHoverChild = NULL;
    }
    m_lstWndChild.RemoveAll();
    NotifyInvalidate();
    m_iSelectRows.clear();
    SetShowTop(0);
    return bRet;
}

inline BOOL CBkListBody::OnWindowPosChanged(LPWINDOWPOS lpWndPos)
{
	BOOL bRet = __super::OnWindowPosChanged(lpWndPos);
	if (m_iShowTop > 0)
	{
		CBkWnd* pBkWndTail = m_lstWndChild.GetAt(m_lstWndChild.GetTailPosition());
		if (pBkWndTail != NULL)
		{
			CRect rcTail;
			pBkWndTail->GetRect(&rcTail);
			if (rcTail.bottom < m_rcWindow.bottom)
			{
				m_iShowTop -= m_rcWindow.bottom - rcTail.bottom;
				if (m_iShowTop < 0)
					m_iShowTop = 0;
			}
		}
	}
	return bRet;
}


inline int CBkListBody::GetRowCount()
{
    return (int)m_lstWndChild.GetCount();
}

inline int CBkListBody::GetSelectRowCount()
{
    return (int)m_iSelectRows.size();
}

inline IBkWnd* CBkListBody::GetItem(int nRow, int nCol, int nIndex)
{
    return _GetItem(nRow, nCol, nIndex);
}

inline void CBkListBody::SortChildren(ISortCompare* compare)
{
    CBkDivV::SortChildren(compare);

    m_iSelectRows.clear();
    int index = 0;
    POSITION pos = m_lstWndChild.GetHeadPosition();
    while (pos != NULL)
    {
        CBkWnd* pBkWnd = m_lstWndChild.GetNext(pos);
        if (pBkWnd && pBkWnd->IsChecked())
        {
            m_iSelectRows.push_back(index);
        }
        index++;
    }
}

// virtual CBkWnd method
inline BOOL CBkListBody::Load(TiXmlElement* pTiXmlElem)
{
    if (!CBkWnd::Load(pTiXmlElem))
        return FALSE;

    if (pTiXmlElem->FirstChildElement() != NULL)
    {
        m_pDefaultRow = pTiXmlElem->FirstChildElement()->Clone();
    }
    return TRUE;
}

inline BOOL CBkListBody::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
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
    if (m_pNotify != NULL)
    {
        NotifyInfo info = {BKM_LIST_MOUSEWHEEL, CPoint(0, 0), this, m_iShowTop};
        m_pNotify->Action(&info);
    }
    return FALSE;
}

inline BOOL CBkListBody::OnMouseMove(UINT nFlags, CPoint point)
{
	if (IsDragInner()&&m_rcWindow.PtInRect(point)&&m_pDropTarget
		&&m_rcWindow.PtInRect(m_pDropTarget->GetLBDownPoint()))
	{
		BkDragData DropItem;
		DropItem.bkItem = this;
		DropItem.dwCmdID = GetCmdID();
		DropItem.ptSrc = point;
		DropItem.dwDragOpt = DRAGOPT_FORM_MYSELF;
		m_pDropTarget->DoDragDrop(&DropItem);
	}

    return __super::OnMouseMove(nFlags, point);
}

inline BOOL CBkListBody::OnMouseLeave()
{
    BOOL bRet = CBkDivV::OnMouseLeave();
    if (m_pNotify != NULL)
    {
        NotifyInfo info;
        info.nAction    = BKM_LIST_LEAVE;
        info.pFrom      = this;
        info.pt         = CPoint(0, 0);
        info.dwData     = (DWORD)MAKELONG(-1, -1);
        m_pNotify->Action(&info);
    }
    return bRet;
}

inline void CBkListBody::OnMouseLeaveChild(CBkWnd* pChild, const CPoint& point)
{
    CBkDivV::OnMouseLeaveChild(pChild, point);
    if (m_pNotify != NULL)
    {
        // 离开Row，鼠标处于Body的空白区域的时候
        if (HitTest(point) < 0)
        {
            int nRow = 0;
            int nCol = -1;
            int nIndex = -1;
            if (pChild != NULL)
            {
                nRow = GetIndexByItem(pChild);
            }

            NotifyInfo info;
            info.nAction    = BKM_LIST_LEAVE;
            info.pFrom      = this;
            info.pt         = CPoint(0, 0);
            info.dwData     = (DWORD)MAKELONG(MAKEWORD(nCol, nIndex), MAKEWORD(nRow, 0));
            m_pNotify->Action(&info);
        }
    }
}

inline BOOL CBkListBody::OnLButtonDown(UINT nFlags, CPoint point)
{
    SetFocus();
    m_lDownPt = point;

    int nRow = HitTest( point );
    if (nRow >= 0)
    {
        ModifyState(BkWndState_PushDown, NULL);
        m_nDownRow = nRow;
        CBkWnd* pDownRow = dynamic_cast<CBkWnd*>(GetItemByIndex(nRow));
        if (pDownRow && pDownRow->IsChecked())
        {
            m_bDownRowChecked = TRUE;
        }
        else
        {
            SelectRow(m_nDownRow, TRUE);
        }

		if (m_pNotify)
		{
			NotifyInfo info = {WM_LBUTTONDOWN, point, this, nRow};
			m_pNotify->Action(&info);
		}
    }
    __super::OnLButtonDown(nFlags, point);
    return FALSE;
}

inline BOOL CBkListBody::OnLButtonUp(UINT nFlags, CPoint point,BOOL bAct)
{
    ModifyState(NULL, BkWndState_PushDown);

    BOOL bSelectRowChanged = FALSE;
    if (m_nDownRow < 0)
    {
        if (!m_bNavigator)
        {
            ClearSelectRow();
            SelectRow(-1);
            bSelectRowChanged = TRUE;
        }
    }
    else
    {
        if ( IsMultSelect() && (nFlags & MK_CONTROL || nFlags & MK_SHIFT) )
        {
            if (m_bDownRowChecked && nFlags & MK_CONTROL)
            {
                SelectRow(m_nDownRow, FALSE);
                bSelectRowChanged = TRUE;
            }

            if (nFlags & MK_SHIFT)
            {
                std::vector<int> selList;
                GetSelectRows(selList);

                if (selList.size())
                {
                    int nBase = selList[0];
                    selList.clear();
                    selList.push_back(nBase);

                    for (int i = nBase + 1; i <= m_nDownRow; i++)
                        selList.push_back(i);
                    for (int i = m_nDownRow; i < nBase; i++)
                        selList.push_back(i);

                    SetSelectRows(selList);
                }
            }
        }
        else
        {
            if (!(m_bDownRowChecked && m_iSelectRows.size() == 1))
            {
                ClearSelectRow();
                SelectRow(m_nDownRow, TRUE);
                bSelectRowChanged = TRUE;
            }
        }
    }

    BOOL bRet = __super::OnLButtonUp(nFlags, point,bAct);
	if (bAct)
	{
		if (m_pNotify != NULL )
		{
			if (bSelectRowChanged)
			{ 
				NotifyInfo info = {BKM_LIST_SELCHANGED, point, this, GetSelectRow()} ;		
				m_pNotify->Action(&info);
			}
			else if(bRet)
			{
				NotifyInfo info = {WM_LBUTTONUP, point, this, GetSelectRow()};
				m_pNotify->Action(&info);
			}
		}
	}

    m_nDownRow = -1;
    m_bDownRowChecked = FALSE;
    return FALSE;
}

inline BOOL CBkListBody::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    __super::OnLButtonDblClk(nFlags, point);

    int nRow = HitTest( point );
    if (nRow >= 0)
    {
        CBkWnd* pDownRow = dynamic_cast<CBkWnd*>(GetItemByIndex(nRow));
        if (pDownRow != NULL)
        {
            ModifyState(BkWndState_PushDown, NULL);
            m_nDownRow = nRow;
            if (pDownRow->IsChecked())
            {
                m_bDownRowChecked = TRUE;
            }
            else
            {
                SelectRow(m_nDownRow, TRUE);
            }
        }
    }
    //CBkWnd* pDownRow = FindChildByPoint(point, TRUE);
    //if (pDownRow != NULL)
    //{
    //    ModifyState(BkWndState_PushDown, NULL);
    //    m_nDownRow = GetIndexByItem(pDownRow);
    //    if (pDownRow->IsChecked())
    //    {
    //        m_bDownRowChecked = TRUE;
    //    }
    //    else
    //    {
    //        SelectRow(m_nDownRow, TRUE);
    //    }
    //}
    return FALSE;
}

inline BOOL CBkListBody::OnDropState(UINT nFlags, CPoint point)
{
	OnRButtonDown(nFlags,point);
    __super::OnRButtonUp(nFlags, point,TRUE);

	return FALSE;
}

inline BOOL CBkListBody::OnResetState(int nRow)
{
	if (find(m_iSelectRows.begin(), m_iSelectRows.end(), nRow) == m_iSelectRows.end())
	{
		ClearSelectRow();
		SelectRow(nRow, TRUE);
	}
	return TRUE;
}

inline BOOL CBkListBody::OnRButtonDown(UINT nFlags, CPoint point)
{
    SetFocus();
    __super::OnRButtonDown(nFlags, point);

	BOOL bSelectRowChanged = FALSE;
    int nRow = HitTest( point );
    if (nRow >= 0)
    {
        if (find(m_iSelectRows.begin(), m_iSelectRows.end(), nRow) == m_iSelectRows.end())
        {
            ClearSelectRow();
            SelectRow(nRow, TRUE);
			bSelectRowChanged = TRUE;
        }
    }else
	{
		ClearSelectRow();
		bSelectRowChanged = TRUE;
	}

	if (m_pNotify != NULL && bSelectRowChanged)
	{
		NotifyInfo info = {BKM_LIST_SELCHANGED, point, this, GetSelectRow()};
		m_pNotify->Action(&info);
	}

    return FALSE;
}

inline BOOL CBkListBody::OnRButtonUp(UINT nFlags, CPoint point,BOOL bAct)
{
    __super::OnRButtonUp(nFlags, point,bAct);
	if (bAct)
	{
		if (m_pNotify != NULL)
		{
			NotifyInfo info = {WM_RBUTTONUP, point, this, GetSelectRow()};
			m_pNotify->Action(&info);
		}
	}
    return FALSE;
}

inline BOOL CBkListBody::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    return FALSE;
}

inline CBkWnd* CBkListBody::OnSetFocus(CBkWnd* which)
{
    CBkWnd* pParentBkWnd = BkWnds::GetWindow(GetParent());
    if (pParentBkWnd)
    {
        m_bFocus = TRUE;
        pParentBkWnd->OnSetFocus(this);
        return this;
    }
    return which;
}

inline BOOL CBkListBody::Action(NotifyInfo* pInfo)
{
    BOOL bRet = FALSE;
    if (m_pNotify != NULL)
    {
        int nRow = GetIndexByItem(dynamic_cast<CBkWnd*>(pInfo->pFrom));
        int nCol = LOWORD(pInfo->dwData);
        int nIndex = HIWORD(pInfo->dwData);

        NotifyInfo info = *pInfo;
        info.pFrom = this;
        info.dwData = (DWORD)MAKELONG(MAKEWORD(nCol, nIndex), MAKEWORD(nRow, 0));
        bRet = m_pNotify->Action(&info);
    }
    return bRet;
}

inline void CBkListBody::ClearSelectRow()
{
    CBkListRow* pRow = NULL;
    for (size_t i = 0; i < m_iSelectRows.size(); i++)
    {
        CBkListRow* pRow = dynamic_cast<CBkListRow*>(GetItemByIndex(m_iSelectRows[i]));
        if (pRow != NULL)
        {
            pRow->SetChecked(FALSE);
        }
    }
    m_iSelectRows.clear();
}

inline BOOL CBkListBody::SelectRow(int nRow, BOOL bSelect)
{
    CBkListRow* pRow = dynamic_cast<CBkListRow*>(GetItemByIndex(nRow));
    if (pRow != NULL)
    {
        pRow->SetChecked(bSelect);
        if (bSelect)
        {
            if (find(m_iSelectRows.begin(), m_iSelectRows.end(), nRow) == m_iSelectRows.end())
            {
                m_iSelectRows.push_back(nRow);
            }
        }
        else
        {
            std::vector<int>::iterator it = find(m_iSelectRows.begin(), m_iSelectRows.end(), nRow);
            if (m_iSelectRows.end() != it)
            {
                m_iSelectRows.erase(it);
            }
        }
    }
    return pRow != NULL;
}

inline void CBkListBody::_UpdateSelectRow(int nRow, BOOL bAdd)
{
    if (bAdd)
    {
        for (std::vector<int>::iterator it = m_iSelectRows.begin(); it != m_iSelectRows.end(); it++)
        {
            if (*it >= nRow)
            {
                (*it)++;
            }
        }
    }
    else
    {
        BOOL bFind = FALSE;
        for (std::vector<int>::iterator it = m_iSelectRows.begin(); it != m_iSelectRows.end(); )
        {
            if (!bFind && *it == nRow)
            {
                it = m_iSelectRows.erase(it);
                bFind = TRUE;
            }
            else
            {
                if (*it >= nRow)
                {
                    (*it)--;
                }
                it++;
            }
        }
    }
}

inline BOOL CBkListBody::IsRowSelected(int nRow)
{
    for (size_t i=0; i<m_iSelectRows.size(); i++)
    {
        if (m_iSelectRows[i] == nRow)
        {
            return TRUE;
        }
    }
    return FALSE;
}

inline int CBkListBody::OnDrop(POINT pt)
{
    if (m_iSelectRows.size() == 0 || m_iSelectRows.size() == GetRowCount())
    {
        return -1;
    }
    std::sort(m_iSelectRows.begin(), m_iSelectRows.end());
    std::vector<CBkWnd*> vBkWnd;
    // O(N*m_iSelectRows.size()) -> O(N) how
    for (std::vector<int>::reverse_iterator rit = m_iSelectRows.rbegin(); rit != m_iSelectRows.rend(); rit++)
    {
        POSITION pos = m_lstWndChild.FindIndex(*rit);
        if (pos)
        {
            vBkWnd.insert(vBkWnd.begin(), m_lstWndChild.GetAt(pos));
        }
        m_lstWndChild.RemoveAt(pos);
    }
    // Find the insert position
    int nRes = 0;
    POSITION posBefore = NULL, posAfter = NULL;
    POSITION visiblePos = NULL;
    CRect rect;
    POSITION pos = m_lstWndChild.GetHeadPosition();
    for (; NULL != pos; ++nRes)// O(N)
    {
        POSITION curPos = pos;
        CBkWnd* pBkWndChild = m_lstWndChild.GetNext(pos);
        pBkWndChild->GetRect(rect);
        if( !pBkWndChild->IsVisibleDeep())
        {
            if (rect.top > m_iShowTop)
            {
                posBefore = curPos;
                break;
            }
            continue;
        }
        if (rect.top >= pt.y)// 判断落点是否在第一行之前
        {
            posBefore = curPos;
            break;
        }
        if (rect.PtInRect(pt))
        {
            if ((rect.top + rect.bottom)/2 >= pt.y)
            {
                posBefore = curPos;
            }
            else
            {
                ++nRes;
                posAfter = curPos;
            }
            break;
        }
        visiblePos = pos;
    }
    if (NULL == posBefore && NULL == posAfter)
    {
        if (visiblePos)//落点在最后一行之后
        {
            nRes = GetIndexByItem(m_lstWndChild.GetAt(visiblePos)) + 1;
            posAfter = visiblePos;
        }
        else
        {
            nRes = static_cast<int>(m_lstWndChild.GetCount());
            posAfter = m_lstWndChild.GetTailPosition();
        }
    }
    if (posBefore)
    {
        for (std::vector<CBkWnd*>::iterator it = vBkWnd.begin(); it != vBkWnd.end(); it++)
        {
            m_lstWndChild.InsertBefore(posBefore, *it);
        }
    }
    else//posAfter
    {
        for (std::vector<CBkWnd*>::reverse_iterator it = vBkWnd.rbegin(); it != vBkWnd.rend(); it++)
        {
            m_lstWndChild.InsertAfter(posAfter, *it);
        }
    }
    for (unsigned int i = 0; i < m_iSelectRows.size(); ++i)
    {
        m_iSelectRows[i] = nRes + i;
    }
    RepositionChilds();
    NotifyInvalidate();
    return nRes;
}

inline BOOL CBkListBody::SetColumnWidth(int nCol, int nWidth)
{
    BOOL bRet = FALSE;
    POSITION pos = m_lstWndChild.GetHeadPosition();
    while (pos != NULL)
    {
        CBkListRow *pBkWndChild = (CBkListRow*)m_lstWndChild.GetNext(pos);
        if (pBkWndChild)
        {
            bRet = pBkWndChild->SetColumnWidth(nCol, nWidth);
        }    
    }
    if (nCol >= (int)m_iColWidths.size())
    {
        m_iColWidths.resize(nCol + 1, -1);
    }
    m_iColWidths[nCol] = nWidth;
    return bRet;
}

inline BOOL CBkListBody::MoveRow(int sourceIdx, int targetIdx)
{
    if (sourceIdx == targetIdx)
    {
        return TRUE;
    }
    POSITION sourcePos = this->GetPosByIndex(sourceIdx);
    if (sourcePos == NULL || targetIdx < 0 || targetIdx >= GetRowCount())
    {
        return FALSE;
    }
    CBkWnd * pBkWnd = m_lstWndChild.GetAt(sourcePos);
    m_lstWndChild.RemoveAt(sourcePos);
    {
        ClearSelectRow();
        POSITION targetPos = this->GetPosByIndex(targetIdx);
        if (targetPos == NULL)
        {
            targetPos = m_lstWndChild.AddTail(pBkWnd);
        }
        else
        {
            targetPos = m_lstWndChild.InsertBefore(targetPos, pBkWnd);
        }
        m_iSelectRows.resize(1, targetIdx);
        m_lstWndChild.GetAt(targetPos)->SetChecked(TRUE);
    }
    RepositionChilds();
    NotifyInvalidate();
    return TRUE;
}

inline BOOL CBkListBody::IsMultSelect()
{
    return m_bMultSelect;
}

inline BOOL CBkListBody::IsNavigator()
{
	return m_bNavigator;
}

//////////////////////////////////////////////////////////////////////////
// CBkList实现

inline CBkWnd* CBkListBody::_GetItem(int nRow, int nCol, int nIndex)
{
    CBkWnd* pBkItem = NULL;
    do 
    {
        CBkListRow* pRow = dynamic_cast<CBkListRow*>(GetItemByIndex(nRow));
        if (pRow == NULL)
        {
            break;
        }

        CBkDiv* pCol = dynamic_cast<CBkDiv*>(pRow->GetItemByIndex(nCol));
        if (pCol == NULL)
        {
            break;
        }

        pBkItem = pCol->_GetItemByIndex(nIndex);
    } while (FALSE);
    return pBkItem;
}

inline CBkListRow* CBkListBody::_NewRow()
{
    CBkListRow* pNewRow = CBkListRow::CheckAndNew(m_pDefaultRow->Value());
    if (pNewRow != NULL)
    {
        pNewRow->RegisterNotify(this);
        pNewRow->SetParent(m_hBkWnd);
        pNewRow->SetContainer(m_hWndContainer);
		pNewRow->SetDropTarget(m_pDropTarget);
        pNewRow->SetRealContainer(m_hWndRealContainer);
        pNewRow->SetParentsVisible(IsVisible());
        pNewRow->SetTabControler(m_tabControler);
        pNewRow->SetTooltipCtrl(m_wndToolTip);
        pNewRow->Load(m_pDefaultRow->ToElement());

        for (size_t i = 0; i < m_iColWidths.size(); i++)
        {
            if (m_iColWidths[i] >= 0)
            {
                pNewRow->SetColumnWidth((int)i, m_iColWidths[i]);
            }
        }
    }
    return pNewRow;
}

inline CBkListRow* CBkListBody::_GetRow(int nRow)
{
    CBkListRow* pBkRow = NULL;
    do 
    {
        POSITION pos = GetPosByIndex(nRow);
        if (pos == NULL)
        {
            break;
        }
        
        pBkRow = dynamic_cast<CBkListRow*>(m_lstWndChild.GetAt(pos));
    } while (FALSE);
    return pBkRow;
}

inline BOOL CBkListBody::IsRowVisible(int nRow)
{
	CBkListRow* pBkRow = _GetRow(nRow);
	if (pBkRow)
		return pBkRow->IsVisible();
	return FALSE;
}

inline BOOL CBkListBody::UpdateRowPosition(int nOldPos, int nNewPos)
{
    BOOL bRet = FALSE;
    do 
    {
        POSITION posOld = GetPosByIndex(nOldPos);
        if (posOld == NULL)
        {
            break;
        }

        CBkWnd* pBkWndChild = m_lstWndChild.GetAt(posOld);
        if (pBkWndChild == NULL)
        {
            break;
        }

        BOOL bTail = FALSE;
        POSITION posNew = GetPosByIndex(nNewPos);
        if (posNew == NULL)
        {
            bTail = TRUE;
        }

        m_lstWndChild.RemoveAt(posOld);
        if (!bTail)
        {
            m_lstWndChild.InsertBefore(posNew, pBkWndChild);
        }
        else
        {
            m_lstWndChild.AddTail(pBkWndChild);
        }

        RepositionChilds();
        NotifyInvalidate();
        bRet = TRUE;
    } while (FALSE);
    return bRet;
}

inline void CBkListBody::SetShowLeft(int iLeft)
{
    POSITION pos = m_lstWndChild.GetHeadPosition();
    while (pos != NULL)
    {
        CBkListRow *pBkWndChild = (CBkListRow*)m_lstWndChild.GetNext(pos);
        if (pBkWndChild)
        {
            pBkWndChild->SetShowLeft(iLeft);
        }
    }
}

inline BOOL CBkListBody::BoldRow(int nRow, BOOL bBold)
{
    CBkListRow* pBkRow = _GetRow(nRow);
    if (pBkRow != NULL)
    {
        pBkRow->Bold(bBold);
    }
    return pBkRow != NULL;
}

inline int CBkListBody::HitTest(CPoint point)
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

inline int CBkListBody::FindRow(LIST_FIND_TYPE emType, DWORD dwData)
{
	for (int i = 0; ; i++)
	{
		CBkListRow* pRow = dynamic_cast<CBkListRow*>(GetItemByIndex(i));
		if (!pRow)
			break;

		if (emType == LIST_FIND_TEXT)
		{
			if (wcscmp(pRow->GetText(), (LPCTSTR)(LPARAM)dwData) == 0)
				return i;
		}
		else if (emType == LIST_FIND_DATA)
		{
			if (pRow->GetData() == dwData)
				return i;
		}
	}

	return -1;
}

inline int CBkListBody::GetRealRowTop(int nRow)
{
	int index = 0;
	int iScrollPos = 0;
	POSITION pos = m_lstWndChild.GetHeadPosition();
	while (pos != NULL)
	{
		if (index++ == nRow)
		{
			break;
		}

		CBkWnd *pBkWndChild = (CBkWnd*)m_lstWndChild.GetNext(pos);
		if (pBkWndChild)
		{
			iScrollPos += pBkWndChild->GetHeight();
		}
	}
	return iScrollPos;
}

//////////////////////////////////////////////////////////////////////////
// CBkListCol
inline CBkListCol::CBkListCol() : CBkDiv()
{
    m_iMinWidth = 0;
}

inline CBkListCol::CBkListCol(const CBkListCol& other) : CBkDiv(other)
{
    m_iMinWidth = other.m_iMinWidth;
}

inline IBkWnd* CBkListCol::MakeCopy()
{
    return new CBkListCol(*this);
}

inline int CBkListCol::GetMinWidth()
{
    return m_iMinWidth;
}



//////////////////////////////////////////////////////////////////////////
// CBkListRow实现
inline CBkListRow::CBkListRow()
{
    m_bClickDnoeSub = TRUE;
}

inline BOOL CBkListRow::NeedRedrawWhenStateChange()
{
    return TRUE;
}

inline void CBkListRow::Bold(BOOL bBold)
{
    POSITION pos = m_lstWndChild.GetHeadPosition();
    while (pos != NULL)
    {
        CBkWnd *pBkWndChild = (CBkWnd*)m_lstWndChild.GetNext(pos);
        CBkText* pBkText = dynamic_cast<CBkText*>(pBkWndChild);
        if (pBkText != NULL)
        {
            pBkText->SetBold(bBold);
            continue;
        }
        CBkPanel* pBkPanel = dynamic_cast<CBkPanel*>(pBkWndChild);
        if (pBkPanel != NULL)
        {
            BoldPanel(pBkPanel, bBold);
        }
    }
}

inline void CBkListRow::BoldPanel(CBkPanel* pBkPanel, BOOL bBold)
{
    // 这么做效率不如放在Panel中遍历，但是Bold这个操作现阶段还不算是Panel的一个普遍方法
    int iCount = pBkPanel->_GetItemCount();
    for (int i = 0; i < iCount; i++)
    {
        CBkWnd *pBkWndChild = pBkPanel->_GetItemByIndex(i);
        CBkText* pBkText = dynamic_cast<CBkText*>(pBkWndChild);
        if (pBkText != NULL)
        {
            pBkText->SetBold(bBold);
            continue;
        }
        CBkPanel* pBkPanel2 = dynamic_cast<CBkPanel*>(pBkWndChild);
        if (pBkPanel2 != NULL)
        {
            BoldPanel(pBkPanel2, bBold);
        }
    }
}

inline BOOL CBkListRow::SetColumnWidth(int nCol, int nWidth)
{
    CBkWnd* pBkItem = NULL;
    do 
    {
        if (nWidth < 0)
        {
            nWidth = 0;
        }

        CBkDiv* pCol = dynamic_cast<CBkDiv*>(GetItemByIndex(nCol));
        if (pCol == NULL)
        {
            break;
        }
        pCol->SetWidth(nWidth);
    } while (FALSE);
    return pBkItem != NULL;
}

inline BOOL CBkListRow::OnMouseHover()
{
    CBkWnd::OnMouseHover();
    __super::OnMouseHover();
    return TRUE;
}

inline BOOL CBkListRow::OnMouseLeave()
{
    CBkWnd::OnMouseLeave();
    __super::OnMouseLeave();
    return TRUE;
}

inline void CBkListRow::OnMouseHoverChild(CBkWnd* pChild, const CPoint& point)
{
    if (m_pNotify != NULL)
    {
        int nCol = 0;
        int nIndex = 0;
        if (pChild != NULL)
        {
            nCol = GetIndexByItem(pChild);
            CBkPanel* pBkPanel = dynamic_cast<CBkPanel*>(pChild);
            if (pBkPanel != NULL)
            {
                CBkWnd* pBkPanelChild = pBkPanel->_GetHoverChild();
                if (pBkPanelChild != NULL)
                {
                    nIndex = pBkPanel->GetIndexByItem(pBkPanelChild);
                }
            }
        }

        NotifyInfo info;
        info.nAction    = BKM_LIST_HOVER;
        info.pFrom      = this;
        info.pt         = CPoint(0, 0);
        info.dwData     = (DWORD)MAKELONG(nCol, nIndex);
        m_pNotify->Action(&info);
    }
}

inline void CBkListRow::OnMouseLeaveChild(CBkWnd* pChild, const CPoint& point)
{
    if (m_pNotify != NULL)
    {
        int nIndex = 0;
        int nCol = 0;
        if (pChild != NULL)
        {
            nCol = GetIndexByItem(pChild);
        }

        NotifyInfo info;
        info.nAction    = BKM_LIST_LEAVE;
        info.pFrom      = this;
        info.pt         = CPoint(0, 0);
        info.dwData     = (DWORD)MAKELONG(nCol, nIndex);
        m_pNotify->Action(&info);
    }
}

inline LPCTSTR CBkListRow::GetText() const
{
	//must like this "<div><text></text></div>"
	IBkWnd *pWndDiv = this->GetItem(0);
	if (pWndDiv)
	{
		IBkWnd *pWndTxt = pWndDiv->GetItem(0);
		if (pWndTxt)
		{
			return pWndTxt->GetText();
		}
	}
	return NULL;
}

inline HBITMAP CBkListRow::GetBitmap()
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


//////////////////////////////////////////////////////////////////////////
// CBkList实现
inline CBkList::CBkList()
{
    m_pListHead = NULL;
    m_pListBody = NULL;
    m_pHScroll = NULL;
    m_pVScroll = NULL;
}

// IDropContainer members
inline HRESULT CBkList::OnDragEnter(IDataObject*, POINT pt, DWORD *pdwEffect)
{
    if (NULL == pdwEffect)
    {
        return E_INVALIDARG;
    }
    assert(m_pListBody != NULL);
    CRect rect;
    m_pListBody->GetRect(rect);
    if (IsDragable()&&rect.PtInRect(pt))
    {
        *pdwEffect = DROPEFFECT_COPY;
    }
    return S_OK;
}

inline HRESULT CBkList::OnDragOver(POINT pt, DWORD *pdwEffect)
{
    if (NULL == pdwEffect)
    {
        return E_INVALIDARG;
    }
    assert(m_pListBody != NULL);
    CRect rect;
    m_pListBody->GetRect(rect);
    if (IsDragable()&&rect.PtInRect(pt))
    {
        *pdwEffect = DROPEFFECT_COPY;
		NotifyInfo info;
		if (m_pNotify != NULL)
		{
			if(m_pDropTarget)
			{	
				int nCurSelRow = GetSelectRow();
				int nRow = HitTest(CPoint(pt));
				info.pFrom      = this;
				info.pt         = pt;
				m_pDropTarget->GetDragInfo()->dwDestIndex = nRow;
				m_pDropTarget->GetDragInfo()->dwEffect = *pdwEffect;
				info.dwData     = (DWORD)m_pDropTarget->GetDragInfo();
				if (m_pDropTarget->GetDragInfo()->dwDragOpt == DRAGOPT_FROM_EXPLORER)
				{
					info.nAction    = BKM_DRAGDROP_DRAGOVER_FROM_EXPLORER;
					m_pNotify->Action(&info);
				}
				else
				{
					info.nAction    = BKM_DRAGDROP_DRAGOVER_FROM_WINDOW_INNER;
					m_pNotify->Action(&info);
				}
				*pdwEffect = m_pDropTarget->GetDragInfo()->dwEffect;
			}
		}
        return S_OK;
    }
    return S_FALSE;
}

inline HRESULT CBkList::OnDrop(IDataObject *pDataObj, POINT pt, DWORD *pdwEffect)
{
    if (NULL == pdwEffect)
    {
        return E_INVALIDARG;
    }
    assert(m_pListBody != NULL);
    if (m_rcWindow.PtInRect(pt))
    {
		NotifyInfo info;
		if (m_pNotify != NULL)
		{
			if(m_pDropTarget)
			{	
				int nCurSelRow = GetSelectRow();
				int nRow = HitTest(CPoint(pt));
				info.pFrom      = this;
				info.pt         = pt;
				info.dwData     = (DWORD)m_pDropTarget->GetDragInfo();
				if (m_pDropTarget->GetDragInfo()->dwDragOpt == DRAGOPT_FROM_EXPLORER)
				{
					info.nAction    = BKM_DRAGDROP_ACCEPTDROP_FROM_EXPLORER;
					m_pListBody->OnDropState(0,pt);
					m_pNotify->Action(&info);
				}
				else
				{
 					info.nAction    = BKM_DRAGDROP_ACCEPTDROP_FROM_WINDOW_INNER;
					m_pDropTarget->GetDragInfo()->dwDestIndex = nRow;
					m_pNotify->Action(&info);
					m_pListBody->OnMouseLeave();
					m_pListBody->OnResetState(nCurSelRow);
				}
			}
		}
        return S_OK;
    }
    return S_FALSE;
}

inline HRESULT CBkList::OnDragLeave( void)
{
	m_pListBody->OnMouseLeave();
	return S_OK;
}

inline int CBkList::AddRow()
{
    assert(m_pListBody != NULL);
    int iRet = m_pListBody->AddRow();
    _CalcSize();
    return iRet;
}

inline int CBkList::InsertRow(int nRow)
{
    assert(m_pListBody != NULL);
    int iRet = m_pListBody->InsertRow(nRow);
    _CalcSize();
    return iRet;
}

inline BOOL CBkList::SetItemText(int nRow, int nCol, int nIndex, LPCTSTR szText)
{
    assert(m_pListBody != NULL);
    return m_pListBody->SetItemText(nRow, nCol, nIndex, szText);
}

inline LPCWSTR CBkList::GetItemText(int nRow, int nCol, int nIndex)
{
    assert(m_pListBody != NULL);
    return m_pListBody->GetItemText(nRow, nCol, nIndex);
}

inline BOOL CBkList::SetRowData(int nRow, DWORD dwData)
{
    assert(m_pListBody != NULL);
    return m_pListBody->SetRowData(nRow, dwData);
}

inline DWORD CBkList::GetRowData(int nRow)
{
    assert(m_pListBody != NULL);
    return m_pListBody->GetRowData(nRow);
}

inline CRect CBkList::GetRowRect(int nRow)
{
	assert(m_pListBody != NULL);
	return m_pListBody->GetRowRect(nRow);
}

inline int CBkList::GetSelectRow()
{
    assert(m_pListBody != NULL);
    return m_pListBody->GetSelectRow();
}

inline BOOL CBkList::SetSelectRows(const std::vector<int>& nRows)
{
    assert(m_pListBody != NULL);
    return m_pListBody->SetSelectRows(nRows);
}

inline BOOL CBkList::GetSelectRows(std::vector<int>& nRows)
{
    assert(m_pListBody != NULL);
    return m_pListBody->GetSelectRows(nRows);
}

inline void CBkList::SelectAll()
{
    assert(m_pListBody != NULL);
    return m_pListBody->SelectAll();
}

inline BOOL CBkList::DeleteRow(int nRow)
{
    assert(m_pListBody != NULL);
    BOOL bRet = m_pListBody->DeleteRow(nRow);
    if (bRet)
    {
        _CalcSize();
    }
    return bRet;
}

inline BOOL CBkList::DeleteAllRows()
{
    assert(m_pListBody != NULL);
    BOOL bRet = m_pListBody->DeleteAllRows();
    if (bRet)
    {
        m_pVScroll->SetValue(0);
        _CalcSize();
    }
    return bRet;
}

inline int CBkList::GetRowCount()
{
    assert(m_pListBody != NULL);
    return m_pListBody->GetRowCount();
}

inline int CBkList::GetSelectRowCount()
{
    assert(m_pListBody != NULL);
    return m_pListBody->GetSelectRowCount();
}

inline IBkWnd* CBkList::GetItem(int nRow, int nCol, int nIndex)
{
    assert(m_pListBody != NULL);
    return m_pListBody->GetItem(nRow, nCol, nIndex);
}

inline BOOL CBkList::UpdateRowPosition(int nOldPos, int nNewPos)
{
    assert(m_pListBody != NULL);
    return m_pListBody->UpdateRowPosition(nOldPos, nNewPos);
}

inline int CBkList::GetRealHeight()
{
    int iHeight = 0;
    if (m_pListHead != NULL)
    {
        CRect rc;
        m_pListHead->GetRect(&rc);
        iHeight += rc.Height();
    }
    if (m_pListBody != NULL)
    {
        iHeight += m_pListBody->GetRealHeight();
    }
    return iHeight;
}

inline int CBkList::GetColumnCount()
{
    if (m_pListHead)
    {
        return m_pListHead->_GetItemCount();
    }
    return 0;
}

inline IBkWnd* CBkList::GetHeadItem(int nCol, int index)
{
    if (m_pListHead)
    {
        return m_pListHead->GetItem(nCol, index);
    }
    return 0;
}

inline BOOL CBkList::BoldRow(int nRow, BOOL bBold)
{
    assert(m_pListBody != NULL);
    return m_pListBody->BoldRow(nRow, bBold);
}

inline BOOL CBkList::HitTest(CPoint point)
{
	assert(m_pListBody != NULL);
	return m_pListBody->HitTest(point);
}

inline int CBkList::FindRow(LIST_FIND_TYPE emType, DWORD dwData)
{
	assert(m_pListBody != NULL);
	return m_pListBody->FindRow(emType, dwData);
}


inline void CBkList::ScrollToTop()
{
    if (m_pVScroll && m_pVScroll->IsVisible())
    {
        m_pVScroll->Scroll(SB_TOP, 0);
    }
}

inline void CBkList::ScrollToBottom()
{
    if (m_pVScroll && m_pVScroll->IsVisible())
    {
        m_pVScroll->Scroll(SB_BOTTOM, 0);
    }
}

inline void CBkList::ScrollToRow(int nRow)
{
    if ( m_pVScroll && m_pVScroll->IsVisible() && !m_pListBody->IsRowVisible(nRow))
    {
        //CRect rcRow = m_pListBody->GetRowRect(nRow);
        //if (rcRow.Width() <= 0 || rcRow.Height() <= 0)
        //{
        //    return;
        //}
        if (nRow < 0)
        {
            return;
        }
		int iScrollPos = m_pListBody->GetRealRowTop(nRow);
        m_pVScroll->Scroll(SB_THUMBPOSITION, iScrollPos);
    }
}

inline BOOL CBkList::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{  
	BOOL bRet = __super::OnKeyDown(nChar, nRepCnt, nFlags);

	if (nChar == VK_UP || nChar == VK_DOWN)
	{
		std::vector<int> selList;
		GetSelectRows(selList);

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

			int nBaseTmp = nBase;
			if (nChar == VK_DOWN)
				nBaseTmp = nBase > nFirst ? nFirst : nLast;
			else
				nBaseTmp = nLast > nBase ? nLast : nFirst;

			if (nChar == VK_DOWN)
				nCur = nBaseTmp + 1;
			else
				nCur = nBaseTmp - 1;
		}

		if (nCur >= GetRowCount())
			nCur = GetRowCount() - 1;

		if (nCur >= 0)
		{
			if (bShift && m_pListBody->IsMultSelect())
			{
				selList.clear();
				selList.push_back(nBase);

				for (int i = nBase + 1; i <= nCur; i++)
					selList.push_back(i);
				for (int i = nCur; i < nBase; i++)
					selList.push_back(i);

				m_pListBody->SetSelectRows(selList);
			}
			else
			{
				ClearSelectRow();
				SelectRow(nCur, TRUE);

				if (m_pNotify != NULL)
				{
					NotifyInfo info = {BKM_LIST_SELCHANGED, CPoint(0,0), this, nCur};
					m_pNotify->Action(&info);
				}
			}

			ScrollToRow(nCur);
		}
	}
	else if (nChar == 'a' || nChar == 'A')
	{
		SHORT nKeyState = GetKeyState(VK_CONTROL);
		BOOL bCtrlDown = nKeyState & 0x80;
		if (bCtrlDown)
		{
			SelectAll();
		}
	}
	else if (nChar == VK_RETURN || nChar == VK_DELETE || nChar == VK_DELETE || nChar == VK_F2)
	{
		NotifyInfo info;
		info.nAction = BKM_LIST_KEYDOWN;
		info.pFrom = this;
		info.dwData = nChar;
		m_pNotify->Action(&info);
		return TRUE;
	}

	return FALSE;
}

inline BOOL CBkList::LoadChilds(TiXmlElement* pTiXmlChildElem)
{
    BOOL bVisible = IsVisibleDeep(TRUE);
    for (TiXmlElement* pXmlChild = pTiXmlChildElem; NULL != pXmlChild; pXmlChild = pXmlChild->NextSiblingElement())
    {
        CString strNode = pXmlChild->Value();
        CBkWnd* pBkWnd = NULL;
        if (strNode == CBkListHead::GetClassName())
        {
            m_pListHead = new CBkListHead();
            pBkWnd = m_pListHead;
        }
        else if (strNode == CBkListBody::GetClassName())
        {
            m_pListBody = new CBkListBody();
            pBkWnd = m_pListBody;
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

    m_pHScroll = new CBkScroll();
    m_pHScroll->RegisterNotify(this);
    m_pHScroll->SetType(CBkScroll::STE_H);
    m_pHScroll->SetSkin(m_style.m_strScrollH);
    m_pHScroll->SetParent(m_hBkWnd);
    m_pHScroll->SetContainer(m_hWndContainer);
	m_pVScroll->SetDropTarget(m_pDropTarget);
    m_pHScroll->SetRealContainer(m_hWndRealContainer);
    m_pHScroll->SetVisible(FALSE);
    m_lstWndChild.AddTail(m_pHScroll);

    assert(m_pListBody != NULL);
    return TRUE;
}

inline BOOL CBkList::Action(NotifyInfo* pInfo)
{
    if ( pInfo->nAction == BKM_LIST_HIT_HEAD || 
         pInfo->nAction == BKM_LIST_SELCHANGED ||
         pInfo->nAction == BKM_LIST_KEYDOWN ||
         pInfo->nAction == BKM_LIST_HOVER ||
         pInfo->nAction == BKM_LIST_LEAVE ||
         pInfo->nAction == WM_RBUTTONUP ||
         pInfo->nAction == BKM_LIST_MOUSEWHEEL ||
		 pInfo->nAction == WM_LBUTTONUP ||
		 pInfo->nAction == WM_LBUTTONDOWN )
	{
        if (m_pNotify != NULL)
        {
            NotifyInfo info = *pInfo;
            info.pFrom = this;
            m_pNotify->Action(&info);
        }
    }
    
    if (pInfo->nAction == BKM_LIST_MOUSEWHEEL)
    {
        m_pVScroll->SetValue(pInfo->dwData);
    }
    else if (pInfo->nAction == BKM_LIST_SCROLL_V)
    {
        m_pListBody->SetShowTop(pInfo->dwData);
    }
    else if (pInfo->nAction == BKM_LIST_SCROLL_H)
    {
        if (m_pListHead != NULL)
        {
            m_pListHead->SetShowLeft(pInfo->dwData);
        }
        m_pListBody->SetShowLeft(pInfo->dwData);
    }
    else if (pInfo->nAction == BKM_LIST_UPDATE_COL_WIDTH)
    {
        SetColumnWidth(LOWORD(pInfo->dwData), HIWORD(pInfo->dwData));
    }

    return TRUE;
}

inline void CBkList::_CalcSize()
{
    CRect rcBody;
    m_pListBody->GetRect(&rcBody);
    int iBodyHeight = rcBody.Height();
    int iRealHeight = m_pListBody->GetRealHeight();
    int iHeadWidth = 0;
    int iRealWidth = 0;
    if (m_pListHead != NULL)
    {
        CRect rcHead;
        m_pListHead->GetRect(&rcHead);
        iHeadWidth = rcHead.Width();
        iRealWidth = m_pListHead->GetRealWidth();
    }

    BOOL bRepositionAll = (iRealHeight > iBodyHeight) != m_pVScroll->NeedShow();
    bRepositionAll |= (iRealWidth > iHeadWidth) != m_pHScroll->NeedShow();
    if (iRealHeight > iBodyHeight || m_pVScroll->NeedShow())
    {
        m_pVScroll->SetMax(iRealHeight);
        m_pVScroll->SetPage(iBodyHeight);

        if (bRepositionAll)
        {
            BKDLG_POSITION pos;
            m_pListBody->GetPos(&pos);
            pos.Right.bMinus = TRUE;
            pos.Right.nPos = (iRealHeight > iBodyHeight) ? m_pVScroll->GetWidth() : pos.Left.nPos;
            m_pListBody->SetPos(&pos, FALSE);

            pos.Left.bMinus = TRUE;
            pos.Left.nPos = m_pVScroll->GetWidth();
            pos.Right.bMinus = TRUE;
            pos.Right.nPos = 0;
            if (m_pHScroll->NeedShow())
            {
                pos.Bottom.bMinus = TRUE;
                pos.Bottom.nPos = m_pHScroll->GetHeight();
            }
            m_pVScroll->SetPos(&pos, FALSE);
            m_pVScroll->SetVisible(TRUE);
        }
        else
        {
            m_pVScroll->NotifyInvalidate();
        }
    }
    if (iRealWidth > iHeadWidth || m_pHScroll->NeedShow())
    {
        m_pHScroll->SetMax(iRealWidth);
        m_pHScroll->SetPage(iHeadWidth);

        if (bRepositionAll)
        {
            BKDLG_POSITION pos;
            m_pListBody->GetPos(&pos);
            pos.Bottom.nPos = (iRealWidth > iHeadWidth) ? m_pHScroll->GetHeight() : 0;
            m_pListBody->SetPos(&pos, FALSE);

            pos.Top.bMinus = TRUE;
            pos.Top.nPos = m_pHScroll->GetHeight();
            pos.Bottom.bMinus = TRUE;
            pos.Bottom.nPos = 0;
            if (m_pVScroll->NeedShow())
            {
                pos.Right.bMinus = TRUE;
                pos.Right.nPos = m_pVScroll->GetWidth();
            }
            m_pHScroll->SetPos(&pos, FALSE);
            m_pHScroll->SetVisible(TRUE);
        }
        else
        {
            m_pHScroll->NotifyInvalidate();
        }
    }

    if (bRepositionAll)
    {
        RepositionChilds();
        NotifyInvalidate();
    }
}

inline BOOL CBkList::OnWindowPosChanged(LPWINDOWPOS lpWndPos)
{
    __super::OnWindowPosChanged(lpWndPos);
    _CalcSize();
    return FALSE;
}

inline BOOL CBkList::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    __super::OnLButtonDblClk(nFlags, point);
    if (m_pNotify != NULL)
    {
        CRect rect;
        if (m_pListBody)
        {
            m_pListBody->GetRect(&rect);
            if (rect.PtInRect(point))
            {
                NotifyInfo info = {BKM_LIST_LDBCLICK, point, this, m_pListBody->GetSelectRow()};
                m_pNotify->Action(&info);
            }
        }
    }
    return FALSE;
}

inline BOOL CBkList::OnLButtonUp(UINT nFlags, CPoint point,BOOL bAct)
{
    __super::OnLButtonUp(nFlags, point,bAct);
    return FALSE;
}

inline BOOL CBkList::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    m_pListBody->OnMouseWheel( nFlags, zDelta, pt );
    return FALSE;
}

inline BOOL CBkList::IsRowSelected(int nRow)
{
    return m_pListBody->IsRowSelected(nRow);
}

inline void CBkList::ClearSelectRow()
{
    m_pListBody->ClearSelectRow();
}

inline BOOL CBkList::SelectRow(int nRow, BOOL bSelect)
{
    return m_pListBody->SelectRow(nRow, bSelect);
}

inline BOOL CBkList::SetColumnWidth(int nCol, int nWidth)
{
    if (m_pListHead != NULL)
    {
        m_pListHead->SetColumnWidth(nCol, nWidth);
    }
    assert(m_pListBody != NULL);
    BOOL bRet = m_pListBody->SetColumnWidth(nCol, nWidth);
    RepositionChilds();
    NotifyInvalidate();
    _CalcSize();
    return bRet;
}

inline BOOL CBkList::MoveRow(int sourceIdx, int targetIdx)
{
    return m_pListBody->MoveRow(sourceIdx, targetIdx);
}

inline void CBkList::SortRows( ISortCompare* compare )
{
    assert(m_pListBody != NULL);
    m_pListBody->SortChildren( compare );
}

inline CBkWnd* CBkList::OnSetFocus( CBkWnd* which )
{
    m_bFocus = TRUE;
	__super::OnSetFocus( this );
	m_focusChild = which;
    return this;
}
