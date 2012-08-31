#include "math.h"
#pragma warning(disable:4244)

inline CBkEffectTurnPage::CBkEffectTurnPage()
:m_nStep(0)
,m_nTurnPageAngle(45)
,m_nPageSpine(0)
,m_nHotspotAxisX(0.0)
,m_clrTurnPageSpin(RGB(235, 234, 227))
,m_clrTurnPageCenter(RGB(250, 249, 243))
,m_clrTurnPageEdge(RGB(178, 177, 170))
,m_pSkinUnderPage(NULL)
{
    m_marginBorder.cx = m_marginBorder.cy = 0;
}

inline CBkEffectTurnPage::~CBkEffectTurnPage()
{

}

inline void CBkEffectTurnPage::Init(EffectActionInfo info)
{
    CBkEffectObject::Init(info);
    m_nHotspotAxisX = 0.0;
}

inline void CBkEffectTurnPage::Render(CDCHandle& dc)
{
    Update_Hotspot();

    Render_FullPage(dc);
    Render_UnderPage(dc);
    Render_TurnPage(dc);
}

inline BOOL CBkEffectTurnPage::UpdateInfo()
{
    CRect rcPage = m_effectInfo.infoAction.rcEffect;
    rcPage.DeflateRect( m_marginBorder.cx, m_marginBorder.cy );

    if (m_nHotspotAxisX >= (rcPage.Width()-m_nPageSpine)/2)
    {
        Stop();
    }
	return TRUE;
}

inline void CBkEffectTurnPage::Render_FullPage(CDCHandle& dc)
{
    CRect rcDraw = m_effectInfo.infoAction.rcEffect;
    dc.BitBlt(rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height(), m_MemDC, rcDraw.left, rcDraw.top, SRCCOPY);
}

inline void CBkEffectTurnPage::Render_UnderPage(CDCHandle& dc)
{
    double angle = 0.0;
    Gdiplus::GraphicsPath* pathUnderPage = _GetUnderPagePath(angle, m_nHotspotAxisX, m_effectInfo.nType, TRUE);
    if (pathUnderPage)
    {
        Gdiplus::Graphics g(dc.m_hDC);
        //Gdiplus::Color cr;
        ////cr.SetFromCOLORREF(RGB(250, 249, 243));
        //cr.SetFromCOLORREF(RGB(255, 255, 255));
        //g.FillPath(&Gdiplus::SolidBrush(cr), pathUnderPage);

        if (m_pSkinUnderPage)
        {
            CRect rcPage = m_effectInfo.infoAction.rcEffect;
            rcPage.DeflateRect( m_marginBorder.cx, m_marginBorder.cy );

            
            CRgn rgnPath;
            INT count = pathUnderPage->GetPointCount();
            POINT* pptPath = new POINT[count];
            Point* dataPoints = new Point[count]; 
            pathUnderPage->GetPathPoints(dataPoints, count);
            for (int i = 0; i < count; ++i)
            {
                pptPath[i].x = dataPoints[i].X;
                pptPath[i].y = dataPoints[i].Y;
            }
            rgnPath.CreatePolygonRgn(pptPath, count, ALTERNATE);
            delete[] dataPoints;
            delete[] pptPath;

            CRgn rgnPanel;
            dc.GetClipRgn(rgnPanel);
            dc.SelectClipRgn(rgnPath);
            m_pSkinUnderPage->Draw(dc, rcPage, 0);
            dc.SelectClipRgn(rgnPanel);

            //Gdiplus::Region reg;
            //g.GetClip(&reg);
            //g.SetClip(pathUnderPage);
            //m_pSkinUnderPage->Draw(g.GetHDC(), rcPage, 0);
            //g.SetClip(&reg);
        }

        delete pathUnderPage;
        pathUnderPage = NULL;
    }
}

inline void CBkEffectTurnPage::Render_TurnPage(CDCHandle& dc)
{
    double angle = 0.0;
    Gdiplus::GraphicsPath* pathHotspot = _GetTurnPagePath(angle, m_nHotspotAxisX, m_effectInfo.nType);
    if (pathHotspot)
    {
        CRect rcPage = m_effectInfo.infoAction.rcEffect;
        rcPage.DeflateRect( m_marginBorder.cx, m_marginBorder.cy );

        Gdiplus::Color clr[] = { Gdiplus::Color(255, 235, 234, 227),
                                 Gdiplus::Color(255, 250, 249, 243),
                                 Gdiplus::Color(255, 178, 177, 170) };

        Gdiplus::REAL positions[] = { 0.0f, 0.8f, 1.0f };

        int x = 0;
        int y = 0;
        int w = m_nHotspotAxisX;
        int h = tan(angle)*w;

        Gdiplus::PointF ptAxis(x, rcPage.bottom);
        switch (m_effectInfo.nType)
        {
        case EffectTurnPage_Previous:
            x = rcPage.left;
            y = rcPage.bottom - h;
            ptAxis.X = rcPage.left + w;
            ptAxis.Y = rcPage.bottom;
            clr[0].SetFromCOLORREF(m_clrTurnPageEdge);
            clr[1].SetFromCOLORREF(m_clrTurnPageCenter);
            clr[2].SetFromCOLORREF(m_clrTurnPageSpin);
            positions[1] = 0.2f;
            break;
        case EffectTurnPage_Next:
            x = rcPage.right - w;
            y = rcPage.bottom - h;
            ptAxis.X = rcPage.right - w;
            ptAxis.Y = rcPage.bottom;
            clr[0].SetFromCOLORREF(m_clrTurnPageSpin);
            clr[1].SetFromCOLORREF(m_clrTurnPageCenter);
            clr[2].SetFromCOLORREF(m_clrTurnPageEdge);
            positions[1] = 0.8f;
            break;
        }

        Gdiplus::Graphics g(dc.m_hDC);

        Gdiplus::Color cr1(255, 0, 0, 0);
        Gdiplus::Color cr2(255, 255, 255, 255);
        Gdiplus::Rect rcBrush(x, y, w, h);
        Gdiplus::LinearGradientBrush lb(rcBrush, cr1, cr2, LinearGradientModeHorizontal);
        lb.SetInterpolationColors(clr, positions, 3);


        Gdiplus::Matrix mx;
        mx.RotateAt(angle, ptAxis);
        lb.SetTransform(&mx);

        Gdiplus::InterpolationMode ipMode = g.GetInterpolationMode();
        Gdiplus::PixelOffsetMode pom = g.GetPixelOffsetMode();
        //g.SetInterpolationMode( InterpolationModeNearestNeighbor );
        //g.SetPixelOffsetMode(PixelOffsetModeHalf);
        g.FillPath(&lb, pathHotspot);
        //g.SetPixelOffsetMode(pom);
        //g.SetInterpolationMode( ipMode );

        delete pathHotspot;
        pathHotspot = NULL;
    }
}

inline void CBkEffectTurnPage::Update_Hotspot()
{
    m_nHotspotAxisX += m_nStep;
}

inline Gdiplus::GraphicsPath* CBkEffectTurnPage::_GetUnderPagePath(double& angle, double x, int pageType, BOOL isUnderPage)
{
    // 页面区域
    CRect rcPage = m_effectInfo.infoAction.rcEffect;
    rcPage.DeflateRect( m_marginBorder.cx, m_marginBorder.cy );

    angle = 0.0;
    double radians = 0.0;

    // 计算新的角度
    angle = m_nTurnPageAngle + (((90-m_nTurnPageAngle) * x)/rcPage.Width());
    if (angle > 90.0)
    {
        angle = 90.0;
    }
    radians = angle * (3.14/180);

    BOOL bTrapezoid = FALSE;
    double h = 0.0;
    h = (x) * (tan(radians));
    if (h > rcPage.Height())
    {
        bTrapezoid = TRUE;
        h = rcPage.Height();
    }

    if (!isUnderPage)
    {
        switch (pageType)
        {
        case EffectTurnPage_Previous:
            pageType = EffectTurnPage_Next;
            break;
        case EffectTurnPage_Next:
            pageType = EffectTurnPage_Previous;
            break;
        }
    }

    int pointCount = 0;
    Gdiplus::PointF* points;
    BYTE* pointsTypes;
    switch (pageType)
    {
    case EffectTurnPage_Previous:
        if (bTrapezoid)
        {
            double x1 = 0.0;
            double x2 = 0.0;
            x1 = h / tan(radians);
            x2 = x - x1;

            pointCount = 4;
            points = new Gdiplus::PointF[pointCount];
            pointsTypes  = new BYTE[pointCount];
            points[0].X = rcPage.left;
            points[0].Y = rcPage.bottom - h;
            points[1].X = rcPage.left;
            points[1].Y = rcPage.bottom;
            points[2].X = rcPage.left + x;
            points[2].Y = rcPage.bottom;
            points[3].X = rcPage.left + x2;
            points[3].Y = rcPage.bottom - h;
            pointsTypes[0] = Gdiplus::PathPointTypeLine;
            pointsTypes[1] = Gdiplus::PathPointTypeLine;
            pointsTypes[2] = Gdiplus::PathPointTypeLine;
            pointsTypes[3] = Gdiplus::PathPointTypeLine;
        }
        else
        {
            pointCount = 3;
            points = new Gdiplus::PointF[pointCount];
            pointsTypes  = new BYTE[pointCount];
            points[0].X = rcPage.left;
            points[0].Y = rcPage.bottom - h;
            points[1].X = rcPage.left;
            points[1].Y = rcPage.bottom;
            points[2].X = rcPage.left + x;
            points[2].Y = rcPage.bottom;
            pointsTypes[0] = Gdiplus::PathPointTypeLine;
            pointsTypes[1] = Gdiplus::PathPointTypeLine;
            pointsTypes[2] = Gdiplus::PathPointTypeLine;
        }
        break;
    case EffectTurnPage_Next:
        if (bTrapezoid)
        {
            double x1 = 0.0;
            double x2 = 0.0;
            x1 = h / tan(radians);
            x2 = x - x1;

            pointCount = 4;
            points = new Gdiplus::PointF[pointCount];
            pointsTypes  = new BYTE[pointCount];
            points[0].X = rcPage.right;
            points[0].Y = rcPage.bottom - h;
            points[1].X = rcPage.right;
            points[1].Y = rcPage.bottom;
            points[2].X = rcPage.right - x;
            points[2].Y = rcPage.bottom;
            points[3].X = rcPage.right - x2;
            points[3].Y = rcPage.bottom - h;
            pointsTypes[0] = Gdiplus::PathPointTypeLine;
            pointsTypes[1] = Gdiplus::PathPointTypeLine;
            pointsTypes[2] = Gdiplus::PathPointTypeLine;
            pointsTypes[3] = Gdiplus::PathPointTypeLine;
        }
        else
        {
            pointCount = 3;
            points = new Gdiplus::PointF[pointCount];
            pointsTypes  = new BYTE[pointCount];
            points[0].X = rcPage.right;
            points[0].Y = rcPage.bottom - h;
            points[1].X = rcPage.right;
            points[1].Y = rcPage.bottom;
            points[2].X = rcPage.right - x;
            points[2].Y = rcPage.bottom;
            pointsTypes[0] = Gdiplus::PathPointTypeLine;
            pointsTypes[1] = Gdiplus::PathPointTypeLine;
            pointsTypes[2] = Gdiplus::PathPointTypeLine;
        }
        break;
    }

    //if ((pageType == EffectTurnPage_Previous && !isUnderPage) ||
    //    (pageType == EffectTurnPage_Next && !isUnderPage))
    //{
    //    pointsTypes[0] = Gdiplus::PathPointTypeStart;
    //    pointsTypes[1] = Gdiplus::PathPointTypeBezier;
    //    pointsTypes[2] = Gdiplus::PathPointTypeLine;
    //}

    Gdiplus::GraphicsPath* gp = new Gdiplus::GraphicsPath(points, pointsTypes, pointCount);

    delete[] points;
    points = NULL;
    delete[] pointsTypes;
    pointsTypes = NULL;

    return gp;
}

inline Gdiplus::GraphicsPath* CBkEffectTurnPage::_GetTurnPagePath(double& angle, double x, int pageType)
{
    Gdiplus::GraphicsPath* pathTurnPage = _GetUnderPagePath(angle, x, pageType, FALSE);
    if (pathTurnPage)
    {
        CRect rcPage = m_effectInfo.infoAction.rcEffect;
        rcPage.DeflateRect( m_marginBorder.cx, m_marginBorder.cy );

        Gdiplus::PointF ptAxis(0, 0);
        double offsetX = 0.0;
        Gdiplus::Matrix mx;
        switch (m_effectInfo.nType)
        {
        case EffectTurnPage_Previous:
            angle = -(180.0 - (2 * angle));
            offsetX = -(rcPage.Width() - (2 * m_nHotspotAxisX));
            ptAxis.X = rcPage.left + x;
            ptAxis.Y = rcPage.bottom;
            break;
        case EffectTurnPage_Next:
            angle = 180.0 - (2 * angle);
            offsetX = rcPage.Width() - (2 * m_nHotspotAxisX);
            ptAxis.X = rcPage.right - x;
            ptAxis.Y = rcPage.bottom;
            break;
        }
        mx.RotateAt(angle, ptAxis);
        mx.Translate(offsetX, 0);
        pathTurnPage->Transform(&mx);
    }
    return pathTurnPage;
}

#pragma warning(default:4244)