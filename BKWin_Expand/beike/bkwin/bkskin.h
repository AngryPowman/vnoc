//////////////////////////////////////////////////////////////////////////
//   File Name: BkSkin
// Description: BkWindow Skin Definition
//     Creator: ZhangXiaoxuan
//     Version: 2009.4.22 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "atlbase.h"
#include "atlapp.h"
#include "atlgdi.h"
#include "atlmisc.h"
#include "bkobject.h"
#include <bkres/bkpngpool.h>

using namespace Gdiplus;

// State Define
enum
{
    BkWndState_Normal       = 0x00000000UL, 
    BkWndState_Hover        = 0x00000001UL, 
    BkWndState_PushDown     = 0x00000002UL, 
    BkWndState_Check        = 0x00000004UL, 
    BkWndState_Accept       = 0x00000004UL, 
    BkWndState_Invisible    = 0x00000008UL, 
    BkWndState_Disable      = 0x00000010UL, 
};

#define IIF_STATE1(the_state, the_value)   \
    the_value == (the_state & the_value)

#define IIF_STATE2(the_state, normal_value, hover_value) \
    ((BkWndState_Hover == ((the_state) & BkWndState_Hover)) ? (hover_value) : (normal_value))

#define IIF_STATE3(the_state, normal_value, hover_value, pushdown_value) \
    ((BkWndState_PushDown == ((the_state) & BkWndState_PushDown)) ? (pushdown_value) : IIF_STATE2(the_state, normal_value, hover_value))

#define IIF_STATE4(the_state, normal_value, hover_value, pushdown_value, disable_value) \
    ((BkWndState_Disable == ((the_state) & BkWndState_Disable)) ? (disable_value) : IIF_STATE3(the_state, normal_value, hover_value, pushdown_value))

#define IIF_STATE5(the_state, normal_value, hover_value, pushdown_value, disable_value, check_value) \
	((BkWndState_Check == ((the_state) & BkWndState_Check)) ? (check_value) : IIF_STATE4(the_state, normal_value, hover_value, pushdown_value, disable_value))


//////////////////////////////////////////////////////////////////////////
// CBkSkinBase
class CBkSkinBase : public CBkObject
{
public:
    virtual void Draw(CDCHandle dc, CRect rcDraw, DWORD dwState) = 0;
    virtual void Init();
    virtual void Release();
    virtual SIZE GetSkinSize();
    virtual BOOL IgnoreState();

    enum
    {
        Tile = 0,               // 平铺
        Clamp,                  // 固定大小绘制
        Stretch,                // 拉伸
        StretchTileX,           // 先水平拉伸再平铺
        StretchTileY,           // 先竖直拉伸再平铺
        StretchX,               // 水平拉伸
        StretchY,               // 竖直拉伸
    };

    enum
    {
        Top     = 0x1,          // 从上开始
        Bottom  = 0x2,          // 从左开始
        Left    = 0x4,          // 从下开始
        Right   = 0x8,          // 从右开始
    };

    enum
    {
        TileClamp = 0,          // TILE原始大小
        TileX,                  // TILE水平填满
        TileY,                  // TILE竖直填满
    };

    int  GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
    void SaveImage2File(Gdiplus::Image &image, const CString& filePath, const CString format);
    void FillRectangle(Gdiplus::Graphics &g, COLORREF cr, int x, int y, int width, int height);
    void DrawImage(Gdiplus::Graphics &g, Gdiplus::Image &bmp, int x, int y, int w, int h);
    void DrawImage(Gdiplus::Graphics &g, Gdiplus::Image &bmp, int des_x, int des_y, int des_w, int des_h, int src_x, int src_y, int src_w, int src_h);
    void DrawImage(Gdiplus::Graphics& g, int desX, int desY, int desW, int desH, Gdiplus::Image& srcImg, int srcX, int srcY, int srcW, int srcH);
    void DrawStretchXY(Gdiplus::Graphics& g, CRect rcDes, Gdiplus::Image& img, int nSrcIndex, int nSrcSubWidth, int nSrcFixSize);
    void DrawStretch(Gdiplus::Graphics &g, Gdiplus::Image &bmp, int x, int y, int w, int h);
    void DrawStretch(Gdiplus::Graphics &g, Gdiplus::Image &bmp, int des_x, int des_y, int des_w, int des_h, int src_x, int src_y, int src_w, int src_h);
    void DrawTile(Gdiplus::Graphics &g, Gdiplus::Image &bmp, int x, int y, int w, int h, int margin_x, int margin_y, int tileMode);
    void AlphaImage(Gdiplus::Graphics& g, int desX, int desY, int desW, int desH, Gdiplus::Image& srcImg, int srcX, int srcY, int srcW, int srcH, REAL alpha);
    void AlphaImage(Gdiplus::Graphics& g, int desX, int desY, int desW, int desH, Gdiplus::Image& srcImg, REAL alpha);

    typedef struct _FRG_PARAM 
    {
        LONG lOffset;
        COLORREF crColor;
    } FRG_PARAM;
    void GradientFillRectV(HDC hdc, CRect &rcFill, FRG_PARAM params[], int nCount);
    void GradientFillRectH(HDC hdc, CRect &rcFill, FRG_PARAM params[], int nCount);
    void GradientFillRectV(HDC hdc, CRect &rcFill, COLORREF crTop, COLORREF crBottom);
    void GradientFillRectH(HDC hdc, CRect &rcFill, COLORREF crLeft, COLORREF crRight);
};


//////////////////////////////////////////////////////////////////////////
// CBkImageSkin
class CBkImageSkin : public CBkSkinBase
{
    BKOBJ_DECLARE_CLASS_NAME(CBkImageSkin, "imglst")
 
public:
    CBkImageSkin();
    virtual void Draw(CDCHandle dc, CRect rcDraw, DWORD dwState);
    virtual SIZE GetSkinSize();
    virtual BOOL IgnoreState();

protected:
    Gdiplus::Image* m_pImg;
    LONG            m_lSubImageWidth;

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_PNG_ATTRIBUTE("src", m_pImg, FALSE)
        BKWIN_INT_ATTRIBUTE("subwidth", m_lSubImageWidth, FALSE)
    BKWIN_DECLARE_ATTRIBUTES_END()
};

//////////////////////////////////////////////////////////////////////////
// CBkProgressSkin
class CBkProgressSkin : public CBkSkinBase
{
    BKOBJ_DECLARE_CLASS_NAME(CBkProgressSkin, "progress")

    enum
    {
        ProgressType_binding = 1,
        ProgressType_ring,
    };

public:
    CBkProgressSkin();
    virtual void Draw(CDCHandle dc, CRect rcDraw, DWORD dwState);

protected:
    Gdiplus::Image*     m_pImg;
    int                 m_fixedSize;
    int                 m_drawType;

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_PNG_ATTRIBUTE("src", m_pImg, FALSE)
        BKWIN_INT_ATTRIBUTE("fixed_size", m_fixedSize, FALSE)
        BKWIN_ENUM_ATTRIBUTE("type", int, FALSE)
            BKWIN_ENUM_VALUE("binding", ProgressType_binding)             // 条状
            BKWIN_ENUM_VALUE("ring", ProgressType_ring)                   // 环状
        BKWIN_ENUM_END(m_drawType)
    BKWIN_DECLARE_ATTRIBUTES_END()
};

//////////////////////////////////////////////////////////////////////////
// CBkSkinBgBorder
class CBkSkinBgBorder : public CBkSkinBase
{
    BKOBJ_DECLARE_CLASS_NAME(CBkSkinBgBorder, "bgborder")
   
public:
    CBkSkinBgBorder();
    virtual void Draw(CDCHandle dc, CRect rcDraw, DWORD dwState);

protected:
    Gdiplus::Image* m_pImgTL;
    Gdiplus::Image* m_pImgTR;
    Gdiplus::Image* m_pImgBL;
    Gdiplus::Image* m_pImgBR;
    COLORREF        m_crBackground;
    COLORREF        m_crBorder;
    int             m_nBorder;
    UINT            m_nAlpha;

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_PNG_ATTRIBUTE("src_tl", m_pImgTL, FALSE)
        BKWIN_PNG_ATTRIBUTE("src_tr", m_pImgTR, FALSE)
        BKWIN_PNG_ATTRIBUTE("src_bl", m_pImgBL, FALSE)
        BKWIN_PNG_ATTRIBUTE("src_br", m_pImgBR, FALSE)
        BKWIN_COLOR_ATTRIBUTE("cr_background", m_crBackground, FALSE)
        BKWIN_COLOR_ATTRIBUTE("cr_border", m_crBorder, FALSE)
        BKWIN_INT_ATTRIBUTE("border", m_nBorder, FALSE)
        BKWIN_UINT_ATTRIBUTE("alpha", m_nAlpha, FALSE)
    BKWIN_DECLARE_ATTRIBUTES_END()
};


//////////////////////////////////////////////////////////////////////////
// CBkSkinBackground
class CBkSkinBackground : public CBkSkinBase
{
    BKOBJ_DECLARE_CLASS_NAME(CBkSkinBackground, "background")

public:
    CBkSkinBackground();
    virtual void Draw(CDCHandle dc, CRect rcDraw, DWORD dwState);

protected:
    Gdiplus::Image* m_pImgT;
    Gdiplus::Image* m_pImgTL;
    Gdiplus::Image* m_pImgTR;
    Gdiplus::Image* m_pImgB;
    Gdiplus::Image* m_pImgBL;
    Gdiplus::Image* m_pImgBR;
    Gdiplus::Image* m_pImgC;
    Gdiplus::Image* m_pImgL;
    Gdiplus::Image* m_pImgR;
    Gdiplus::Image* m_pImgOverlay;
    int             m_topWrapMode;
    int             m_bottomWrapMode;
    int             m_leftWrapMode;
    int             m_rightWrapMode;
    int             m_centerWrapMode;
    COLORREF        m_crCenter;

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_PNG_ATTRIBUTE("src_t", m_pImgT, FALSE)
        BKWIN_PNG_ATTRIBUTE("src_tl", m_pImgTL, FALSE)
        BKWIN_PNG_ATTRIBUTE("src_tr", m_pImgTR, FALSE)
        BKWIN_PNG_ATTRIBUTE("src_b", m_pImgB, FALSE)
        BKWIN_PNG_ATTRIBUTE("src_bl", m_pImgBL, FALSE)
        BKWIN_PNG_ATTRIBUTE("src_br", m_pImgBR, FALSE)
        BKWIN_PNG_ATTRIBUTE("src_c", m_pImgC, FALSE)
        BKWIN_PNG_ATTRIBUTE("src_l", m_pImgL, FALSE)
        BKWIN_PNG_ATTRIBUTE("src_r", m_pImgR, FALSE)
        BKWIN_ENUM_ATTRIBUTE("top_wrapmode", INT, FALSE)
            BKWIN_ENUM_VALUE("clamp", Clamp)
            BKWIN_ENUM_VALUE("stretch", Stretch)
            BKWIN_ENUM_VALUE("tile", Tile)
            BKWIN_ENUM_VALUE("stretchtilex", StretchTileX)
            BKWIN_ENUM_VALUE("stretchtiley", StretchTileY)
        BKWIN_ENUM_END(m_topWrapMode)
        BKWIN_ENUM_ATTRIBUTE("bottom_wrapmode", INT, FALSE)
            BKWIN_ENUM_VALUE("clamp", Clamp)
            BKWIN_ENUM_VALUE("stretch", Stretch)
            BKWIN_ENUM_VALUE("tile", Tile)
            BKWIN_ENUM_VALUE("stretchtilex", StretchTileX)
            BKWIN_ENUM_VALUE("stretchtiley", StretchTileY)
        BKWIN_ENUM_END(m_bottomWrapMode)
        BKWIN_ENUM_ATTRIBUTE("left_wrapmode", INT, FALSE)
            BKWIN_ENUM_VALUE("clamp", Clamp)
            BKWIN_ENUM_VALUE("stretch", Stretch)
            BKWIN_ENUM_VALUE("tile", Tile)
            BKWIN_ENUM_VALUE("stretchtilex", StretchTileX)
            BKWIN_ENUM_VALUE("stretchtiley", StretchTileY)
        BKWIN_ENUM_END(m_leftWrapMode)
        BKWIN_ENUM_ATTRIBUTE("right_wrapmode", INT, FALSE)
            BKWIN_ENUM_VALUE("clamp", Clamp)
            BKWIN_ENUM_VALUE("stretch", Stretch)
            BKWIN_ENUM_VALUE("tile", Tile)
            BKWIN_ENUM_VALUE("stretchtilex", StretchTileX)
            BKWIN_ENUM_VALUE("stretchtiley", StretchTileY)
        BKWIN_ENUM_END(m_rightWrapMode)
        BKWIN_ENUM_ATTRIBUTE("center_wrapmode", INT, FALSE)
            BKWIN_ENUM_VALUE("clamp", Clamp)
            BKWIN_ENUM_VALUE("stretch", Stretch)
            BKWIN_ENUM_VALUE("tile", Tile)
            BKWIN_ENUM_VALUE("stretchtilex", StretchTileX)
            BKWIN_ENUM_VALUE("stretchtiley", StretchTileY)
        BKWIN_ENUM_END(m_centerWrapMode)
        BKWIN_COLOR_ATTRIBUTE("cr_center", m_crCenter, FALSE)
        BKWIN_PNG_ATTRIBUTE("overlay", m_pImgOverlay, FALSE)
    BKWIN_DECLARE_ATTRIBUTES_END()
};


//////////////////////////////////////////////////////////////////////////
// CBkSkinTile
class CBkSkinTile : public CBkSkinBase
{
    BKOBJ_DECLARE_CLASS_NAME(CBkSkinTile, "tile")

public:
    CBkSkinTile();
    virtual void Draw(CDCHandle dc, CRect rcDraw, DWORD dwState);

protected:
    Gdiplus::Image* m_pImgC;
    Gdiplus::Image* m_pImgL;
    Gdiplus::Image* m_pImgR;
    int             m_centerWrapMode;       // TILE的中间段绘制方式，可以拉伸，也可以平铺
    int             m_tileWrapMode;         // TILE的绘制方式
    int             m_marginX;
    int             m_marginY;

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_PNG_ATTRIBUTE("src_c", m_pImgC, TRUE)
        BKWIN_PNG_ATTRIBUTE("src_l", m_pImgL, TRUE)
        BKWIN_PNG_ATTRIBUTE("src_r", m_pImgR, TRUE)
        BKWIN_ENUM_ATTRIBUTE("center_wrapmode", INT, TRUE)
            BKWIN_ENUM_VALUE("clamp", Clamp)
            BKWIN_ENUM_VALUE("stretch", Stretch)
            BKWIN_ENUM_VALUE("tile", Tile)
            BKWIN_ENUM_VALUE("stretchtilex", StretchTileX)
            BKWIN_ENUM_VALUE("stretchtiley", StretchTileY)
        BKWIN_ENUM_END(m_centerWrapMode)
        BKWIN_ENUM_ATTRIBUTE("tile_wrapmode", INT, TRUE)
            BKWIN_ENUM_VALUE("tile", Tile)
            BKWIN_ENUM_VALUE("stretchtilex", StretchTileX)
            BKWIN_ENUM_VALUE("stretchtiley", StretchTileY)
        BKWIN_ENUM_END(m_tileWrapMode)
        BKWIN_INT_ATTRIBUTE("margin_x", m_marginX, TRUE)
        BKWIN_INT_ATTRIBUTE("margin_y", m_marginY, TRUE)
   BKWIN_DECLARE_ATTRIBUTES_END()
};


//////////////////////////////////////////////////////////////////////////
// CBkSkinColor
class CBkSkinColor : public CBkSkinBase
{
    BKOBJ_DECLARE_CLASS_NAME(CBkSkinColor, "color")

public:
    CBkSkinColor();
    virtual void Draw(CDCHandle dc, CRect rcDraw, DWORD dwState);
    virtual BOOL IgnoreState();

protected:
    int         m_iBorderWidth;
    COLORREF    m_crBg;
    COLORREF    m_crBorder;
    COLORREF    m_crBgUpNormal;
    COLORREF    m_crBgUpHover;
    COLORREF    m_crBgUpPush;
    COLORREF    m_crBgUpDisable;
    COLORREF    m_crBgDownNormal;
    COLORREF    m_crBgDownHover;
    COLORREF    m_crBgDownPush;
    COLORREF    m_crBgDownDisable;
    UINT        m_alphaUp;
    UINT        m_alphaDown;

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_INT_ATTRIBUTE("border_width", m_iBorderWidth, FALSE)
        BKWIN_COLOR_ATTRIBUTE("border", m_crBorder, FALSE)
        BKWIN_COLOR_ATTRIBUTE("bgup", m_crBgUpNormal, FALSE)
        BKWIN_COLOR_ATTRIBUTE("bguphover", m_crBgUpHover, FALSE)
        BKWIN_COLOR_ATTRIBUTE("bguppush", m_crBgUpPush, FALSE)
        BKWIN_COLOR_ATTRIBUTE("bgupdisable", m_crBgUpDisable, FALSE)
        BKWIN_COLOR_ATTRIBUTE("bgdown", m_crBgDownNormal, FALSE)
        BKWIN_COLOR_ATTRIBUTE("bgdownhover", m_crBgDownHover, FALSE)
        BKWIN_COLOR_ATTRIBUTE("bgdownpush", m_crBgDownPush, FALSE)
        BKWIN_COLOR_ATTRIBUTE("bgdowndisable", m_crBgDownDisable, FALSE)
        BKWIN_UINT_ATTRIBUTE("alpha-up", m_alphaUp, FALSE)
        BKWIN_UINT_ATTRIBUTE("alpha-down", m_alphaDown, FALSE)
    BKWIN_DECLARE_ATTRIBUTES_END()
};


//////////////////////////////////////////////////////////////////////////
// CBkSkinGradation
class CBkSkinGradation : public CBkSkinBase
{
    BKOBJ_DECLARE_CLASS_NAME(CBkSkinGradation, "gradation")

public:
    CBkSkinGradation();
    virtual void Draw(CDCHandle dc, CRect rcDraw, DWORD dwState);

protected:
    COLORREF m_crFrom;
    COLORREF m_crTo;
    UINT     m_uDirection;
    int      m_nSize; 
    
    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_COLOR_ATTRIBUTE("from", m_crFrom, FALSE)
        BKWIN_COLOR_ATTRIBUTE("to", m_crTo, FALSE)
        BKWIN_INT_ATTRIBUTE("size", m_nSize, FALSE)
        BKWIN_ENUM_ATTRIBUTE("direction", UINT, FALSE)
            BKWIN_ENUM_VALUE("horz", 0)
            BKWIN_ENUM_VALUE("vert", 1)
        BKWIN_ENUM_END(m_uDirection)
    BKWIN_DECLARE_ATTRIBUTES_END()
};


//////////////////////////////////////////////////////////////////////////
// CBkPngSkin
class CBkPngSkin : public CBkSkinBase
{
    BKOBJ_DECLARE_CLASS_NAME(CBkPngSkin, "png")

public:
    CBkPngSkin();
    virtual void Draw(CDCHandle dc, CRect rcDraw, DWORD dwState);
    virtual SIZE GetSkinSize();
    virtual BOOL IgnoreState();

protected:
    Gdiplus::Image* m_pImg;
    LONG            m_lSubImageWidth;
    int             m_drawMode;             // 绘制模式
    int             m_fixedSize;            // 固定部分的长度（左右端/上下端）
    int             m_iFixedL;              // 左侧固定区域
    int             m_iFixedR;              // 右侧固定区域
    int             m_iFixedT;              // 上侧固定区域
    int             m_iFixedB;              // 下侧固定区域
    int             m_iAlign;               // 对齐方式(上、下、左、右、左上、右上、左下、右下)

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_PNG_ATTRIBUTE("src", m_pImg, FALSE)
        BKWIN_INT_ATTRIBUTE("subwidth", m_lSubImageWidth, FALSE)
        BKWIN_INT_ATTRIBUTE("fixed_size", m_fixedSize, FALSE)
        BKWIN_INT_ATTRIBUTE("fixed_l", m_iFixedL, FALSE)
        BKWIN_INT_ATTRIBUTE("fixed_r", m_iFixedR, FALSE)
        BKWIN_INT_ATTRIBUTE("fixed_t", m_iFixedT, FALSE)
        BKWIN_INT_ATTRIBUTE("fixed_b", m_iFixedB, FALSE)
        BKWIN_ENUM_ATTRIBUTE("align", INT, FALSE)
            BKWIN_ENUM_VALUE("left", Left)
            BKWIN_ENUM_VALUE("right", Right)
            BKWIN_ENUM_VALUE("top", Top)
            BKWIN_ENUM_VALUE("bottom", Bottom)
            BKWIN_ENUM_VALUE("topleft", Top | Left)
            BKWIN_ENUM_VALUE("topright", Top | Right)
            BKWIN_ENUM_VALUE("bottomleft", Bottom | Left)
            BKWIN_ENUM_VALUE("bottomright", Bottom | Right)
        BKWIN_ENUM_END(m_iAlign)
        BKWIN_ENUM_ATTRIBUTE("drawmode", INT, FALSE)
            BKWIN_ENUM_VALUE("clamp", Clamp)
            BKWIN_ENUM_VALUE("stretch", Stretch)
            BKWIN_ENUM_VALUE("stretchx", StretchX)
            BKWIN_ENUM_VALUE("stretchy", StretchY)
        BKWIN_ENUM_END(m_drawMode)
    BKWIN_DECLARE_ATTRIBUTES_END()
};

//////////////////////////////////////////////////////////////////////////
// CBkGifSkin
class CBkGifSkin : public CBkSkinBase
{
    BKOBJ_DECLARE_CLASS_NAME(CBkGifSkin, "gif")

    enum
    {
        GifDraw_Once = 1,           // 只播放一遍
        GifDraw_Repeat,             // 循环播放
        GifDraw_RepeatFrame,        // 从某一帧播放
    };

public:
    CBkGifSkin();
    virtual void Draw(CDCHandle dc, CRect rcDraw, DWORD dwState);
    virtual void Init();
    virtual void Release();
    virtual SIZE GetSkinSize();
    virtual BOOL IgnoreState();

    void    Reset();
    UINT    UpdateFrame( int frame );
    UINT    GetFrameFirst();
    int     GetFrameCount();
    long    GetFrameTime( int frame );

protected:
    Gdiplus::Image* m_pImg;
    
    PropertyItem*   m_pPropertyItem;            // 属性
    UINT            m_iFrameCount;              // 帧数
    UINT            m_iFrameFirst;              // 开始显示的第一帧
    UINT            m_iFrameRepeat;             // 开始循环的帧数，当m_iDrawMode设定为GifDraw_RepeatFrame时有效
    int             m_iDrawMode;                // 绘制方式

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_GIF_ATTRIBUTE("src", m_pImg, FALSE)
        BKWIN_UINT_ATTRIBUTE("frame-first", m_iFrameFirst, FALSE)
        BKWIN_UINT_ATTRIBUTE("frame-repeat", m_iFrameRepeat, FALSE)
        BKWIN_ENUM_ATTRIBUTE("drawmode", INT, FALSE)
            BKWIN_ENUM_VALUE("once", GifDraw_Once)
            BKWIN_ENUM_VALUE("repeat", GifDraw_Repeat)
            BKWIN_ENUM_VALUE("repeat-frame", GifDraw_RepeatFrame)
        BKWIN_ENUM_END(m_iDrawMode)
    BKWIN_DECLARE_ATTRIBUTES_END()
};

//////////////////////////////////////////////////////////////////////////
// CBkSkinButton
class CBkSkinButton : public CBkSkinBase
{
    BKOBJ_DECLARE_CLASS_NAME(CBkSkinButton, "button")

public:
    CBkSkinButton();
    virtual void Draw(CDCHandle dc, CRect rcDraw, DWORD dwState);
    virtual SIZE GetSkinSize();
    virtual BOOL IgnoreState();

protected:
    Gdiplus::Image* m_pImg;
    LONG            m_lSubImageWidth;
    int             m_fixedSize;            // 固定部分的长度（左右端/上下端）
    int             m_drawMode;             // 绘制模式
    int             m_drawIndex[5];         // 绘制索引

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_PNG_ATTRIBUTE("src", m_pImg, FALSE)
        BKWIN_INT_ATTRIBUTE("subwidth", m_lSubImageWidth, FALSE)
        BKWIN_INT_ATTRIBUTE("fixed_size", m_fixedSize, FALSE)
        BKWIN_INT_ATTRIBUTE("sub-normal", m_drawIndex[0], FALSE)
        BKWIN_INT_ATTRIBUTE("sub-hover", m_drawIndex[1], FALSE)
        BKWIN_INT_ATTRIBUTE("sub-down", m_drawIndex[2], FALSE)
        BKWIN_INT_ATTRIBUTE("sub-disabled", m_drawIndex[3], FALSE)
        BKWIN_INT_ATTRIBUTE("sub-check", m_drawIndex[4], FALSE)
        BKWIN_ENUM_ATTRIBUTE("drawmode", INT, FALSE)
            BKWIN_ENUM_VALUE("clamp", Clamp)
            BKWIN_ENUM_VALUE("stretch", Stretch)
            BKWIN_ENUM_VALUE("stretchx", StretchX)
            BKWIN_ENUM_VALUE("stretchy", StretchY)
        BKWIN_ENUM_END(m_drawMode)
    BKWIN_DECLARE_ATTRIBUTES_END()
};

//////////////////////////////////////////////////////////////////////////
// CBkSkinEdit
class CBkSkinEdit : public CBkSkinBase
{
	BKOBJ_DECLARE_CLASS_NAME(CBkSkinEdit, "edit")

public:
	CBkSkinEdit();
	virtual void Draw(CDCHandle dc, CRect rcDraw, DWORD dwState);
	virtual BOOL IgnoreState();
	void GetColors(COLORREF &clrBack, COLORREF &clrBorder, COLORREF &clrText, DWORD dwState);

protected:
	COLORREF m_crBg;
	COLORREF m_crBgHover;
	COLORREF m_crBgReadonly;
	COLORREF m_crBgDisable;
	COLORREF m_crBgSelect;

	COLORREF m_crBorder;
	COLORREF m_crBorderHover;
	COLORREF m_crBorderReadonly;
	COLORREF m_crBorderDisable;
	COLORREF m_crBorderShadow;

	COLORREF m_crText;
	COLORREF m_crTextHover;
	COLORREF m_crTextReadonly;
	COLORREF m_crTextDisable;
	COLORREF m_crTextDefault;

	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_COLOR_ATTRIBUTE("bg", m_crBg, TRUE)
		BKWIN_COLOR_ATTRIBUTE("bghover", m_crBgHover, TRUE)
		BKWIN_COLOR_ATTRIBUTE("bgreadonly", m_crBgReadonly, TRUE)
		BKWIN_COLOR_ATTRIBUTE("bgdisable", m_crBgDisable, TRUE)
		BKWIN_COLOR_ATTRIBUTE("bgselect", m_crBgSelect, TRUE)
		BKWIN_COLOR_ATTRIBUTE("border", m_crBorder, TRUE)
		BKWIN_COLOR_ATTRIBUTE("bordershadow", m_crBorderShadow, TRUE)
		BKWIN_COLOR_ATTRIBUTE("borderhover", m_crBorderHover, TRUE)
		BKWIN_COLOR_ATTRIBUTE("borderreadonly", m_crBorderReadonly, TRUE)
		BKWIN_COLOR_ATTRIBUTE("borderdisable", m_crBorderDisable, TRUE)
		BKWIN_COLOR_ATTRIBUTE("text", m_crText, TRUE)
		BKWIN_COLOR_ATTRIBUTE("texthover", m_crTextHover, TRUE)
		BKWIN_COLOR_ATTRIBUTE("textreadonly", m_crTextReadonly, TRUE)
		BKWIN_COLOR_ATTRIBUTE("textdisable", m_crTextDisable, TRUE)
		BKWIN_COLOR_ATTRIBUTE("textdefault", m_crTextDefault, TRUE)
	BKWIN_DECLARE_ATTRIBUTES_END()
};


//////////////////////////////////////////////////////////////////////////
// CBkSkinScroll
class CBkSkinScroll: public CBkSkinBase
{
    BKOBJ_DECLARE_CLASS_NAME(CBkSkinScroll, "scroll")

    enum ScrollTypeEnum { STE_NONE, STE_V, STE_H };
public:
    CBkSkinScroll();
    virtual void Draw(CDCHandle dc, CRect rcDraw, DWORD dwState);
    
    int  GetWidth();
    int  GetHeight();
    int  GetThumbMinSize();
    BOOL HasArrow();
    void DrawBackground(CDCHandle dc, CRect rcDraw);
    void DrawThumb(CDCHandle dc, CRect rcDraw, DWORD dwState, BOOL bIsH);
    void DrawArrow(CDCHandle dc, CRect rcDraw, DWORD dwState, BOOL bIs1);

protected:
    Gdiplus::Image* m_pImgBackground;
    Gdiplus::Image* m_pImgThumb;
    Gdiplus::Image* m_pImgThumb2;
    Gdiplus::Image* m_pImgArrow1;   // up or left
    Gdiplus::Image* m_pImgArrow2;   // down or right
    int             m_Type;
    int             m_bgFixedSize;
    int             m_thumbMinSize;

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_PNG_ATTRIBUTE("bg", m_pImgBackground, FALSE)
        BKWIN_PNG_ATTRIBUTE("thumb", m_pImgThumb, FALSE)
        BKWIN_PNG_ATTRIBUTE("thumb2", m_pImgThumb2, FALSE)
        BKWIN_PNG_ATTRIBUTE("arrow1", m_pImgArrow1, FALSE)
        BKWIN_PNG_ATTRIBUTE("arrow2", m_pImgArrow2, FALSE)
        BKWIN_INT_ATTRIBUTE("bg_fixed_size", m_bgFixedSize, FALSE)
        BKWIN_INT_ATTRIBUTE("thumb_min_size", m_thumbMinSize, FALSE)
        BKWIN_ENUM_ATTRIBUTE("type", INT, FALSE)
            BKWIN_ENUM_VALUE("vert", STE_V)
            BKWIN_ENUM_VALUE("horz", STE_H)
        BKWIN_ENUM_END(m_Type)
    BKWIN_DECLARE_ATTRIBUTES_END()
};


//////////////////////////////////////////////////////////////////////////
// BkSkin
class BkSkin
{
public:
    ~BkSkin();
    static BOOL LoadSkins(LPCTSTR szRes);
    static CBkSkinBase* GetSkin(LPCSTR lpszSkinName);

protected:
    typedef CAtlMap<CStringA, CBkSkinBase *> __BkSkinPool;
    void            _Clear();
    void            _LoadSkins(TiXmlElement *pXmlSkinRootElem);
    CBkSkinBase*    _CreateBkSkinByName(LPCSTR lpszName);
    static BkSkin*  _Instance();

    __BkSkinPool    m_mapPool;
};

#include "bkskin.inl"