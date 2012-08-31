//////////////////////////////////////////////////////////////////////////
//   File Name: bkwndBtn.h
// Description: Button
//     Creator: DongM
//     Version: 2011.04.28 - 1.0 - Create
//       Usage: <button id=xx>inner text example</button>
//////////////////////////////////////////////////////////////////////////

#pragma once
#define TIME_BUTTON_SHORTCLICKS     100

//////////////////////////////////////////////////////////////////////////
// CBkButton定义
class CBkButton : public CBkText
{
    BKOBJ_DECLARE_CLASS_NAME(CBkButton, "button")

public:
    CBkButton();
    virtual IBkWnd* MakeCopy();
    virtual BOOL    NeedRedrawWhenStateChange();
    virtual BOOL    OnLButtonUp(UINT nFlags, CPoint point,BOOL bAct = TRUE);
    virtual BOOL    OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_INT_ATTRIBUTE("x-margin", m_style.m_nMarginX, FALSE)
        BKWIN_INT_ATTRIBUTE("y-margin", m_style.m_nMarginY, FALSE)
    BKWIN_DECLARE_ATTRIBUTES_END()

protected:
    CBkButton(const CBkButton& bkButton);
};


//////////////////////////////////////////////////////////////////////////
// CBkButton实现
inline CBkButton::CBkButton():CBkText(){}
inline CBkButton::CBkButton(const CBkButton& bkButton):CBkText(bkButton){}
inline IBkWnd* CBkButton::MakeCopy()
{
    return new CBkButton(*this);
}
inline BOOL CBkButton::NeedRedrawWhenStateChange()
{
    return TRUE;
}

inline BOOL CBkButton::OnLButtonUp(UINT nFlags, CPoint point, BOOL bAct)
{
    __super::OnLButtonUp(nFlags, point, bAct);
	if (bAct)
	{
		if (m_pNotify != NULL)
		{
			NotifyInfo info = {WM_LBUTTONUP, point, this};
			m_pNotify->Action(&info);
		}
	}
    return FALSE;
}

inline BOOL CBkButton::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if( nChar == VK_RETURN && IsAccept() )
    {
        OnLButtonUp( 0, CPoint(0,0));
        return TRUE;
    }
    else
    {
        return __super::OnKeyDown( nChar, nRepCnt, nFlags );
    }

    return FALSE;
}


//////////////////////////////////////////////////////////////////////////
// CBkImageBtnWnd定义
class CBkImageBtnWnd : public CBkDiv
{
    BKOBJ_DECLARE_CLASS_NAME(CBkImageBtnWnd, "imgbtn")
public:
    enum { Normal, Push, Switch }; // 普通按钮,单选按钮,开关按钮

    CBkImageBtnWnd();

    virtual BOOL NeedRedrawWhenStateChange();

    int     GetType();
    BOOL    GetIsDown();
    void    SetIsDown( BOOL isDown );
    void    SetSkin( CBkSkinBase* pSkin );
    void    NotifyClick(const CPoint& pt);

    virtual BOOL    OnLButtonDown( UINT nFlags, CPoint point );
    virtual BOOL    OnLButtonUp( UINT nFlags, CPoint point, BOOL bAct = TRUE );
    virtual BOOL    OnLButtonDblClk(UINT nFlags, CPoint point);
    virtual BOOL    OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
    virtual BOOL    OnMouseHover();
    virtual BOOL    OnMouseLeave();
    virtual BOOL    OnNcCalcSize(LPSIZE pSize);
    virtual BOOL    OnPaint(CDCHandle dc);

    static VOID CALLBACK OnTimer_ShortClicks( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime );

protected:
    int     m_buttonType;	    // 按钮的类型
    BOOL    m_isDown;		    // 按钮是否被设置按下
	BOOL    m_bIsClickDown;     // 按钮是否操作BUTTONDOWN
    BOOL    m_bIsShortClicks;   // 是否支持连续短击
	BOOL	m_nShortClickTick;

    CBkSkinBase* m_pSkin;

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_SKIN_ATTRIBUTE("skin", m_pSkin, TRUE)
        BKWIN_INT_ATTRIBUTE("short-clicks", m_bIsShortClicks, FALSE)
        BKWIN_ENUM_ATTRIBUTE("type", int, TRUE)
            BKWIN_ENUM_VALUE("normal", Normal)
            BKWIN_ENUM_VALUE("push", Push)
            BKWIN_ENUM_VALUE("switch", Switch)
        BKWIN_ENUM_END(m_buttonType)
    BKWIN_DECLARE_ATTRIBUTES_END()
};

//////////////////////////////////////////////////////////////////////////
// CBkLink实现
inline CBkImageBtnWnd::CBkImageBtnWnd()
                      :m_pSkin(NULL)
                      ,m_buttonType(Normal)
                      ,m_isDown(FALSE)
					  ,m_bIsClickDown(FALSE)
                      ,m_bIsShortClicks(FALSE)
					  ,m_nShortClickTick(0)
{
}

inline BOOL CBkImageBtnWnd::NeedRedrawWhenStateChange()
{
    return TRUE;
}

inline int CBkImageBtnWnd::GetType()
{ 
    return m_buttonType; 
}

inline BOOL CBkImageBtnWnd::GetIsDown()
{
    return m_isDown;
}

inline void CBkImageBtnWnd::SetIsDown( BOOL isDown )
{
    m_isDown = isDown;
    if( isDown )
    {
        ModifyState( BkWndState_PushDown,0 );
    }
    else
    {
        ModifyState( 0,BkWndState_PushDown );
    }

    NotifyInvalidate();
}

inline void CBkImageBtnWnd::SetSkin(CBkSkinBase* pSkin)
{
    m_pSkin = pSkin; 
    NotifyInvalidate();
}

inline void CBkImageBtnWnd::NotifyClick(const CPoint& pt)
{
    if (m_pNotify)
    {
        NotifyInfo info = {WM_LBUTTONUP, pt, this};
        m_pNotify->Action(&info);
    } 
}

inline BOOL CBkImageBtnWnd::OnLButtonDown( UINT nFlags, CPoint point )
{
    if( IsDisable() )
    {
        return FALSE;
    }

    CBkWnd::OnLButtonDown(nFlags,point);
    CBkDiv::OnLButtonDown(nFlags,point);
    if (m_pNotify != NULL)
    {
        NotifyInfo info = {WM_LBUTTONDOWN, point, this};
        m_pNotify->Action(&info);
    }
	
	m_bIsClickDown = TRUE ;
    switch( GetType() )
    {
    case Normal:
		{
			if (m_bIsShortClicks)
			{
				m_nShortClickTick = 0;
				::SetTimer( GetContainer(), (UINT_PTR)this, TIME_BUTTON_SHORTCLICKS, OnTimer_ShortClicks );
			}
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(m_hWndContainer,&pt);
			if (!m_rcWindow.PtInRect(pt))
			{
				ModifyState(BkWndState_Normal, BkWndState_Hover|BkWndState_PushDown);
				NotifyInvalidate();
			}
		}
        break;
    case Push:
        SetIsDown( TRUE );
        break;
    case Switch:
        SetIsDown( !GetIsDown() ); 
        break;
    }

    return FALSE;
}

inline BOOL CBkImageBtnWnd::OnLButtonUp(UINT nFlags, CPoint point,BOOL bAct)
{
    KillTimer(GetContainer(), (UINT_PTR)this);

    if( IsDisable() )
    {
        return FALSE;
    }

    if( GetType() != Switch )
    {
        CBkWnd::OnLButtonUp(nFlags,point,bAct);
    }

    CBkDiv::OnLButtonUp(nFlags, point,bAct);

	if (bAct)
	{
		if (m_pNotify != NULL)
		{
			if ( TRUE == m_bIsClickDown && TRUE == m_rcWindow.PtInRect(point))
			{
				POINT pt;
				GetCursorPos(&pt);
				ScreenToClient(m_hWndContainer,&pt);
				if (!m_rcWindow.PtInRect(pt))
				{
					ModifyState(BkWndState_Normal, BkWndState_Hover|BkWndState_PushDown);
					NotifyInvalidate();
				}
				NotifyInfo info = {WM_LBUTTONUP, point, this};
				m_pNotify->Action(&info);
			}
			m_bIsClickDown = FALSE ;
		}
	}

    return FALSE;
}

inline BOOL CBkImageBtnWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // 用LBUTTONDOWN代替LBUTTONDBCLICK操作，
    // 原因为button不需要双击操作，但多次单击时会有一次DOWN被转化为DBCLICK，使点击不流畅，不能进行快速连续单击操作
    return OnLButtonDown(nFlags,point);
}

inline BOOL CBkImageBtnWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if( (nChar == VK_RETURN && IsAccept()) ||
        (nChar == VK_SPACE && IsFocus()) )
    {
        if (m_pNotify != NULL && !IsDisable())
        {   
			
            NotifyInfo info = {WM_LBUTTONUP, CPoint(0,0), this, VK_RETURN};
            m_pNotify->Action(&info);
        }
        return TRUE;
    }
    else
    {
        return __super::OnKeyDown( nChar, nRepCnt, nFlags );
    }

    return FALSE;
}

inline BOOL CBkImageBtnWnd::OnMouseHover()
{
    //if (GetType() == Normal || GetType() == Push)
    {
        ModifyState(BkWndState_Hover,0);
        NotifyInvalidate();
        //return TRUE;
    }
    CBkDiv::OnMouseHover();
    if (m_pNotify)
    {
        NotifyInfo info = {BKM_BTN_HOVER, CPoint(0,0), this};
        m_pNotify->Action(&info);
    }
    return FALSE;
}

inline BOOL CBkImageBtnWnd::OnMouseLeave()
{
    CBkWnd::OnMouseLeave();
    CBkDiv::OnMouseLeave();
    if (m_pNotify)
    {
        NotifyInfo info = {BKM_BTN_LEAVE, CPoint(0,0), this};
        m_pNotify->Action(&info);
    }
    return FALSE;
}

inline BOOL CBkImageBtnWnd::OnNcCalcSize(LPSIZE pSize)
{
    CBkDiv::OnNcCalcSize(pSize);

    if (m_pSkin)
    {
        if( 2 == m_dlgpos.nType )
        {
            SIZE sizeImage = m_pSkin->GetSkinSize();

            if (sizeImage.cx)
                pSize->cx = sizeImage.cx;
            if (sizeImage.cy)
                pSize->cy = sizeImage.cy;
        }
    }

    return FALSE;
}

inline BOOL CBkImageBtnWnd::OnPaint(CDCHandle dc)
{
    if (m_pSkin)
    {
        DWORD dwState = IIF_STATE5(GetState(), 0, 1, 2, 3, 4);

        if( GetState() != BkWndState_Accept )
        {
            dwState = IIF_STATE4(GetState(), 0, 1, 2, 3);
        }

        // 第5状态分别为:Normal,Hover,Push,Disable,Accept
        m_pSkin->Draw(dc, m_rcWindow, dwState);
    }
    return FALSE;
}

inline VOID CBkImageBtnWnd::OnTimer_ShortClicks( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime )
{
    CBkImageBtnWnd* pButton = (CBkImageBtnWnd*)idEvent;
    if (pButton)
    {
		if (pButton->m_nShortClickTick++ >= 3)	// 3个时钟后启动连续点击
		{
			CPoint pt;
			::GetCursorPos(&pt);
			::ScreenToClient(hwnd, &pt);

			CRect rcButton;
			pButton->GetRect(&rcButton);
			if (rcButton.PtInRect(pt))
			{
				if (pButton->IsDisable())
				{
					KillTimer(hwnd, idEvent);
					return;
				}
				pButton->NotifyClick(pt);
			}
		}
    }
}


//////////////////////////////////////////////////////////////////////////
// CBkSplitterButton定义
class CBkSplitterButton : public CBkImageBtnWnd
{
    BKOBJ_DECLARE_CLASS_NAME(CBkSplitterButton, "splitter_button")

public:
    CBkSplitterButton();
    virtual IBkWnd* MakeCopy();
    virtual BOOL    NeedRedrawWhenStateChange();
    virtual BOOL    OnLButtonDown(UINT nFlags, CPoint point);
    virtual BOOL    OnLButtonUp(UINT nFlags, CPoint point);
    virtual BOOL    OnMouseMove(UINT nFlags, CPoint point);
	//virtual BOOL    OnMouseLeave();
	virtual BOOL    OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void    OnSetCursor();

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_INT_ATTRIBUTE("x-margin", m_style.m_nMarginX, FALSE)
        BKWIN_INT_ATTRIBUTE("y-margin", m_style.m_nMarginY, FALSE)
    BKWIN_DECLARE_ATTRIBUTES_END()

	HCURSOR GetOldCursor() const;

protected:
    CBkSplitterButton(const CBkSplitterButton& bkButton);
	HCURSOR m_oldcursor;
};


//////////////////////////////////////////////////////////////////////////
// CBkSplitterButton实现
inline CBkSplitterButton::CBkSplitterButton():CBkImageBtnWnd(){}
inline CBkSplitterButton::CBkSplitterButton(const CBkSplitterButton& bkButton):CBkImageBtnWnd(bkButton){}
inline IBkWnd* CBkSplitterButton::MakeCopy()
{
    return new CBkSplitterButton(*this);
}
inline BOOL CBkSplitterButton::NeedRedrawWhenStateChange()
{
    return TRUE;
}

inline BOOL CBkSplitterButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	BOOL bRet = __super::OnLButtonDown(nFlags, point);
    if (m_pNotify != NULL)
    {
        NotifyInfo info = {WM_LBUTTONDOWN, point, this};
        m_pNotify->Action(&info);
    }
	return bRet;
}

inline BOOL CBkSplitterButton::OnLButtonUp(UINT nFlags, CPoint point)
{
    __super::OnLButtonUp(nFlags, point);
    if (m_pNotify != NULL)
    {
        NotifyInfo info = {WM_LBUTTONUP, point, this};
        m_pNotify->Action(&info);
    }
    return FALSE;
}
inline BOOL CBkSplitterButton::OnMouseMove(UINT nFlags, CPoint point)
{
    __super::OnMouseMove(nFlags, point);
    if (m_pNotify != NULL)
    {
        NotifyInfo info = {WM_MOUSEMOVE, point, this};
        m_pNotify->Action(&info);
    }
    return FALSE;
}
//inline BOOL CBkSplitterButton::OnMouseLeave()
//{
//	//HCURSOR hCur = ::LoadCursor(NULL, IDC_ARROW);
//	//::SetCursor(hCur);
//	//return FALSE;
//}

inline BOOL CBkSplitterButton::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if( nChar == VK_ESCAPE)
    {
        NotifyInfo info = {WM_KEYDOWN, CPoint(), this, nChar};
        m_pNotify->Action(&info);
        return TRUE;
    }
    else if (m_pNotify != NULL)
    {
        return __super::OnKeyDown( nChar, nRepCnt, nFlags );
    }

    return FALSE;
}

inline void CBkSplitterButton::OnSetCursor()
{
	m_oldcursor = GetCursor();
    HCURSOR hCur = ::LoadCursor(NULL, IDC_SIZEWE);
    ::SetCursor(hCur);
}

inline HCURSOR CBkSplitterButton::GetOldCursor() const
{
	return m_oldcursor;
}



//////////////////////////////////////////////////////////////////////////
// CBkLink定义
class CBkLink : public CBkText
{
    BKOBJ_DECLARE_CLASS_NAME(CBkLink, "link")

protected:
    CBkLink(const CBkLink& bkLink);
    CString             m_strLinkUrl;

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_TSTRING_ATTRIBUTE("href", m_strLinkUrl, FALSE)
    BKWIN_DECLARE_ATTRIBUTES_END()

public:
    CBkLink();
    virtual IBkWnd* MakeCopy();
    LPCTSTR GetLinkUrl();
    void SetLinkUrl(LPCTSTR szUrl);
    virtual BOOL OnLButtonUp(UINT nFlags, CPoint point,BOOL bAct = TRUE);
};

//////////////////////////////////////////////////////////////////////////
// CBkLink实现
inline CBkLink::CBkLink():CBkText(){}
inline CBkLink::CBkLink(const CBkLink& bkLink):CBkText(bkLink){}
inline IBkWnd* CBkLink::MakeCopy()
{
    return new CBkLink(*this);
}
inline BOOL CBkLink::OnLButtonUp(UINT nFlags, CPoint point,BOOL bAct )
{
    if (!m_strLinkUrl.IsEmpty())
    {
        HINSTANCE hRet = ::ShellExecute(NULL, L"open", m_strLinkUrl, NULL, NULL, SW_SHOWNORMAL);
    }
	if (bAct)
	{
		if (m_pNotify != NULL)
		{
			NotifyInfo info = {WM_LBUTTONUP, point, this};
			m_pNotify->Action(&info);
		}
	}

    return FALSE;
}

inline LPCTSTR CBkLink::GetLinkUrl()
{
    return m_strLinkUrl;
}

inline void CBkLink::SetLinkUrl(LPCTSTR szUrl)
{
    m_strLinkUrl = szUrl;
}

//////////////////////////////////////////////////////////////////////////
// CBkDropDown定义
class CBkDropDownBtn : public CBkImageBtnWnd
{
    BKOBJ_DECLARE_CLASS_NAME(CBkDropDownBtn, "dropdownbtn")
public:
    CBkDropDownBtn();
    BOOL Load(TiXmlElement* pTiXmlElem);
    virtual BOOL OnMouseMove(UINT nFlags, CPoint point);
    virtual BOOL OnLButtonUp(UINT nFlags, CPoint point, BOOL bAct = TRUE);
    virtual BOOL OnLButtonDown(UINT nFlags, CPoint point);

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_INT_ATTRIBUTE("separator", m_iSeparator, FALSE)
        BKWIN_SKIN_ATTRIBUTE("skinnormal", m_pSkinNormal, TRUE)
        BKWIN_SKIN_ATTRIBUTE("skindrop", m_pSkinDrop, TRUE)
    BKWIN_DECLARE_ATTRIBUTES_END()

protected:
    int         m_iSeparator;
    CBkSkinBase *m_pSkinNormal;
    CBkSkinBase *m_pSkinDrop;
};

//////////////////////////////////////////////////////////////////////////
// CBkDropdown实现
inline CBkDropDownBtn::CBkDropDownBtn() :
        m_iSeparator(0)
        {}

inline BOOL CBkDropDownBtn::Load(TiXmlElement* pTiXmlElem)
{
    if (!__super::Load(pTiXmlElem))
        return FALSE;

    OnDestroy();

    m_pSkin = m_pSkinNormal;
    NotifyInvalidate();

    return TRUE;
}

inline BOOL CBkDropDownBtn::OnMouseMove(UINT nFlags, CPoint point)
{
    CRect rcLeft = m_rcWindow;
    rcLeft.right = rcLeft.left+m_iSeparator;
    CRect rcRight = m_rcWindow;
    rcRight.left = rcRight.left+m_iSeparator;
    if (rcLeft.PtInRect(point) && m_pSkin != m_pSkinNormal)
    {
        m_pSkin = m_pSkinNormal;
        NotifyInvalidate();
    }
    else if (rcRight.PtInRect(point) && m_pSkin != m_pSkinDrop)
    {
        m_pSkin = m_pSkinDrop;
        NotifyInvalidate();
    }
    
    __super::OnMouseMove(nFlags, point);
    return FALSE;
}

inline BOOL CBkDropDownBtn::OnLButtonDown(UINT nFlags, CPoint point)
{
    CRect rcLeft = m_rcWindow;
    rcLeft.right = rcLeft.left+m_iSeparator;
    CRect rcRight = m_rcWindow;
    rcRight.left = rcRight.left+m_iSeparator;
    if (rcLeft.PtInRect(point))
    {
        m_pSkin = m_pSkinNormal;
    }
    else if (rcRight.PtInRect(point))
    {
        m_pSkin = m_pSkinDrop;
    }
    __super::OnLButtonDown(nFlags, point);
    return FALSE;
}

inline BOOL CBkDropDownBtn::OnLButtonUp(UINT nFlags, CPoint point,BOOL bAct)
{
    CBkWnd::OnLButtonUp(nFlags, point,bAct);
	if (bAct)
	{
		if (m_pNotify != NULL)
		{
			CRect rcLeft = m_rcWindow;
			rcLeft.right = rcLeft.left+m_iSeparator;
			CRect rcRight = m_rcWindow;
			rcRight.left = rcRight.left+m_iSeparator;
			if (rcLeft.PtInRect(point))
			{
				NotifyInfo info = {BKM_BTN_CLICK, point, this};
				m_pNotify->Action(&info);
			}
			else if (rcRight.PtInRect(point))
			{
				NotifyInfo info = {BKM_BTN_DROPDOWN, point, this};
				m_pNotify->Action(&info);
			}
		}
	}
    return FALSE;
}