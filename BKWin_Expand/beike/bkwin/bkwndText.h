//////////////////////////////////////////////////////////////////////////
//   File Name: bkwndBtn.h
// Description: Button
//     Creator: DongM
//     Version: 2011.05.08 - 1.0 - Create
//       Usage: <text id=xx>inner text example</button>
//////////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////////
// Text Control
// Usage: <text>inner text example</text>
class CBkText : public CBkWnd
{
    BKOBJ_DECLARE_CLASS_NAME(CBkText, "text")

public:
    CBkText();

    // interface of IBkWnd
    virtual IBkWnd* MakeCopy();
    virtual void    SetText(LPCWSTR szText);
    virtual LPCWSTR GetText() const;
    virtual void    SetTextColor(COLORREF crText);
    virtual void    SetBold(BOOL bBold);

    // virtual method of CBkWnd
    virtual BOOL    Load(TiXmlElement* pTiXmlElem);
    virtual BOOL    PreAddTooltip( CBkWnd* pBkWnd );
    virtual BOOL    PreUpdateTooltip( CBkWnd* pBkWnd );

    void MeasureDrawText( SIZE& szDraw );

protected:
    // interface Message Event Handle
    virtual BOOL    OnPaint(CDCHandle dc);
    virtual BOOL    OnNcCalcSize(LPSIZE pSize);
    virtual BOOL    OnRButtonUp(UINT nFlags, CPoint point,BOOL bAct = TRUE);

protected:
    struct BkDCPaint 
    {
        BkDCPaint()
            : bBkModeChanged(FALSE)
            , nOldBkMode(OPAQUE)
            , bFontChanged(FALSE)
            , hftOld(NULL)
            , bTextColorChanged(FALSE)
            , crOld(CLR_INVALID)
            , bBgColorChanged(FALSE)
            , crOldBg(CLR_INVALID)
        {
        }
        BOOL     bBkModeChanged;
        int      nOldBkMode;
        BOOL     bFontChanged;
        HFONT    hftOld;
        BOOL     bTextColorChanged;
        COLORREF crOld;
        BOOL     bBgColorChanged;
        COLORREF crOldBg;
        CRect    rcClient;

        COLORREF crText;
        HFONT    hftDraw;
    };
    void    BeforePaint(CDCHandle& dc, BkDCPaint& BkDC);
    void    AfterPaint(CDCHandle& dc, BkDCPaint& BkDC);

    void    CalculateTextDrawRect( CRect& rcDrawText );
    void    CalculateTextColorFont( HFONT& font, COLORREF& clr );
    void    CalculateTextAlignment( StringFormat& fmt );
    void    CalculateTextEllipsis( StringFormat& fmt );
    void    CalculateTextHotkeyPrefix( StringFormat& fmt );
    void    CalculateTextFormat( StringFormat& fmt );

protected:
    CBkText(const CBkText& bkText);
    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_INT_ATTRIBUTE("tooltip_when_over", m_bShowTipWhenOver, FALSE)
    BKWIN_DECLARE_ATTRIBUTES_END()

    CString m_strText;
    BOOL    m_bBold;     
    BOOL    m_bShowTipWhenOver;
};


inline CBkText::CBkText()
{
    m_bBold = FALSE;
    m_bShowTipWhenOver = FALSE;
}

inline CBkText::CBkText(const CBkText& bkText): CBkWnd(bkText)
{
    m_strText = bkText.m_strText;
    m_bBold = bkText.m_bBold;
    m_bShowTipWhenOver = bkText.m_bShowTipWhenOver;
}

inline IBkWnd* CBkText::MakeCopy()
{
    return new CBkText(*this);
}

inline void CBkText::SetTextColor(COLORREF crText)
{
    m_style.m_crText = crText;
    NotifyInvalidate();
}

inline void CBkText::SetBold(BOOL bBold)
{
    m_bBold = bBold;
    _RepositionSelf();
    NotifyInvalidate();
}

inline void CBkText::BeforePaint(CDCHandle& dc, BkDCPaint& BkDC)
{
    HFONT hftDraw = NULL;
    COLORREF crDraw = CLR_INVALID;

    BkDC.rcClient = m_rcWindow;
    BkDC.rcClient.DeflateRect(m_style.m_nMarginX, m_style.m_nMarginY);

    if (!m_style.m_strSkinName.IsEmpty())
    {
        BkDC.bBkModeChanged = TRUE;
        BkDC.nOldBkMode = dc.SetBkMode(TRANSPARENT);
    }
    if (CLR_INVALID != m_style.m_crBg)
    {
        BkDC.bBgColorChanged = TRUE;
        BkDC.crOldBg = dc.SetBkColor(m_style.m_crBg);
    }

    if (m_style.m_ftText)
        hftDraw = m_style.m_ftText;

    if (m_style.m_crText != CLR_INVALID)
        crDraw = m_style.m_crText;

    if (IsDisableDeep(TRUE)/*BkWndState_Disable == (GetState() & BkWndState_Disable)*/)
    {
        if (m_style.m_crDisabledText != CLR_INVALID)
            crDraw = m_style.m_crDisabledText;
    }
    else if (BkWndState_Hover == (GetState() & BkWndState_Hover))
    {
        if (m_style.m_ftHover)
            hftDraw = m_style.m_ftHover;

        if (m_style.m_crHoverText != CLR_INVALID)
            crDraw = m_style.m_crHoverText;
    }

    if (hftDraw == NULL)
    {
        hftDraw = BkFontPool::GetDefaultFont();
    }

    if (hftDraw)
    {
        if (m_bBold)
        {
            hftDraw = BkFontPool::GetBoldFont(hftDraw);
        }

        BkDC.bFontChanged = TRUE;
        BkDC.hftOld = dc.SelectFont(hftDraw);
    }

    if (crDraw != CLR_INVALID)
    {
        BkDC.bTextColorChanged = TRUE;
        BkDC.crOld = dc.SetTextColor(crDraw);
    }
}

inline void CBkText::AfterPaint(CDCHandle& dc, BkDCPaint& BkDC)
{
    if (BkDC.bFontChanged)
        dc.SelectFont(BkDC.hftOld);

    if (BkDC.bTextColorChanged)
        dc.SetTextColor(BkDC.crOld);

    if (BkDC.bBkModeChanged)
        dc.SetBkMode(BkDC.nOldBkMode);

    if (BkDC.bBgColorChanged)
        dc.SetBkColor(BkDC.crOldBg);
}

inline BOOL CBkText::Load(TiXmlElement* pTiXmlElem)
{
    if (__super::Load(pTiXmlElem))
    {
        m_strText = CA2T(pTiXmlElem->GetText(), CP_UTF8);
        if (!m_strText.IsEmpty())
        {
            int nSubStringStart = 0, nSubStringEnd = 0;

            while (TRUE)
            {
                nSubStringStart = m_strText.Find(L"%str", nSubStringEnd);
                if (-1 == nSubStringStart)
                    break;

                int nID = ::StrToInt((LPCTSTR)m_strText + nSubStringStart + 4);
                if (0 >= nID)
                    break;

                nSubStringEnd = m_strText.Find(L'%', nSubStringStart + 4) + 1;
                if (-1 == nSubStringEnd)
                    break;

                m_strText = m_strText.Left(nSubStringStart)
                    + BkString::Get((UINT)nID)
                    + m_strText.Mid(nSubStringEnd);
            }
        }
    }
    return TRUE;
}

inline BOOL CBkText::PreAddTooltip( CBkWnd* pBkWnd )
{
    if( pBkWnd == this && m_bShowTipWhenOver )
    {
        CRect rcTest;
        CalculateTextDrawRect( rcTest );
        if( rcTest.Width() > m_rcWindow.Width() && m_rcWindow.Width() > 0 )
        {
            return TRUE;
        }
        return FALSE;
    }
    return TRUE;
}

inline BOOL CBkText::PreUpdateTooltip( CBkWnd* pBkWnd )
{
    if( m_tooltip.IsEmpty() )
    {
        return FALSE;
    }

    if( pBkWnd == this && m_bShowTipWhenOver )
    {
        CRect rcTest;
        CalculateTextDrawRect( rcTest );
        if( rcTest.Width() > m_rcWindow.Width() && m_rcWindow.Width() > 0 )
        {
            return TRUE;
        }
        return FALSE;
    }
    return TRUE;
}

inline void CBkText::MeasureDrawText( SIZE& szDraw )
{
    //Size sz;
    //Size szBound(0,0);
    //HDC hDC = ::GetDC( GetContainer() );

    //CDCHandle dc(hDC);
    //BkDCPaint BkDC;
    //BeforePaint( dc, BkDC );
    //UINT nFormat = m_style.m_nTextAlign;
    //nFormat &= ~(DT_WORDBREAK|DT_PATH_ELLIPSIS|DT_END_ELLIPSIS|DT_WORD_ELLIPSIS);
    //nFormat |= (DT_SINGLELINE|DT_CALCRECT);
    //dc.DrawText( m_strText, m_strText.GetLength(), BkDC.rcClient, nFormat );
    //AfterPaint( dc, BkDC );
    //szDraw.cx = BkDC.rcClient.Width();
    //szDraw.cy = BkDC.rcClient.Height();
    //
    //::ReleaseDC( GetContainer(),hDC );    

    CRect rcDraw;
    CalculateTextDrawRect( rcDraw );
    szDraw.cx = rcDraw.Width();
    szDraw.cy = rcDraw.Height();
}

inline BOOL CBkText::OnPaint(CDCHandle dc)
{
    BkDCPaint BkDC;
    //BeforePaint(dc, BkDC);
    //dc.DrawText(m_strText, m_strText.GetLength(), BkDC.rcClient, m_style.m_nTextAlign);
    //AfterPaint(dc, BkDC);

    Gdiplus::Graphics g(dc.m_hDC);

    CRect rcDraw = m_rcWindow;
    rcDraw.DeflateRect( m_style.m_nMarginX, m_style.m_nMarginY );

    HFONT fnt = NULL;
    COLORREF clr = RGB(0,0,0);
    CalculateTextColorFont( fnt, clr );

    StringFormat fmt;
    CalculateTextAlignment( fmt );
    CalculateTextEllipsis( fmt );
    CalculateTextHotkeyPrefix( fmt );
    CalculateTextFormat( fmt );

    Gdiplus::RectF rc( rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height() );
    Gdiplus::Font fntText(dc.m_hDC, fnt);
    Gdiplus::SolidBrush clrText( Gdiplus::Color(254, GetRValue(clr), GetGValue(clr), GetBValue(clr)) );

    Gdiplus::TextRenderingHint textRenderHint = g.GetTextRenderingHint();
    //g.SetTextRenderingHint(Gdiplus::TextRenderingHintClearTypeGridFit);
    g.DrawString( m_strText, m_strText.GetLength(), &fntText, rc, &fmt, &clrText );
    //g.SetTextRenderingHint(textRenderHint);

    return TRUE;
}

inline void CBkText::SetText(LPCTSTR szText)
{
    if (m_strText != szText)
    {
        m_strText = szText;
        _RepositionSelf();
        NotifyInvalidate();
    }
}

inline LPCTSTR CBkText::GetText() const
{
    return m_strText;
}

inline BOOL CBkText::OnNcCalcSize(LPSIZE pSize)
{
    if (2 != m_dlgpos.nType)
    {
        return __super::OnNcCalcSize(pSize);
    }
    else
    {
        //CDCHandle dcDesktop = ::GetDC(::GetDesktopWindow());
        //CDC dcTest;
        //HFONT hftOld = NULL, hftTest = NULL;
        //CRect rcTest = m_rcWindow;

        //dcTest.CreateCompatibleDC(dcDesktop);
        //if (m_style.m_ftText)
        //    hftTest = m_style.GetFont(GetState());
        //else
        //{
        //    CBkWnd* pWnd = this;
        //    HBKWND hWndParent = NULL;
        //    HFONT ftParent = NULL;
        //    while (hWndParent = pWnd->GetParent())
        //    {
        //        pWnd = BkWnds::GetWindow(hWndParent);
        //        ftParent = pWnd->GetStyle()->GetFont(GetState());
        //        if (ftParent != NULL)
        //        {
        //            hftTest = ftParent;
        //        }
        //    }

        //}

        //if (NULL == hftTest)
        //{
        //    if (m_bBold)
        //    {
        //        hftTest = BkFontPool::GetFont(BKF_BOLDFONT);
        //    }
        //    else
        //    {
        //        hftTest = BkFontPool::GetFont(BKF_DEFAULTFONT);
        //    }
        //}

        //hftOld = dcTest.SelectFont(hftTest);

        //rcTest.DeflateRect(m_style.m_nMarginX, m_style.m_nMarginY);

        //int nTestDrawMode = m_style.m_nTextAlign & ~(DT_CENTER | DT_RIGHT | DT_VCENTER | DT_BOTTOM);

        //if (nTestDrawMode & DT_WORDBREAK)
        //    rcTest.bottom = 0x7FFF;

        //dcTest.DrawText(m_strText, m_strText.GetLength(), rcTest, nTestDrawMode | DT_CALCRECT);

        //pSize->cx = rcTest.Width() + 2 * m_style.m_nMarginX;
        //pSize->cy = rcTest.Height() + 2 * m_style.m_nMarginY;

        //dcTest.SelectFont(hftOld);
        //::ReleaseDC(::GetDesktopWindow(), dcDesktop);
        
        CRect rcDraw;
        CalculateTextDrawRect( rcDraw );
        pSize->cx = rcDraw.Width() + 2 * m_style.m_nMarginX;
        pSize->cy = rcDraw.Height() + 2 * m_style.m_nMarginY;
    }
    return TRUE;
}

inline BOOL CBkText::OnRButtonUp(UINT nFlags, CPoint point,BOOL bAct)
{
	if (bAct)
	{
		if (m_pNotify != NULL)
		{
			NotifyInfo info = {WM_RBUTTONUP, point, this};
			m_pNotify->Action(&info);
		}
	}
    return FALSE;
}

inline void CBkText::CalculateTextDrawRect( CRect& rcDrawText )
{
    //CDCHandle dcDesktop = ::GetDC(::GetDesktopWindow());
    //CDC dcTest;
    //HFONT hftOld = NULL, hftTest = NULL;
    //rcDrawText = m_rcWindow;

    //dcTest.CreateCompatibleDC(dcDesktop);
    //if (m_style.m_ftText)
    //    hftTest = m_style.GetFont(GetState());
    //else
    //{
    //    CBkWnd* pWnd = this;
    //    HBKWND hWndParent = NULL;
    //    HFONT ftParent = NULL;
    //    while (hWndParent = pWnd->GetParent())
    //    {
    //        pWnd = BkWnds::GetWindow(hWndParent);
    //        ftParent = pWnd->GetStyle()->GetFont(GetState());
    //        if (ftParent != NULL)
    //        {
    //            hftTest = ftParent;
    //        }
    //    }

    //}

    //if (NULL == hftTest)
    //{
    //    if (m_bBold)
    //    {
    //        hftTest = BkFontPool::GetFont(BKF_BOLDFONT);
    //    }
    //    else
    //    {
    //        hftTest = BkFontPool::GetFont(BKF_DEFAULTFONT);
    //    }
    //}

    //hftOld = dcTest.SelectFont(hftTest);

    //int nTestDrawMode = m_style.m_nTextAlign & ~(DT_CENTER|DT_RIGHT|DT_VCENTER|DT_BOTTOM|DT_PATH_ELLIPSIS|DT_END_ELLIPSIS|DT_WORD_ELLIPSIS);
    //if (nTestDrawMode & DT_WORDBREAK)
    //    rcDrawText.bottom = 0x7FFF;

    //dcTest.DrawText(m_strText, m_strText.GetLength(), rcDrawText, nTestDrawMode | DT_CALCRECT);

    //dcTest.SelectFont(hftOld);
    //::ReleaseDC(::GetDesktopWindow(), dcDesktop);

    //// 

    rcDrawText = m_rcWindow;

    CDCHandle dcMem = ::GetDC( GetContainer() );

    Gdiplus::Graphics g(dcMem);
    CRect rcDraw = m_rcWindow;
    rcDraw.DeflateRect( m_style.m_nMarginX, m_style.m_nMarginY );

    HFONT fnt = NULL;
    COLORREF clr = RGB(0,0,0);
    CalculateTextColorFont( fnt, clr );

    StringFormat fmt;
    CalculateTextAlignment( fmt );
    CalculateTextHotkeyPrefix( fmt );
    CalculateTextFormat( fmt );
    fmt.SetFormatFlags( StringFormatFlagsMeasureTrailingSpaces );
    fmt.SetTrimming( StringTrimmingNone );

    Gdiplus::RectF rc( rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height() );
    Gdiplus::Font fntText(dcMem, fnt);
    Gdiplus::SolidBrush clrText( Gdiplus::Color(254, GetRValue(clr), GetGValue(clr), GetBValue(clr)) );

    RectF szText;
    int iCount = 0;
    g.MeasureString( m_strText, m_strText.GetLength(), &fntText, RectF(0,0,0,0), &fmt, &szText, &iCount );
    rcDrawText.right = rcDrawText.left + szText.Width;
    rcDrawText.bottom = rcDrawText.top + szText.Height;

    ::ReleaseDC( GetContainer(), dcMem );
}

inline void CBkText::CalculateTextColorFont( HFONT& font, COLORREF& clr )
{
    if (m_style.m_ftText)
        font = m_style.m_ftText;

    if (m_style.m_crText != CLR_INVALID)
        clr = m_style.m_crText;

    if (IsDisableDeep(TRUE))
    {
        if (m_style.m_crDisabledText != CLR_INVALID)
            clr = m_style.m_crDisabledText;
    }
    else if (BkWndState_Hover == (GetState() & BkWndState_Hover))
    {
        if (m_style.m_ftHover)
            font = m_style.m_ftHover;

        if (m_style.m_crHoverText != CLR_INVALID)
            clr = m_style.m_crHoverText;
    }

    if (font == NULL)
    {
        font = BkFontPool::GetDefaultFont();
    }

    if (font)
    {
        if (m_bBold)
        {
            font = BkFontPool::GetBoldFont(font);
        }
    }
}

inline void CBkText::CalculateTextAlignment( StringFormat& fmt )
{
    if( DT_CENTER == (m_style.m_nTextAlign & DT_CENTER) )
    {
        fmt.SetAlignment( StringAlignmentCenter );
    }
    else if( DT_RIGHT == (m_style.m_nTextAlign & DT_RIGHT) )
    {
        fmt.SetAlignment( StringAlignmentFar );
    }
    else if( DT_LEFT == (m_style.m_nTextAlign & DT_LEFT) )
    {
        fmt.SetAlignment( StringAlignmentNear );
    }

    if( DT_VCENTER == (m_style.m_nTextAlign & DT_VCENTER) )
    {
        fmt.SetLineAlignment( StringAlignmentCenter );
    }
    else if( DT_BOTTOM == (m_style.m_nTextAlign & DT_BOTTOM) )
    {
        fmt.SetLineAlignment( StringAlignmentFar );
    }
    else if( DT_TOP == (m_style.m_nTextAlign & DT_TOP) )
    {
        fmt.SetLineAlignment( StringAlignmentNear );
    }
}

inline void CBkText::CalculateTextEllipsis( StringFormat& fmt )
{
    if( DT_PATH_ELLIPSIS == (m_style.m_nTextAlign & DT_PATH_ELLIPSIS) )
    {
       fmt.SetTrimming( StringTrimmingEllipsisCharacter );
    }
    else if( DT_END_ELLIPSIS == (m_style.m_nTextAlign & DT_END_ELLIPSIS) )
    {
        fmt.SetTrimming( StringTrimmingEllipsisCharacter );
    }
    else if( DT_WORD_ELLIPSIS == (m_style.m_nTextAlign & DT_WORD_ELLIPSIS) )
    {
        fmt.SetTrimming( StringTrimmingEllipsisWord );
    }
    else
    {
        fmt.SetTrimming( StringTrimmingNone );
    }
}

inline void CBkText::CalculateTextHotkeyPrefix( StringFormat& fmt )
{
    if( DT_NOPREFIX == (m_style.m_nTextAlign & DT_NOPREFIX) )
    {
        fmt.SetHotkeyPrefix( HotkeyPrefixNone );
    }
    else
    {
        fmt.SetHotkeyPrefix( HotkeyPrefixShow );
    }
}

inline void CBkText::CalculateTextFormat( StringFormat& fmt )
{
    fmt.SetFormatFlags( StringFormatFlagsLineLimit );
    if( !(DT_WORDBREAK == (m_style.m_nTextAlign & DT_WORDBREAK)) )
    {
        fmt.SetFormatFlags( StringFormatFlagsNoWrap );
    }
    else if( DT_NOCLIP == (m_style.m_nTextAlign & DT_NOCLIP) )
    {
        fmt.SetFormatFlags( StringFormatFlagsNoClip );
    }
}