//////////////////////////////////////////////////////////////////////////
//  Class Name: CMemDC
// Description: Memory DC
//     Creator: DongM
//     Version: 2011.07.11 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#pragma once

class CMemDC : public CDC
{
public:
	CMemDC()
	{
		m_hBitmap = NULL;
	}
	~CMemDC()
	{
		if (m_hBitmap)
		{
			DeleteObject(m_hBitmap);
			m_hBitmap = NULL;
		}
	}
    BOOL Create(HDC hDC, int x, int y, int cx, int cy, BOOL bReLoad = FALSE)
    {
        return Create(hDC, CRect(x, y, cx, cy), bReLoad);
    }

    BOOL Create(HDC hDC, CRect& rc, BOOL bReLoad = FALSE)
    {
        if (!m_rc.EqualRect(rc) || bReLoad)
        {
            if (m_rc.Width() != rc.Width() || m_rc.Height() != rc.Height() || bReLoad)
            {
				if (m_hBitmap)
				{
					DeleteObject(m_hBitmap);
					m_hBitmap = NULL;
				}

                m_rc = rc;
                HDC hDCDesktop = ::GetDC(NULL);
                HDC hNewDC = ::CreateCompatibleDC(hDCDesktop);
                m_hBitmap = ::CreateCompatibleBitmap(hDCDesktop, m_rc.Width(), m_rc.Height());
                ::ReleaseDC(NULL, hDCDesktop);

                HGDIOBJ hOld = ::SelectObject(hNewDC, m_hBitmap);
                if (m_hDC != NULL)
                {
                    hOld = ::SelectObject(m_hDC, hOld);
                    ::DeleteObject(hOld);
                }

				Attach(hNewDC);
                OffsetViewportOrg(-m_rc.left, -m_rc.top);
                SetBkMode(TRANSPARENT);

                if (hDC != NULL)
                {
                    ::BitBlt(hNewDC, m_rc.left, m_rc.top, m_rc.Width(), m_rc.Height(), hDC, m_rc.left, m_rc.top, SRCCOPY);
                }

                return m_hBitmap != NULL;
            }
            else
            {
                OffsetViewportOrg(m_rc.left - rc.left, m_rc.top - rc.top);
                m_rc = rc;
                if (hDC != NULL)
                {
                    ::BitBlt(m_hDC, m_rc.left, m_rc.top, m_rc.Width(), m_rc.Height(), hDC, m_rc.left, m_rc.top, SRCCOPY);
                }
                return FALSE;
            }
        }
        return FALSE;
    }

private:
    CRect m_rc;
	HBITMAP m_hBitmap;
};
