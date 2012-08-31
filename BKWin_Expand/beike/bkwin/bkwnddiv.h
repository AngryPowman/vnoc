#pragma once


//////////////////////////////////////////////////////////////////////////
// CBkDiv声明
class CBkDiv : public CBkPanel
{
    BKOBJ_DECLARE_CLASS_NAME(CBkDiv, "div")

public:
    virtual BOOL OnWindowPosChanged(LPWINDOWPOS lpWndPos);

protected:
    virtual void RepositionChilds();
    virtual void RepositionChildsFrom(CBkWnd *pBkWndChild);
    virtual void RepositionChild(CBkWnd *pBkWndChild);
    virtual void RepositionChild(CBkWnd *pBkWndChild, const CRect& rcWnd);
};


//////////////////////////////////////////////////////////////////////////
// CBkDivH
class CBkDivH : public CBkPanel
{
    BKOBJ_DECLARE_CLASS_NAME(CBkDivH, "divH")

public:
    CBkDivH();
    void SetShowLeft(int iLeft);
    int  GetRealWidth();

    // virtual method of CBkPanel
    virtual BOOL OnWindowPosChanged(LPWINDOWPOS lpWndPos);
	virtual void RepositionChilds();
protected:
    virtual void RepositionChildsFrom(CBkWnd *pBkWndChild);
    virtual void RepositionChild(CBkWnd *pBkWndChild);

	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_INT_ATTRIBUTE("right_align", bRightAlign, FALSE)
	BKWIN_DECLARE_ATTRIBUTES_END()	

protected:
	BOOL bRightAlign;
    int m_iShowLeft;
};


//////////////////////////////////////////////////////////////////////////
// CBkDivH
class CBkDivV : public CBkPanel
{
    BKOBJ_DECLARE_CLASS_NAME(CBkDivV, "divV")

public:
    CBkDivV();
    void SetShowTop(int iTop);
	int GetShowTop();
    int  GetRealHeight();
    
    // virtual method of CBkPanel
    virtual BOOL OnWindowPosChanged(LPWINDOWPOS lpWndPos);
	virtual void RepositionChilds();
protected:
    
    virtual void RepositionChildsFrom(CBkWnd *pBkWndChild);
    virtual void RepositionChild(CBkWnd *pBkWndChild);

protected:
    int m_iShowTop;
};


//////////////////////////////////////////////////////////////////////////
// CBkDivH
class CBkDivZ : public CBkPanel
{
    BKOBJ_DECLARE_CLASS_NAME(CBkDivZ, "divZ")

public:
    CBkDivZ() : m_iShowTop(0)
              , m_itemsOffsetH(0)
              , m_itemsOffsetV(0)
              , m_isScrollBK(FALSE) {}
    
    void    SetShowTop(int iTop);
    int     GetShowTop();
    int     GetRealHeight();
    int     GetItemsOffsetH();
    int     GetItemsOffsetV();

    virtual BOOL OnWindowPosChanged(LPWINDOWPOS lpWndPos);
    virtual void DrawBkgnd(CDCHandle& dc);

protected:
    virtual void RepositionChilds();
    virtual void RepositionChildsFrom(CBkWnd *pBkWndChild);
    virtual void RepositionChild(CBkWnd *pBkWndChild);

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_INT_ATTRIBUTE("scrollbg", m_isScrollBK, FALSE)
        BKWIN_INT_ATTRIBUTE("items_offset_h", m_itemsOffsetH, FALSE)
        BKWIN_INT_ATTRIBUTE("items_offset_v", m_itemsOffsetV, FALSE)
    BKWIN_DECLARE_ATTRIBUTES_END()

protected:
    int     m_iShowTop;             // [临时]滚动的顶
    int     m_itemsOffsetH;         // 绘制Items的水平偏移值，设置后可从此点开始作为顶点开始绘制Items
    int     m_itemsOffsetV;         // 绘制Items的竖直偏移值，设置后可从此点开始作为顶点开始绘制Items
    BOOL    m_isScrollBK;           // 是否滚动背景
};


//////////////////////////////////////////////////////////////////////////
// CBkFile声明
class CBkFile : public CBkDiv
{
    BKOBJ_DECLARE_CLASS_NAME(CBkFile, "file")

protected:
    virtual BOOL Load(TiXmlElement* pTiXmlElem);
};



//////////////////////////////////////////////////////////////////////////
// CBkDiv实现
inline BOOL CBkDiv::OnWindowPosChanged(LPWINDOWPOS lpWndPos)
{
    CBkWnd::OnWindowPosChanged(lpWndPos);
    RepositionChilds();
    return TRUE;
}

inline void CBkDiv::RepositionChilds()
{
    POSITION pos = m_lstWndChild.GetHeadPosition();

    while (pos != NULL)
    {
        CBkWnd *pBkWndChild = m_lstWndChild.GetNext(pos);

        RepositionChild(pBkWndChild);
    }
}

inline void CBkDiv::RepositionChildsFrom(CBkWnd *pBkWndChild)
{
    CBkWnd* pBkWndPos = pBkWndChild;
    POSITION pos = GetPosByItem(pBkWndChild);
    do
    {
        pBkWndPos = m_lstWndChild.GetNext(pos);
        RepositionChild(pBkWndPos);
    } while( pos != NULL );
}

inline void CBkDiv::RepositionChild(CBkWnd *pBkWndChild)
{
    BKDLG_POSITION dlgPos;
    WINDOWPOS WndPos = {0};
    CRect rcDlg = m_rcWindow;

    rcDlg.DeflateRect(m_style.m_nMarginX, m_style.m_nMarginY);

    pBkWndChild->GetPos(&dlgPos);

    if (dlgPos.Left.bCenter)
        WndPos.x = rcDlg.left + rcDlg.Width() / 2 + (dlgPos.Left.bMinus ? -1 : 1) *  dlgPos.Left.nPos;
    else if (dlgPos.Left.bMinus)
        WndPos.x = rcDlg.left + rcDlg.Width() - dlgPos.Left.nPos;
    else
        WndPos.x = rcDlg.left + dlgPos.Left.nPos;

    if (dlgPos.Top.bCenter)
        WndPos.y = rcDlg.top + rcDlg.Height() / 2 + (dlgPos.Top.bMinus ? -1 : 1) *  dlgPos.Top.nPos;
    else if (dlgPos.Top.bMinus)
        WndPos.y = rcDlg.top + rcDlg.Height() - dlgPos.Top.nPos;
    else
        WndPos.y = rcDlg.top + dlgPos.Top.nPos;

    WndPos.cx = rcDlg.Width();
    WndPos.cy = rcDlg.Height();
    pBkWndChild->OnWindowPosChanged( &WndPos );
}

inline void CBkDiv::RepositionChild(CBkWnd *pBkWndChild, const CRect& rcWnd)
{
    if (!rcWnd.IsRectNull() && pBkWndChild)
    {
        BKDLG_POSITION dlgPos;
        WINDOWPOS WndPos = {0};
        CRect rcDlg = rcWnd;

        rcDlg.DeflateRect(m_style.m_nMarginX, m_style.m_nMarginY);

        pBkWndChild->GetPos(&dlgPos);

        if (dlgPos.Left.bCenter)
            WndPos.x = rcDlg.left + rcDlg.Width() / 2 + (dlgPos.Left.bMinus ? -1 : 1) *  dlgPos.Left.nPos;
        else if (dlgPos.Left.bMinus)
            WndPos.x = rcDlg.left + rcDlg.Width() - dlgPos.Left.nPos;
        else
            WndPos.x = rcDlg.left + dlgPos.Left.nPos;

        if (dlgPos.Top.bCenter)
            WndPos.y = rcDlg.top + rcDlg.Height() / 2 + (dlgPos.Top.bMinus ? -1 : 1) *  dlgPos.Top.nPos;
        else if (dlgPos.Top.bMinus)
            WndPos.y = rcDlg.top + rcDlg.Height() - dlgPos.Top.nPos;
        else
            WndPos.y = rcDlg.top + dlgPos.Top.nPos;

        WndPos.cx = rcDlg.Width();
        WndPos.cy = rcDlg.Height();
        pBkWndChild->OnWindowPosChanged( &WndPos );
    }

}

//////////////////////////////////////////////////////////////////////////
// CBkDivH实现
inline CBkDivH::CBkDivH()
{
    m_iShowLeft = 0;
	bRightAlign = FALSE;
}

inline void CBkDivH::SetShowLeft(int iLeft)
{
    if (m_iShowLeft != iLeft)
    {
        m_iShowLeft = iLeft;
        if (IsVisible())
        {
            RepositionChilds();
            NotifyInvalidate();
        }
    }
}

inline int CBkDivH::GetRealWidth()
{
    int iWidth = 0;
    POSITION pos = m_lstWndChild.GetHeadPosition();
    while (pos != NULL)
    {
        CBkWnd *pBkWndChild = (CBkWnd*)m_lstWndChild.GetNext(pos);
        if (pBkWndChild)
        {
            iWidth += pBkWndChild->GetWidth();
        }
    }
    return iWidth;
}

inline BOOL CBkDivH::OnWindowPosChanged(LPWINDOWPOS lpWndPos)
{
    CBkWnd::OnWindowPosChanged(lpWndPos);
    RepositionChilds();
    return TRUE;
}

inline void CBkDivH::RepositionChilds()
{
	if (bRightAlign)
	{
		CRect rcCol = m_rcWindow;
		rcCol.left -= m_iShowLeft;
		CRect rc2;

		POSITION pos = m_lstWndChild.GetHeadPosition();
		while (pos != NULL)
		{
			CBkWnd *pBkWndChild = (CBkWnd*)m_lstWndChild.GetNext(pos);
			if (pBkWndChild && pBkWndChild->IsVisibleDeep())
			{
				SIZE size;
				pBkWndChild->OnNcCalcSize(&size);

				WINDOWPOS WndPos = {0};
				WndPos.x = rcCol.right - size.cx;
				WndPos.y = rcCol.top;
				WndPos.cx = rcCol.Width();
				WndPos.cy = rcCol.Height();
				pBkWndChild->OnWindowPosChanged(&WndPos);

				pBkWndChild->GetRect(rc2);
				rcCol.left = m_rcWindow.left;
				rcCol.right = rc2.left;
			}
		}
	}
	else
	{
		CRect rcCol = m_rcWindow;
		rcCol.left -= m_iShowLeft;
		CRect rc2;

		POSITION pos = m_lstWndChild.GetHeadPosition();
		while (pos != NULL)
		{
			CBkWnd *pBkWndChild = (CBkWnd*)m_lstWndChild.GetNext(pos);
			if (pBkWndChild && pBkWndChild->IsVisibleDeep())
			{
				WINDOWPOS WndPos = {0};
				WndPos.x = rcCol.left;
				WndPos.y = rcCol.top;
				WndPos.cx = rcCol.Width();
				WndPos.cy = rcCol.Height();
				pBkWndChild->OnWindowPosChanged(&WndPos);

				pBkWndChild->GetRect(rc2);
				rcCol.left = rc2.right;
				rcCol.right = m_rcWindow.right;
			}
		}
	}
}

inline void CBkDivH::RepositionChildsFrom(CBkWnd *pBkWndChild)
{
    CBkWnd* pBkWndPos = pBkWndChild;
    POSITION pos = GetPosByItem(pBkWndChild);
    do
    {
        pBkWndPos = m_lstWndChild.GetNext(pos);
        RepositionChild(pBkWndPos);
    } while( pos != NULL );
}

inline void CBkDivH::RepositionChild(CBkWnd *pBkWndChild)
{
    POSITION posChild = GetPosByItem(pBkWndChild);
    POSITION posHead = m_lstWndChild.GetHeadPosition();
    if (posChild == posHead)
    {
        WINDOWPOS WndPos = {0};
        WndPos.x = m_rcWindow.left - m_iShowLeft;
        WndPos.y = m_rcWindow.top;
        WndPos.cx = m_rcWindow.Width() + m_iShowLeft;
        WndPos.cy = m_rcWindow.Height();
        pBkWndChild->OnWindowPosChanged(&WndPos);
    }
    else
    {
        CRect rcPre;
        m_lstWndChild.GetPrev(posChild);
        CBkWnd* preChild = m_lstWndChild.GetAt(posChild);
        if( preChild )
        {
            preChild->GetRect(&rcPre);
            int nChildWidth = pBkWndChild->GetWidth();
            if (rcPre.left > m_rcWindow.right || 
                rcPre.right + nChildWidth < m_rcWindow.left ||
                !preChild->IsVisible())
            {
                pBkWndChild->SetVisible(FALSE);
            }
            else
            {
                WINDOWPOS WndPos = {0};
                WndPos.x = rcPre.right;
                WndPos.y = m_rcWindow.top;
                WndPos.cx = m_rcWindow.right - rcPre.right;
                WndPos.cy = m_rcWindow.Height();
                pBkWndChild->OnWindowPosChanged(&WndPos);
            }
        }
    }
}


//////////////////////////////////////////////////////////////////////////
// CBkDivV
inline CBkDivV::CBkDivV()
{
    m_iShowTop = 0;
}

inline void CBkDivV::SetShowTop(int iTop)
{
    if (iTop != m_iShowTop)
    {
        m_iShowTop = iTop;
        if (IsVisible())
        {
            RepositionChilds();
            NotifyInvalidate();
        }
    }
}

inline int CBkDivV::GetShowTop()
{
	return m_iShowTop;
}

inline int CBkDivV::GetRealHeight()
{
    int iHeight = 0;
    POSITION pos = m_lstWndChild.GetHeadPosition();
    while (pos != NULL)
    {
        CBkWnd *pBkWndChild = (CBkWnd*)m_lstWndChild.GetNext(pos);
        if (pBkWndChild)
        {
            iHeight += pBkWndChild->GetHeight();
        }
    }
    return iHeight;
}

inline BOOL CBkDivV::OnWindowPosChanged(LPWINDOWPOS lpWndPos)
{
    CBkWnd::OnWindowPosChanged(lpWndPos);
    RepositionChilds();
    return TRUE;
}

inline void CBkDivV::RepositionChilds()
{
    int iCurTop = 0 - m_iShowTop;
    POSITION pos = m_lstWndChild.GetHeadPosition();
    while (pos != NULL)
    {
        CBkWnd *pBkWndChild = (CBkWnd*)m_lstWndChild.GetNext(pos);
        int nChildHeight = pBkWndChild->GetHeight();
        if (iCurTop + nChildHeight <= 0)
        {
            pBkWndChild->SetVisible(FALSE);
        }
        else
        {
            pBkWndChild->SetVisible(TRUE);
            WINDOWPOS WndPos = {0};
            WndPos.x = m_rcWindow.left;
            WndPos.y = m_rcWindow.top + iCurTop;
            WndPos.cx = m_rcWindow.Width();
            WndPos.cy = m_rcWindow.Height() - iCurTop;
            pBkWndChild->OnWindowPosChanged(&WndPos);
        }
        iCurTop += nChildHeight;
        if (iCurTop >= m_rcWindow.Height())
        {
            break;
        }
    }
    while (pos != NULL)
    {
        CBkWnd *pBkWndChild = (CBkWnd*)m_lstWndChild.GetNext(pos);
        pBkWndChild->SetVisible(FALSE);
    }
}

inline void CBkDivV::RepositionChildsFrom(CBkWnd *pBkWndChild)
{
    CBkWnd* pBkWndPos = pBkWndChild;
    POSITION pos = GetPosByItem(pBkWndChild);
    do
    {
        pBkWndPos = m_lstWndChild.GetNext(pos);
        RepositionChild(pBkWndPos);
    } while( pos != NULL );
}

inline void CBkDivV::RepositionChild(CBkWnd *pBkWndChild)
{
    POSITION posChild = GetPosByItem(pBkWndChild);
    POSITION posHead = m_lstWndChild.GetHeadPosition();
    if (posChild == posHead)
    {
        WINDOWPOS WndPos = {0};
        WndPos.x = m_rcWindow.left;
        WndPos.y = m_rcWindow.top - m_iShowTop;
        WndPos.cx = m_rcWindow.Width();
        WndPos.cy = m_rcWindow.Height() + m_iShowTop;
        pBkWndChild->OnWindowPosChanged(&WndPos);
    }
    else
    {
        CRect rcPre;
        m_lstWndChild.GetPrev(posChild);
        CBkWnd* preChild = m_lstWndChild.GetAt(posChild);
        if( preChild )
        {
            preChild->GetRect(&rcPre);
            int nChildHeight = pBkWndChild->GetHeight();
            if (rcPre.bottom >= m_rcWindow.bottom || 
                rcPre.bottom + nChildHeight <= m_rcWindow.top ||
                !preChild->IsVisibleDeep(FALSE))
            {
                pBkWndChild->SetVisible(FALSE);
            }
            else
            {
                WINDOWPOS WndPos = {0};
                WndPos.x = m_rcWindow.left;
                WndPos.y = rcPre.bottom;
                WndPos.cx = m_rcWindow.Width();
                WndPos.cy = m_rcWindow.bottom - rcPre.bottom;
                pBkWndChild->OnWindowPosChanged(&WndPos);
            }
        }
    }
}


//////////////////////////////////////////////////////////////////////////
// CBkDivZ
inline void CBkDivZ::SetShowTop(int iTop)
{
    if (iTop != m_iShowTop)
    {
        m_iShowTop = iTop;
        if (IsVisible())
        {
            RepositionChilds();
            NotifyInvalidate();
        }
    }
}

inline int CBkDivZ::GetShowTop()
{
    return m_iShowTop;
}

inline int CBkDivZ::GetRealHeight()
{
    int lineWidth = 0;
    int iHeight = 0;
    POSITION pos = m_lstWndChild.GetHeadPosition();
    while (pos != NULL)
    {
        CBkWnd *pBkWndChild = (CBkWnd*)m_lstWndChild.GetNext(pos);
        if (pBkWndChild)
        {
            lineWidth += pBkWndChild->GetWidth();
            if( iHeight <= 0 )
            {
                iHeight += pBkWndChild->GetHeight();
            }
            else if( lineWidth > m_rcWindow.Width() )
            {
                iHeight += pBkWndChild->GetHeight();
                lineWidth = pBkWndChild->GetWidth();
            }
        }
    }
    return iHeight;
}

inline int CBkDivZ::GetItemsOffsetH()
{
    return m_itemsOffsetH;
}

inline int CBkDivZ::GetItemsOffsetV()
{
    return m_itemsOffsetV;
}

inline BOOL CBkDivZ::OnWindowPosChanged(LPWINDOWPOS lpWndPos)
{
    CBkWnd::OnWindowPosChanged(lpWndPos);
    RepositionChilds();
    return TRUE;
}

inline void CBkDivZ::RepositionChilds()
{
    int iCurLeft = m_itemsOffsetH;
    int iCurTop = m_itemsOffsetV - m_iShowTop;
    POSITION pos = m_lstWndChild.GetHeadPosition();
    while (pos != NULL)
    {
        CBkWnd* pBkWndChild = (CBkWnd*)m_lstWndChild.GetNext(pos);
        int nChildWidth = pBkWndChild->GetWidth();
        int nChildHeight = pBkWndChild->GetHeight();
        if( iCurTop + nChildHeight <= 0 )
        {
            pBkWndChild->SetVisible(FALSE);
        }
        else
        {
            pBkWndChild->SetVisible(TRUE);

            WINDOWPOS WndPos = {0};
            WndPos.x = m_rcWindow.left + iCurLeft;
            WndPos.y = m_rcWindow.top + iCurTop;
            WndPos.cx = m_rcWindow.Width() - iCurLeft;
            WndPos.cy = m_rcWindow.Height() - iCurTop;
            pBkWndChild->OnWindowPosChanged(&WndPos);
        }

        iCurLeft += nChildWidth;
        if( iCurLeft > m_rcWindow.Width() )
        {
            iCurLeft = 0;
            iCurTop += nChildHeight;
        }

        // 判断一下此ITEM尺寸是否越界，若越界更换一行重新计算位置
        if( iCurLeft == 0 )
        {
            CRect rcChild;
            pBkWndChild->GetRect(rcChild);
            //if( rcChild.right > m_rcWindow.right )
            {
                //// 若重新计算的位置已经不属于显示区域了，将此ITEM设置为不显示
                //if( iCurTop - m_iShowTop > m_rcWindow.Height() )
                //{
                //    pBkWndChild->SetVisible(FALSE);
                //}
                //else
                //{
                //    pBkWndChild->SetVisible(TRUE);

                //    WINDOWPOS WndPos = {0};
                //    WndPos.x = m_rcWindow.left + iCurLeft;
                //    WndPos.y = m_rcWindow.top + iCurTop;
                //    WndPos.cx = m_rcWindow.Width() - iCurLeft;
                //    WndPos.cy = m_rcWindow.Height() - iCurTop;
                //    pBkWndChild->OnWindowPosChanged(&WndPos);

                //    iCurLeft += nChildWidth;
                //}

                ///////////////////////////////////
                WINDOWPOS WndPos = {0};
                WndPos.x = m_rcWindow.left + iCurLeft;
                WndPos.y = m_rcWindow.top + iCurTop;
                WndPos.cx = m_rcWindow.Width() - iCurLeft;
                WndPos.cy = m_rcWindow.Height() - iCurTop;
                pBkWndChild->OnWindowPosChanged(&WndPos);

                // 若重新计算的位置已经不属于显示区域了，将此ITEM设置为不显示
                if( iCurTop + nChildHeight <= 0 || iCurTop >= m_rcWindow.Height() )
                {
                    pBkWndChild->SetVisible(FALSE);
                }
                else
                {
                    pBkWndChild->SetVisible(TRUE);
                }
				iCurLeft += nChildWidth;
            }
        }

        if( iCurTop >= m_rcWindow.Height() )
        {
            break;
        }
    }
    while (pos != NULL)
    {
        CBkWnd *pBkWndChild = (CBkWnd*)m_lstWndChild.GetNext(pos);
        pBkWndChild->SetVisible(FALSE);
    }
}

inline void CBkDivZ::RepositionChildsFrom(CBkWnd *pBkWndChild)
{
    CBkWnd* pBkWndPos = pBkWndChild;
    POSITION pos = GetPosByItem(pBkWndChild);
    do
    {
        pBkWndPos = m_lstWndChild.GetNext(pos);
        RepositionChild(pBkWndPos);
    } while( pos != NULL );
}

inline void CBkDivZ::RepositionChild(CBkWnd *pBkWndChild)
{
    POSITION posChild = GetPosByItem(pBkWndChild);
    POSITION posHead = m_lstWndChild.GetHeadPosition();
    if (posChild == posHead)
    {
        WINDOWPOS WndPos = {0};
        WndPos.x = m_rcWindow.left + m_itemsOffsetH;
        WndPos.y = m_rcWindow.top + m_itemsOffsetV - m_iShowTop;
        WndPos.cx = m_rcWindow.Width();
        WndPos.cy = m_rcWindow.Height() + m_iShowTop;
        pBkWndChild->OnWindowPosChanged(&WndPos);
    }
    else
    {
        m_lstWndChild.GetPrev(posChild);
        CBkWnd* preChild = m_lstWndChild.GetAt(posChild);
        if( preChild )
        {
            if( preChild->IsVisibleDeep(FALSE) )
            {
                CRect rcPre;
                preChild->GetRect( &rcPre );
                int nChildWidth = pBkWndChild->GetWidth();
                int nChildHeight = pBkWndChild->GetHeight();

                WINDOWPOS WndPos = {0};
                WndPos.x = rcPre.right;
                WndPos.y = rcPre.top;
                WndPos.cx = m_rcWindow.right - rcPre.right;
                WndPos.cy = m_rcWindow.bottom - rcPre.bottom;

                if( rcPre.right + nChildWidth > m_rcWindow.right )
                {
                    if( rcPre.bottom + nChildHeight > m_rcWindow.bottom )
                    {
                        pBkWndChild->SetVisible(FALSE);
                    }     
                    else
                    {
                        WndPos.x = m_rcWindow.left;
                        WndPos.y += nChildHeight;
                        WndPos.cy -= nChildHeight;
                        pBkWndChild->OnWindowPosChanged(&WndPos);
                    }
                }
                else
                {
                    pBkWndChild->OnWindowPosChanged(&WndPos);
                }
            }
            else
            {
                pBkWndChild->SetVisible(FALSE);
            }
        }
    }
}

inline void CBkDivZ::DrawBkgnd(CDCHandle& dc)
{
    CRect rtDraw = m_rcWindow;
    rtDraw.OffsetRect(m_itemsOffsetH, m_itemsOffsetV);
    
    if( m_isScrollBK )
    {
        rtDraw.top = rtDraw.top - m_iShowTop;
        if( m_rcWindow.Height() < GetRealHeight() )
        {
            rtDraw.bottom = rtDraw.top + GetRealHeight();
        }
    }

    if( m_style.m_strSkinName.IsEmpty() )
    {
        COLORREF crBg = m_style.m_crBg;

        if (BkWndState_Hover == (GetState() & BkWndState_Hover) && CLR_INVALID != m_style.m_crBgHover)
        {
            crBg = m_style.m_crBgHover;
        }

        if (CLR_INVALID != crBg)
            dc.FillSolidRect(rtDraw, crBg);
    }
    else
    {
        CBkSkinBase* pSkin = BkSkin::GetSkin(m_style.m_strSkinName);
        if (pSkin)
        {
            pSkin->Draw(dc, rtDraw, m_dwState);
        }
    }
}

//////////////////////////////////////////////////////////////////////////
// CBkFile实现
inline BOOL CBkFile::Load(TiXmlElement* pTiXmlElem)
{
    if (!CBkWnd::Load(pTiXmlElem))
        return FALSE;

    CStringA strSrc = pTiXmlElem->Attribute("src");

    ATLASSERT(!strSrc.IsEmpty());

    if (strSrc.IsEmpty())
        return FALSE;

    CStringA strXML;
    BOOL bRet = BkResManager::LoadResource(CA2T(strSrc), strXML);

    ATLASSERT(bRet);

    if (!bRet)
        return TRUE;

    TiXmlDocument xmlDoc;
    {
        xmlDoc.Parse(strXML, NULL, TIXML_ENCODING_UTF8);
    }
    if (xmlDoc.Error())
    {
        ATLASSERT(NULL);
        return FALSE;
    }

    TiXmlElement *pSubTiElement = xmlDoc.RootElement();
    bRet = __super::LoadChilds(pSubTiElement);

    return bRet;
}