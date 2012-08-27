#pragma once

#include "../bkwin/bkwnd.h"
#include "../utility/AnimateTimer.h"

//////////////////////////////////////////////////////////////////////////
//   File Name: CBkEffectObject.h
// Description: ��̬��Ч����
//              �涨�˶�̬��Ч�ľ������ͺͻ��Ʒ���
//     Creator: YangDa
//     Version: 2011.09.07 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

typedef struct _EffectActionInfo
{
    HWND                    hWndTimer;                  // ������ʱ��
    CRect                   rcEffect;                   // ������������
    CRect                   rcEffectOffset;             // ������������ƫ��
    CBkWnd*                 pCtrl;                      // ���������Ŀؼ�
    //IBkEffectRenderer*      pRenderer;                  // ������Ⱦ��
    INotify*                pNotify;                    // �����¼���Ӧ����
    DWORD                   dwExtra;                    // ��������
} EffectActionInfo;

typedef struct _EffectInfo
{
    UINT                    nID;                        // ����ID
    UINT                    nType;                      // ��������
    UINT                    nTimePlay;                  // ����ʱ��
    UINT                    nTimeFrameInterval;         // ����ÿ֡ʱ����
    BOOL                    bUpdateCache;               // �Ƿ����ɱ�������
    BOOL                    bKeepEnd;                   // �Ƿ��ڲ�����϶����󱣳����һ֡״̬
    CString                 name;                       // ��������
    EffectActionInfo        infoAction;                 // ����������Ϣ
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
    EffectInfo          m_effectInfo;               // ��Ч������Ϣ
    CMemDC              m_MemDC;                    // �������ж�̬��Ч�Ļ���ͼ��
    UINT                m_nFrame;                   // ��ǰ��֡��
    BOOL                m_bFrameUpdate;             // �Ƿ�����µĶ���֡
    BOOL                m_bPlay;                    // �Ƿ����ڲ���
};

#include "bkEffectObject.inl"