inline CBkEffectPositionFrame::CBkEffectPositionFrame()

{
}

inline CBkEffectPositionFrame::~CBkEffectPositionFrame()
{
}

inline void CBkEffectPositionFrame::Init(EffectActionInfo info)
{
    CBkEffectObject::Init(info);
	m_rcOriginal=m_rcDraw = info.rcEffect;
}

inline void CBkEffectPositionFrame::Render(CDCHandle& dc)
{
	if (m_effectInfo.infoAction.pCtrl)
	{
		CRgn rgnWnd;
		rgnWnd.CreateRectRgn(m_rcDraw.left, m_rcDraw.top, m_rcDraw.right+1, m_rcDraw.bottom+1);
		dc.SelectClipRgn(rgnWnd,RGN_OR);
		dc.BitBlt(m_rcDraw.left,m_rcDraw.top,m_rcDraw.Width(),m_rcDraw.Height(),m_MemDC,m_rcOriginal.left,m_rcOriginal.top, SRCCOPY);
	}
}

inline BOOL CBkEffectPositionFrame::UpdateInfo()
{
	if (m_effectInfo.infoAction.pCtrl)
	{
		CBkWnd * pwnd = m_effectInfo.infoAction.pCtrl;
		CRect invalidRect = m_rcDraw,parentRect;
		BKMath::Vector2 &temp = m_spline.evaluate(m_nFrame);
		CBkWnd *pWndParent = BkWnds::GetWindow(pwnd->GetParent());
		pWndParent->GetRect(&parentRect);
		m_rcDraw.MoveToXY(parentRect.left+temp.x,parentRect.top+temp.y);
		invalidRect.UnionRect(invalidRect,m_rcDraw);
		CBkWnd * pct = m_effectInfo.infoAction.pCtrl;
		WINDOWPOS WndPos = {0};
		WndPos.x = m_rcDraw.left;
		WndPos.y = m_rcDraw.top;
		WndPos.cx = m_rcDraw.Width();
		WndPos.cy = m_rcDraw.Height();
		pct->OnWindowPosChanged(&WndPos);
		m_effectInfo.infoAction.pCtrl->NotifyInvalidateRect(invalidRect);
	}
	return FALSE;
}
inline BOOL CBkEffectPositionFrame::OnPreStart()
{
	return TRUE;
}

inline BOOL CBkEffectPositionFrame::OnPreEnd()
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


inline BOOL CBkEffectPositionFrame::_CreateEffectImage()
{
	BOOL bRet = FALSE;
	if (m_effectInfo.infoAction.pCtrl)
	{
		HWND hWnd = m_effectInfo.infoAction.pCtrl->GetContainer();
		if (hWnd)
		{
			CRect rc;
			rc.left = 0;
			rc.right = m_rcDraw.right;
			rc.top = 0;
			rc.bottom = m_rcDraw.bottom;
			HDC hDC = ::GetDC(hWnd);
			bRet = m_MemDC.Create(hDC, rc, TRUE);
			::ReleaseDC(hWnd, hDC);
		}
	}
	::SetViewportOrgEx(m_MemDC.m_hDC,0,0,0);
	CRgn rgnWnd,trgn;
	HRGN hRgn = rgnWnd.CreateRectRgn(m_rcDraw.left,m_rcDraw.top,m_rcDraw.right,m_rcDraw.bottom);
	int nRet = m_MemDC.SelectClipRgn(hRgn, RGN_OR);

	if (m_effectInfo.infoAction.pCtrl)
	{
		CBkWnd * pct = m_effectInfo.infoAction.pCtrl;
		pct->RedrawRegion(CDCHandle(m_MemDC.m_hDC));
	}
	return TRUE;
}