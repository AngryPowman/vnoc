#pragma once
#include "bkEffectObject.h"

//////////////////////////////////////////////////////////////////////////
//   File Name: CBkEffectTurnPage.h
// Description: 动态特效 - 图书翻页
//              表现图书翻页特效
//     Creator: YangDa
//     Version: 2011.09.13 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

enum 
{
    EffectTurnPage_None = 0,
    EffectTurnPage_Previous = 1,
    EffectTurnPage_Next,
};

class CBkEffectTurnPage : public CBkEffectObject
{
    BKOBJ_DECLARE_CLASS_NAME(CBkEffectTurnPage, "effect-turnpage")

public:
    CBkEffectTurnPage();
    virtual ~CBkEffectTurnPage();

    virtual void Init(EffectActionInfo info);
    virtual void Render(CDCHandle& dc);
    virtual BOOL UpdateInfo();

protected:
    void Render_FullPage(CDCHandle& dc);
    void Render_UnderPage(CDCHandle& dc);
    void Render_TurnPage(CDCHandle& dc);

    void Update_Hotspot();

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_INT_ATTRIBUTE("step", m_nStep, FALSE)
        BKWIN_INT_ATTRIBUTE("turnpage-angle", m_nTurnPageAngle, FALSE)
        BKWIN_INT_ATTRIBUTE("page-spine", m_nPageSpine, FALSE)
        BKWIN_COLOR_ATTRIBUTE("color-turnpage-spin", m_clrTurnPageSpin, FALSE)
        BKWIN_COLOR_ATTRIBUTE("color-turnpage-center", m_clrTurnPageCenter, FALSE)
        BKWIN_COLOR_ATTRIBUTE("color-turnpage-edge", m_clrTurnPageEdge, FALSE)
        BKWIN_SKIN_ATTRIBUTE("skin-underpage", m_pSkinUnderPage, FALSE)
        BKWIN_ENUM_ATTRIBUTE("type", INT, TRUE)
            BKWIN_ENUM_VALUE("previous", EffectTurnPage_Previous)
            BKWIN_ENUM_VALUE("next", EffectTurnPage_Next)
        BKWIN_ENUM_END(m_effectInfo.nType)
        BKWIN_INT_ATTRIBUTE("border-margin-x", m_marginBorder.cx, FALSE)
        BKWIN_INT_ATTRIBUTE("border-margin-y", m_marginBorder.cy, FALSE)
    BKWIN_DECLARE_ATTRIBUTES_END()

private:
    // 获得被翻页下面部分的区域
    Gdiplus::GraphicsPath* _GetUnderPagePath(double& angle, double x, int pageType, BOOL isUnderPage);
    Gdiplus::GraphicsPath* _GetTurnPagePath(double& angle, double x, int pageType);


protected:
    int             m_nStep;                    // 翻页移动步长
    int             m_nTurnPageAngle;           // 翻页起始角度

    int             m_nPageSpine;               // 书脊宽度
    int             m_nPageWidth;               // 书页宽度
    int             m_nPageHeight;              // 书页高度
    
    SIZE            m_marginBorder;             // 边框与书内容边距

    COLORREF        m_clrTurnPageSpin;          // 被翻页接近书脊处的颜色
    COLORREF        m_clrTurnPageCenter;        // 被翻页书脊与书皮边缘间的颜色
    COLORREF        m_clrTurnPageEdge;          // 被翻页接近书皮边缘的颜色

    CBkSkinBase*    m_pSkinUnderPage;
    
    double          m_nHotspotAxisX;            // 翻页区域与下一页的对称轴X坐标  
};

#include "bkEffectTurnPage.inl"