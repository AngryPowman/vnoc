#pragma once
#include "bkEffectObject.h"

//////////////////////////////////////////////////////////////////////////
//   File Name: CBkEffectTurnPage.h
// Description: 动态特效 - 图书翻页
//              表现图书翻页特效
//     Creator: YangDa
//     Version: 2011.10.21 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

enum 
{
    EffectTurnPage2_None = 0,
    EffectTurnPage2_Previous = 1,
    EffectTurnPage2_Next,
};

typedef struct _ExtraData_TurnPage
{
    BOOL                    bRealDrawUnderPage;     // 是否实时刷新下一页内容
    SIZE                    szNextPageCache;        // 下一页缓存区域
} ExtraData_TurnPage;

class CBkEffectTurnPage2 : public CBkEffectObject
{
    BKOBJ_DECLARE_CLASS_NAME(CBkEffectTurnPage2, "effect-turnpage2")

public:
    CBkEffectTurnPage2();
    virtual ~CBkEffectTurnPage2();

    virtual void Init(EffectActionInfo info);
    virtual void Render(CDCHandle& dc);
    virtual BOOL UpdateInfo();

protected:
    virtual void OnStart();

    void Render_Background(CDCHandle& dc, const CRect& rcDraw);
    void Render_FullPage(CDCHandle& dc, const CRect& rcDraw);
    void Render_UnderPage(CDCHandle& dc, const CRect& rcDraw);
    void Render_TurnPage(Gdiplus::Graphics& g, const CRect& rcDraw);

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_INT_ATTRIBUTE("canvas-padding", m_nCanvasPadding, FALSE)
        BKWIN_INT_ATTRIBUTE("frames", m_nFrames, FALSE)
        BKWIN_SKIN_ATTRIBUTE("skin-underpage", m_pSkinUnderPage, FALSE)
        BKWIN_COLOR_ATTRIBUTE("color-underpage", m_clrUnderPage, FALSE)
        BKWIN_ENUM_ATTRIBUTE("type", INT, TRUE)
            BKWIN_ENUM_VALUE("previous", EffectTurnPage2_Previous)
            BKWIN_ENUM_VALUE("next", EffectTurnPage2_Next)
        BKWIN_ENUM_END(m_effectInfo.nType)
    BKWIN_DECLARE_ATTRIBUTES_END()

private:
    BOOL _CreateAfterPageCache();


protected:
    CBkSkinBase*    m_pSkinUnderPage;
    
    int             m_nCanvasPadding;           // 画布填充区大小（被翻页显示时会超出书皮的部分）
    int             m_nFrames;                  // 动画帧数
    COLORREF        m_clrUnderPage;             // 被翻页下一页的填充颜色

    int             m_nFlip;                    // 已经翻过的距离
    double          m_nProcess;                 // 被翻页处理进度
    CMemDC          m_MemDCAfterPage;           // 翻页后的图像缓存
    
    ExtraData_TurnPage m_extraData;
    HDC             m_hDCAfterPage;
    HBITMAP         m_hBitmapAfterPage;
};

#include "bkEffectTurnPage2.inl"