

inline void CBkSkinBase::Init()
{

}

inline void CBkSkinBase::Release()
{

}

inline SIZE CBkSkinBase::GetSkinSize()
{
    SIZE ret = {0, 0};
    return ret;
}

inline BOOL CBkSkinBase::IgnoreState()
{
    return TRUE;
}

inline int CBkSkinBase::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
    UINT     num   =   0;                     //   number   of   image   encoders   
    UINT     size   =   0;                   //   size   of   the   image   encoder   array   in   bytes   

    ImageCodecInfo*   pImageCodecInfo   =   NULL;   

    GetImageEncodersSize(&num,   &size);   
    if(size   ==   0)   
        return   -1;     //   Failure   

    pImageCodecInfo   =   (ImageCodecInfo*)(malloc(size));   
    if(pImageCodecInfo   ==   NULL)   
        return   -1;     //   Failure   

    GetImageEncoders(num,   size,   pImageCodecInfo);   

    for(UINT   j   =   0;   j   <   num;   ++j)   
    {   
        if(   wcscmp(pImageCodecInfo[j].MimeType,   format)   ==   0   )   
        {   
            *pClsid   =   pImageCodecInfo[j].Clsid;   
            free(pImageCodecInfo);   
            return   j;     //   Success   
        }           
    }   

    free(pImageCodecInfo);   
    return   -1;     //   Failure  
}

inline void CBkSkinBase::SaveImage2File(Gdiplus::Image &image, const CString& filePath, const CString format)
{
    CLSID formatCLSID;
    GetEncoderClsid(L"image/png", &formatCLSID);
    image.Save(filePath, &formatCLSID, NULL);
}

inline void CBkSkinBase::FillRectangle(Gdiplus::Graphics &g, COLORREF cr, int x, int y, int width, int height)
{
    Gdiplus::Color _cr(GetRValue(cr), GetGValue(cr), GetBValue(cr));
    Gdiplus::SolidBrush _br(_cr);
    g.FillRectangle(&_br, x, y, width, height);
}

inline void CBkSkinBase::DrawImage(Gdiplus::Graphics &g, Gdiplus::Image &bmp, int x, int y, int w, int h)
{
    Gdiplus::Rect rc(x, y, w, h);
    g.DrawImage(&bmp, rc);
}

inline void CBkSkinBase::DrawImage(Gdiplus::Graphics &g, Gdiplus::Image &bmp, int des_x, int des_y, int des_w, int des_h, int src_x, int src_y, int src_w, int src_h)
{
    //Gdiplus::Rect rc(des_x, des_y, des_w, des_h);
    //g.DrawImage(&bmp, rc, src_x, src_y, src_w, src_h, Gdiplus::UnitPixel);

    Gdiplus::Rect rcDraw( des_x, des_y, des_w, des_h );
    Gdiplus::ImageAttributes imAtt; 
    imAtt.SetWrapMode( Gdiplus::WrapModeTileFlipXY );

    g.SetCompositingMode( CompositingModeSourceOver );
    g.SetCompositingQuality( CompositingQualityHighSpeed );

    Gdiplus::InterpolationMode ipMode = g.GetInterpolationMode();
    Gdiplus::PixelOffsetMode pixeloffsetMode = g.GetPixelOffsetMode();
    g.SetInterpolationMode( InterpolationModeNearestNeighbor );
    g.SetPixelOffsetMode( PixelOffsetModeHalf );
    g.DrawImage( &bmp,rcDraw,src_x,src_y,src_w,src_h,Gdiplus::UnitPixel,&imAtt,NULL,NULL );
    g.SetPixelOffsetMode( pixeloffsetMode );
    g.SetInterpolationMode( ipMode );
}

inline void CBkSkinBase::DrawStretch( Gdiplus::Graphics &g, Gdiplus::Image &bmp, int x, int y, int w, int h )
{
    Gdiplus::Rect rcDraw( x,y,w,h );
    Gdiplus::ImageAttributes imAtt; 
    imAtt.SetWrapMode( Gdiplus::WrapModeTileFlipXY );

    g.SetCompositingMode( CompositingModeSourceOver );
    g.SetCompositingQuality( CompositingQualityHighSpeed );

    Gdiplus::InterpolationMode ipMode = g.GetInterpolationMode();
    Gdiplus::PixelOffsetMode pixeloffsetMode = g.GetPixelOffsetMode();
    g.SetInterpolationMode( InterpolationModeNearestNeighbor );
    g.SetPixelOffsetMode( PixelOffsetModeHalf );
    g.DrawImage( &bmp,rcDraw,0,0,bmp.GetWidth(),bmp.GetHeight(),Gdiplus::UnitPixel,&imAtt,NULL,NULL );
    g.SetPixelOffsetMode( pixeloffsetMode );
    g.SetInterpolationMode( ipMode );
}

inline void CBkSkinBase::DrawStretch( Gdiplus::Graphics &g, Gdiplus::Image &bmp, int des_x, int des_y, int des_w, int des_h, int src_x, int src_y, int src_w, int src_h )
{
    //Gdiplus::ImageAttributes imAtt; 
    //imAtt.SetWrapMode( Gdiplus::WrapModeTileFlipXY );
    //Gdiplus::Rect rcDraw( des_x, des_y, des_w, des_h );
    //g.DrawImage( &bmp,rcDraw,src_x,src_y,src_w,src_h,Gdiplus::UnitPixel,&imAtt,NULL,NULL );
    DrawImage( g, bmp, des_x, des_y, des_w, des_h, src_x, src_y, src_w, src_h );
}

inline void CBkSkinBase::DrawImage(Gdiplus::Graphics& g, int desX, int desY, int desW, int desH, Gdiplus::Image& srcImg, int srcX, int srcY, int srcW, int srcH)
{
    Gdiplus::Rect rcDes(desX, desY, desW, desH);
    g.DrawImage(&srcImg, rcDes, srcX, srcY, srcW, srcH, Gdiplus::UnitPixel);
}

inline void CBkSkinBase::DrawStretchXY(Gdiplus::Graphics& g, CRect rcDes, Gdiplus::Image& img, int nSrcIndex, int nSrcSubWidth, int nSrcFixSize)
{
    int iSrcW = nSrcSubWidth;
    int iSrcH = img.GetHeight();
    int iDesW = rcDes.Width();
    int iDesH = rcDes.Height();
    if (iDesH > iSrcH)
    {
        DrawImage(g, rcDes.left, rcDes.top, iDesW, nSrcFixSize
            , img, iSrcW * nSrcIndex, 0, iSrcW, nSrcFixSize);
        DrawImage(g, rcDes.left, rcDes.top + nSrcFixSize, iDesW, rcDes.Height() - nSrcFixSize * 2
            , img, nSrcIndex * nSrcSubWidth, nSrcFixSize, iSrcW, iSrcH - nSrcFixSize * 2);
        DrawImage(g, rcDes.left, rcDes.bottom - nSrcFixSize, iDesW, nSrcFixSize
            , img, nSrcIndex * nSrcSubWidth, iSrcH - nSrcFixSize, iSrcW, nSrcFixSize);
    }
    else if (iDesW > iSrcW)
    {
        DrawImage(g, rcDes.left, rcDes.top, nSrcFixSize, iDesH
            , img, iSrcW * nSrcIndex, 0, nSrcFixSize, iSrcH);
        DrawImage(g, rcDes.left + nSrcFixSize, rcDes.top, rcDes.Width() - nSrcFixSize * 2, iDesH
            , img, nSrcIndex * nSrcSubWidth + nSrcFixSize, 0, nSrcSubWidth - nSrcFixSize * 2, iSrcH);
        DrawImage(g, rcDes.right - nSrcFixSize, rcDes.top, nSrcFixSize, iDesH
            , img, nSrcIndex * nSrcSubWidth + nSrcSubWidth - nSrcFixSize, 0, nSrcFixSize, iSrcH);
    }
    else
    {
        DrawImage(g, rcDes.left, rcDes.top, iDesW, iDesH
            , img, iSrcW * nSrcIndex, 0, iSrcW, iSrcH);
    }
}

inline void CBkSkinBase::DrawTile( Gdiplus::Graphics &g, Gdiplus::Image &bmp, int x, int y, int w, int h, int margin_x, int margin_y, int tileMode/* = WrapModeTile*/ )
{
    if( w <= 0 || h <= 0)
        return;

    int textureWidth = bmp.GetWidth();
    int textureHeight = bmp.GetHeight();
    switch( tileMode )
    {
    case Tile:
        break;
    case StretchTileX:
        textureWidth = w;
        break;
    case StretchTileY:
        textureHeight = h;
        break;
    }

    Gdiplus::Bitmap bmpTexture( textureWidth, textureHeight );
    Gdiplus::Graphics* gtexture = Gdiplus::Graphics::FromImage( &bmpTexture );
    DrawStretch( *gtexture, bmp, 0, 0, textureWidth, textureHeight );
    delete gtexture;

    Gdiplus::Rect rect( 0, 0, textureWidth, textureHeight );
    Gdiplus::TextureBrush brush( &bmpTexture, Gdiplus::WrapModeTile, rect );
    brush.TranslateTransform((REAL)x, (REAL)y, MatrixOrderAppend );
    g.FillRectangle( &brush, x, y, w, h );
}

inline void CBkSkinBase::AlphaImage(Gdiplus::Graphics& g, int desX, int desY, int desW, int desH, Gdiplus::Image& srcImg, int srcX, int srcY, int srcW, int srcH, REAL alpha)
{
    ColorMatrix colorMatrix = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, 0.0f, alpha, 0.0f,
                                0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

    Gdiplus::ImageAttributes imageAttr;
    imageAttr.SetColorMatrix( &colorMatrix );

    Gdiplus::Rect rcDes(desX, desY, desW, desH);
    g.DrawImage(&srcImg, rcDes, srcX, srcY, srcW, srcH, Gdiplus::UnitPixel, &imageAttr);
}

inline void CBkSkinBase::AlphaImage(Gdiplus::Graphics& g, int desX, int desY, int desW, int desH, Gdiplus::Image& srcImg, REAL alpha)
{
    AlphaImage( g, desX, desY, desW, desH, srcImg, 0, 0, srcImg.GetWidth(), srcImg.GetHeight(), alpha );
}

typedef BOOL (WINAPI * FnGradientFill)(HDC, PTRIVERTEX, ULONG, PVOID, ULONG, ULONG);
static BOOL WINAPI GradientFill2(HDC hDC, PTRIVERTEX pVertices, DWORD nVertices, PVOID pMeshElements, ULONG nMeshElements, ULONG dwMode)
{
    HMODULE hMod = ::LoadLibrary(L"msimg32.dll");
    if (hMod)
    {
        FnGradientFill pfnGradientFill = (FnGradientFill)::GetProcAddress(hMod, "GradientFill");
        if (pfnGradientFill)
            pfnGradientFill(hDC, pVertices, nVertices, pMeshElements, nMeshElements, dwMode);
        ::FreeLibrary(hMod);
    }

    return TRUE;
}

inline void CBkSkinBase::GradientFillRectV(HDC hdc, CRect &rcFill, FRG_PARAM params[], int nCount)
{
    GRADIENT_RECT gRect = {0, 1};
    TRIVERTEX vert[2] = {
        {rcFill.left, rcFill.top, 0, 0, 0, 0}, 
        {rcFill.right, rcFill.top, 0, 0, 0, 0}
    };
    int i = 0;

    for (i = 1; i < nCount && vert[0].y <= rcFill.bottom; i ++)
    {
        vert[0].y = vert[1].y;
        vert[1].y += params[i].lOffset;
        vert[0].Red     = GetRValue(params[i - 1].crColor) << 8;
        vert[0].Green   = GetGValue(params[i - 1].crColor) << 8;
        vert[0].Blue    = GetBValue(params[i - 1].crColor) << 8;
        vert[1].Red     = GetRValue(params[i].crColor) << 8;
        vert[1].Green   = GetGValue(params[i].crColor) << 8;
        vert[1].Blue    = GetBValue(params[i].crColor) << 8;

        GradientFill2(hdc, vert, 2, &gRect, 1, GRADIENT_FILL_RECT_V);
    }
}

inline void CBkSkinBase::GradientFillRectH(HDC hdc, CRect &rcFill, FRG_PARAM params[], int nCount)
{
    GRADIENT_RECT gRect = {0, 1};
    TRIVERTEX vert[2] = {
        {rcFill.left, rcFill.top, 0, 0, 0, 0}, 
        {rcFill.left, rcFill.bottom, 0, 0, 0, 0}
    };
    int i = 0;

    for (i = 1; i < nCount && vert[0].x <= rcFill.right; i ++)
    {
        vert[0].x = vert[1].x;
        vert[1].x += params[i].lOffset;
        vert[0].Red     = GetRValue(params[i - 1].crColor) << 8;
        vert[0].Green   = GetGValue(params[i - 1].crColor) << 8;
        vert[0].Blue    = GetBValue(params[i - 1].crColor) << 8;
        vert[1].Red     = GetRValue(params[i].crColor) << 8;
        vert[1].Green   = GetGValue(params[i].crColor) << 8;
        vert[1].Blue    = GetBValue(params[i].crColor) << 8;
        GradientFill2(hdc, vert, 2, &gRect, 1, GRADIENT_FILL_RECT_H);
    }
}

inline void CBkSkinBase::GradientFillRectV(HDC hdc, CRect &rcFill, COLORREF crTop, COLORREF crBottom)
{
    FRG_PARAM frgDraw[2] =
    {
        {0, crTop}, 
        {rcFill.Height(), crBottom}, 
    };
    GradientFillRectV(hdc, rcFill, frgDraw, 2);
}

inline void CBkSkinBase::GradientFillRectH(HDC hdc, CRect &rcFill, COLORREF crLeft, COLORREF crRight)
{
    FRG_PARAM frgDraw[2] =
    {
        {0, crLeft}, 
        {rcFill.Width(), crRight}, 
    };
    GradientFillRectH(hdc, rcFill, frgDraw, 2);
}


//////////////////////////////////////////////////////////////////////////
// CBkImageSkin
inline CBkImageSkin::CBkImageSkin()
{
    m_pImg = NULL;
    m_lSubImageWidth = 0;
}

inline void CBkImageSkin::Draw(CDCHandle dc, CRect rcDraw, DWORD dwState)
{
    if (m_pImg != NULL)
    {
        Gdiplus::Graphics graphics(dc);
        SIZE size = GetSkinSize();
        DrawImage(graphics, *m_pImg, rcDraw.left, rcDraw.top, size.cx, size.cy, m_lSubImageWidth * dwState, 0, size.cx, size.cy);
    }
}

inline SIZE CBkImageSkin::GetSkinSize()
{
    SIZE ret = {0, 0};
    if (m_pImg != NULL)
    {
        if (0 == m_lSubImageWidth)
            ret.cx = m_pImg->GetWidth();
        else
            ret.cx = m_lSubImageWidth;
        ret.cy = m_pImg->GetHeight();
    }
    return ret;
}

inline BOOL CBkImageSkin::IgnoreState()
{
    return (0 == m_lSubImageWidth);
}

//////////////////////////////////////////////////////////////////////////
// CBkProgressSkin
inline CBkProgressSkin::CBkProgressSkin()
{
    m_pImg = NULL;
    m_fixedSize = -1;
    m_drawType = ProgressType_binding;
}

inline void CBkProgressSkin::Draw(CDCHandle dc, CRect rcDraw, DWORD dwState)
{
    int iProgressWidth = dwState;
    if (m_pImg)
    {
        Gdiplus::Graphics graphics(dc);
        switch( m_drawType )
        {
        case ProgressType_binding:
            if ( rcDraw.Width() < m_fixedSize )
            {
                DrawImage( graphics, *m_pImg, rcDraw.left, rcDraw.top, rcDraw.Width(), m_pImg->GetHeight(), 0, 0, rcDraw.Width(), m_pImg->GetHeight());
            }
            else
            {
                DrawImage( graphics, *m_pImg, rcDraw.left, rcDraw.top, m_fixedSize, m_pImg->GetHeight(), 0, 0, m_fixedSize, m_pImg->GetHeight());
                if (0 == dwState)//0的意思是不画皮肤里面最右边的那一块
                {
                    DrawStretch( graphics, *m_pImg, rcDraw.left+m_fixedSize, rcDraw.top, rcDraw.Width()-m_fixedSize, m_pImg->GetHeight(), 
                        m_fixedSize, 0, m_pImg->GetWidth()-2*m_fixedSize, m_pImg->GetHeight());
                }
                else
                {
                    DrawImage( graphics, *m_pImg, rcDraw.right-m_fixedSize, rcDraw.top, m_fixedSize, m_pImg->GetHeight(), 
                        m_pImg->GetWidth() - m_fixedSize, 0, m_fixedSize, m_pImg->GetHeight());

                    DrawStretch( graphics, *m_pImg, rcDraw.left+m_fixedSize, rcDraw.top, rcDraw.Width()-m_fixedSize*2, m_pImg->GetHeight(), 
                        m_fixedSize, 0, m_pImg->GetWidth()-m_fixedSize*2, m_pImg->GetHeight());
                }
            }   
            break;
        case ProgressType_ring:
            if( 0 == (dwState%360) && 0 != dwState )
            {
                int w = m_pImg->GetWidth();
                int h = m_pImg->GetHeight();
                DrawImage( graphics, *m_pImg, rcDraw.left, rcDraw.top, m_pImg->GetWidth(), m_pImg->GetHeight(), 0, 0, m_pImg->GetWidth(), m_pImg->GetHeight() );
            }
            else if( 0 != dwState )
            {
                Gdiplus::GraphicsPath path;
                path.AddPie( rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height(), -90.0f, (REAL)dwState );
                Gdiplus::Region rgn;
                graphics.GetClip( &rgn );
                graphics.SetClip( &path );
                DrawImage( graphics, *m_pImg, rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height(), 0, 0, m_pImg->GetWidth(), m_pImg->GetHeight() );
                graphics.SetClip( &rgn );
            }
            break;
        }

    }
}

//////////////////////////////////////////////////////////////////////////
// CBkSkinBgBorder
inline CBkSkinBgBorder::CBkSkinBgBorder()
{
    m_pImgTL = NULL;
    m_pImgTR = NULL;
    m_pImgBL = NULL;
    m_pImgBR = NULL;
    m_crBackground = CLR_INVALID;
    m_crBorder = CLR_INVALID;
    m_nBorder = 1;
    m_nAlpha = 254;
}

inline void CBkSkinBgBorder::Draw(CDCHandle dc, CRect rcDraw, DWORD dwState)
{
    Gdiplus::Graphics g(dc.m_hDC);
    if (m_crBackground != CLR_INVALID)
    {
        //FillRectangle(g, m_crBackground, rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height());

        Gdiplus::SolidBrush b( Color(m_nAlpha, GetRValue(m_crBackground), GetGValue(m_crBackground), GetBValue(m_crBackground) ));
        g.FillRectangle( &b, rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height() );
    }
    if (m_nBorder > 0 && m_crBorder != CLR_INVALID)
    {
        Gdiplus::Color _cr(GetRValue(m_crBorder), GetGValue(m_crBorder), GetBValue(m_crBorder));
        Gdiplus::Pen _pen(_cr, (REAL)m_nBorder);
        g.DrawRectangle(&_pen, rcDraw.left, rcDraw.top, rcDraw.Width() - m_nBorder, rcDraw.Height() - m_nBorder);
    }
    CSize size;
    if (m_pImgTL != NULL)
    {
        size.cx = m_pImgTL->GetWidth();
        size.cy = m_pImgTL->GetHeight();
        DrawImage(g, *m_pImgTL, rcDraw.left, rcDraw.top, size.cx, size.cy);
    }
    if (m_pImgTR != NULL)
    {
        size.cx = m_pImgTR->GetWidth();
        size.cy = m_pImgTR->GetHeight();
        DrawImage(g, *m_pImgTR, rcDraw.right - size.cx, rcDraw.top, size.cx, size.cy);
    }
    if (m_pImgBL  != NULL)
    {
        size.cx = m_pImgBL->GetWidth();
        size.cy = m_pImgBL->GetHeight();
        DrawImage(g, *m_pImgBL, rcDraw.left, rcDraw.bottom - size.cy, size.cx, size.cy);
    }
    if (m_pImgBR  != NULL)
    {
        size.cx = m_pImgBR->GetWidth();
        size.cy = m_pImgBR->GetHeight();
        DrawImage(g, *m_pImgBR, rcDraw.right - size.cx, rcDraw.bottom - size.cy, size.cx, size.cy);
    }
}


//////////////////////////////////////////////////////////////////////////
// CBkSkinBackground

inline CBkSkinBackground::CBkSkinBackground()
{
    m_pImgT = NULL;
    m_pImgTL = NULL;
    m_pImgTR = NULL;
    m_pImgB = NULL;
    m_pImgBL = NULL;
    m_pImgBR = NULL;
    m_pImgC = NULL;
    m_pImgL = NULL;
    m_pImgR = NULL;
    m_pImgOverlay = NULL;
    m_topWrapMode = Stretch;
    m_bottomWrapMode = Stretch;
    m_leftWrapMode = Stretch;
    m_rightWrapMode = Stretch;
    m_centerWrapMode = Tile;
    m_crCenter = CLR_INVALID;
}

inline void CBkSkinBackground::Draw(CDCHandle dc, CRect rcDraw, DWORD dwState)
{
    Gdiplus::Graphics g(dc.m_hDC);

    CSize sizeTL,sizeT,sizeTR,sizeBL,sizeB,sizeBR,sizeL,sizeR;

    CRgn rgnOldClip;
    Region rgOldClip;
    Rect tagRect( rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height() );
    g.GetClip( &rgOldClip );
    Region rgClip(tagRect);
    rgClip.Intersect( &rgOldClip );
    g.SetClip( &rgClip );

    if( m_pImgTL )
    {
        sizeTL.cx = m_pImgTL->GetWidth();
        sizeTL.cy = m_pImgTL->GetHeight();
        DrawImage( g, *m_pImgTL, rcDraw.left, rcDraw.top, sizeTL.cx, sizeTL.cy );
    }
    if( m_pImgTR )
    {
        sizeTR.cx = m_pImgTR->GetWidth();
        sizeTR.cy = m_pImgTR->GetHeight();
        DrawImage( g, *m_pImgTR, rcDraw.right-sizeTR.cx, rcDraw.top, sizeTR.cx, sizeTR.cy );
    }
    if( m_pImgT )
    {
        sizeT.cx = rcDraw.Width() - sizeTL.cx - sizeTR.cx;
        sizeT.cy = m_pImgT->GetHeight();
        switch( m_topWrapMode )
        {
        case Clamp:
            DrawImage( g, *m_pImgT, rcDraw.left+sizeTL.cx, rcDraw.top, sizeT.cx, sizeT.cy );
            break;
        case Stretch:
            DrawStretch( g, *m_pImgT, rcDraw.left+sizeTL.cx, rcDraw.top, sizeT.cx, sizeT.cy );
            break;
        case Tile:
        case StretchTileX:
        case StretchTileY:
            DrawTile( g, *m_pImgT, rcDraw.left+sizeTL.cx, rcDraw.top, sizeT.cx, sizeT.cy, 0, 0, m_topWrapMode );
            break;
        }
    }

    if( m_pImgBL )
    {
        sizeBL.cx = m_pImgBL->GetWidth();
        sizeBL.cy = m_pImgBL->GetHeight();
        DrawImage( g, *m_pImgBL, rcDraw.left, rcDraw.bottom-sizeBL.cy, sizeBL.cx, sizeBL.cy );
    }
    if( m_pImgBR )
    {
        sizeBR.cx = m_pImgBR->GetWidth();
        sizeBR.cy = m_pImgBR->GetHeight();
        DrawImage( g, *m_pImgBR, rcDraw.right-sizeBR.cx, rcDraw.bottom-sizeBR.cy, sizeBR.cx, sizeBR.cy );
    }
    if( m_pImgB )
    {
        sizeB.cx = rcDraw.Width() - sizeBL.cx - sizeBR.cx;
        sizeB.cy = m_pImgB->GetHeight();
        switch( m_bottomWrapMode )
        {
        case Clamp:
            DrawImage( g, *m_pImgB, rcDraw.left+sizeBL.cx, rcDraw.bottom-sizeB.cy, sizeB.cx, sizeB.cy );
            break;
        case Stretch:
            DrawStretch( g, *m_pImgB, rcDraw.left+sizeBL.cx, rcDraw.bottom-sizeB.cy, sizeB.cx, sizeB.cy );
            break;
        case Tile:
        case StretchTileX:
        case StretchTileY:
            DrawTile( g, *m_pImgB, rcDraw.left+sizeBL.cx, rcDraw.bottom-sizeB.cy, sizeB.cx, sizeB.cy, 0, 0, m_bottomWrapMode );
            break;
        }
    }

    if( m_pImgL )
    {
        sizeL.cx = m_pImgL->GetWidth();
        sizeL.cy = rcDraw.Height() - sizeTL.cy - sizeBL.cy;
        switch( m_leftWrapMode )
        {
        case Clamp:
            DrawImage( g, *m_pImgL, rcDraw.left, rcDraw.top+sizeTL.cy, sizeL.cx, sizeL.cy );
            break;
        case Stretch:
            DrawStretch( g, *m_pImgL, rcDraw.left, rcDraw.top+sizeTL.cy, sizeL.cx, sizeL.cy );
            break;
        case Tile:
        case StretchTileX:
        case StretchTileY:
            DrawTile( g, *m_pImgL, rcDraw.left, rcDraw.top+sizeTL.cy, sizeL.cx, sizeL.cy, 0, 0, m_leftWrapMode );
            break;
        }
    }
    if( m_pImgR )
    {
        sizeR.cx = m_pImgR->GetWidth();
        sizeR.cy = rcDraw.Height() - sizeTR.cy - sizeBR.cy;
        switch( m_rightWrapMode )
        {
        case Clamp:
            DrawImage( g, *m_pImgR, rcDraw.right-sizeR.cx, rcDraw.top+sizeTR.cy, sizeR.cx, sizeR.cy );
            break;
        case Stretch:
            DrawStretch( g, *m_pImgR, rcDraw.right-sizeR.cx, rcDraw.top+sizeTR.cy, sizeR.cx, sizeR.cy );
            break;
        case Tile:
        case StretchTileX:
        case StretchTileY:
            DrawTile( g, *m_pImgR, rcDraw.right-sizeR.cx, rcDraw.top+sizeTR.cy, sizeR.cx, sizeR.cy, 0, 0, m_rightWrapMode );
            break;
        }
    }
    if( m_pImgC )
    {
        switch( m_centerWrapMode )
        {
        case Clamp:
            DrawImage( g, *m_pImgC, rcDraw.left+sizeL.cx, rcDraw.top+sizeT.cy,
                m_pImgC->GetWidth(), m_pImgC->GetHeight() );
            break;
        case Stretch:
            DrawStretch( g, *m_pImgC, rcDraw.left+sizeL.cx, rcDraw.top+sizeT.cy,
                rcDraw.Width()-sizeL.cx-sizeR.cx, rcDraw.Height()-sizeT.cy-sizeB.cy );
            break;
        case Tile:
        case StretchTileX:
        case StretchTileY:
            DrawTile( g, *m_pImgC, rcDraw.left+sizeL.cx, rcDraw.top+sizeT.cy,
                rcDraw.Width()-sizeL.cx-sizeR.cx, rcDraw.Height()-sizeT.cy-sizeB.cy, 0, 0,
                m_centerWrapMode );
            break;
        }
    }
    else if (CLR_INVALID != m_crCenter)
    {
        FillRectangle(g, m_crCenter, rcDraw.left+sizeL.cx, rcDraw.top+sizeT.cy
            , rcDraw.Width()-sizeL.cx-sizeR.cx, rcDraw.Height()-sizeT.cy-sizeB.cy);
    }
    if (m_pImgOverlay != NULL)
    {
        DrawImage(g, *m_pImgOverlay, rcDraw.left, rcDraw.top +  rcDraw.Height() - m_pImgOverlay->GetHeight()
            , m_pImgOverlay->GetWidth(), m_pImgOverlay->GetHeight());
    }

    g.SetClip(&rgOldClip);
}


//////////////////////////////////////////////////////////////////////////
// CBkSkinTile
inline CBkSkinTile::CBkSkinTile()
{
    m_pImgC = NULL;
    m_pImgL = NULL;
    m_pImgR = NULL;
    m_centerWrapMode = Tile;
    m_tileWrapMode = Tile;
    m_marginX = 0;
    m_marginY = 0;
}

inline void CBkSkinTile::Draw( CDCHandle dc, CRect rcDraw, DWORD dwState )
{
    Gdiplus::Graphics g(dc.m_hDC);

    CSize sizeL,sizeR,sizeC;

    CRgn rgnOldClip;
    Region rgOldClip;
    Rect tagRect( rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height() );
    g.GetClip( &rgOldClip );
    Region rgClip(tagRect);
    rgClip.Intersect( &rgOldClip );
    g.SetClip( &rgClip );

    if( m_pImgL )
    {
        sizeL.cx = m_pImgL->GetWidth();
        sizeL.cy = m_pImgL->GetHeight();
    }

    if( m_pImgR )
    {
        sizeR.cx = m_pImgR->GetWidth();
        sizeR.cy = m_pImgR->GetHeight();
    }

    if( m_pImgC )
    {
        switch( m_tileWrapMode )
        {
        case Tile:
            sizeC.cx = m_pImgC->GetWidth();
            sizeC.cy = m_pImgC->GetHeight();
            break;
        case StretchTileX:
            sizeC.cx = rcDraw.Width() - sizeL.cx - sizeR.cx;
            sizeC.cy = m_pImgC->GetHeight();
            break;
        case StretchTileY:
            sizeC.cx = m_pImgC->GetWidth();
            sizeC.cy = rcDraw.Height() - sizeL.cy - sizeR.cy;
            break;
        }
    }

    CSize sizeTile;
    switch( m_tileWrapMode )
    {
    case Tile:     
        sizeTile.cx = sizeL.cx + sizeR.cx + sizeC.cx;
        sizeTile.cy = sizeC.cy;
        break;
    case StretchTileX:
        sizeTile.cx = sizeL.cx + sizeR.cx + sizeC.cx;
        sizeTile.cy = sizeC.cy;
        break;
    case StretchTileY:
        sizeTile.cx = sizeC.cx;
        sizeTile.cy = sizeL.cy + sizeR.cy + sizeC.cy;
        break;
    }

    // 绘制TILE
    Bitmap bmpTile( sizeTile.cx, sizeTile.cy, PixelFormat32bppPARGB );
    Graphics* gTile = Graphics::FromImage( &bmpTile );
    if( m_pImgL )
    {
        DrawImage( *gTile, *m_pImgL, 0, 0, sizeL.cx, sizeL.cy );
    }

    if( m_pImgR )
    {
        sizeR.cx = m_pImgR->GetWidth();
        sizeR.cy = m_pImgR->GetHeight();
        DrawImage( *gTile, *m_pImgR, sizeTile.cx-sizeR.cx, 0, sizeR.cx, sizeR.cy );
    }

    if( m_pImgC )
    {
        switch( m_centerWrapMode )
        {
        case Clamp:
            DrawImage( *gTile, *m_pImgC, sizeL.cx, 0, m_pImgC->GetWidth(), m_pImgC->GetHeight() );
            break;
        case Stretch:
            DrawStretch( *gTile, *m_pImgC, sizeL.cx, 0, sizeC.cx, sizeC.cy );
            break;
        case Tile:
        case StretchTileX:
        case StretchTileY:
            DrawTile( *gTile, *m_pImgC, sizeL.cx, 0, sizeC.cx, sizeC.cy, 0, 0, m_centerWrapMode );
            break;
        }
    }
    delete gTile;
    gTile = NULL;

    if (rcDraw.top < 0 && sizeTile.cy > 0)
    {
        rcDraw.top = rcDraw.top % sizeTile.cy;
    }

    Gdiplus::Rect rect( 0, 0, sizeTile.cx, sizeTile.cy );
    Gdiplus::TextureBrush brush( &bmpTile, Gdiplus::WrapModeTile, rect );
    brush.TranslateTransform((REAL)rcDraw.left+m_marginX, (REAL)rcDraw.top+m_marginY, MatrixOrderAppend );
    g.FillRectangle( &brush, rcDraw.left+m_marginX, rcDraw.top+m_marginY, rcDraw.Width()-m_marginX, rcDraw.Height()-m_marginY);

    g.SetClip( &rgOldClip  );
}

//////////////////////////////////////////////////////////////////////////
// CBkSkinButton
inline CBkSkinColor::CBkSkinColor()
{
    m_crBorder =RGB(0x70, 0x70, 0x70);
    m_iBorderWidth = 0;
    m_crBgUpNormal = RGB(0xEE, 0xEE, 0xEE);
    m_crBgUpHover = RGB(0xEE, 0xEE, 0xEE);
    m_crBgUpPush = RGB(0xCE, 0xCE, 0xCE);
    m_crBgUpDisable = RGB(0xff, 0xff, 0xff);
    m_crBgDownNormal = RGB(0xD6, 0xD6, 0xD6);
    m_crBgDownHover = RGB(0xE0, 0xE0, 0xE0);
    m_crBgDownPush = RGB(0xC0, 0xC0, 0xC0);
    m_crBgDownDisable = RGB(0xff, 0xff, 0xff);
    m_alphaUp = 254;
    m_alphaDown = 254;
}

inline void CBkSkinColor::Draw(CDCHandle dc, CRect rcDraw, DWORD dwState)
{
    //CPen penFrame;
    CRect rcBg = rcDraw;

    //GradientFillRectV(
    //    dc, rcBg, 
    //    IIF_STATE5(dwState, m_crBgUpNormal, m_crBgUpHover, m_crBgUpPush, m_crBgUpDisable, m_crBgUpPush), 
    //    IIF_STATE5(dwState, m_crBgDownNormal, m_crBgDownHover, m_crBgDownPush, m_crBgDownDisable, m_crBgDownPush));

    COLORREF clrStart = IIF_STATE5(dwState, m_crBgUpNormal, m_crBgUpHover, m_crBgUpPush, m_crBgUpDisable, m_crBgUpPush);
    COLORREF clrEnd = IIF_STATE5(dwState, m_crBgDownNormal, m_crBgDownHover, m_crBgDownPush, m_crBgDownDisable, m_crBgDownPush);
    Gdiplus::Graphics g(dc.m_hDC);
    Gdiplus::Color color1(m_alphaUp, GetRValue(clrStart), GetGValue(clrStart), GetBValue(clrStart) ); 
    Gdiplus::Color color2(m_alphaDown, GetRValue(clrEnd), GetGValue(clrEnd), GetBValue(clrEnd) );

    Gdiplus::Rect rc(rcBg.left, rcBg.top, rcBg.Width(), rcBg.Height());
    Gdiplus::LinearGradientBrush brush(rc, color1, color2, LinearGradientModeVertical); 
    g.FillRectangle(&brush, rc);

    if (m_iBorderWidth > 0)
    {
        //penFrame.CreatePen(PS_SOLID, 1, m_crBorder);
        //HPEN hpenOld = dc.SelectPen(penFrame);
        //HBRUSH hbshNull = (HBRUSH)::GetStockObject(NULL_BRUSH);
        //HBRUSH hbshOld = dc.SelectBrush(hbshNull);
        //dc.Rectangle(rcDraw);
        //dc.SelectBrush(hbshOld);
        //dc.SelectPen(hpenOld);

        Gdiplus::Color _cr(GetRValue(m_crBorder), GetGValue(m_crBorder), GetBValue(m_crBorder));
        Gdiplus::Pen _pen(_cr, (REAL)m_iBorderWidth);
        g.DrawRectangle(&_pen, rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height());
    }
}

inline BOOL CBkSkinColor::IgnoreState()
{
    return FALSE;
}


//////////////////////////////////////////////////////////////////////////
// CBkSkinGradation
inline CBkSkinGradation::CBkSkinGradation()
{
    m_uDirection = 0;
    m_crFrom = CLR_INVALID;
    m_crTo = CLR_INVALID;
    m_nSize = 0;
}

inline void CBkSkinGradation::Draw(CDCHandle dc, CRect rcDraw, DWORD dwState)
{
    CRect rcGradation = rcDraw;
    CRgn rgnDraw;

    rgnDraw.CreateRectRgn(rcDraw.left, rcDraw.top, rcDraw.right, rcDraw.bottom);

    dc.FillSolidRect(rcDraw, m_crTo);

    int nID = dc.SaveDC();

    dc.SelectClipRgn(rgnDraw);

    if (0 == m_uDirection)
    {
        if (0 < m_nSize)
            rcGradation.right = rcGradation.left + m_nSize;
        GradientFillRectH(dc, rcGradation, m_crFrom, m_crTo);
    }
    else
    {
        if (0 < m_nSize)
            rcGradation.bottom = rcGradation.top + m_nSize;
        GradientFillRectV(dc, rcGradation, m_crFrom, m_crTo);
    }

    dc.RestoreDC(nID);
}


//////////////////////////////////////////////////////////////////////////
// CBkPngSkin
inline CBkPngSkin::CBkPngSkin()
{
    m_lSubImageWidth = 0;
    m_pImg = NULL;
    m_drawMode = Clamp;
    m_fixedSize = 0;
    m_iFixedL = 0;
    m_iFixedR = 0;
    m_iFixedT = 0;
    m_iFixedB = 0;
    m_iAlign = 0;
}

inline void CBkPngSkin::Draw(CDCHandle dc, CRect rcDraw, DWORD dwState)
{
    if (m_pImg)
    {
        int index = dwState;//(-1 == dwState) ? -1 : IIF_STATE5(dwState, 0, 1, 2, 3, 4);
        Gdiplus::Graphics graphics(dc);

        SIZE size = {0, 0};
        if (m_pImg)
        {
            if (0 == m_lSubImageWidth)
                size.cx = m_pImg->GetWidth();
            else
                size.cx = m_lSubImageWidth;
            size.cy = m_pImg->GetHeight();
        }
		int px = rcDraw.left,py = rcDraw.top;
		if(m_iAlign & Top)
		{
			py = rcDraw.top;
		}
		else if(m_iAlign & Bottom)
		{
			py = rcDraw.bottom - size.cy;
		}

		if(m_iAlign & Left)
		{
			px = rcDraw.left;
		}
		else if(m_iAlign & Right)
		{
			px = rcDraw.right - size.cx;
		}

            switch (m_drawMode)
            {
            case Clamp:
                 DrawImage( graphics, *m_pImg, px,py, size.cx, size.cy, m_lSubImageWidth * index, 0, size.cx, size.cy );
                 break;
            case Stretch:
                 DrawStretch( graphics, *m_pImg, px, py, rcDraw.Width(), rcDraw.Height(), m_lSubImageWidth * index, 0, size.cx, size.cy );
                 break;
            case StretchX:
                 if( rcDraw.Width() > 0 )
                 {
                     DrawImage( graphics, *m_pImg, px,py, m_fixedSize, size.cy, m_lSubImageWidth * index, 0, m_fixedSize, size.cy );
                     DrawImage( graphics, *m_pImg, rcDraw.right-m_fixedSize, py, m_fixedSize, size.cy, 
                         m_lSubImageWidth * index + m_lSubImageWidth - m_fixedSize, 0, m_fixedSize, size.cy );
                     DrawStretch( graphics, *m_pImg, px+m_fixedSize, py, rcDraw.Width()-m_fixedSize*2, size.cy, 
                         m_lSubImageWidth * index + m_fixedSize, 0, m_lSubImageWidth-m_fixedSize*2, size.cy );
                 }
                 break;
            case StretchY:
                if( rcDraw.Height() > 0 )
                {
                    DrawImage( graphics, *m_pImg, px,py, size.cx, m_fixedSize, m_lSubImageWidth * index, 0, size.cx, m_fixedSize );
                    DrawImage( graphics, *m_pImg, px, rcDraw.bottom-m_fixedSize, size.cx, m_fixedSize, 
                        m_lSubImageWidth * index, m_pImg->GetHeight()-m_fixedSize, size.cx, m_fixedSize );
                    DrawStretch( graphics, *m_pImg, px, py+m_fixedSize, size.cx, rcDraw.Height()-m_fixedSize*2, 
                        m_lSubImageWidth * index, m_fixedSize, size.cx, m_pImg->GetHeight()-m_fixedSize*2 );
                }
                break;
            }
    }
}

inline SIZE CBkPngSkin::GetSkinSize()
{
    SIZE ret = {0, 0};
    if (m_pImg)
    {
     if (0 == m_lSubImageWidth)
         ret.cx = m_pImg->GetWidth();
     else
         ret.cx = m_lSubImageWidth;
     ret.cy = m_pImg->GetHeight();
    }

    return ret;
}

inline BOOL CBkPngSkin::IgnoreState()
{
    return 0 == m_lSubImageWidth;
}

//////////////////////////////////////////////////////////////////////////
// CBkGifSkin
inline CBkGifSkin::CBkGifSkin()
{
    m_pImg = NULL;
    m_pPropertyItem = NULL;
    m_iFrameCount = 0;
    m_iFrameFirst = 0;
    m_iFrameRepeat = 0;
    m_iDrawMode = GifDraw_Repeat;
}

inline void CBkGifSkin::Draw(CDCHandle dc, CRect rcDraw, DWORD dwState)
{
    if (m_pImg)
    {
        Gdiplus::Graphics graphics(dc);

        SIZE size = {0, 0};
        size.cx = m_pImg->GetWidth();
        size.cy = m_pImg->GetHeight();

        DrawImage( graphics, *m_pImg, rcDraw.left, rcDraw.top, size.cx, size.cy );
    }
}

inline void CBkGifSkin::Init()
{
    if( m_pImg )
    {
        UINT nCount = m_pImg->GetFrameDimensionsCount();
        if( nCount )
        {
            GUID* pDimensionIDs = new GUID[nCount];
            m_pImg->GetFrameDimensionsList(pDimensionIDs, nCount);
            m_iFrameCount = m_pImg->GetFrameCount(&pDimensionIDs[0]);
            int nSize = m_pImg->GetPropertyItemSize(PropertyTagFrameDelay);
            if (nSize > 0)
            {
                m_pPropertyItem = new PropertyItem[nSize];
                m_pImg->GetPropertyItem(PropertyTagFrameDelay, nSize, m_pPropertyItem);
            }
            delete[] pDimensionIDs;
            pDimensionIDs = NULL;
        }
    }
}

inline void CBkGifSkin::Release()
{
    m_iFrameCount = 0;
    if( m_pPropertyItem )
    {
        delete[] m_pPropertyItem;
        m_pPropertyItem = NULL;
    }
}

inline SIZE CBkGifSkin::GetSkinSize()
{
    SIZE ret = {0, 0};
    if (m_pImg)
    {
        ret.cx = m_pImg->GetWidth();
        ret.cy = m_pImg->GetHeight();
    }

    return ret;
}

inline BOOL CBkGifSkin::IgnoreState()
{
    return FALSE;
}

inline void CBkGifSkin::Reset()
{
    if (m_pImg)
    {
        GUID pageGUID = Gdiplus::FrameDimensionTime;
        m_pImg->SelectActiveFrame( &pageGUID , m_iFrameFirst );
    }
}

inline UINT CBkGifSkin::UpdateFrame( int frame )
{
    if( frame >= 0 && frame <= m_iFrameCount )
    {
        if( frame >= m_iFrameCount )
        {
            switch( m_iDrawMode )
            {
            case GifDraw_Once:
                frame = m_iFrameCount;
                break;
            case GifDraw_Repeat:
                frame = 0;
                break;
            case GifDraw_RepeatFrame:
                frame = m_iFrameRepeat;
                break;
            }
        }
        GUID pageGUID = Gdiplus::FrameDimensionTime;
        m_pImg->SelectActiveFrame( &pageGUID , frame );
        return frame;
    }
    return 0;
}

inline UINT CBkGifSkin::GetFrameFirst()
{
    return m_iFrameFirst;
}

inline int CBkGifSkin::GetFrameCount()
{
    return m_iFrameCount;
}

inline long CBkGifSkin::GetFrameTime( int frame )
{
    if( frame >= 0 && frame < m_iFrameCount )
    {
        if( m_pPropertyItem )
        {
            return (((long*)(m_pPropertyItem->value))[frame]) * 10;
        }
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////////
// CBkSkinButton
inline CBkSkinButton::CBkSkinButton()
{
    m_lSubImageWidth = 0;
    m_pImg = NULL;
    m_drawMode = Clamp;
    for (int i = 0; i < 5; ++i)
    {
        m_drawIndex[i] = i;
    }
}

inline void CBkSkinButton::Draw(CDCHandle dc, CRect rcDraw, DWORD dwState)
{
    if (m_pImg)
    {
        int index = (-1 == dwState) ? -1 : IIF_STATE5(dwState, 0, 1, 2, 3, 4);
        if (index < 0/* || index > 5*/)
        {
            return;
        }

        index = m_drawIndex[index];
        if (index < 0/* || index > 5*/)
        {
            return;
        }

        Gdiplus::Graphics graphics(dc);

        SIZE size = {0, 0};
        if (m_pImg)
        {
            if (0 == m_lSubImageWidth)
                size.cx = m_pImg->GetWidth();
            else
                size.cx = m_lSubImageWidth;
            size.cy = m_pImg->GetHeight();
        }

        switch (m_drawMode)
        {
        case Clamp:
            DrawImage( graphics, *m_pImg, rcDraw.left, rcDraw.top, size.cx, size.cy, m_lSubImageWidth * index, 0, size.cx, size.cy );
            break;
        case Stretch:
            DrawStretch( graphics, *m_pImg, rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height(), m_lSubImageWidth * index, 0, size.cx, size.cy );
            break;
        case StretchX:
            if( rcDraw.Width() > 0 )
            {
                DrawImage( graphics, *m_pImg, rcDraw.left, rcDraw.top, m_fixedSize, size.cy, m_lSubImageWidth * index, 0, m_fixedSize, size.cy );
                DrawImage( graphics, *m_pImg, rcDraw.right-m_fixedSize, rcDraw.top, m_fixedSize, size.cy, 
                    m_lSubImageWidth * index + m_lSubImageWidth - m_fixedSize, 0, m_fixedSize, size.cy );
                DrawStretch( graphics, *m_pImg, rcDraw.left+m_fixedSize, rcDraw.top, rcDraw.Width()-m_fixedSize*2, size.cy, 
                    m_lSubImageWidth * index + m_fixedSize, 0, m_lSubImageWidth-m_fixedSize*2, size.cy );
            }
            break;
        case StretchY:
            if( rcDraw.Height() > 0 )
            {
                DrawImage( graphics, *m_pImg, rcDraw.left, rcDraw.top, size.cx, m_fixedSize, m_lSubImageWidth * index, 0, size.cx, m_fixedSize );
                DrawImage( graphics, *m_pImg, rcDraw.left, rcDraw.bottom-m_fixedSize, size.cx, m_fixedSize, 
                    m_lSubImageWidth * index, m_pImg->GetHeight()-m_fixedSize, size.cx, m_fixedSize );
                DrawStretch( graphics, *m_pImg, rcDraw.left, rcDraw.top+m_fixedSize, size.cx, rcDraw.Height()-m_fixedSize*2, 
                    m_lSubImageWidth * index, m_fixedSize, size.cx, m_pImg->GetHeight()-m_fixedSize*2 );
            }
            break;
        }
    }
}

inline SIZE CBkSkinButton::GetSkinSize()
{
    SIZE ret = {0, 0};
    if (m_pImg)
    {
        if (0 == m_lSubImageWidth)
            ret.cx = m_pImg->GetWidth();
        else
            ret.cx = m_lSubImageWidth;
        ret.cy = m_pImg->GetHeight();
    }

    return ret;
}

inline BOOL CBkSkinButton::IgnoreState()
{
    return 0 == m_lSubImageWidth;
}


//////////////////////////////////////////////////////////////////////////
// CBkSkinEdit
inline CBkSkinEdit::CBkSkinEdit()
{
    m_crBg = RGB(0xFF, 0xFF, 0xFF);
    m_crBgHover = RGB(0xFF, 0xFF, 0xFF);
    m_crBgReadonly = RGB(0xF0, 0xF0, 0xF0);
    m_crBgDisable = RGB(0xF0, 0xF0, 0xF0);
    m_crBgSelect = RGB(0xFF, 0xFF, 0xFF);
    m_crBorder = RGB(0xA4, 0xC2, 0xD4);
    m_crBorderHover = RGB(0xA4, 0xC2, 0xD4);
    m_crBorderReadonly = RGB(0xA4, 0xC2, 0xD4);
    m_crBorderDisable = RGB(0xA4, 0xC2, 0xD4);
    m_crBorderShadow = RGB(0x60, 0xC8, 0xFD);
    m_crText = RGB(0x00, 0x00, 0x00);
    m_crTextHover = RGB(0x00, 0x00, 0x00);
    m_crTextReadonly = RGB(0x00, 0x00, 0x00);
    m_crTextDisable = RGB(0xAC, 0xA8, 0x99);
    m_crTextDefault = RGB(0x00, 0x00, 0x00);
}

inline void CBkSkinEdit::Draw(CDCHandle dc, CRect rcDraw, DWORD dwState)
{
}

inline BOOL CBkSkinEdit::IgnoreState()
{
    return FALSE;
}

inline void CBkSkinEdit::GetColors( COLORREF &clrBack,COLORREF &clrBorder,COLORREF &clrText,DWORD dwState )
{
    clrBack = IIF_STATE5(dwState,m_crBg,m_crBgHover,m_crBgReadonly,m_crBgDisable,m_crBgSelect);
    clrBorder = IIF_STATE5(dwState,m_crBorder,m_crBorderHover,m_crBorderReadonly,m_crBorderDisable,m_crBorderShadow);
    clrText = IIF_STATE5(dwState,m_crText,m_crTextHover,m_crTextReadonly,m_crTextDisable,m_crTextDefault);
}


//////////////////////////////////////////////////////////////////////////
// CBkSkinScroll
inline CBkSkinScroll::CBkSkinScroll()
{
    m_pImgBackground = NULL;
    m_pImgThumb = NULL;
    m_pImgThumb2 = NULL;
    m_pImgArrow1 = NULL;
    m_pImgArrow2 = NULL;
    m_Type = STE_NONE;
    m_bgFixedSize = 0;
    m_thumbMinSize = 0;
}

inline void CBkSkinScroll::Draw(CDCHandle dc, CRect rcDraw, DWORD dwState)
{

}

inline int CBkSkinScroll::GetWidth()
{
    int iWidth = -1;
    if (m_pImgBackground != NULL)
    {
        iWidth = m_pImgBackground->GetWidth();
    }
    return iWidth;
}

inline int CBkSkinScroll::GetHeight()
{
    int iHeight = -1;
    if (m_pImgBackground != NULL)
    {
        iHeight = m_pImgBackground->GetHeight();
    }
    return iHeight;
}

inline int  CBkSkinScroll::GetThumbMinSize()
{
    return m_thumbMinSize;
}

inline BOOL CBkSkinScroll::HasArrow()
{
    return (m_pImgArrow1 != NULL && m_pImgArrow2 != NULL);
}

inline void CBkSkinScroll::DrawBackground(CDCHandle dc, CRect rcDraw)
{
    if (m_pImgBackground != NULL)
    {
        Gdiplus::Graphics graphics(dc);
        if( STE_NONE == m_Type )
        {
            DrawStretch(graphics, *m_pImgBackground, rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height());
        }
        if( STE_V == m_Type )
        {
            DrawImage( graphics, *m_pImgBackground, rcDraw.left, rcDraw.top, GetWidth(), m_bgFixedSize, 0, 0, GetWidth(), m_bgFixedSize );
            DrawImage( graphics, *m_pImgBackground, rcDraw.left, rcDraw.bottom-m_bgFixedSize, GetWidth(), m_bgFixedSize, 
                       0, GetHeight()-m_bgFixedSize, GetWidth(), m_bgFixedSize );
            DrawStretch( graphics, *m_pImgBackground, rcDraw.left, rcDraw.top+m_bgFixedSize, GetWidth(), rcDraw.Height()-m_bgFixedSize*2, 
                         0, m_bgFixedSize, GetWidth(), GetHeight()-m_bgFixedSize*2 );
        }
        else if( STE_H == m_Type )
        {
            DrawImage( graphics, *m_pImgBackground, rcDraw.left, rcDraw.top, m_bgFixedSize, GetHeight(), 0, 0, m_bgFixedSize, GetHeight() );
            DrawImage( graphics, *m_pImgBackground, rcDraw.right-m_bgFixedSize, rcDraw.top, m_bgFixedSize, GetHeight(), 
                       GetWidth()-m_bgFixedSize, 0, m_bgFixedSize, GetHeight() );
            DrawStretch( graphics, *m_pImgBackground, rcDraw.left+m_bgFixedSize, rcDraw.top, GetWidth()-m_bgFixedSize*2, GetHeight(),
                         GetWidth()-m_bgFixedSize, 0, GetWidth()-m_bgFixedSize*2, GetHeight() );
        }
    }
}

inline void CBkSkinScroll::DrawThumb(CDCHandle dc, CRect rcDraw, DWORD dwState, BOOL bIsH)
{
    if (m_pImgThumb != NULL)
    {
        int index = (-1 == dwState) ? -1 : IIF_STATE5(dwState, 0, 1, 2, 3, 4);
        Gdiplus::Graphics graphics(dc);
        int iSubWidth = m_pImgThumb->GetWidth() / 4;
        int iHeight = m_pImgThumb->GetHeight();

        if (bIsH)
        {
            DrawStretchXY(graphics, rcDraw, *m_pImgThumb, index, iSubWidth, iSubWidth / 2);
            if (m_pImgThumb2 != NULL)
            {
                int iW2 = m_pImgThumb2->GetWidth();
                DrawStretch(graphics, *m_pImgThumb2, rcDraw.left + rcDraw.Width() / 2 - iW2 / 2, rcDraw.top, iW2, rcDraw.Height());
            }
        }
        else
        {
            DrawStretchXY(graphics, rcDraw, *m_pImgThumb, index, iSubWidth, iHeight / 2);
            if (m_pImgThumb2 != NULL)
            {
                int iH2 = m_pImgThumb2->GetHeight();
                DrawStretch(graphics, *m_pImgThumb2, rcDraw.left, rcDraw.top + rcDraw.Height() / 2 - iH2 / 2, rcDraw.Width(), iH2);
            }
        }
    }
}

inline void CBkSkinScroll::DrawArrow(CDCHandle dc, CRect rcDraw, DWORD dwState, BOOL bIs1)
{
    Gdiplus::Image* pImage = bIs1 ? m_pImgArrow1 : m_pImgArrow2;
    if (pImage != NULL)
    {
        int index = (-1 == dwState) ? -1 : IIF_STATE5(dwState, 0, 1, 2, 3, 4);
        Gdiplus::Graphics graphics(dc);
        int iSubWidth = pImage->GetWidth() / 4;
        int iHeight = pImage->GetHeight();
        DrawStretchXY(graphics, rcDraw, *pImage, index, iSubWidth, iSubWidth / 2);
    }
}


//////////////////////////////////////////////////////////////////////////
// BkSkin
inline BkSkin::~BkSkin()
{
    _Clear();
}

inline BOOL BkSkin::LoadSkins(LPCTSTR szRes)
{
    _Instance()->_Clear();

    CStringA strXml;
    BOOL bRet = BkResManager::LoadResource(szRes, strXml);
    if (!bRet)
        return FALSE;

    TiXmlDocument xmlDoc;
    xmlDoc.Parse(strXml, NULL, TIXML_ENCODING_UTF8);
    if (xmlDoc.Error())
        return FALSE;

    _Instance()->_LoadSkins(xmlDoc.RootElement());
    return TRUE;
}

inline CBkSkinBase* BkSkin::GetSkin(LPCSTR lpszSkinName)
{
    __BkSkinPool::CPair *pairRet = _Instance()->m_mapPool.Lookup(lpszSkinName);

    if (pairRet)
        return pairRet->m_value;
    else
        return NULL;
}

inline BkSkin* BkSkin::_Instance()
{
	static BkSkin skin;
    return &skin;
}

inline void BkSkin::_Clear()
{
    POSITION pos = m_mapPool.GetStartPosition();

    while (pos)
    {
        CBkSkinBase *pSkin = m_mapPool.GetNextValue(pos);
        pSkin->Release();
        delete pSkin;
    }

    m_mapPool.RemoveAll();
}

inline void BkSkin::_LoadSkins(TiXmlElement *pXmlSkinRootElem)
{
    LPCSTR lpszSkinName = NULL, lpszTypeName = NULL;

    if (!pXmlSkinRootElem)
        return;

    if (strcmp(pXmlSkinRootElem->Value(), "skins") != 0)
        return;

    for (TiXmlElement* pXmlChild = pXmlSkinRootElem->FirstChildElement(); NULL != pXmlChild; pXmlChild = pXmlChild->NextSiblingElement())
    {
        lpszSkinName = pXmlChild->Attribute("name");
        lpszTypeName = pXmlChild->Value();
        if (!lpszSkinName || !lpszTypeName)
            continue;

        CBkSkinBase *pSkin = _CreateBkSkinByName(lpszTypeName);
        if (!pSkin)
            continue;

        pSkin->Load(pXmlChild);
        pSkin->Init();
        m_mapPool[lpszSkinName] = pSkin;
    }
}

inline CBkSkinBase* BkSkin::_CreateBkSkinByName(LPCSTR lpszName)
{
    CBkSkinBase *pNewSkin = NULL;

    pNewSkin = CBkImageSkin::CheckAndNew(lpszName);
    if (pNewSkin)
        return pNewSkin;

    pNewSkin = CBkSkinColor::CheckAndNew(lpszName);
    if (pNewSkin)
        return pNewSkin;

    pNewSkin = CBkSkinGradation::CheckAndNew(lpszName);
    if (pNewSkin)
        return pNewSkin;

    pNewSkin = CBkPngSkin::CheckAndNew(lpszName);
    if (pNewSkin)
        return pNewSkin;

    pNewSkin = CBkGifSkin::CheckAndNew(lpszName);
    if (pNewSkin)
        return pNewSkin;

    pNewSkin = CBkSkinButton::CheckAndNew(lpszName);
    if (pNewSkin)
        return pNewSkin;

    pNewSkin = CBkSkinEdit::CheckAndNew(lpszName);
    if (pNewSkin)
        return pNewSkin;

    pNewSkin = CBkSkinBackground::CheckAndNew(lpszName);
    if (pNewSkin)
        return pNewSkin;

    pNewSkin = CBkSkinTile::CheckAndNew(lpszName);
    if (pNewSkin)
        return pNewSkin;

    pNewSkin = CBkSkinBgBorder::CheckAndNew(lpszName);
    if (pNewSkin)
        return pNewSkin;

    pNewSkin = CBkProgressSkin::CheckAndNew(lpszName);
    if (pNewSkin)
        return pNewSkin;

    pNewSkin = CBkSkinScroll::CheckAndNew(lpszName);
    if (pNewSkin)
        return pNewSkin;

    return NULL;
}
