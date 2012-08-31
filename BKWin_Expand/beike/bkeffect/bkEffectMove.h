#pragma once
#include "bkEffectObject.h"

//////////////////////////////////////////////////////////////////////////
//   File Name: CBkEffectObject.h
// Description: ��̬��Ч - �ƶ�
//              �����ƶ���Ч
//     Creator: YangDa
//     Version: 2011.09.08 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

enum
{
    EffectMove_None     = 0,
    EffectMove_Left     = 0x0001,
    EffectMove_Right    = 0x0002,
    EffectMove_Up       = 0x0004,
    EffectMove_Down     = 0x0008,
};

class CBkEffectMove : public CBkEffectObject
{
    BKOBJ_DECLARE_CLASS_NAME(CBkEffectMove, "effect-move")

public:
    CBkEffectMove();
    virtual ~CBkEffectMove();

    virtual void Init(EffectActionInfo info);
    virtual void Render(CDCHandle& dc);
    virtual BOOL UpdateInfo();

protected:
    void Render_Move(CDCHandle& dc);
    void Init_Render_Region(CRect& rcDraw, CRect& rcDrawTo);
    void Update_Render_Region(CRect& rcDraw, CRect& rcDrawTo);

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_INT_ATTRIBUTE("step-horz", m_nStepHorz, FALSE)
        BKWIN_INT_ATTRIBUTE("step-vert", m_nStepVert, FALSE)
        BKWIN_ENUM_ATTRIBUTE("from", INT, TRUE)
            BKWIN_ENUM_VALUE("left", EffectMove_Left)
            BKWIN_ENUM_VALUE("right", EffectMove_Right)
			BKWIN_ENUM_VALUE("top", EffectMove_Up)
			BKWIN_ENUM_VALUE("bottom", EffectMove_Down)
        BKWIN_ENUM_END(m_nDirectionFrom)
        BKWIN_ENUM_ATTRIBUTE("to", INT, TRUE)
            BKWIN_ENUM_VALUE("left", EffectMove_Left)
            BKWIN_ENUM_VALUE("right", EffectMove_Right)
			BKWIN_ENUM_VALUE("up", EffectMove_Up)
			BKWIN_ENUM_VALUE("down", EffectMove_Down)
        BKWIN_ENUM_END(m_nDirectionTo)
    BKWIN_DECLARE_ATTRIBUTES_END()

protected:
    int         m_nStepHorz;        // ˮƽ�ƶ�����
    int         m_nStepVert;        // ��ֱ�ƶ�����
    int         m_nDirectionFrom;   // ��ʼ����
    int         m_nDirectionTo;     // ��������

    CRect       m_rcDraw;           // Ҫ���Ƶ���Ч����ͼ������
    CRect       m_rcDrawTo;
};

#include "bkEffectMove.inl"