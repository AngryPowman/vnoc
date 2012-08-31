#pragma once
#include "bkEffectObject.h"

//////////////////////////////////////////////////////////////////////////
//   File Name: CBkEffectScroll.h
// Description: ��̬��Ч - ���ᶯ��
//              ���־���Ч������/չ����Ч
//     Creator: YangDa
//     Version: 2011.10.28 - 1.0 - Create
//              2011.11.14 - 1.1 - �Ż�������ʾЧ��
//////////////////////////////////////////////////////////////////////////

enum 
{
    EffectScroll_None = 0,
    EffectScroll_Horz_Expand = 1,       // ˮƽչ��
    EffectScroll_Horz_Collaps,          // ˮƽ����
    EffectScroll_Vert_Expand,           // ��ֱչ��
    EffectScroll_Vert_Collaps,          // ��ֱ����
};

typedef struct _ExtraData_Scroll
{
    int                     nFrameStart;                     // ��ʼ�׶ζ�����ʼ֡
    int                     nFrameEnd;                       // ��ֹ�׶ζ�����ʼ֡
    CBkWnd*                 pCtrlStart;                      // ������ʼ�ζ����Ŀؼ�
    CBkWnd*                 pCtrlEnd;                        // ������ֹ�ζ����Ŀؼ�

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

    int                 m_nFrames;                  // ����֡��
    int                 m_nStart;                   // ������㣨�����Ч����������
    int                 m_nEnd;                     // �����յ㣨�����Ч����������
    int                 m_nStartTime;               // ������ʼ�γ���ʱ��
    int                 m_nEndTime;                 // ������ֹ�γ���ʱ��
    int                 m_nXOffsetScroll;           // ��ҳ����Xƫ��
    int                 m_nYOffsetScroll;           // ��ҳ����Yƫ��

    BOOL                m_bStartFinished;           // ��ʼ���Ƿ��Ѿ�����
    int                 m_nPlay;                    // �Ѿ����ŵĽ���
    int                 m_nPlayEnd;                 // ������ֹ�εĽ���
    double              m_nProcess;                 // ����ҳ�������
    ExtraData_Scroll    m_extraData;                // ��������
};

#include "bkEffectScroll.inl"