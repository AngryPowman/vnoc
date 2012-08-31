//////////////////////////////////////////////////////////////////////////
//   File Name: bkwndProgress.h
// Description: Progress control
//     Creator: DongM
//     Version: 2011.04.24 - 1.0 - Create
//       Usage: <progress id="1104" pos="70,245,-80,262" min="0" max="100" value="15" showpercent="1" bgskin="progressbg" posskin="progresspos"/>
//////////////////////////////////////////////////////////////////////////
#pragma once

class CBkProgress : public CBkWnd
                  , public IBkProgress
{
    BKOBJ_DECLARE_CLASS_NAME(CBkProgress, "progress")

    enum
    {
        ProgressType_binding = 1,
        ProgressType_ring,
    };

public:
    CBkProgress();
    
    // interface IBkProgress
    virtual double  GetValue();
    virtual void    SetValue(DOUBLE dValue);
    virtual void    SetRange(UINT uMin, UINT uMax);
    virtual void    SetShowPercent(BOOL bShow);
	void SetPosSkin(LPCSTR szSkin);

protected:
    BOOL OnNcCalcSize(LPSIZE pSize);
    void DrawBkgnd(CDCHandle& dc);
    BOOL OnPaint(CDCHandle dc);
    HRESULT ParseDouble(LPCSTR strValue, BOOL);

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_SKIN_ATTRIBUTE("bgskin", m_pSkinBg, TRUE)
        BKWIN_SKIN_ATTRIBUTE("posskin", m_pSkinPos, TRUE)
        BKWIN_DWORD_ATTRIBUTE("min", m_dwMinValue, FALSE)
        BKWIN_DWORD_ATTRIBUTE("max", m_dwMaxValue, FALSE)
        BKWIN_CUSTOM_ATTRIBUTE("value", ParseDouble)
        BKWIN_UINT_ATTRIBUTE("showpercent", m_bShowPercent, FALSE)
        BKWIN_UINT_ATTRIBUTE("fractional", m_bPercentFractional, FALSE)
        BKWIN_ENUM_ATTRIBUTE("type", int, FALSE)
            BKWIN_ENUM_VALUE("binding", ProgressType_binding)             // 条状
            BKWIN_ENUM_VALUE("ring", ProgressType_ring)                   // 环状
        BKWIN_ENUM_END(m_drawType)
    BKWIN_DECLARE_ATTRIBUTES_END()
    
    int             m_drawType;                 
    DWORD           m_dwMinValue;
    DWORD           m_dwMaxValue;
    DOUBLE          m_dbValue;
    BOOL            m_bShowPercent;
    BOOL            m_bPercentFractional;       // 百分比是否显示小数
    CBkSkinBase*    m_pSkinBg;
    CBkSkinBase*    m_pSkinPos;
};


//////////////////////////////////////////////////////////////////////////
// CBkProgress实现
inline CBkProgress::CBkProgress()
    : m_dwMinValue(0)
    , m_dwMaxValue(0)
    , m_dbValue(0.0f)
    , m_bShowPercent(FALSE)
    , m_pSkinBg(NULL)
    , m_pSkinPos(NULL)
    , m_drawType(ProgressType_binding)
    , m_bPercentFractional(FALSE)
{
}

inline void CBkProgress::SetPosSkin(LPCSTR szSkin)
{
	CBkSkinBase *pSkin = BkSkin::GetSkin(szSkin);
	if(pSkin)
	{
		m_pSkinPos = pSkin; 
		NotifyInvalidate();
	}
}

inline HRESULT CBkProgress::ParseDouble(LPCSTR strValue, BOOL)
{
    if (NULL != strValue || *strValue != 0)
    {
        sscanf_s(strValue, "%lf", &m_dbValue);
    }
    return S_FALSE;
}

inline double CBkProgress::GetValue()
{
    return m_dbValue;
}

inline void CBkProgress::SetValue(DOUBLE dValue)
{
    TestMainThread();
    m_dbValue = dValue;
    NotifyInvalidate();
}

inline void CBkProgress::SetRange(UINT uMin, UINT uMax)
{
    TestMainThread();
    m_dwMinValue = min(uMin, uMax);
    m_dwMaxValue = max(uMin, uMax);
    NotifyInvalidate();
}

inline void CBkProgress::SetShowPercent(BOOL bShow)
{
    TestMainThread();
    m_bShowPercent = bShow;
    NotifyInvalidate();
}

inline BOOL CBkProgress::OnNcCalcSize(LPSIZE pSize)
{
    __super::OnNcCalcSize(pSize);
    if (m_pSkinBg)
    {
        SIZE sizeBg = m_pSkinBg->GetSkinSize();
        pSize->cy = sizeBg.cy;
    }
    return FALSE;
}

inline void CBkProgress::DrawBkgnd(CDCHandle& dc)
{
}

inline BOOL CBkProgress::OnPaint(CDCHandle dc)
{
    Gdiplus::Graphics g( dc.m_hDC );

    double dPos = 0;
    if (m_pSkinBg)
    {
        switch( m_drawType )
        {
        case ProgressType_binding:
            m_pSkinBg->Draw(dc, m_rcWindow, BkWndState_Normal);
            break;
        case ProgressType_ring:
            m_pSkinBg->Draw(dc, m_rcWindow, 360);
            break;
        }
    }
    else
    {
        Gdiplus::SolidBrush brushBK( Color(255, GetRValue(m_style.m_crBg), GetGValue(m_style.m_crBg), GetBValue(m_style.m_crBg) ));
        g.FillRectangle( &brushBK, m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height() );
    }

    if (m_dwMaxValue == m_dwMinValue)
    {
        dPos = 0;
    }
    else
    {
        if (m_dbValue < m_dwMinValue)
        {
            m_dbValue = m_dwMinValue;
        }
        else if (m_dbValue > m_dwMaxValue)
        {
            m_dbValue = m_dwMaxValue;
        }

        dPos = (double)(m_dbValue - m_dwMinValue) / (double)(m_dwMaxValue - m_dwMinValue);
    }

    CRect rcPosBar = m_rcWindow;

    if (m_pSkinPos)
    {
        SIZE sizePosBar = m_pSkinPos->GetSkinSize();

        //rcPosBar.DeflateRect(2, (m_rcWindow.Height() - sizePosBar.cy) / 2);
        rcPosBar.right = rcPosBar.left + (LONG)(rcPosBar.Width() * dPos);

        switch( m_drawType )
        {
        case ProgressType_binding:
            if (m_dwMaxValue == m_dwMinValue + m_dbValue)
            {
                m_pSkinPos->Draw(dc, rcPosBar, 1);
            }
            else
            {
                m_pSkinPos->Draw(dc, rcPosBar, 0);
            }
            break;
        case ProgressType_ring:
            {
                DWORD radian = 360.0*dPos;
                m_pSkinPos->Draw(dc, m_rcWindow, radian);
            }
            break;
        }

    }
    else
    {
        rcPosBar.DeflateRect(2, 2);
        rcPosBar.right = rcPosBar.left + (LONG)(rcPosBar.Width() * dPos);

        Gdiplus::SolidBrush brushPos( Color(255, GetRValue(m_style.m_crBgHover), GetGValue(m_style.m_crBgHover), GetBValue(m_style.m_crBgHover) ));
        g.FillRectangle( &brushPos, rcPosBar.left, rcPosBar.top, rcPosBar.Width(), rcPosBar.Height() );
    }

    if (m_bShowPercent)
    {
        CString strPercent;
        if( m_bPercentFractional )
        {
            strPercent.Format(_T("%.1lf%%"), (dPos * 100));
        }
        else
        {
            strPercent.Format(_T("%d%%"), (int)(dPos * 100));
        }

        COLORREF crText;
        if (CLR_INVALID == m_style.m_crText)
            crText = RGB( 0xFF, 0xFF, 0xFF );
        else
            crText = m_style.m_crText;

        HFONT fnt = NULL;
        if (m_style.m_ftText)
            fnt = m_style.m_ftText;
        if (fnt == NULL)
        {
            fnt = BkFontPool::GetDefaultFont();
        }

        StringFormat fmt;
        fmt.SetAlignment( StringAlignmentCenter );
        fmt.SetLineAlignment( StringAlignmentCenter );

        Gdiplus::RectF rc( m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height() );
        Gdiplus::Font fntText(dc.m_hDC, fnt);
        Gdiplus::SolidBrush clrText( Gdiplus::Color(254, GetRValue(crText), GetGValue(crText), GetBValue(crText)) );
        g.DrawString( strPercent, strPercent.GetLength(), &fntText, rc, &fmt, &clrText );
    }

    return FALSE;
}