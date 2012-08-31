#pragma once
#include "bkEffectObject.h"

//////////////////////////////////////////////////////////////////////////
//   File Name: bkEffectSprite.h
// Description: 动态特效 - 精灵动画
//              表现精灵连续动作小坐
//     Creator: YangDa
//     Version: 2011.11.02 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

enum 
{
    EffectSprite_None = 0,
    EffectSprite_Once = 1,               // 仅播放一次
    EffectSprite_Loop,                   // 单向循环播放
    EffectSprite_Loop_Bidirection,       // 双向循环播放（一个方向播放完成后，使用相反方向在播放，往复继续）
};

enum 
{
    EffectSprite_Aligment = 0,
    EffectSprite_Aligment_Left = 1,         // 左对齐
    EffectSprite_Aligment_Right,            // 右对齐
};

class CBkEffectSprite : public CBkEffectObject
{
    BKOBJ_DECLARE_CLASS_NAME(CBkEffectSprite, "effect-sprite")

public:
    CBkEffectSprite();
    virtual ~CBkEffectSprite();

    int GetPlayFrame();

    virtual void Init(EffectActionInfo info);
    virtual void Render(CDCHandle& dc);
    virtual BOOL UpdateInfo();

protected:
    virtual BOOL OnPreStart();
    virtual BOOL OnPreEnd();

    void Render_Sprite(Gdiplus::Graphics& g, const CRect& rcDraw);

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_PNG_ATTRIBUTE("src", m_pImageSprite, FALSE)
        BKWIN_INT_ATTRIBUTE("sprite-width", m_nSpriteWidth, FALSE)
        BKWIN_INT_ATTRIBUTE("aligment", m_nAligment, FALSE)
        BKWIN_INT_ATTRIBUTE("frames", m_nFrames, FALSE)
        BKWIN_INT_ATTRIBUTE("start", m_nStart, FALSE)
        BKWIN_INT_ATTRIBUTE("end", m_nEnd, FALSE)
        BKWIN_ENUM_ATTRIBUTE("type", INT, TRUE)
            BKWIN_ENUM_VALUE("play-once", EffectSprite_Once)
            BKWIN_ENUM_VALUE("play-loop", EffectSprite_Loop)
            BKWIN_ENUM_VALUE("play-loop-bidirection", EffectSprite_Loop_Bidirection)
        BKWIN_ENUM_END(m_effectInfo.nType)
        BKWIN_ENUM_ATTRIBUTE("aligment", INT, TRUE)
            BKWIN_ENUM_VALUE("left", EffectSprite_Aligment_Left)
            BKWIN_ENUM_VALUE("right", EffectSprite_Aligment_Right)
        BKWIN_ENUM_END(m_nAligment)
    BKWIN_DECLARE_ATTRIBUTES_END()

private:
    void SwapFrames();


protected:
    Gdiplus::Image* m_pImageSprite;             // 精灵连续图片

    int             m_nAligment;                // 精灵对齐方式
    int             m_nSpriteWidth;             // 精灵宽度
    int             m_nFrames;                  // 动画帧数
    int             m_nStart;                   // 动画帧起点
    int             m_nEnd;                     // 动画帧终点

    int             m_nPlay;                    // 已经播放的进度
    double          m_nProcess;                 // 被翻页处理进度
};

#include "bkEffectSprite.inl"