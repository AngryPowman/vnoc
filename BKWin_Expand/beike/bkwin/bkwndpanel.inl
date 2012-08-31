//////////////////////////////////////////////////////////////////////////
// CBkPanel实现

inline CBkPanel::CBkPanel(const CBkPanel& bkPanel) : CBkWnd(bkPanel)
{
    POSITION pos = bkPanel.m_lstWndChild.GetHeadPosition();
    while (pos)
    {
        CBkWnd* pChild = dynamic_cast<CBkWnd*>(bkPanel.m_lstWndChild.GetNext(pos)->MakeCopy());
        INotify* pNotify = dynamic_cast<INotify*>(this);
        //pChild->RegisterNotify(this);
        pChild->SetParent(m_hBkWnd);
        pChild->SetContainer(m_hWndContainer);
		pChild->SetDropTarget(m_pDropTarget);
        pChild->SetRealContainer(m_hWndRealContainer);
        pChild->SetParentsVisible(IsVisible());
        pChild->SetTabControler(m_tabControler);
        pChild->SetTooltipCtrl(m_wndToolTip);
        pChild->CreateHandle(pChild->GetContainer());
        m_lstWndChild.AddTail(pChild);
    }
    m_lastHoverChild = bkPanel.m_lastHoverChild;
    m_focusChild = bkPanel.m_focusChild;
    m_bUpdateFocus = bkPanel.m_bUpdateFocus;
    m_bClickDnoeSub = bkPanel.m_bClickDnoeSub;
}

inline CBkPanel::CBkPanel()
{
    m_lastHoverChild = NULL;
    m_focusChild = NULL;
    m_bUpdateFocus = TRUE;
    m_bClickDnoeSub = FALSE;
	m_iNeedScale = 0;
	m_fScaleX = 0.0f;
	m_fScaleY = 0.0f;
}

inline BOOL CBkPanel::Load(TiXmlElement* pTiXmlElem)
{
    if (!CBkWnd::Load(pTiXmlElem))
        return FALSE;

    return LoadChilds(pTiXmlElem->FirstChildElement());
}

inline void CBkPanel::SetDropTarget(IBkDropTarget* pDropTarget)
{
	__super::SetDropTarget(pDropTarget);
	POSITION pos = m_lstWndChild.GetHeadPosition();
	while (pos != NULL)
	{
		CBkWnd *pBkWndChild = m_lstWndChild.GetNext(pos);
		if (pBkWndChild)
		{
			pBkWndChild->SetDropTarget(pDropTarget);
		}
	}
}

inline void CBkPanel::SetContainer(HWND hWndContainer)
{
    __super::SetContainer(hWndContainer);
    POSITION pos = m_lstWndChild.GetHeadPosition();
    while (pos != NULL)
    {
        CBkWnd *pBkWndChild = m_lstWndChild.GetNext(pos);
        if (pBkWndChild)
        {
            pBkWndChild->SetContainer(hWndContainer);
        }
    }
}

inline void CBkPanel::SetRealContainer(HWND hWndRealContainer)
{
    __super::SetRealContainer(hWndRealContainer);
    POSITION pos = m_lstWndChild.GetHeadPosition();
    while (pos != NULL)
    {
        CBkWnd *pBkWndChild = m_lstWndChild.GetNext(pos);
        if (pBkWndChild)
        {
            pBkWndChild->SetRealContainer(hWndRealContainer);
        }
    }
}

inline BOOL CBkPanel::IsContainer()
{
    return TRUE;
}

inline CBkWnd* CBkPanel::FindChildByCmdID(UINT uCmdID) const
{
    CBkWnd *pChildFind = NULL;
    POSITION pos = m_lstWndChild.GetHeadPosition();
    while (pos != NULL)
    {
        CBkWnd *pBkWndChild = m_lstWndChild.GetNext(pos);

        if (pBkWndChild->GetCmdID() == uCmdID)
            return pBkWndChild;
    }
    pos = m_lstWndChild.GetHeadPosition();
    while (pos != NULL)
    {
        CBkWnd *pBkWndChild = m_lstWndChild.GetNext(pos);
        if (pBkWndChild->IsContainer())
        {
            pChildFind = pBkWndChild->FindChildByCmdID(uCmdID);
            if (pChildFind)
                return pChildFind;
        }
    }
    return NULL;
}

inline CBkWnd* CBkPanel::FindChildByPoint(const CPoint& pt, BOOL bBackground)
{
    CBkWnd *pChildFind = NULL;
    POSITION pos = m_lstWndChild.GetHeadPosition();
    while (pos != NULL)
    {
        CBkWnd *pBkWndChild = m_lstWndChild.GetNext(pos);
        if (!bBackground && pBkWndChild->IsBackground())
        {
            continue;
        }
        else if (pBkWndChild->PtInRect(pt))
        {
            return pBkWndChild;
        }
    }
    return NULL;
}

inline BOOL CBkPanel::RedrawRegion(CDCHandle& dc)
{
	HBITMAP hTempDCBitmap = NULL;
	HDC hTempDC = NULL;
	if (m_iNeedScale)
	{
		hTempDC = ::CreateCompatibleDC(dc);
		hTempDCBitmap = _CreateCompatibleBitmap(m_rcWindow.Width(), m_rcWindow.Height());
		::SelectObject(hTempDC, hTempDCBitmap);
		::SetBkMode(hTempDC, TRANSPARENT);
	}

	CDCHandle& targetDC = CDCHandle(m_iNeedScale? hTempDC : dc.m_hDC);

	if (__super::RedrawRegion(targetDC))
    {
        //if (m_pEffect && m_pEffect->IsUpdate())
        if (m_pEffectRender && m_pEffectRender->IsEffects())
        {
        }
        else
        {
            CRgn rgnPanel;
            dc.GetClipRgn(rgnPanel);
            //DPRgn(L"CBkPanel", rgnPanel);
            POSITION pos = m_lstWndChild.GetTailPosition();
            while (pos != NULL)
            {
                CBkWnd *pBkWndChild = m_lstWndChild.GetPrev(pos);
                if (pBkWndChild && pBkWndChild->IsVisibleDeep(TRUE) )
                {
                    targetDC.SelectClipRgn(rgnPanel);
                    pBkWndChild->RedrawRegion(targetDC);
                }
            }

			if (m_iNeedScale)
			{
				int scaledWidth = m_rcWindow.Width() * m_fScaleX;
				int scaledHeight = m_rcWindow.Height() * m_fScaleY;
				int scaledX = m_rcWindow.left + (m_rcWindow.Width() - scaledWidth) / 2;
				int scaledY = m_rcWindow.top + (m_rcWindow.Height() - scaledHeight) / 2;

				BLENDFUNCTION m_BlendFunction;
				m_BlendFunction.BlendOp = AC_SRC_OVER;  
				m_BlendFunction.BlendFlags = 0;
				m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
				m_BlendFunction.SourceConstantAlpha = 255;

				AlphaBlend(
					dc.m_hDC, 
					scaledX,
					scaledY,
					scaledWidth,
					scaledHeight,
					targetDC,
					0,
					0,
					m_rcWindow.Width(),
					m_rcWindow.Height(),
					m_BlendFunction
					);

				DeleteObject(hTempDCBitmap);
				DeleteDC(hTempDC);
			}
        }
        return TRUE;
    }

    return FALSE;
}

inline void CBkPanel::OnChildLoad( TiXmlElement* pTiXmlElem,CBkWnd* pChildWindow )
{

}

inline BOOL CBkPanel::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_hBkWndParent == NULL && _GetDragWnd() != NULL)
    {
        _GetDragWnd()->OnMouseMove(nFlags, point);
    }

    if (!IsVisible() || IsDisableDeep(FALSE))
    {
        return TRUE;
    }

    CBkWnd* pBkWndHover = NULL;
    CBkWnd* pBkWndLeave = NULL;
    POSITION pos = m_lstWndChild.GetHeadPosition();
    CRect rcChild;
    while (pos)
    {
        CBkWnd* pBkWndChild = m_lstWndChild.GetNext(pos);
        if( !pBkWndChild->IsVisible() || pBkWndChild->IsDisableDeep() )
            continue;

        pBkWndChild->GetRect(rcChild);
        if( rcChild.PtInRect(point) )
        {
            pBkWndHover = pBkWndChild;
            break;
        }
        else
        {
            // 需要判断一下上一次的Hover控件是否
            if (m_lastHoverChild == pBkWndChild)
            {
                pBkWndLeave = pBkWndChild;
            }
        }
    } 

    // 处理不在Child列表里的子控件
    OnExtraChildMouseMove(point,pBkWndHover,pBkWndLeave);

    // 若没有离开原来的控件，进入了一个新控件，两个控件有重合，则强制离开上一个Hover控件
    if( pBkWndHover && m_lastHoverChild != pBkWndHover && !pBkWndLeave )
    {
        pBkWndLeave = m_lastHoverChild;
    }

    if( pBkWndHover )
    {
        pBkWndHover->OnMouseMove(nFlags, point);
    }

    if( pBkWndLeave )
    {
        OnMouseLeaveChild(pBkWndLeave, point);
        pBkWndLeave->OnMouseLeave();
    }

    if( pBkWndHover && m_lastHoverChild != pBkWndHover )
    {
        OnMouseHoverChild(pBkWndHover, point);

        pBkWndHover->OnMouseHover();
    }

    m_lastHoverChild = pBkWndHover;
    return TRUE;
}

inline void CBkPanel::OnMouseHoverChild(CBkWnd* pChild, const CPoint& point)
{

}

inline void CBkPanel::OnMouseLeaveChild(CBkWnd* pChild, const CPoint& point)
{

}

inline CBkWnd* CBkPanel::_GetHoverChild()
{
    return m_lastHoverChild;
}

inline BOOL CBkPanel::OnMouseHover()
{
    //OnMouseHoverChild(m_lastHoverChild);
    return TRUE;
}

inline BOOL CBkPanel::OnMouseLeave()
{
    //OnMouseLeaveChild(m_lastHoverChild);

    if (!IsVisible() || IsDisableDeep(FALSE))
    {
        return FALSE;
    }

    CBkWnd* pBkWndLeave = NULL;
    POSITION pos = m_lstWndChild.GetHeadPosition();
    CRect rcChild;
    while( pos )
    {
        CBkWnd* pBkWndChild = m_lstWndChild.GetNext(pos);
        if( !pBkWndChild->IsVisible() || pBkWndChild->IsDisableDeep() )
            continue;

        if( m_lastHoverChild == pBkWndChild )
        {
            pBkWndLeave = pBkWndChild;
        }
    }    

    // 处理不在Child列表里的子控件
    OnExtraChildMouseLeave( pBkWndLeave );

    if( pBkWndLeave )
    {
        pBkWndLeave->OnMouseLeave();
        m_lastHoverChild = NULL;
        return TRUE;
    }
    return FALSE;
}

inline BOOL CBkPanel::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (!IsVisible() || IsDisableDeep(FALSE))
    {
        return FALSE;
    }

    POSITION pos = m_lstWndChild.GetHeadPosition();
    while (pos)
    {
        CBkWnd* pBkWndChild = m_lstWndChild.GetNext(pos);
        if (!pBkWndChild->IsVisible() || pBkWndChild->IsDisableDeep())
            continue;

        if (pBkWndChild->PtInRect(point))
        {
            if( m_bUpdateFocus )
            {
                m_focusChild = pBkWndChild;
            }
            if (!pBkWndChild->OnLButtonDown(nFlags, point))
            {
                if (!pBkWndChild->IsBackground() &&
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
    return TRUE;
}

inline BOOL CBkPanel::OnLButtonUp(UINT nFlags, CPoint point,BOOL bAct)
{
    if (!IsVisible() || IsDisableDeep(FALSE))
    {
        return FALSE;
    }

    POSITION pos = m_lstWndChild.GetHeadPosition();
    BOOL bHandled = FALSE;
    while (pos)
    {
        CBkWnd* pBkWndChild = m_lstWndChild.GetNext(pos);
        if( !pBkWndChild->IsVisible() || pBkWndChild->IsDisableDeep() )
            continue;

        if (pBkWndChild->PtInRect(point))
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
        _GetDragWnd()->OnLButtonUp(nFlags, point,bAct);
        _SetDragWnd(NULL);
    }

    if (m_bClickDnoeSub)
    {
        return !bHandled;
    }

    return TRUE;
}

inline BOOL CBkPanel::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    if (!IsVisible() || IsDisableDeep(FALSE))
    {
        return FALSE;
    }

    BOOL bHitCaption = TRUE;
    POSITION pos = m_lstWndChild.GetHeadPosition();
    while (pos)
    {
        CBkWnd* pBkWndChild = m_lstWndChild.GetNext(pos);
        if( !pBkWndChild->IsVisible() || pBkWndChild->IsDisableDeep() )
            continue;

        if (pBkWndChild->PtInRect(point))
        {
            if( pBkWndChild && !pBkWndChild->IsBackground() )
            {
                bHitCaption = FALSE;
            }

            if (!pBkWndChild->OnLButtonDblClk(nFlags, point))
            {
                break;
            }
        }
    }

    if ( bHitCaption && IsBackground() )
    {
        ::PostMessage(m_hWndContainer, WM_NCLBUTTONDBLCLK, HTCAPTION, MAKELPARAM(point.x, point.y));
        return FALSE;
    }

    return TRUE;
}

inline BOOL CBkPanel::OnRButtonDown(UINT nFlags, CPoint point)
{
    _SetDragWnd(NULL);

    if (!IsVisible() || IsDisableDeep(FALSE))
    {
        return FALSE;
    }

    POSITION pos = m_lstWndChild.GetHeadPosition();
    while (pos)
    {
        CBkWnd* pBkWndChild = m_lstWndChild.GetNext(pos);
        if( !pBkWndChild->IsVisible() || pBkWndChild->IsDisableDeep() )
            continue;

        if (pBkWndChild->PtInRect(point))
        {
            if( m_bUpdateFocus )
            {
                m_focusChild = pBkWndChild;
            }
            if (!pBkWndChild->OnRButtonDown(nFlags, point))
            {
                break;
            }
        }
    }
    return TRUE;
}

inline BOOL CBkPanel::OnRButtonUp(UINT nFlags, CPoint point,BOOL bAct)
{
    if (!IsVisible() || IsDisableDeep(FALSE))
    {
        return FALSE;
    }

    POSITION pos = m_lstWndChild.GetHeadPosition();
    while (pos)
    {
        CBkWnd* pBkWndChild = m_lstWndChild.GetNext(pos);
        if( !pBkWndChild->IsVisible() || pBkWndChild->IsDisableDeep() )
            continue;

        if (pBkWndChild->PtInRect(point))
        {
            if (!pBkWndChild->OnRButtonUp(nFlags, point,bAct))
            {
                return FALSE;
            }
        }
    }
    return TRUE;
}

inline BOOL CBkPanel::OnRButtonDblClk( UINT nFlags, CPoint point )
{
    return TRUE;
}

inline BOOL CBkPanel::OnMouseWheel( UINT nFlags, short zDelta, CPoint pt )
{
    if (!IsVisible() || IsDisableDeep(FALSE))
    {
        return FALSE;
    }

    POSITION pos = m_lstWndChild.GetHeadPosition();
    while (pos)
    {
        CBkWnd* pBkWndChild = m_lstWndChild.GetNext(pos);
        if (!pBkWndChild->IsVisible() || pBkWndChild->IsDisableDeep())
            continue;

        if (pBkWndChild->PtInRect(pt))
        {
            if (!pBkWndChild->OnMouseWheel(nFlags, zDelta, pt))
            {
                break;
            }
        }
    } 
    return TRUE;
}

inline BOOL CBkPanel::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if( m_focusChild && m_focusChild->GetContainer() == m_hWndContainer )
    {
		if (m_focusChild != this)		// 这个地方会出现m_focusChild == this的情况，然后程序挂掉，请杨达看下
			return m_focusChild->OnKeyDown( nChar,nRepCnt,nFlags );
    }
    return FALSE;
}

inline BOOL CBkPanel::OnDestroy()
{
	m_lastHoverChild = NULL;

    POSITION pos = m_lstWndChild.GetHeadPosition();
    while (pos != NULL)
    {
        CBkWnd *pBkWndChild = m_lstWndChild.GetNext(pos);
        pBkWndChild->OnDestroy();
        delete pBkWndChild;
    }
    m_lstWndChild.RemoveAll();

	return __super::OnDestroy();
}


inline void CBkPanel::OnSetCursor()
{
    if (m_lastHoverChild != NULL)
    {
		if (m_lastHoverChild->IsVisible())
			m_lastHoverChild->OnSetCursor();
    }
    else
    {
        __super::OnSetCursor();
    }
}

inline void CBkPanel::OnVisibleChanged(BOOL bVisible)
{
    CBkWnd::OnVisibleChanged(bVisible);

    POSITION pos = m_lstWndChild.GetHeadPosition();
    while (pos != NULL)
    {
        CBkWnd *pBkWndChild = m_lstWndChild.GetNext(pos);
        pBkWndChild->SetParentsVisible( IsVisible() & bVisible );
        pBkWndChild->OnVisibleChanged( IsVisible() & bVisible );
    }
}

inline void CBkPanel::OnDisableChanged(BOOL bDisable)
{
    CBkWnd::OnDisableChanged(bDisable);

    POSITION pos = m_lstWndChild.GetHeadPosition();
    while (pos != NULL)
    {
        CBkWnd *pBkWndChild = m_lstWndChild.GetNext(pos);
        pBkWndChild->OnDisableChanged( IsDisable() & bDisable );
    }
}

inline void CBkPanel::OnEffectChanged(BOOL bEffect)
{
    CBkWnd::OnEffectChanged(bEffect);

    POSITION pos = m_lstWndChild.GetHeadPosition();
    while (pos != NULL)
    {
        CBkWnd *pBkWndChild = m_lstWndChild.GetNext(pos);
        pBkWndChild->OnEffectChanged( bEffect );
    }
}

inline void CBkPanel::DrawDisableShade(CDCHandle& dc)
{
    //POSITION pos = m_lstWndChild.GetHeadPosition();
    //while (pos != NULL)
    //{
    //    CBkWnd *pBkWndChild = m_lstWndChild.GetNext(pos);
    //    pBkWndChild->DrawDisableShade( dc );
    //}

    CBkWnd::DrawDisableShade(dc);
}

inline void CBkPanel::OnExtraChildMouseMove( CPoint point,CBkWnd* &pHover,CBkWnd* &pLeave )
{
}

inline void CBkPanel::OnExtraChildMouseLeave( CBkWnd* &pLeave )
{
}

inline void CBkPanel::_ClearLastHover()
{
    m_lastHoverChild = NULL;
}

inline void CBkPanel::_ClearFocus( CBkWnd* focusChild )
{
    if( focusChild == m_focusChild )
    {
        m_focusChild = NULL;
    }

	__super::_ClearFocus(focusChild);
}

inline void CBkPanel::_ClearDrag(CBkWnd* dragChild)
{
    if( dragChild == this )
    {
        __super::_ClearDrag(dragChild);
    }
    else if( _GetDragWnd() == dragChild )
    {
        _SetDragWnd(NULL);
    }
}

inline void CBkPanel::_Sort( int indexStart, int indexEnd, ISortCompare* compare )
{
    if( indexStart >= 0 && indexStart < (int)m_lstWndChild.GetCount() &&
        indexEnd >= 0 && indexEnd < (int)m_lstWndChild.GetCount() &&
        compare )
    {
        for( int i = indexStart; i < indexEnd; ++i )
        {
            for( int j = indexStart; j < indexEnd-i; ++j )
            {
                if( compare->Compare(j,j+1) )
                {
                    POSITION posLeft = GetPosByIndex(j);
                    POSITION posRight = GetPosByIndex(j+1);
                    m_lstWndChild.SwapElements( posLeft, posRight );
                }
            }
        }
        RepositionChilds();
        NotifyInvalidate();
    }
}

inline int CBkPanel::_HitTest(CPoint point)
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

inline int CBkPanel::_GetItemCount()
{
    return (int)m_lstWndChild.GetCount();
}

inline CBkWnd* CBkPanel::_GetItemByIndex(int nIndex)
{
    CBkWnd* pBkWnd = NULL;
    POSITION pos = GetPosByIndex(nIndex);
    if (pos != NULL)
    {
        pBkWnd = m_lstWndChild.GetAt(pos);
    }
    return pBkWnd;

}

inline IBkWnd* CBkPanel::GetItem(UINT uItemID) const
{
    return FindChildByCmdID( uItemID );
}

inline IBkWnd* CBkPanel::GetItemByIndex(UINT nIndex)
{
    return _GetItemByIndex(nIndex);
}

inline POSITION CBkPanel::GetPosByIndex(UINT nIndex)
{
    return m_lstWndChild.FindIndex(nIndex);
}

inline POSITION CBkPanel::GetPosByItem(CBkWnd* pBkWnd)
{
    POSITION posRet = NULL;
    POSITION pos = m_lstWndChild.GetHeadPosition();
    while (pos != NULL)
    {
        if (pBkWnd == m_lstWndChild.GetAt(pos))
        {
            posRet = pos;
            break;
        }
        else
        {
            m_lstWndChild.GetNext(pos);
        }
    }
    return posRet;
}

inline int CBkPanel::GetIndexByItem(CBkWnd* pBkWnd)
{
    int nRet = -1;
    int nLoop = 0;
    POSITION pos = m_lstWndChild.GetHeadPosition();
    while (pos != NULL)
    {
        if (pBkWnd == m_lstWndChild.GetAt(pos))
        {
            nRet = nLoop;
            break;
        }
        else
        {
            nLoop++;
            m_lstWndChild.GetNext(pos);
        }
    }
    return nRet;
}

inline void CBkPanel::SortChild(CBkWnd* pBkWnd,ISortCompare* compare)
{
    if( pBkWnd &&pBkWnd->IsContainer() )
    {
        CBkPanel* pChild = (CBkPanel*)pBkWnd;
        pChild->SortChildren( compare );
    }
}

inline void CBkPanel::SortChildren(ISortCompare* compare)
{
    _Sort( 0, (int)m_lstWndChild.GetCount()-1, compare );
}

#include "bkwndFactory.h"
inline BOOL CBkPanel::LoadChilds(TiXmlElement* pTiXmlChildElem)
{
    OnDestroy();
    for (TiXmlElement* pXmlChild = pTiXmlChildElem; NULL != pXmlChild; pXmlChild = pXmlChild->NextSiblingElement())
    {
        CBkWnd *pNewChildWindow = _CreateBkWindowByName(pXmlChild->Value());
        if( pNewChildWindow )
        {
            pNewChildWindow->SetParent(m_hBkWnd);
			pNewChildWindow->SetDropTarget(m_pDropTarget);
            pNewChildWindow->SetContainer(m_hWndContainer);
            pNewChildWindow->SetRealContainer(m_hWndRealContainer);
            pNewChildWindow->SetParentsVisible(IsVisible());
            pNewChildWindow->SetTabControler(m_tabControler);
            pNewChildWindow->SetTooltipCtrl(m_wndToolTip);
            pNewChildWindow->Load(pXmlChild);
        }
        OnChildLoad( pXmlChild,pNewChildWindow );
        if (!pNewChildWindow)
            continue;

        m_lstWndChild.AddTail(pNewChildWindow);
    }
    return TRUE;
}

inline CBkWnd* CBkPanel::_GetDragWnd()
{
    BkInMesUpdateDragItem nms;
    nms.hdr.code        = BKINM_UPDATE_DRAGITEM;
    nms.hdr.hwndFrom    = NULL;
    nms.hdr.idFrom      = m_uCmdID;
    nms.hbkWnd          = m_hBkWnd;
    nms.oper            = FALSE;
    nms.pBkWnd          = NULL;
    ::SendMessage(m_hWndContainer, WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);
    return nms.pBkWnd;
}

inline void CBkPanel::_SetDragWnd(CBkWnd* pBkWnd)
{
    BkInMesUpdateDragItem nms;
    nms.hdr.code        = BKINM_UPDATE_DRAGITEM;
    nms.hdr.hwndFrom    = NULL;
    nms.hdr.idFrom      = m_uCmdID;
    nms.hbkWnd          = m_hBkWnd;
    nms.oper            = TRUE;
    nms.pBkWnd          = pBkWnd;
    ::SendMessage(m_hWndContainer, WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);
}


inline HRESULT CBkPanel::OnDragEnter(IDataObject *pDataObj, POINT pt, DWORD *pdwEffect)
{
	if (NULL == pdwEffect)
	{
		return E_INVALIDARG;
	}
	if (m_rcWindow.PtInRect(pt))
	{
		HRESULT hr = S_FALSE;
		if (IsDragable())
		{
			*pdwEffect = DROPEFFECT_COPY;
			hr = S_OK;
		}

		POSITION pos = m_lstWndChild.GetHeadPosition();
		while (pos)
		{
			CBkWnd* pBkWndChild = m_lstWndChild.GetNext(pos);
			if( !pBkWndChild->IsVisible()|| pBkWndChild->IsDisableDeep())
				continue;

			if (pBkWndChild->PtInRect(pt))
			{
				pBkWndChild->OnDragEnter(pDataObj,pt,pdwEffect);
			}
		}
		return hr;
	}

	return S_FALSE;
}

inline HRESULT CBkPanel::OnDragOver(POINT pt, DWORD *pdwEffect)
{
	HRESULT hr = S_FALSE;

	POSITION pos = m_lstWndChild.GetHeadPosition();
	while (pos)
	{
		CBkWnd* pBkWndChild = m_lstWndChild.GetNext(pos);
		if( !pBkWndChild->IsVisible()|| pBkWndChild->IsDisableDeep())
			continue;

		//if (pBkWndChild->PtInRect(pt))
		{
			pBkWndChild->OnDragOver(pt,pdwEffect);
		}
	}

	if (m_rcWindow.PtInRect(pt)&&IsDragable())
	{
		*pdwEffect = DROPEFFECT_COPY;
		hr = S_OK;
	}
	return hr;
}

inline HRESULT CBkPanel::OnDrop(IDataObject *pDataObj, POINT pt, DWORD *pdwEffect)
{
	if (NULL == pdwEffect)
	{
		return E_INVALIDARG;
	}

	if (!IsVisible() || IsDisableDeep(FALSE))
	{
		return S_FALSE;
	}

	BOOL bDroped = FALSE;
	POSITION pos = m_lstWndChild.GetHeadPosition();
	while (pos)
	{
		CBkWnd* pBkWndChild = m_lstWndChild.GetNext(pos);
		if( !pBkWndChild->IsVisible()|| pBkWndChild->IsDisableDeep())
			continue;

		if (pBkWndChild->PtInRect(pt))
		{
			HRESULT hRet = pBkWndChild->OnDrop(pDataObj,pt,pdwEffect);
			if (hRet == S_OK)
			{
				return S_OK;
			}
		}
	}

	return CBkWnd::OnDrop(pDataObj,pt,pdwEffect);
}

inline HRESULT CBkPanel::OnDragLeave(void)
{
	OnMouseLeave();
	return S_OK;
}

inline HBITMAP CBkPanel::_CreateCompatibleBitmap(int width, int height)  
{  
	BYTE*                     pBits = NULL;   
	BITMAPINFOHEADER          bmih;   
	ZeroMemory( &bmih, sizeof(BITMAPINFOHEADER));   

	bmih.biSize                 = sizeof(BITMAPINFOHEADER) ;   
	bmih.biWidth                = width;   
	bmih.biHeight               = height;   
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

inline void CBkPanel::SetScaleX(float x) 
{
	m_fScaleX = x; 

}

inline void CBkPanel::SetScaleY(float y) 
{ 
	m_fScaleY = y; 
}