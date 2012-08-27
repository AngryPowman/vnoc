#pragma warning(disable:4244)


template <typename _TYPEDEST,typename _TYPESRC>
inline _TYPEDEST expandType(const _TYPESRC &src)
{
	ATLASSERT(sizeof(_TYPEDEST) >= sizeof(_TYPESRC));
	_TYPEDEST dest;
	ZeroMemory(dest,sizeof(_TYPEDEST));
	memcpy(&dest,&src,sizeof(src));
	return dest;
}

#define RATIO_TURNPAGE  0.5
#define RATE_PROGRESS   0.9

inline CBkEffectTurnPage2::CBkEffectTurnPage2()
:m_nCanvasPadding(0)
,m_nFrames(20)
,m_nFlip(0)
,m_nProcess(1.0)
,m_pSkinUnderPage(NULL)
,m_clrUnderPage(RGB(255,255,255))
,m_hDCAfterPage(NULL)
,m_hBitmapAfterPage(NULL)
{
}

inline CBkEffectTurnPage2::~CBkEffectTurnPage2()
{
    if (m_hBitmapAfterPage)
    {
        ::DeleteObject(m_hBitmapAfterPage);
        m_hBitmapAfterPage = NULL;
    }

    if (m_hDCAfterPage)
    {
        ::DeleteObject(m_hDCAfterPage);
        m_hDCAfterPage = NULL;
    }
}

inline void CBkEffectTurnPage2::Init(EffectActionInfo info)
{
    CBkEffectObject::Init(info);

    ExtraData_TurnPage* pExtraData = expandType<ExtraData_TurnPage*>(info.dwExtra);
    if (pExtraData)
    {
        m_extraData.bRealDrawUnderPage = pExtraData->bRealDrawUnderPage;
        m_extraData.szNextPageCache = pExtraData->szNextPageCache;
    }

    m_nFlip = 0;
    switch(m_effectInfo.nType)
    {
    case EffectTurnPage2_Previous:
        m_nProcess = -1.0;
        break;
    case EffectTurnPage2_Next:
        m_nProcess = 1.0;
        break;
    }

    if (m_hBitmapAfterPage)
    {
        ::DeleteObject(m_hBitmapAfterPage);
        m_hBitmapAfterPage = NULL;
    }

    if (m_hDCAfterPage)
    {
        ::DeleteObject(m_hDCAfterPage);
        m_hDCAfterPage = NULL;
    }
}

inline void CBkEffectTurnPage2::Render(CDCHandle& dc)
{
    Render_Background(dc, m_effectInfo.infoAction.rcEffect);

    HDC hdcMem = ::CreateCompatibleDC(dc.m_hDC);
    HBITMAP hbmpMem = ::CreateCompatibleBitmap(dc.m_hDC, m_effectInfo.infoAction.rcEffect.Width(), m_effectInfo.infoAction.rcEffect.Height());
    HBITMAP hbmpOld = (HBITMAP)::SelectObject(hdcMem, hbmpMem);
    
    CRect rcCanvas = m_effectInfo.infoAction.rcEffect;
    rcCanvas.DeflateRect(&m_effectInfo.infoAction.rcEffectOffset);
    int nPageW = rcCanvas.Width()/2;

    CRect rcDraw(0, 0, rcCanvas.Width(), rcCanvas.Height());
    Render_FullPage(CDCHandle(hdcMem), rcDraw);
    Render_UnderPage(CDCHandle(hdcMem), rcDraw);

    Gdiplus::Graphics gMem(hdcMem);
    gMem.TranslateTransform(nPageW, m_nCanvasPadding);

    Render_TurnPage(gMem, rcDraw);
    dc.BitBlt(rcCanvas.left, rcCanvas.top, rcCanvas.Width(), rcCanvas.Height(), hdcMem, 0, 0, SRCCOPY);
    
    ::SelectObject(hdcMem, hbmpMem);
    ::DeleteObject(hbmpMem);
    hbmpMem = NULL;
    ::DeleteDC(hdcMem);
    hdcMem = NULL;
}

inline BOOL CBkEffectTurnPage2::UpdateInfo()
{
    CRect rcCanvas = m_effectInfo.infoAction.rcEffect;
    rcCanvas.DeflateRect(&m_effectInfo.infoAction.rcEffectOffset);
    int nPageW = rcCanvas.Width()/2;
    //if (m_nFlip >= rcCanvas.Width())
    //{
    //    Stop();
    //}

    POINT ptDraw = {};

    int step = nPageW/m_nFrames;
    m_nFlip += step;

    double pos = 0.0;
    switch(m_effectInfo.nType)
    {
    case EffectTurnPage2_Previous:
        ptDraw.x = m_nFlip - nPageW;
        break;
    case EffectTurnPage2_Next:
        ptDraw.x = nPageW - m_nFlip;
        break;
    }
    pos = max(min((double)((double)ptDraw.x/(double)nPageW), 1.0), -1.0);
    m_nProcess += (pos-m_nProcess)*RATE_PROGRESS;

    int nFlipW = ( nPageW * RATIO_TURNPAGE ) * ( 1 - m_nProcess );
    int xFoldPaper = nPageW * m_nProcess + nFlipW;
    BOOL bEnd = FALSE;
    switch(m_effectInfo.nType)
    {
    case EffectTurnPage2_Previous:
        if (xFoldPaper >= nPageW-10)
        {
            bEnd = TRUE;
        }
        break;
    case EffectTurnPage2_Next:
        if (xFoldPaper <= 10)
        {
            bEnd = TRUE;
        }
        break;
    }
    if (bEnd)
    {
        Stop();
    }
	return TRUE;
}

inline void CBkEffectTurnPage2::OnStart()
{
    //_CreateAfterPageCache();
}

inline void CBkEffectTurnPage2::Render_Background(CDCHandle& dc, const CRect& rcDraw)
{
    dc.BitBlt(rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height(), 
        m_MemDC, m_effectInfo.infoAction.rcEffect.left, m_effectInfo.infoAction.rcEffect.top, SRCCOPY);
}

inline void CBkEffectTurnPage2::Render_FullPage(CDCHandle& dc, const CRect& rcDraw)
{
    CRect rcSrc = m_effectInfo.infoAction.rcEffect;
    rcSrc.left += m_effectInfo.infoAction.rcEffectOffset.left;
    rcSrc.top += m_effectInfo.infoAction.rcEffectOffset.top;
    rcSrc.right -= m_effectInfo.infoAction.rcEffectOffset.right;
    rcSrc.bottom -= m_effectInfo.infoAction.rcEffectOffset.bottom;
    dc.BitBlt(rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height(), 
              m_MemDC, rcSrc.left, rcSrc.top, SRCCOPY);
}

inline void CBkEffectTurnPage2::Render_UnderPage(CDCHandle& dc, const CRect& rcDraw)
{
    CRect rcCanvas = rcDraw;
    rcCanvas.DeflateRect( m_nCanvasPadding, m_nCanvasPadding );

    int nPageW = rcCanvas.Width()/2;
    int nPageH = rcCanvas.Height();

    int nFlipW = ( nPageW * RATIO_TURNPAGE ) * ( 1 - m_nProcess );
    int xFoldPaper = nPageW * m_nProcess + nFlipW;

    double lenFlip = 1 - fabs(m_nProcess);
    int rightShadowWidth = 0;/* = ( nPageW * RATIO_TURNPAGE ) * max( min( lenFlip, RATIO_TURNPAGE ), 0 )*/;
    int leftShadowWidth = 0;/*= ( nPageW * RATIO_TURNPAGE ) * max( min( lenFlip, RATIO_TURNPAGE ), 0 )*/;

    CRect rc;
    m_effectInfo.infoAction.pCtrl->GetRect(&rc);
    if (!m_hDCAfterPage || m_extraData.bRealDrawUnderPage)
    {
        if (!m_hDCAfterPage)
        {
            m_hDCAfterPage = CreateCompatibleDC(dc);
        }
        if (!m_hBitmapAfterPage || m_extraData.bRealDrawUnderPage)
        {
            if (!m_hBitmapAfterPage)
            {
                m_hBitmapAfterPage = CreateCompatibleBitmap(dc, rc.Width(), rc.Height());
            }
            SelectObject(m_hDCAfterPage, m_hBitmapAfterPage);

            CWindow wnd(m_effectInfo.infoAction.pCtrl->GetRealWnd());
            wnd.ModifyStyle(0, WS_VISIBLE, SWP_NOREDRAW);
            PrintWindow(m_effectInfo.infoAction.pCtrl->GetRealWnd(), m_hDCAfterPage, 0);
            wnd.ModifyStyle(WS_VISIBLE, 0, SWP_NOREDRAW);
        }
    }

    CRect rcUnderSrc = CRect(0, 0, rc.Width(), rc.Height());
    CRect rcUnder = rcDraw;

    int offsetX = m_effectInfo.infoAction.rcEffectOffset.left - m_extraData.szNextPageCache.cx;
    int offsetY = m_effectInfo.infoAction.rcEffectOffset.top - m_extraData.szNextPageCache.cy;
    rcUnderSrc.DeflateRect(offsetX, offsetY);

    int turnWidth = 0;
    switch(m_effectInfo.nType)
    {
    case EffectTurnPage2_Previous:
        turnWidth = leftShadowWidth + (nPageW + xFoldPaper - nFlipW) + 15;
        rcUnder.right = rcUnder.left + turnWidth;
        rcUnderSrc.right  = rcUnderSrc.left + turnWidth;
        break;
    case EffectTurnPage2_Next:
        turnWidth = rightShadowWidth + nFlipW + 15;
        rcUnder.left = rcUnder.right - turnWidth;
        rcUnderSrc.left = rcUnderSrc.right - turnWidth;
        break;
    }

    dc.BitBlt(rcUnder.left, rcUnder.top, rcUnder.Width(), rcUnder.Height(), 
        m_hDCAfterPage, rcUnderSrc.left, rcUnderSrc.top, SRCCOPY);
}

inline void CBkEffectTurnPage2::Render_TurnPage(Gdiplus::Graphics& g, const CRect& rcDraw)
{
    CRect rcCanvas = rcDraw;
    rcCanvas.DeflateRect( m_nCanvasPadding, m_nCanvasPadding );

    int nPageW = rcCanvas.Width()/2;
    int nPageH = rcCanvas.Height();

    double lenFlip = 1 - fabs(m_nProcess);
    int nFlipW = ( nPageW * RATIO_TURNPAGE ) * ( 1 - m_nProcess );
    int vOutdent = 20 * lenFlip;

    int xFoldPaper = nPageW * m_nProcess + nFlipW;

    CRect rcFlip;
    rcFlip.right = xFoldPaper;
    rcFlip.left = xFoldPaper - nFlipW;
    rcFlip.top = 0;
    rcFlip.bottom = nPageH;

    Gdiplus::Color cr;
    cr.SetFromCOLORREF(RGB(255, 0, 255));
    Gdiplus::SolidBrush brush(cr);
    //g.FillRectangle(&brush, rcFlip.left, rcFlip.top, rcFlip.Width(), rcFlip.Height());

    //int paperShadowWidth /*= ( nPageW * RATIO_TURNPAGE ) * max( min( 1 - m_nProcess, RATIO_TURNPAGE ), 0 )*/;
    int rightShadowWidth /*= ( nPageW * RATIO_TURNPAGE ) * max( min( lenFlip, RATIO_TURNPAGE ), 0 )*/;
    int leftShadowWidth /*= ( nPageW * RATIO_TURNPAGE ) * max( min( lenFlip, RATIO_TURNPAGE ), 0 )*/;

    Gdiplus::InterpolationMode ipMode = g.GetInterpolationMode();
    Gdiplus::PixelOffsetMode pom = g.GetPixelOffsetMode();
    g.SetInterpolationMode(InterpolationModeNearestNeighbor);
    g.SetPixelOffsetMode(PixelOffsetModeHalf);

    // 绘制被翻页左侧的阴影
    Gdiplus::Pen penLeftShadow(Gdiplus::Color(15*lenFlip, 0, 0, 0), 30*lenFlip);
    g.DrawLine(&penLeftShadow, Gdiplus::Point(xFoldPaper-nFlipW,-vOutdent*0.5), Gdiplus::Point(xFoldPaper-nFlipW, nPageH+(vOutdent*0.5)));

    // 右侧被翻页在下一显示页上留下的阴影
    Gdiplus::Color clrRightShadow[] = { Gdiplus::Color(30*lenFlip, 0, 0, 0),
                                        Gdiplus::Color(0, 0, 0, 0),
                                        Gdiplus::Color(0, 0, 0, 0) };
    Gdiplus::REAL posRightShadow[] = { 0.0f, 0.8f, 1.0f };
    Gdiplus::Color cr1RightShadow(30*lenFlip, 0, 0, 0);
    Gdiplus::Color cr2RightShadow(0, 0, 0, 0);
    Gdiplus::Rect rcBrushRightShadow(xFoldPaper, 0, rightShadowWidth, nPageH);
    Gdiplus::LinearGradientBrush lbRightShadow(rcBrushRightShadow, cr1RightShadow, cr2RightShadow, LinearGradientModeHorizontal);
    lbRightShadow.SetInterpolationColors(clrRightShadow, posRightShadow, 3);

    Gdiplus::GraphicsPath pathRightShadow;
    pathRightShadow.AddLine(xFoldPaper, 0, xFoldPaper+rightShadowWidth, 0);
    pathRightShadow.AddLine(xFoldPaper+rightShadowWidth, 0, xFoldPaper+rightShadowWidth, nPageH);
    pathRightShadow.AddLine(xFoldPaper+rightShadowWidth, nPageH, xFoldPaper, nPageH);
    pathRightShadow.AddLine(xFoldPaper, nPageH, xFoldPaper, 0);
    g.FillPath(&lbRightShadow, &pathRightShadow);

    // 左侧被翻页在上一显示页上留下的阴影
    Gdiplus::Color clrLeftShadow[] = { Gdiplus::Color(0, 0, 0, 0),
                                       Gdiplus::Color(20*lenFlip, 0, 0, 0),
                                       Gdiplus::Color(20*lenFlip, 0, 0, 0) };
    Gdiplus::REAL posLeftShadow[] = { 0.0f, 0.8f, 1.0f };
    Gdiplus::Color cr1LeftShadow(0, 0, 0, 0);
    Gdiplus::Color cr2LeftShadow(20*lenFlip, 0, 0, 0);
    Gdiplus::Rect rcBrushLeftShadow(xFoldPaper-nFlipW-leftShadowWidth, 0, leftShadowWidth, nPageH);
    Gdiplus::LinearGradientBrush lbLeftShadow(rcBrushLeftShadow, cr1LeftShadow, cr2LeftShadow, LinearGradientModeHorizontal);
    lbLeftShadow.SetInterpolationColors(clrLeftShadow, posLeftShadow, 3);

    Gdiplus::GraphicsPath pathLeftShadow;
    pathLeftShadow.AddLine(xFoldPaper-nFlipW-leftShadowWidth, 0, xFoldPaper-nFlipW, 0);
    pathLeftShadow.AddLine(xFoldPaper-nFlipW, 0, xFoldPaper-nFlipW, nPageH);
    pathLeftShadow.AddLine(xFoldPaper-nFlipW, nPageH, xFoldPaper-nFlipW-leftShadowWidth, nPageH);
    pathLeftShadow.AddLine(xFoldPaper-nFlipW-leftShadowWidth, nPageH, xFoldPaper-nFlipW-leftShadowWidth, 0);
    g.FillPath(&lbLeftShadow, &pathLeftShadow);

    // 绘制被翻页
    Gdiplus::Color clr[] = { Gdiplus::Color(0xFF, 0xFA, 0xFA, 0xFA),
                            Gdiplus::Color(0xFF, 0xEE, 0xEE, 0xEE),
                            Gdiplus::Color(0xFF, 0xFA, 0xFA, 0xFA),
                            Gdiplus::Color(0xFF, 0xE2, 0xE2, 0xE2) };
    Gdiplus::REAL positions[] = { 0.0f, 0.73f, 0.9f, 1.0f };
    Gdiplus::Rect rcBrush(xFoldPaper-nFlipW, 0, nFlipW, 1);
    Gdiplus::LinearGradientBrush lb(rcBrush, Gdiplus::Color(255, 0, 0, 0), Gdiplus::Color(255, 255, 255, 255), LinearGradientModeHorizontal);
    //Gdiplus::LinearGradientBrush lb(Gdiplus::Point(xFoldPaper-nFlipW, 0), Gdiplus::Point(xFoldPaper, 0), cr1, cr2);
    lb.SetInterpolationColors(clr, positions, 4);

    Gdiplus::GraphicsPath pathFolder;
    pathFolder.AddLine(xFoldPaper, 0, xFoldPaper, nPageH);
    //pathFolder.AddBezier(Point(xFoldPaper, nPageH), Point(xFoldPaper, nPageH + (vOutdent*2)), 
    //                     Point(xFoldPaper, nPageH + (vOutdent*2)), Point(xFoldPaper-nFlipW, nPageH+vOutdent));
    pathFolder.AddBezier(Point(xFoldPaper, nPageH), Point(xFoldPaper, nPageH + (vOutdent*2)), 
        Point(xFoldPaper-nFlipW/2, nPageH + (vOutdent)), Point(xFoldPaper-nFlipW, nPageH+vOutdent));
    pathFolder.AddLine(xFoldPaper-nFlipW, nPageH+vOutdent, xFoldPaper-nFlipW, -vOutdent);
    //pathFolder.AddBezier(Point(xFoldPaper-nFlipW, -vOutdent), Point(xFoldPaper, -vOutdent*2), 
    //                     Point(xFoldPaper, -vOutdent*2), Point(xFoldPaper, 0));
    pathFolder.AddBezier(Point(xFoldPaper-nFlipW, -vOutdent), Point(xFoldPaper-nFlipW/2, -vOutdent), 
        Point(xFoldPaper, -vOutdent*2), Point(xFoldPaper, 0));

    g.FillPath(&lb, &pathFolder);
    Gdiplus::Pen penFolder(Gdiplus::Color(15, 0, 0, 0), 0.5);
    g.DrawPath(&penFolder, &pathFolder);

    g.SetPixelOffsetMode(pom);
    g.SetInterpolationMode( ipMode );
}

inline BOOL CBkEffectTurnPage2::_CreateAfterPageCache()
{
    BOOL bRet = FALSE;
    if (m_effectInfo.infoAction.pCtrl)
    {
        HWND hWnd = m_effectInfo.infoAction.pCtrl->GetContainer();
        if (hWnd)
        {
            CRect rc;
            m_effectInfo.infoAction.pCtrl->GetRect(&rc);
            HDC hDC = ::GetDC(hWnd);
            //bRet = m_MemDCAfterPage.Create(hDC, rc, TRUE);

            if (m_hBitmapAfterPage)
            {
                ::DeleteObject(m_hBitmapAfterPage);
                m_hBitmapAfterPage = NULL;
            }

            if (m_hDCAfterPage)
            {
                ::DeleteObject(m_hDCAfterPage);
                m_hDCAfterPage = NULL;
            }

            //CRect rc;
            //m_effectInfo.infoAction.pCtrl->GetRect(&rc);
            m_hDCAfterPage = CreateCompatibleDC(hDC);
            if (m_hDCAfterPage)
            {
                m_hBitmapAfterPage = CreateCompatibleBitmap(hDC, rc.Width(), rc.Height());
                if (m_hBitmapAfterPage)
                {
                    SelectObject(m_hDCAfterPage, m_hBitmapAfterPage);

                    CWindow wnd(m_effectInfo.infoAction.pCtrl->GetRealWnd());
                    wnd.ModifyStyle(0, WS_VISIBLE, SWP_NOREDRAW);
                    PrintWindow(m_effectInfo.infoAction.pCtrl->GetRealWnd(), m_hDCAfterPage, 0);
                    wnd.ModifyStyle(WS_VISIBLE, 0, SWP_NOREDRAW);
                }
            }

            ::ReleaseDC(hWnd, hDC);
        }
    }
    return bRet;
}

#pragma warning(default:4244)