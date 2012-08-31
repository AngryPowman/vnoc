#pragma once
#include "bkEffectObject.h"

//////////////////////////////////////////////////////////////////////////
//   File Name: CBkEffectTurnPage.h
// Description: ��̬��Ч - ͼ�鷭ҳ
//              ����ͼ�鷭ҳ��Ч
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
    // ��ñ���ҳ���沿�ֵ�����
    Gdiplus::GraphicsPath* _GetUnderPagePath(double& angle, double x, int pageType, BOOL isUnderPage);
    Gdiplus::GraphicsPath* _GetTurnPagePath(double& angle, double x, int pageType);


protected:
    int             m_nStep;                    // ��ҳ�ƶ�����
    int             m_nTurnPageAngle;           // ��ҳ��ʼ�Ƕ�

    int             m_nPageSpine;               // �鼹���
    int             m_nPageWidth;               // ��ҳ���
    int             m_nPageHeight;              // ��ҳ�߶�
    
    SIZE            m_marginBorder;             // �߿��������ݱ߾�

    COLORREF        m_clrTurnPageSpin;          // ����ҳ�ӽ��鼹������ɫ
    COLORREF        m_clrTurnPageCenter;        // ����ҳ�鼹����Ƥ��Ե�����ɫ
    COLORREF        m_clrTurnPageEdge;          // ����ҳ�ӽ���Ƥ��Ե����ɫ

    CBkSkinBase*    m_pSkinUnderPage;
    
    double          m_nHotspotAxisX;            // ��ҳ��������һҳ�ĶԳ���X����  
};

#include "bkEffectTurnPage.inl"