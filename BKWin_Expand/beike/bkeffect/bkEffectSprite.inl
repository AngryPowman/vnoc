inline CBkEffectSprite::CBkEffectSprite()
:m_pImageSprite(NULL)
,m_nAligment(EffectSprite_Aligment_Left)
,m_nSpriteWidth(0)
,m_nFrames(1)
,m_nStart(0)
,m_nEnd(0)
,m_nPlay(0)
,m_nProcess(1.0)
{
    m_effectInfo.nType = EffectSprite_Once;
}

inline CBkEffectSprite::~CBkEffectSprite()
{

}

inline int CBkEffectSprite::GetPlayFrame()
{
    return m_nPlay;
}

inline void CBkEffectSprite::Init(EffectActionInfo info)
{
    CBkEffectObject::Init(info);

    if (m_nStart == -1)
    {
        m_nStart = 0;
    }

    if (m_nEnd == -1)
    {
        if (m_nSpriteWidth > 0 && m_pImageSprite)
        {
            m_nFrames = m_pImageSprite->GetWidth()/m_nSpriteWidth;
        }
        m_nEnd = m_nFrames;
    }

    m_nPlay = m_nStart;

    if (info.dwExtra > 0)
    {
        m_nStart = info.dwExtra;
        m_nPlay = m_nStart;
    }
}

inline void CBkEffectSprite::Render(CDCHandle& dc)
{
    Gdiplus::Graphics g(dc);
    Render_Sprite(g, m_effectInfo.infoAction.rcEffect);
}

inline BOOL CBkEffectSprite::UpdateInfo()
{
    if (m_nStart < m_nEnd)
    {
        if (m_nPlay >= m_nEnd)
        {
            switch(m_effectInfo.nType)
            {
            case EffectSprite_Once:
                Stop();
                break;
            case EffectSprite_Loop:
                m_nPlay = m_nStart;
                break;
            case EffectSprite_Loop_Bidirection:
                m_nPlay = m_nEnd;
                SwapFrames();
                break;
            }
        }
        else
        {
            m_nPlay ++;
        }
    }
    else
    {
        if (m_nPlay <= m_nEnd)
        {
            switch(m_effectInfo.nType)
            {
            case EffectSprite_Once:
                Stop();
                break;
            case EffectSprite_Loop:
                m_nPlay = m_nStart;
                break;
            case EffectSprite_Loop_Bidirection:
                m_nPlay = m_nEnd;
                SwapFrames();
                break;
            }
        }
        else
        {
            m_nPlay --;
        }
    }
	return TRUE;
}

inline BOOL CBkEffectSprite::OnPreStart()
{
    return TRUE;
}

inline BOOL CBkEffectSprite::OnPreEnd()
{
    return TRUE;
}

inline void CBkEffectSprite::Render_Sprite(Gdiplus::Graphics& g, const CRect& rcDraw)
{
    if (m_pImageSprite)
    {
        SIZE size = {0, 0};
        size.cx = m_nSpriteWidth;
        size.cy = m_pImageSprite->GetHeight();

        int xSrc = m_nPlay*m_nSpriteWidth;
        int ySrc = 0;
        int xDes = rcDraw.left;
        int yDes = rcDraw.top;

        switch(m_nAligment)
        {
        case EffectSprite_Aligment_Left:
            break;
        case EffectSprite_Aligment_Right:
            xDes = rcDraw.right - m_nSpriteWidth;
            break;
        }

        CBkPngSkin skin;
        skin.DrawImage( g, *m_pImageSprite, xDes, yDes, size.cx, size.cy, xSrc, ySrc, size.cx, size.cy );
    }
}

inline void CBkEffectSprite::SwapFrames()
{
    int tmp = m_nEnd;
    m_nEnd = m_nStart;
    m_nStart = tmp;
}