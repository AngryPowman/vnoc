inline CBkEffectRealMove::CBkEffectRealMove()
:m_pDirection(0,0)

{
}

inline CBkEffectRealMove::~CBkEffectRealMove()
{
}

inline void CBkEffectRealMove::Init(EffectActionInfo info)
{
    CBkEffectObject::Init(info);
	m_rcOriginal=m_rcDraw = info.rcEffect;
}

inline void CBkEffectRealMove::Render(CDCHandle& dc)
{
	if (m_effectInfo.infoAction.pCtrl)
	{
		CRgn rgnWnd;
		rgnWnd.CreateRectRgn(m_rcDraw.left, m_rcDraw.top, m_rcDraw.right+1, m_rcDraw.bottom+1);
		dc.SelectClipRgn(rgnWnd,RGN_OR);
		dc.BitBlt(m_rcDraw.left,m_rcDraw.top,m_rcDraw.Width(),m_rcDraw.Height(),m_MemDC,m_rcOriginal.left,m_rcOriginal.top, SRCCOPY);
	}
}

inline BOOL CBkEffectRealMove::UpdateInfo()
{
	if (m_effectInfo.infoAction.pCtrl)
	{
		CRect invalidRect = m_rcDraw;
		m_rcDraw.top += m_pDirection.y;
		m_rcDraw.bottom += m_pDirection.y;
		m_rcDraw.left += m_pDirection.x;
		m_rcDraw.right += m_pDirection.x;
		invalidRect.bottom = m_rcDraw.bottom;
		invalidRect.right = m_rcDraw.right;
		m_effectInfo.infoAction.pCtrl->NotifyInvalidateRect(invalidRect);
	}
	return FALSE;
}
inline BOOL CBkEffectRealMove::OnPreStart()
{
	if (m_effectInfo.infoAction.pCtrl)
	{
		CBkWnd * pct = m_effectInfo.infoAction.pCtrl;
		pct->RedrawRegion(CDCHandle(m_MemDC.m_hDC));
	}
	return TRUE;
}

inline BOOL CBkEffectRealMove::OnPreEnd()
{
	/*if (m_effectInfo.infoAction.pCtrl)
	{
		CBkWnd * pct = m_effectInfo.infoAction.pCtrl;
		WINDOWPOS WndPos = {0};
		WndPos.x = m_rcOriginal.left;
		WndPos.y = m_rcOriginal.top;
		WndPos.cx = m_rcOriginal.Width();
		WndPos.cy = m_rcOriginal.Height();
		pct->OnWindowPosChanged(&WndPos);
		return TRUE;
	}*/
	return TRUE;
}

