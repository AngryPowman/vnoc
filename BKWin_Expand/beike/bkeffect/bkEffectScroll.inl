inline CBkEffectScroll::CBkEffectScroll()
:m_nFrames(20)
,m_nStart(0)
,m_nEnd(0)
,m_nStartTime(0)
,m_nEndTime(0)
,m_nXOffsetScroll(0)
,m_nYOffsetScroll(0)
,m_bStartFinished(FALSE)
,m_nPlay(0)
,m_nPlayEnd(0)
,m_nProcess(1.0)
,m_pSkinScroll(NULL)
,m_pEffectStart(NULL)
,m_pEffectEnd(NULL)
{
}

inline CBkEffectScroll::~CBkEffectScroll()
{

}

inline void CBkEffectScroll::Init(EffectActionInfo info)
{
    CBkEffectObject::Init(info);

    ExtraData_Scroll* pExtraData = (ExtraData_Scroll*)info.dwExtra;
    if (pExtraData)
    {
        m_extraData.nFrameStart = pExtraData->nFrameStart;
        m_extraData.nFrameEnd = pExtraData->nFrameEnd;
        m_extraData.pCtrlStart = pExtraData->pCtrlStart;
        m_extraData.pCtrlEnd = pExtraData->pCtrlEnd;
    }

    m_bStartFinished = FALSE;
    if (!m_pEffectStart)
    {
        m_bStartFinished = TRUE;
    }
    m_nPlay = m_nStart;
    m_nPlayEnd = 0;

    switch(m_effectInfo.nType)
    {
    case EffectScroll_Vert_Expand:
        m_nProcess = -1.0;
        break;
    case EffectScroll_Vert_Collaps:
        m_nProcess = 1.0;
        break;
    }
}

inline void CBkEffectScroll::Render(CDCHandle& dc)
{
    Render_Background(dc, m_effectInfo.infoAction.rcEffect);
    Render_Scroll(dc, m_effectInfo.infoAction.rcEffect);
}

inline BOOL CBkEffectScroll::UpdateInfo()
{
    if (m_bStartFinished)
    {
        // ÖÐÍ¾½×¶Î
        CRect rcCanvas = m_effectInfo.infoAction.rcEffect;
        rcCanvas.DeflateRect(&m_effectInfo.infoAction.rcEffectOffset);

        int step = rcCanvas.Height()/m_nFrames;
       
        // ½áÊø½×¶Î
        if (m_nPlay >= rcCanvas.Height()-m_nEnd)
        {        
            if (m_nPlayEnd == 0 && m_pEffectEnd )
            {
                EffectActionInfo effectInfo = m_effectInfo.infoAction;
                effectInfo.pNotify = this;
                effectInfo.dwExtra = m_extraData.nFrameEnd;
                if (m_extraData.pCtrlEnd)
                {
                    effectInfo.pCtrl = m_extraData.pCtrlEnd;
                }
                Update_End_Region(effectInfo.rcEffect);
                m_pEffectEnd->Init(effectInfo);
                m_pEffectEnd->Play();
            }

            if (!m_pEffectEnd && m_nPlayEnd >= m_nEndTime)
            {
                Stop();
            }
            m_nPlayEnd += m_effectInfo.nTimeFrameInterval;
        }
        else
        {
             m_nPlay += step;
        }
    }
	return TRUE;
}

inline BOOL CBkEffectScroll::Action(NotifyInfo* pInfo)
{
    switch(pInfo->nAction)
    {
    case BKM_EFFECT_START:
        if (m_pEffectStart && pInfo->pFromEffect == m_pEffectStart)
        {
            if (m_effectInfo.infoAction.pNotify)
            {
                NotifyInfo info = {BKM_EFFECT_PART1_START, CPoint(0,0), m_effectInfo.infoAction.pCtrl, m_effectInfo.nType, this};
                m_effectInfo.infoAction.pNotify->Action(&info);
            }
        }
        break;
    case BKM_EFFECT_END:
        if (m_pEffectStart && pInfo->pFromEffect == m_pEffectStart)
        {
            m_nFrame = m_nStartTime;
            m_bStartFinished = TRUE;
            UpdateInfo();

            if (m_effectInfo.infoAction.pNotify)
            {
                NotifyInfo info = {BKM_EFFECT_PART1_END, CPoint(0,0), m_effectInfo.infoAction.pCtrl, m_effectInfo.nType, this};
                m_effectInfo.infoAction.pNotify->Action(&info);
            }
        }
        else if (m_pEffectEnd && pInfo->pFromEffect == m_pEffectEnd)
        {
            Stop();
        }
        break;
    }

    return TRUE;
}

inline void CBkEffectScroll::OnStart()
{
    if (m_pEffectStart )
    {
        EffectActionInfo effectInfo = m_effectInfo.infoAction;
        effectInfo.pNotify = this;
        effectInfo.dwExtra = m_extraData.nFrameStart;
        if (m_extraData.pCtrlStart)
        {
            effectInfo.pCtrl = m_extraData.pCtrlStart;
        }
        Update_Start_Region(effectInfo.rcEffect);
        m_pEffectStart->Init(effectInfo);
        m_pEffectStart->Play();
    }
}

inline void CBkEffectScroll::Render_Background(CDCHandle& dc, const CRect& rcDraw)
{
    if (m_effectInfo.infoAction.pCtrl)
    {
        CRect rcDrawBK;
        Update_Render_Region(rcDrawBK);
        dc.BitBlt(rcDrawBK.left, rcDrawBK.top, rcDrawBK.Width(), rcDrawBK.Height(),
                  m_MemDC, rcDrawBK.left, rcDrawBK.top, SRCCOPY);
    }
}

inline void CBkEffectScroll::Render_Scroll(CDCHandle& dc, const CRect& rcDraw)
{
    CRect rcCanvas = m_effectInfo.infoAction.rcEffect;
    rcCanvas.DeflateRect(&m_effectInfo.infoAction.rcEffectOffset);

    if (/*m_nFrame >= m_nStartTime*/m_bStartFinished && m_nPlay < rcCanvas.Height()-m_nEnd)
    {
        if (m_pSkinScroll)
        {
            CRect rcDrawScroll;
            Update_Scroll_Region(rcDrawScroll);
            rcDrawScroll.OffsetRect(m_nXOffsetScroll, m_nYOffsetScroll);
            m_pSkinScroll->Draw(dc, rcDrawScroll, 0);
        }
    }
}

inline void CBkEffectScroll::Update_Render_Region(CRect& rcDraw)
{
    rcDraw = m_effectInfo.infoAction.rcEffect;
    rcDraw.DeflateRect(&m_effectInfo.infoAction.rcEffectOffset);

    switch(m_effectInfo.nType)
    {
    case EffectScroll_Vert_Expand:
        rcDraw.bottom = m_effectInfo.infoAction.rcEffect.top + m_nPlay;
        break;
    case EffectScroll_Vert_Collaps:
        rcDraw.bottom = m_effectInfo.infoAction.rcEffect.top + m_effectInfo.infoAction.rcEffect.Height() - m_nPlay;
        break;
    }
}

inline void CBkEffectScroll::Update_Scroll_Region(CRect& rcDraw)
{
    Update_Render_Region(rcDraw);
    SIZE szSkin = m_pSkinScroll->GetSkinSize();
    switch(m_effectInfo.nType)
    {
    case EffectScroll_Vert_Expand:
    case EffectScroll_Vert_Collaps:
        rcDraw.top = rcDraw.bottom - szSkin.cy;
        break;
    }
}

inline void CBkEffectScroll::Update_Start_Region(CRect& rcDraw)
{
    rcDraw = m_effectInfo.infoAction.rcEffect;
    switch(m_effectInfo.nType)
    {
    case EffectScroll_Vert_Expand:
        rcDraw.bottom = rcDraw.top + m_nStart;
        break;
    case EffectScroll_Vert_Collaps:
        rcDraw.top = rcDraw.bottom - m_nStart;
        break;
    }
}

inline void CBkEffectScroll::Update_End_Region(CRect& rcDraw)
{
    rcDraw = m_effectInfo.infoAction.rcEffect;
    switch(m_effectInfo.nType)
    {
    case EffectScroll_Vert_Expand:
        rcDraw.top = rcDraw.bottom - m_nEnd;
        break;
    case EffectScroll_Vert_Collaps:
        rcDraw.bottom = rcDraw.top + m_nEnd;
        break;
    }
}