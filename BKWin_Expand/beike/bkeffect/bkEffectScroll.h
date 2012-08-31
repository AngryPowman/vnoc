#pragma once
#include "bkEffectObject.h"

//////////////////////////////////////////////////////////////////////////
//   File Name: CBkEffectScroll.h
// Description: 动态特效 - 卷轴动画
//              表现卷轴效果收起/展开特效
//     Creator: YangDa
//     Version: 2011.10.28 - 1.0 - Create
//              2011.11.14 - 1.1 - 优化修正显示效果
//////////////////////////////////////////////////////////////////////////

enum 
{
    EffectScroll_None = 0,
    EffectScroll_Horz_Expand = 1,       // 水平展开
    EffectScroll_Horz_Collaps,          // 水平收起
    EffectScroll_Vert_Expand,           // 竖直展开
    EffectScroll_Vert_Collaps,          // 竖直收起
};

typedef struct _ExtraData_Scroll
{
    int                     nFrameStart;                     // 起始阶段动画开始帧
    int                     nFrameEnd;                       // 终止阶段动画开始帧
    CBkWnd*                 pCtrlStart;                      // 产生起始段动画的控件
    CBkWnd*                 pCtrlEnd;                        // 产生终止段动画的控件

    _ExtraData_Scroll()
        :nFrameStart(0)
        ,nFrameEnd(0)
        ,pCtrlStart(NULL)
        ,pCtrlEnd(NULL)
    {
    }

} ExtraData_Scroll;

class CBkEffectScroll : public CBkEffectObject
                      , public INotify
{
    BKOBJ_DECLARE_CLASS_NAME(CBkEffectScroll, "effect-scroll")

public:
    CBkEffectScroll();
    virtual ~CBkEffectScroll();

    virtual void Init(EffectActionInfo info);
    virtual void Render(CDCHandle& dc);
    virtual BOOL UpdateInfo();

protected:
    virtual BOOL Action(NotifyInfo* pInfo);

    virtual void OnStart();

    void Render_Background(CDCHandle& dc, const CRect& rcDraw);
    void Render_Scroll(CDCHandle& dc, const CRect& rcDraw);

    void Update_Render_Region(CRect& rcDraw);
    void Update_Scroll_Region(CRect& rcDraw);
    void Update_Start_Region(CRect& rcDraw);
    void Update_End_Region(CRect& rcDraw);

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_INT_ATTRIBUTE("frames", m_nFrames, FALSE)
        BKWIN_INT_ATTRIBUTE("start", m_nStart, FALSE)
        BKWIN_INT_ATTRIBUTE("end", m_nEnd, FALSE)
        BKWIN_INT_ATTRIBUTE("start-time", m_nStartTime, FALSE)
        BKWIN_INT_ATTRIBUTE("end-time", m_nEndTime, FALSE)
        BKWIN_INT_ATTRIBUTE("xoffset-scroll", m_nXOffsetScroll, FALSE)
        BKWIN_INT_ATTRIBUTE("yoffset-scroll", m_nYOffsetScroll, FALSE)
        BKWIN_SKIN_ATTRIBUTE("skin-scroll", m_pSkinScroll, FALSE)
        BKWIN_EFFECT_ATTRIBUTE("effect-start", m_pEffectStart, FALSE)
        BKWIN_EFFECT_ATTRIBUTE("effect-end", m_pEffectEnd, FALSE)
        BKWIN_ENUM_ATTRIBUTE("type", INT, TRUE)
            BKWIN_ENUM_VALUE("horz-expand", EffectScroll_Horz_Expand)
            BKWIN_ENUM_VALUE("horz-collaps", EffectScroll_Horz_Collaps)
            BKWIN_ENUM_VALUE("vert-expand", EffectScroll_Vert_Expand)
            BKWIN_ENUM_VALUE("vert-collaps", EffectScroll_Vert_Collaps)
        BKWIN_ENUM_END(m_effectInfo.nType)
    BKWIN_DECLARE_ATTRIBUTES_END()

private:
    BOOL OnEvent_EffectStart(NotifyInfo* pInfo);
    BOOL OnEvent_EffectEnd(NotifyInfo* pInfo);


protected:
    CBkSkinBase*        m_pSkinScroll;
    CBkEffectObject*    m_pEffectStart;
    CBkEffectObject*    m_pEffectEnd;

    int                 m_nFrames;                  // 动画帧数
    int                 m_nStart;                   // 绘制起点（相对于效果作用区域）
    int                 m_nEnd;                     // 绘制终点（相对于效果作用区域）
    int                 m_nStartTime;               // 绘制起始段持续时间
    int                 m_nEndTime;                 // 绘制终止段持续时间
    int                 m_nXOffsetScroll;           // 卷页绘制X偏移
    int                 m_nYOffsetScroll;           // 卷页绘制Y偏移

    BOOL                m_bStartFinished;           // 起始段是否已经结束
    int                 m_nPlay;                    // 已经播放的进度
    int                 m_nPlayEnd;                 // 播放终止段的进度
    double              m_nProcess;                 // 被翻页处理进度
    ExtraData_Scroll    m_extraData;                // 额外数据
};

#include "bkEffectScroll.inl"