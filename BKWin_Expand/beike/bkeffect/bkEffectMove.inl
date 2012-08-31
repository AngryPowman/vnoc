inline CBkEffectMove::CBkEffectMove()
:m_nStepHorz(0)
,m_nStepVert(0)
,m_rcDraw(CRect(0,0,0,0))
{
}

inline CBkEffectMove::~CBkEffectMove()
{
}

inline void CBkEffectMove::Init(EffectActionInfo info)
{
    CBkEffectObject::Init(info);
    Init_Render_Region(m_rcDraw, m_rcDrawTo);
}

inline void CBkEffectMove::Render(CDCHandle& dc)
{
    Render_Move(dc);
}

inline BOOL CBkEffectMove::UpdateInfo()
{
    Update_Render_Region( m_rcDraw, m_rcDrawTo);
	return TRUE;
}

inline void CBkEffectMove::Render_Move(CDCHandle& dc)
{
	m_MemDC.m_hDC;
    if (m_effectInfo.infoAction.pCtrl)
    {
        dc.BitBlt(m_rcDrawTo.left, m_rcDrawTo.top, m_rcDrawTo.Width(), m_rcDrawTo.Height(),
                  m_MemDC, m_rcDraw.left, m_rcDraw.top, SRCCOPY);
    }
}

inline void CBkEffectMove::Init_Render_Region(CRect& rcDraw, CRect& rcDrawTo)
{
    if (m_effectInfo.infoAction.pCtrl)
    {
        CRect rcCtrl;
        m_effectInfo.infoAction.pCtrl->GetRect(&rcCtrl);

        m_rcDraw = rcCtrl;
        m_rcDrawTo = m_effectInfo.infoAction.rcEffect;
        switch (m_nDirectionFrom)
        {
        case EffectMove_Left:
            switch (m_nDirectionTo)
            {
            case EffectMove_Right:
                m_rcDraw.left = m_rcDraw.right;
                m_rcDrawTo.right = m_rcDrawTo.left;
                break;
            }
            break;
        case EffectMove_Right:
            switch (m_nDirectionTo)
            {
            case EffectMove_Left:
                m_rcDraw.right = m_rcDraw.left;
                m_rcDrawTo.left = m_rcDrawTo.right;
                break;
            }
            break;
        }
    }
}

inline void CBkEffectMove::Update_Render_Region(CRect& rcDraw, CRect& rcDrawTo)
{

	////支持支上下左右四个方向移动
	//int xDir = 0;
	//int yDir = 0;
	//if(m_nDirectionFrom == EffectMove_Left)
	//	xDir = -1;
	//else if(m_nDirectionFrom == EffectMove_Right)
	//	xDir = 1;
	//if(m_nDirectionTo == EffectMove_Up)
	//	yDir = -1;
	//else if(m_nDirectionFrom == EffectMove_Down)
	//	yDir = 1;
	////由于只支持四个方向,则方向向量x,y坐标必不同
	//assert(xDir ^ yDir);


	//m_rcDraw.left += m_nStepHorz * xDir;
	//m_rcDrawTo.right -= m_nStepHorz * xDir;
	//m_rcDraw.top += m_nStepVert * yDir;
	//m_rcDrawTo.bottom -= m_nStepVert * yDir;
    switch (m_nDirectionFrom)
    {
    case EffectMove_Left:
        switch (m_nDirectionTo)
        {
        case EffectMove_Left:
            m_rcDraw.left += m_nStepHorz;
            m_rcDrawTo.right -= m_nStepHorz;
            break;
        case EffectMove_Right:
            m_rcDraw.left -= m_nStepHorz;
            m_rcDrawTo.right += m_nStepHorz;
            break;
        }

        break;
    case EffectMove_Right:
        switch (m_nDirectionTo)
        {
        case EffectMove_Left:
            m_rcDraw.right += m_nStepHorz;
            m_rcDrawTo.left -= m_nStepHorz;
            break;
        case EffectMove_Right:
            m_rcDraw.right -= m_nStepHorz;
            m_rcDrawTo.left += m_nStepHorz;
            break;
        }
        break;
    }
}