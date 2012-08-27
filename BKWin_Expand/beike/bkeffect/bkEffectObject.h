#pragma once

#include "../bkwin/bkwnd.h"
#include "../utility/AnimateTimer.h"

//////////////////////////////////////////////////////////////////////////
//   File Name: CBkEffectObject.h
// Description: 动态特效对象
//              规定了动态特效的具体类型和绘制方法
//     Creator: YangDa
//     Version: 2011.09.07 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

typedef struct _EffectActionInfo
{
    HWND                    hWndTimer;                  // 动画定时器
    CRect                   rcEffect;                   // 动画作用区域
    CRect                   rcEffectOffset;             // 动画作用区域偏移
    CBkWnd*                 pCtrl;                      // 产生动画的控件
    //IBkEffectRenderer*      pRenderer;                  // 动画渲染器
    INotify*                pNotify;                    // 动画事件响应对象
    DWORD                   dwExtra;                    // 额外数据
} EffectActionInfo;

typedef struct _EffectInfo
{
    UINT                    nID;                        // 动画ID
    UINT                    nType;                      // 动画类型
    UINT                    nTimePlay;                  // 动画时长
    UINT                    nTimeFrameInterval;         // 动画每帧时间间隔
    BOOL                    bUpdateCache;               // 是否生成背景缓存
    BOOL                    bKeepEnd;                   // 是否在播放完毕动画后保持最后一帧状态
    CString                 name;                       // 动画名称
    EffectActionInfo        infoAction;                 // 动画动作信息
} EffectInfo;

class CBkEffectObject : public IBkEffectObject
                      , public CBkObject
{
public:
    CBkEffectObject();
    virtual ~CBkEffectObject();

    UINT GetID();
    void SetID(UINT id);

    void Init();
    void Release();
    void Play();
    void Stop();
    void Update(HWND hWnd, CDCHandle& dc);
    BOOL IsUpdate();
    BOOL IsPlay();

    void UpdateFrame(HWND hWnd, UINT_PTR frameTimer);
    void CopyCache(CBkEffectObject* pSrcEffect);

    virtual void Init(EffectActionInfo info);
    virtual void Render(CDCHandle& dc);
    virtual BOOL UpdateInfo();

protected:
    void _ClearActionInfo();
    virtual BOOL _CreateEffectImage();

    virtual void OnStart();
    virtual void OnEnd();
    virtual BOOL OnPreStart();
    virtual BOOL OnPreEnd();

    static VOID CALLBACK OnTimer_Effect( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime );

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_UINT_ATTRIBUTE("play-time", m_effectInfo.nTimePlay, FALSE)
        BKWIN_UINT_ATTRIBUTE("frame-interval", m_effectInfo.nTimeFrameInterval, FALSE)
        BKWIN_UINT_ATTRIBUTE("update-cache", m_effectInfo.bUpdateCache, FALSE)
        BKWIN_UINT_ATTRIBUTE("keep-end", m_effectInfo.bKeepEnd, FALSE)
    BKWIN_DECLARE_ATTRIBUTES_END()

protected:
    CAnimateTimer       m_animateTimer;
    EffectInfo          m_effectInfo;               // 特效基础信息
    CMemDC              m_MemDC;                    // 用来进行动态特效的基础图像
    UINT                m_nFrame;                   // 当前的帧数
    BOOL                m_bFrameUpdate;             // 是否绘制新的动画帧
    BOOL                m_bPlay;                    // 是否正在播放
};

#include "bkEffectObject.inl"