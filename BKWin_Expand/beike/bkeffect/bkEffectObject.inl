

inline CBkEffectObject::CBkEffectObject()
{
    m_effectInfo.nType = 0;
    m_effectInfo.nTimePlay = 0;
    m_effectInfo.nTimeFrameInterval = 0;
    m_effectInfo.bUpdateCache = TRUE;
    m_effectInfo.infoAction.hWndTimer = NULL;
    m_effectInfo.infoAction.rcEffect.SetRect(0,0,0,0);
    m_effectInfo.infoAction.pCtrl = NULL;
    // m_effectInfo.infoAction.pRenderer = NULL;
    m_effectInfo.infoAction.pNotify = NULL;

    m_nFrame = 0;
    m_bFrameUpdate = FALSE;
    m_bPlay = FALSE;
}

inline CBkEffectObject::~CBkEffectObject()
{
}

inline UINT CBkEffectObject::GetID()
{
    return m_effectInfo.nID;
}

inline void CBkEffectObject::SetID(UINT id)
{
    m_effectInfo.nID = id;
}

inline void CBkEffectObject::Init(EffectActionInfo info)
{
    m_effectInfo.infoAction = info;
    m_bPlay = FALSE;
}

inline void CBkEffectObject::Init()
{
}

inline void CBkEffectObject::Release()
{
}

inline void CBkEffectObject::Play()
{
    // 锁定控件绘制，开启特效绘制
    if (m_effectInfo.infoAction.pCtrl)
    {
        //_CreateEffectImage();
        //m_effectInfo.infoAction.pCtrl->SetEffect(this);
        m_nFrame = 0;
        //::KillTimer(m_effectInfo.infoAction.hWndTimer, (UINT_PTR)this);
        //::SetTimer(m_effectInfo.infoAction.hWndTimer, (UINT_PTR)this, 0, OnTimer_Effect );
        m_animateTimer.KillTimer();
        //m_animateTimer.SetTimer(m_effectInfo.infoAction.hWndTimer, 0, (int)GetID());
        m_animateTimer.SetTimer(m_effectInfo.infoAction.hWndTimer, m_effectInfo.nTimeFrameInterval, (int)GetID());
        //m_effectInfo.infoAction.pCtrl->NotifyInvalidate();
        
        if (m_effectInfo.bUpdateCache)
        {
            _CreateEffectImage();
        }
        //if (m_effectInfo.infoAction.pNotify)
        //{
        //    NotifyInfo info = {BKM_EFFECT_START, CPoint(0,0), m_effectInfo.infoAction.pCtrl, m_effectInfo.nType, this};
        //    m_effectInfo.infoAction.pNotify->Action(&info);
        //}
    }
}

inline void CBkEffectObject::Stop()
{
    // 关闭特效绘制，恢复控件绘制
    if (m_effectInfo.infoAction.pCtrl)
    {
        m_bPlay = FALSE;
        //::KillTimer(m_effectInfo.infoAction.hWndTimer, (UINT_PTR)this);
        m_animateTimer.KillTimer();
        
        //if (!m_effectInfo.bKeepEnd)
        {
            //m_effectInfo.infoAction.pCtrl->SetEffect(NULL);
            m_effectInfo.infoAction.pCtrl->RemoveEffect(this);
        }
        if (OnPreEnd())
        {
            OnEnd();
            if (m_effectInfo.infoAction.pNotify)
            {
                NotifyInfo info = {BKM_EFFECT_END, CPoint(0,0), m_effectInfo.infoAction.pCtrl, m_effectInfo.nType, this};
                m_effectInfo.infoAction.pNotify->Action(&info);
            }
        }
        m_effectInfo.infoAction.pCtrl->NotifyInvalidate();
    }
    //if (!m_effectInfo.bKeepEnd)
    {
        _ClearActionInfo();
    }
}

inline void CBkEffectObject::Update(HWND hWnd, CDCHandle& dc)
{
    if (hWnd && m_effectInfo.infoAction.pCtrl)
    {
        if (m_effectInfo.infoAction.pCtrl->GetContainer() == hWnd && m_bFrameUpdate)
        {
            //m_bFrameUpdate = FALSE;
            Render(dc);
        }
    }
}

inline BOOL CBkEffectObject::IsUpdate()
{
    return m_bFrameUpdate;
}

inline BOOL CBkEffectObject::IsPlay()
{
    return m_bPlay;
}

inline void CBkEffectObject::UpdateFrame(HWND hWnd, UINT_PTR frameTimer)
{
    if (hWnd == m_effectInfo.infoAction.hWndTimer &&
        frameTimer == (UINT_PTR)this &&
        m_effectInfo.infoAction.pCtrl)
    {
        //::KillTimer(m_effectInfo.infoAction.hWndTimer, (UINT_PTR)this);
        //::SetTimer(m_effectInfo.infoAction.hWndTimer, (UINT_PTR)this, m_effectInfo.nTimeFrameInterval, OnTimer_Effect );
        //m_animateTimer.KillTimer();
        //m_animateTimer.SetTimer(m_effectInfo.infoAction.hWndTimer, m_effectInfo.nTimeFrameInterval, (int)GetID());

        if (m_nFrame == 0)
        {
            // 动画开始
            //m_effectInfo.infoAction.pCtrl->SetEffect(this);
            m_effectInfo.infoAction.pCtrl->AddEffect(this);
            if (OnPreStart())
            {
                //_CreateEffectImage();
                if (m_effectInfo.infoAction.pNotify)
                {
                    NotifyInfo info = {BKM_EFFECT_START, CPoint(0,0), m_effectInfo.infoAction.pCtrl, m_effectInfo.nType, this};
                    m_effectInfo.infoAction.pNotify->Action(&info);
                }
                OnStart();
                m_bPlay = TRUE;
            }
        }

        if (IsPlay())
        {
            m_nFrame += m_effectInfo.nTimeFrameInterval;
            m_bFrameUpdate = TRUE;
            BOOL bUpdate =  UpdateInfo();
            //m_effectInfo.infoAction.pCtrl->NotifyInvalidate();
            if (m_nFrame >= m_effectInfo.nTimePlay)
            {
                Stop();
            }

            if (bUpdate && m_effectInfo.infoAction.pCtrl)
            {
                m_effectInfo.infoAction.pCtrl->NotifyInvalidate();
            }
        }

    }
}

inline void CBkEffectObject::CopyCache(CBkEffectObject* pSrcEffect)
{
    if (pSrcEffect)
    {
        _CreateEffectImage();
        ::BitBlt(m_MemDC.m_hDC, 0, 0, m_effectInfo.infoAction.rcEffect.Width(), m_effectInfo.infoAction.rcEffect.Height(), 
                 pSrcEffect->m_MemDC.m_hDC, 0, 0, SRCCOPY);
    }
}

inline void CBkEffectObject::Render(CDCHandle& dc)
{
}

inline BOOL CBkEffectObject::UpdateInfo()
{
	return FALSE;
}

inline void CBkEffectObject::_ClearActionInfo()
{
    m_effectInfo.infoAction.hWndTimer = NULL;
    m_effectInfo.infoAction.rcEffect.SetRect(0,0,0,0);
    m_effectInfo.infoAction.pCtrl = NULL;
    // m_effectInfo.infoAction.pRenderer = NULL;
    m_effectInfo.infoAction.pNotify = NULL;
}

inline BOOL CBkEffectObject::_CreateEffectImage()
{
    BOOL bRet = FALSE;
    if (m_effectInfo.infoAction.pCtrl)
    {
        HWND hWnd = m_effectInfo.infoAction.pCtrl->GetContainer();
        if (hWnd)
        {
            CRect rc;
            m_effectInfo.infoAction.pCtrl->GetRect(&rc);
            HDC hDC = ::GetDC(hWnd);
            bRet = m_MemDC.Create(hDC, rc, TRUE);
            ::ReleaseDC(hWnd, hDC);
        }
    }
    return bRet;
}

inline void CBkEffectObject::OnStart()
{
}

inline void CBkEffectObject::OnEnd()
{
}

inline BOOL CBkEffectObject::OnPreStart()
{
    return TRUE;
}

inline BOOL CBkEffectObject::OnPreEnd()
{
    return TRUE;
}

inline VOID CBkEffectObject::OnTimer_Effect( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime )
{
    CBkEffectObject* pEffectObj = (CBkEffectObject*)idEvent;
    if (pEffectObj)
    {
        pEffectObj->UpdateFrame(hwnd, (UINT_PTR)pEffectObj);
    }
}