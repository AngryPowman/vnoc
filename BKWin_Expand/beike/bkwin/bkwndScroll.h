//////////////////////////////////////////////////////////////////////////
//   File Name: bkwndScroll.h
// Description: CBkScroll
//     Creator: DongM
//     Version: 2011.05.15 - 1.0 - Create
//              2011.08.17 - 1.1 - 添加连续点击空白功能
//////////////////////////////////////////////////////////////////////////

#pragma once
#define     TIME_INTERCLICK    100

//////////////////////////////////////////////////////////////////////////
// CBkButton定义
class CBkScroll : public CBkWnd
{
    BKOBJ_DECLARE_CLASS_NAME(CBkScroll, "scroll")
        
public:
    CBkScroll();

    enum    ScrollTypeEnum {STE_V, STE_H};
    void    SetType(ScrollTypeEnum nType);
    void    SetSkin(LPCSTR strSkin);
    BOOL    NeedShow();
    void    Scroll(int nSBCode, int nPos);
    int     GetValue();
    void    SetValue(int nValue);
    void    SetPage(int nPage);
	int     GetPage();
    void    SetMin(int nMim);
	int     GetMin();
    void    SetMax(int nMax);
	int     GetMax();
    void    SetStep(int nLine, int nPage);
    void    SetWidth(int nWidth);
    void    SetHeight(int nHeight);
    int     GetWidth();
    int     GetHeight();
    CRect   GetRect(int btnType);
    int     GetClick();
    BOOL    IsVScroll ();
    void    NotifyScroll(UINT nAction, POINT pt, IBkWnd* pFrom, DWORD& dwData);

protected:
    virtual BOOL    OnMouseMove(UINT nFlags, CPoint point);
    virtual BOOL    OnMouseLeave();
    virtual BOOL    OnLButtonDown(UINT nFlags, CPoint point);
    virtual BOOL    OnLButtonUp(UINT nFlags, CPoint point,BOOL bAct = TRUE);
    virtual BOOL    OnLButtonDblClk(UINT nFlags, CPoint point);
    virtual BOOL    OnWindowPosChanged(LPWINDOWPOS lpWndPos);
    virtual BOOL    OnPaint(CDCHandle dc);

    static VOID CALLBACK OnTimer_InterClick( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime );

private:
    void _CalcSize();
    BOOL _IsVScroll();
    BOOL _IsHScroll();
    void _ModifyState(UINT nBtn, DWORD dwAdd, DWORD dwDel);
    BOOL _IsDragThumb();
    BOOL _NotifyAction(UINT nAction, POINT pt, IBkWnd* pFrom, DWORD& dwData);

    enum BTN_ENUM
    {
        BE_NONE = -1,    
        BE_THUMB,       // thumb
        BE_ARROW1,      // up or left
        BE_ARROW2,      // down or right
        BE_COUNT,
    };

protected:
    ScrollTypeEnum  m_nType;
	int             m_nMin; 
    int             m_nMax;
    int             m_nBegin;
    int             m_nPage;
    int             m_nStepLine;
    int             m_nStepPage;
    BOOL            m_bHasArrow;
    CRect           m_rcBtn[BE_COUNT];          // BTN_ENUM
    DWORD           m_dwBtnState[BE_COUNT];     // BTN_ENUM
    CPoint          m_ptDown;
    int             m_nBegin2;
    int             m_nBegin3;
    int             m_nMinThumbSize;
    int             m_iWidth;
    int             m_iHeight;
    int             m_nClickBtn;                // BTN_ENUM, LBUTTONDOWN点击的区域, 用作连续点击判定
    CBkSkinScroll*  m_pSkin;
};

inline CBkScroll::CBkScroll()
{
    m_nType = STE_V;
	m_nMin = 0;
    m_nMax = 0;
    m_nBegin = 0;
    m_nPage = 0;
    m_nStepLine = 10;
    m_nStepPage = 3;
    m_bHasArrow = FALSE;
    for (int i = 0; i < BE_COUNT; i++)
    {
        m_dwBtnState[i] = 0;
    }
    m_nBegin2 = 0;
    m_nBegin3 = 0;
    m_nMinThumbSize = 0;
    m_iWidth = 8;
    m_iHeight = 8;
    m_nClickBtn = BE_NONE;
    m_pSkin = NULL;
}

inline void CBkScroll::SetType(ScrollTypeEnum nType)
{
    m_nType = nType;
}

inline void CBkScroll::SetSkin(LPCSTR szSkin)
{
    m_pSkin = dynamic_cast<CBkSkinScroll*>(BkSkin::GetSkin(szSkin));
    if (m_pSkin != NULL)
    {
        if (_IsHScroll())
        {
            m_iHeight = m_pSkin->GetHeight();
            m_nMinThumbSize = (m_pSkin->GetThumbMinSize() ? m_pSkin->GetThumbMinSize() : m_iHeight);
        }
        else
        {
            m_iWidth = m_pSkin->GetWidth();
            m_nMinThumbSize = (m_pSkin->GetThumbMinSize() ? m_pSkin->GetThumbMinSize() : m_iWidth);
        }
        m_bHasArrow = m_pSkin->HasArrow();
    }
}

inline BOOL CBkScroll::NeedShow()
{
    return m_nPage >= 0 && m_nPage < m_nMax;
}

inline void CBkScroll::Scroll(int nSBCode, int nPos)
{
    if (!IsVisibleDeep(FALSE))
        return;

    UINT bkmEvent = BKM_SCROLL_BASE;
    int nPosOld = m_nBegin;
    int nPosNew = m_nBegin;
    switch (nSBCode)
    {
    case SB_LINEUP:
        nPosNew -= m_nStepLine;
        bkmEvent = BKM_SCROLL_LINE_UP;
        break;
    case SB_LINEDOWN:
        nPosNew += m_nStepLine;
        bkmEvent = BKM_SCROLL_LINE_DOWN;
        break;
    case SB_PAGEUP:
        nPosNew -= m_nStepPage;
        bkmEvent = BKM_SCROLL_PAGE_UP;
        break;
    case SB_PAGEDOWN:
        nPosNew += m_nStepPage;
        bkmEvent = BKM_SCROLL_PAGE_DOWN;
        break;
    case SB_TOP:
        nPosNew = 0;
        bkmEvent = BKM_SCROLL_THUMB_POSITION;
        break;
    case SB_BOTTOM:
        nPosNew = m_nMax;
        bkmEvent = BKM_SCROLL_THUMB_POSITION;
        break;
    case SB_THUMBPOSITION:
        nPosNew = nPos;
        bkmEvent = BKM_SCROLL_THUMB_POSITION;
        break;
    default:
        return;
    }
    if (nPosNew < 0)
        nPosNew = 0;
    else if (nPosNew > m_nMax - m_nPage)
        nPosNew = m_nMax - m_nPage;

    DWORD nBegin = nPosNew;
    if (m_pNotify != NULL)
    {
        NotifyScroll(bkmEvent, CPoint(0,0), this, nBegin);
    }

    if (nBegin != nPosNew)
    {
        nPosNew = nBegin;
    }

    SetValue(nPosNew);

    if (m_pNotify != NULL)
    {
        UINT nAction = _IsVScroll() ? BKM_LIST_SCROLL_V : BKM_LIST_SCROLL_H;
        NotifyInfo info = {nAction, CPoint(0,0), this, m_nBegin};
        m_pNotify->Action(&info);
    }
}

inline int CBkScroll::GetValue()
{
    return m_nBegin;
}

inline void CBkScroll::SetValue(int nValue)
{
    if( nValue > m_nMax - m_nPage )
    {
        nValue = m_nMax - m_nPage;
    }

    if( nValue < 0 )
    {
        nValue = 0;
    }

    m_nBegin = nValue;
    _CalcSize();
}

inline void CBkScroll::SetPage(int nPage)
{
    m_nPage = nPage;
    m_nStepPage = nPage;
    m_nStepLine = nPage / 10;
    if (m_nStepPage <= 0)
    {
        m_nStepPage = 10;
    }
    if (m_nStepLine <= 0)
    {
        m_nStepLine = 3;
    }
    _CalcSize();
}

inline int CBkScroll::GetPage()
{
	return m_nPage;
}
inline void CBkScroll::SetMin(int nMin)
{
	m_nMin =  nMin;
	//_CalcSize();
}
inline int CBkScroll::GetMin()
{
	return m_nMin;
}
inline void CBkScroll::SetMax(int nMax)
{
    m_nMax = nMax;
    _CalcSize();
}

inline int CBkScroll::GetMax()
{
	return m_nMax;
}

inline void CBkScroll::SetStep(int nLine, int nPage)
{
    m_nStepLine = nLine;
    m_nStepPage = nPage;
}

inline void CBkScroll::SetWidth(int nWidth)
{
    m_iWidth = nWidth;
    m_nMinThumbSize = (m_pSkin->GetThumbMinSize() ? m_pSkin->GetThumbMinSize() : m_iWidth);
}

inline void CBkScroll::SetHeight(int nHeight)
{
    m_iHeight = nHeight;
    m_nMinThumbSize = (m_pSkin->GetThumbMinSize() ? m_pSkin->GetThumbMinSize() : m_iHeight);
}

inline int CBkScroll::GetWidth()
{
    return m_iWidth;
}

inline int CBkScroll::GetHeight()
{
    return m_iHeight;
}

inline CRect CBkScroll::GetRect(int btnType)
{
    CRect rc(0,0,0,0);
    switch(btnType)
    {
    case BE_THUMB:
        rc = m_rcBtn[BE_THUMB];
        break;
    case BE_ARROW1:
        rc = m_rcBtn[BE_ARROW1];
        break;
    case BE_ARROW2:
        rc = m_rcBtn[BE_ARROW2];
        break;
    }
    return rc;
}

inline int CBkScroll::GetClick()
{
    return m_nClickBtn;
}

inline BOOL CBkScroll::IsVScroll()
{   
    return _IsVScroll();
}

inline void CBkScroll::NotifyScroll(UINT nAction, POINT pt, IBkWnd* pFrom, DWORD& dwData)
{
    _NotifyAction(nAction, pt, pFrom, dwData);
}

inline BOOL CBkScroll::_IsVScroll()
{
    return m_nType == STE_V;
}

inline BOOL CBkScroll::_IsHScroll()
{
    return m_nType == STE_H;
}

inline void CBkScroll::_CalcSize()
{
    if (NeedShow())
    {
        int nThumbMinSize = 0;

        CRect rcBackground = m_rcWindow;
        if (_IsVScroll())
        {
            if (m_bHasArrow)
            {
                rcBackground.top += m_iWidth;
                rcBackground.bottom -= m_iWidth;
                m_rcBtn[BE_ARROW1]= m_rcWindow;
                m_rcBtn[BE_ARROW1].bottom = m_rcBtn[BE_ARROW1].top + m_iWidth;
                m_rcBtn[BE_ARROW2] = m_rcWindow;
                m_rcBtn[BE_ARROW2].top = m_rcBtn[BE_ARROW2].bottom - m_iWidth;
            }
            m_rcBtn[BE_THUMB] = rcBackground;
            int nBarHeight = rcBackground.Height() * m_nPage / m_nMax;
            if (nBarHeight < m_nMinThumbSize)
            {
                nBarHeight = m_nMinThumbSize;
            }
            if (m_nBegin + m_nPage > m_nMax)
            {
                m_rcBtn[BE_THUMB].top = m_rcBtn[BE_THUMB].bottom - nBarHeight;
            }
            else
            {
                m_rcBtn[BE_THUMB].top += (double)(rcBackground.Height() - nBarHeight) * m_nBegin / (m_nMax - m_nPage);
                m_rcBtn[BE_THUMB].bottom = m_rcBtn[BE_THUMB].top + nBarHeight;
            }
        }
        else if (_IsHScroll())
        {
            if (m_bHasArrow)
            {
                rcBackground.left += m_iHeight;
                rcBackground.right -= m_iHeight;
                m_rcBtn[BE_ARROW1] = m_rcWindow;
                m_rcBtn[BE_ARROW1].right = m_rcBtn[BE_ARROW1].left + m_iHeight;
                m_rcBtn[BE_ARROW2] = m_rcWindow;
                m_rcBtn[BE_ARROW2].left = m_rcBtn[BE_ARROW2].right - m_iHeight;
            }
            m_rcBtn[BE_THUMB] = rcBackground;
            int nBarWidth = rcBackground.Width() * m_nPage / m_nMax;
            if (nBarWidth < m_nMinThumbSize)
            {
                nBarWidth = m_nMinThumbSize;
            }
            if (m_nBegin + m_nPage > m_nMax)
            {
                m_rcBtn[BE_THUMB].left = m_rcBtn[BE_THUMB].right - nBarWidth;
            }
            else
            {
                m_rcBtn[BE_THUMB].left += (rcBackground.Width() - nBarWidth) * m_nBegin / (m_nMax - m_nPage);
                m_rcBtn[BE_THUMB].right = m_rcBtn[BE_THUMB].left + nBarWidth;
            }
        }
        NotifyInvalidate();
    }
}

inline BOOL CBkScroll::OnPaint(CDCHandle dc)
{
    do 
    {
        if (!NeedShow())
        {
            break;
        }

        if (m_pSkin != NULL)
        {
            m_pSkin->DrawBackground(dc, m_rcWindow);
            m_pSkin->DrawThumb(dc, m_rcBtn[BE_THUMB], m_dwBtnState[BE_THUMB], _IsHScroll());
            m_pSkin->DrawArrow(dc, m_rcBtn[BE_ARROW1], m_dwBtnState[BE_ARROW1], TRUE);
            m_pSkin->DrawArrow(dc, m_rcBtn[BE_ARROW2], m_dwBtnState[BE_ARROW2], FALSE);
        }
        else
        {
            dc.FillSolidRect(m_rcBtn[BE_THUMB], RGB(48, 44, 41));
            dc.FillSolidRect(m_rcWindow, RGB(209, 209, 188));
        }
    } while (FALSE);
    return FALSE;
}

inline BOOL CBkScroll::OnWindowPosChanged(LPWINDOWPOS lpWndPos)
{
    __super::OnWindowPosChanged(lpWndPos);
    _CalcSize();
    return FALSE;
}

inline BOOL CBkScroll::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (::PtInRect(m_rcBtn[BE_THUMB], point))
    {
        _ModifyState(BE_THUMB, BkWndState_PushDown, 0);
        m_nBegin2 = m_nBegin;
        m_nBegin3 = m_nBegin;
        m_ptDown = point;
    }
    else
    {
        DWORD nBegin = -1;
        UINT nAction = 0;
        int nOffset = 0;
        if (_IsVScroll())
        {
            nAction = BKM_LIST_SCROLL_V;
            if (point.y < m_rcBtn[BE_THUMB].top)
            {
                if (m_bHasArrow && ::PtInRect(m_rcBtn[BE_ARROW1], point))
                {
                    m_nClickBtn = BE_ARROW1;
                    _ModifyState(BE_ARROW1, BkWndState_PushDown, 0);
                    if (_NotifyAction(BKM_SCROLL_LINE_UP, point, this, nBegin))
                    {
                        nOffset = 0 -  m_nStepLine;
                    }
                }
                else
                {
                    m_nClickBtn = BE_THUMB;
                    if (_NotifyAction(BKM_SCROLL_PAGE_UP, point, this, nBegin))
                    {
                        nOffset = 0 -  m_nStepPage;
                    }
                }
            }
            else
            {
                if (m_bHasArrow && ::PtInRect(m_rcBtn[BE_ARROW2], point))
                {
                    m_nClickBtn = BE_ARROW2;
                    _ModifyState(BE_ARROW2, BkWndState_PushDown, 0);
                    if (_NotifyAction(BKM_SCROLL_LINE_DOWN, point, this, nBegin))
                    {
                        nOffset =  m_nStepLine;
                    }
                }
                else
                {
                    m_nClickBtn = BE_THUMB;
                    if (_NotifyAction(BKM_SCROLL_PAGE_DOWN, point, this, nBegin))
                    {
                        nOffset =  m_nStepPage;
                    }
                }
            }
        }
        else
        {
            nAction = BKM_LIST_SCROLL_H;
            if (point.x < m_rcBtn[BE_THUMB].left)
            {
                if (m_bHasArrow && ::PtInRect(m_rcBtn[BE_ARROW1], point))
                {
                    m_nClickBtn = BE_ARROW1;
                    _ModifyState(BE_ARROW1, BkWndState_PushDown, 0);
                    if (_NotifyAction(BKM_SCROLL_LINE_UP, point, this, nBegin))
                    {
                        nOffset = 0 -  m_nStepLine;
                    }
                }
                else
                {
                    m_nClickBtn = BE_THUMB;
                    if (_NotifyAction(BKM_SCROLL_PAGE_UP, point, this, nBegin))
                    {
                        nOffset = 0 -  m_nStepPage;
                    }
                }
            }
            else
            {
                if (m_bHasArrow && ::PtInRect(m_rcBtn[BE_ARROW2], point))
                {
                    m_nClickBtn = BE_ARROW2;
                    _ModifyState(BE_ARROW2, BkWndState_PushDown, 0);
                    if (_NotifyAction(BKM_SCROLL_LINE_DOWN, point, this, nBegin))
                    {
                        nOffset =  m_nStepLine;
                    }
                }
                else
                {
                    m_nClickBtn = BE_THUMB;
                    if (_NotifyAction(BKM_SCROLL_PAGE_DOWN, point, this, nBegin))
                    {
                        nOffset =  m_nStepPage;
                    }
                }
            }
        }
        if (nBegin != -1)
        {
            m_nBegin = nBegin;
        }
        else
        {
            m_nBegin += nOffset;
        }
        if (m_nBegin < 0)
        {
            m_nBegin = 0;
        }
        else if (m_nBegin + m_nPage > m_nMax)
        {
            m_nBegin = m_nMax - m_nPage;
        }
        _CalcSize();
        if (m_pNotify != NULL)
        {
            NotifyInfo info = {nAction, point, this, m_nBegin};
            m_pNotify->Action(&info);
        }

        ::SetTimer( GetContainer(), (UINT_PTR)this, TIME_INTERCLICK, OnTimer_InterClick );
    }
    return __super::OnLButtonDown(nFlags, point);
}

inline BOOL CBkScroll::OnLButtonUp(UINT nFlags, CPoint point,BOOL bAct/* = TRUE*/)
{
    KillTimer(GetContainer(), (UINT_PTR)this);
    _ModifyState(BE_THUMB, 0, BkWndState_PushDown);
    m_nClickBtn = BE_NONE;
    return __super::OnLButtonUp(nFlags, point,bAct);
}

inline BOOL CBkScroll::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // 用LBUTTONDOWN代替LBUTTONDBCLICK操作，
    // 原因为scroll不需要双击操作，但多次单击时会有一次DOWN被转化为DBCLICK，使点击不流畅，不能进行快速连续单击操作
    return OnLButtonDown(nFlags,point);
}

inline BOOL CBkScroll::OnMouseMove(UINT nFlags, CPoint point)
{
    if (NeedShow())
    {
        if (_IsDragThumb())
        {
            int nOffset = 0;
            int nWnd = 0;
            int nBar = 0;
            UINT nAction = 0;
            if (_IsVScroll())
            {
                nOffset = point.y - m_ptDown.y;
                nWnd = m_rcWindow.Height();
                nBar = m_rcBtn[BE_THUMB].Height();
                nAction = BKM_LIST_SCROLL_V;
            }
            else if (_IsHScroll())
            {
                nOffset = point.x - m_ptDown.x;
                nWnd = m_rcWindow.Width();
                nBar = m_rcBtn[BE_THUMB].Width();
                nAction = BKM_LIST_SCROLL_H;
            }
			m_nBegin = m_nBegin2 + (double)(m_nMax - m_nPage) * nOffset / (nWnd - nBar);
            if (m_nBegin < 0)
            {
                m_nBegin = 0;
            }
            else if (m_nBegin + m_nPage > m_nMax)
            {
                m_nBegin = m_nMax - m_nPage;
            }
            if (m_nBegin3 != m_nBegin)
            {
                m_nBegin3 = m_nBegin;
                _CalcSize();
                if (m_pNotify != NULL)
				{
					NotifyInfo info1 = {BKM_SCROLL_THUMB_POSITION, point, this, m_nBegin};
					m_pNotify->Action(&info1);

                    NotifyInfo info = {nAction, point, this, m_nBegin};
                    m_pNotify->Action(&info);
                }
            }
        }
        else
        {
            BOOL nHoverBtn = -1; 
            for (int i = 0; i < BE_COUNT; i++)
            {
                if (::PtInRect(m_rcBtn[i], point))
                {
                    nHoverBtn = i;
                    break;
                }
            }
            _ModifyState(nHoverBtn, BkWndState_Hover, 0);
        }
    }
    return FALSE;
}

inline VOID CBkScroll::OnTimer_InterClick( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime )
{
    CBkScroll* pScroll = (CBkScroll*)idEvent;
    if (pScroll)
    {
		KillTimer(hwnd, idEvent);
        CPoint pt;
        ::GetCursorPos(&pt);
        ::ScreenToClient(hwnd, &pt);
        if (pScroll->GetRect(BE_THUMB).PtInRect(pt) && pScroll->GetClick() == BE_THUMB)
        {
            return;
        }

        DWORD dwData = pScroll->GetValue();
        if (pScroll->GetRect(BE_ARROW1).PtInRect(pt) && pScroll->GetClick() == BE_ARROW1)
        {
            //pScroll->NotifyScroll(BKM_SCROLL_LINE_UP, pt, pScroll, dwData);
            pScroll->Scroll(SB_LINEUP, 0);
        }
        else if(pScroll->GetRect(BE_ARROW2).PtInRect(pt) && pScroll->GetClick() == BE_ARROW2)
        {
            //pScroll->NotifyScroll(BKM_SCROLL_LINE_DOWN, pt, pScroll, dwData);
            pScroll->Scroll(SB_LINEDOWN, 0);
        }
        else if (pScroll->GetClick() == BE_THUMB)
        {
            CRect rcThumb = pScroll->GetRect(BE_THUMB);
            if (pScroll->IsVScroll())
            {
                //pScroll->NotifyScroll((pt.y < rcThumb.top)?BKM_SCROLL_PAGE_UP:BKM_SCROLL_PAGE_DOWN, pt, pScroll, dwData);
                pScroll->Scroll((pt.y < rcThumb.top)?SB_PAGEUP:SB_PAGEDOWN, 0);
            }
            else
            {
                //pScroll->NotifyScroll((pt.y < rcThumb.left)?BKM_SCROLL_PAGE_UP:BKM_SCROLL_PAGE_DOWN, pt, pScroll, dwData);
                pScroll->Scroll((pt.y < rcThumb.left)?SB_PAGEUP:SB_PAGEDOWN, 0);
            }
        }
		SetTimer( hwnd, (UINT_PTR)pScroll, TIME_INTERCLICK, OnTimer_InterClick );
    }
}

inline void CBkScroll::_ModifyState(UINT nBtn, DWORD dwAdd, DWORD dwDel)
{
    TestMainThread();
    for (int i = 0; i < BE_COUNT; i++)
    {
        if (i == nBtn)
        {
            m_dwBtnState[i] |= dwAdd;
            m_dwBtnState[i] &= ~dwDel;
        }
        else
        {
            m_dwBtnState[i] = 0;
        }
    }
    NotifyInvalidate();
}

inline BOOL CBkScroll::_IsDragThumb()
{
    return IIF_STATE1(m_dwBtnState[BE_THUMB], BkWndState_PushDown);
}

inline BOOL CBkScroll::OnMouseLeave()
{
    if (!_IsDragThumb())
    {
        _ModifyState(-1, NULL, BkWndState_Hover);
    }
    return FALSE;
}

inline BOOL CBkScroll::_NotifyAction(UINT nAction, POINT pt, IBkWnd* pFrom, DWORD& dwData)
{
    BOOL bRet = TRUE;
    if (m_pNotify != NULL)
    {
        NotifyInfo info = {nAction, pt, this, dwData};
        bRet = m_pNotify->Action(&info);
        if (!bRet)
        {
            dwData = info.dwData;
        }
    }
    return bRet;
}