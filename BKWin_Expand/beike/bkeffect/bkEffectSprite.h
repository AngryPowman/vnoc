#pragma once
#include "bkEffectObject.h"

//////////////////////////////////////////////////////////////////////////
//   File Name: bkEffectSprite.h
// Description: ��̬��Ч - ���鶯��
//              ���־�����������С��
//     Creator: YangDa
//     Version: 2011.11.02 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

enum 
{
    EffectSprite_None = 0,
    EffectSprite_Once = 1,               // ������һ��
    EffectSprite_Loop,                   // ����ѭ������
    EffectSprite_Loop_Bidirection,       // ˫��ѭ�����ţ�һ�����򲥷���ɺ�ʹ���෴�����ڲ��ţ�����������
};

enum 
{
    EffectSprite_Aligment = 0,
    EffectSprite_Aligment_Left = 1,         // �����
    EffectSprite_Aligment_Right,            // �Ҷ���
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
    Gdiplus::Image* m_pImageSprite;             // ��������ͼƬ

    int             m_nAligment;                // ������뷽ʽ
    int             m_nSpriteWidth;             // ������
    int             m_nFrames;                  // ����֡��
    int             m_nStart;                   // ����֡���
    int             m_nEnd;                     // ����֡�յ�

    int             m_nPlay;                    // �Ѿ����ŵĽ���
    double          m_nProcess;                 // ����ҳ�������
};

#include "bkEffectSprite.inl"