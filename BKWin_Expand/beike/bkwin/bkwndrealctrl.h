#ifndef _BK_WND_REAL_CTRL_H_
#define _BK_WND_REAL_CTRL_H_

#include "bkwnd.h"

class CBkRealCtrl : public CBkWnd
{
    BKOBJ_DECLARE_CLASS_NAME(CBkRealCtrl, "realctrl")
    
    CBkRealCtrl()
    {
        m_nStyle = 0;
        m_nStyleEx = 0;
        m_hRealWnd = NULL;

        m_bWantReturn = FALSE;
        m_bWantEsc = FALSE;
        m_bWantTab = FALSE;
    }

    void SetIsWantReturn( BOOL isWantReturn )
    {
        m_bWantReturn = isWantReturn;
    }

    BOOL GetIsWantReturn()
    {
        return m_bWantReturn;
    }

    void SetIsWantEsc( BOOL isWantEsc)
    {
        m_bWantEsc = isWantEsc;
    }

    BOOL GetIsWantEsc()
    {
        return m_bWantEsc;
    }

    void SetIsWantTab( BOOL isWantTab )
    {
        m_bWantTab = isWantTab;
    }

    BOOL GetIsWantTab()
    {
        return m_bWantTab;
    }

    virtual void CreateHandle(HWND hParent)
    {
        if( m_hWndRealContainer )
        {
            hParent = m_hWndRealContainer;
        }

        CBkWnd::CreateHandle( hParent );
        if( !m_strClass.IsEmpty() )
        {
            m_nStyle |= WS_CHILD|WS_VISIBLE;
            m_hRealWnd = ::CreateWindowEx(m_nStyleEx, m_strClass, L"", m_nStyle
                , m_rcWindow.left, m_rcWindow.top, m_rcWindow.right - m_rcWindow.left, m_rcWindow.bottom - m_rcWindow.top
                , hParent, (HMENU)(LPARAM)m_uCmdID, _AtlBaseModule.GetModuleInstance(), NULL);
            if (m_hRealWnd == NULL)
            {
                DWORD dw = GetLastError(); 
                LPVOID lpMsgBuf;
                FormatMessage(
                    FORMAT_MESSAGE_ALLOCATE_BUFFER | 
                    FORMAT_MESSAGE_FROM_SYSTEM,
                    NULL,
                    dw,
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                    (LPTSTR) &lpMsgBuf,
                    0, NULL );
                CString strErr;
                strErr.Format(L"create %s(%d) failed with error %d: %s", 
                    m_strClass, m_uCmdID, dw, lpMsgBuf); 
                OutputDebugString(strErr);

                LocalFree(lpMsgBuf);
            }
            else
            {
                HFONT hFont = NULL;
                if (m_style.m_ftText)
                    hFont = m_style.m_ftText;
                else
                    hFont = BkFontPool::GetDefaultFont();
                ::SendMessage(m_hRealWnd, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
            }
        }
    }

    virtual BOOL IsFocus()
    {
        return (::GetFocus() == m_hRealWnd) ? TRUE : FALSE;
    }

    virtual HWND GetRealWnd()
    {
        return m_hRealWnd;
    }

    void SetRealWnd( HWND realWnd )
    {
        m_hRealWnd = realWnd;
        _RepositionSelf();
    }

    virtual void SetDisable(BOOL bDisable)
    {
        __super::SetDisable( bDisable );

        if( GetRealWnd() )
        {
            ::EnableWindow( GetRealWnd(),!bDisable );
        }
    }

    virtual void DrawDisableShade(CDCHandle& dc)
    {
        if (m_hRealWnd)
        {
            CRect rcWindow;
            ::GetWindowRect(m_hRealWnd, rcWindow);

            CWindow wnd(GetContainer());
            wnd.ScreenToClient(&rcWindow);

            HDC hDC = ::GetDC(m_hRealWnd);
            ::BitBlt(dc.m_hDC, rcWindow.left, rcWindow.top, rcWindow.Width(), rcWindow.Height(), hDC, 0, 0, SRCCOPY);
            ::ReleaseDC(m_hRealWnd, hDC);
        }
    }

    BOOL OnDestroy()
    {
        CBkWnd::OnDestroy();
        if( ::IsWindow(m_hRealWnd) )
        {
            ::DestroyWindow(m_hRealWnd);
            SetRealWnd(NULL);
        }
        return FALSE;
    }

    BOOL OnPaint(CDCHandle dc)
    {
        return FALSE;
    }

    virtual void OnVisibleChanged(BOOL bVisible)
    {
        if (m_hRealWnd != NULL)
        {
            ::ShowWindow(m_hRealWnd, (bVisible&&IsVisibleDeep(FALSE)) ? SW_SHOW : SW_HIDE);
            //::RedrawWindow(m_hRealWnd,NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE);
        }
    }

    virtual void OnDisableChanged(BOOL bDisable)
    {
        if (m_hRealWnd != NULL)
        {
            ::EnableWindow(m_hRealWnd, bDisable ? FALSE : TRUE);
            ::RedrawWindow(m_hRealWnd,NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE);
        }
    }

    virtual void OnEffectChanged(BOOL bEffect)
    {
        if (m_hRealWnd != NULL)
        {
            ::ShowWindow(m_hRealWnd, bEffect ? SW_HIDE : SW_SHOW);
        }
    }

    BOOL OnWindowPosChanged(LPWINDOWPOS lpWndPos)
    {
        __super::OnWindowPosChanged(lpWndPos);
        
        if( m_hRealWnd )
        {
            ::MoveWindow(m_hRealWnd, m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height(), TRUE);

            CRect rc;
            ::GetWindowRect(m_hRealWnd, rc);
            HWND hWnd = GetContainer();
            ::ScreenToClient(hWnd, &rc.TopLeft());
            ::ScreenToClient(hWnd, &rc.BottomRight());
            m_rcWindow = rc;
        }
        return FALSE;
    }

protected:
    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_TSTRING_ATTRIBUTE("ctrl", m_strClass, FALSE)
        BKWIN_HEX_ATTRIBUTE("style", m_nStyle, FALSE)
        BKWIN_HEX_ATTRIBUTE("styleEx", m_nStyleEx, FALSE)
        BKWIN_UINT_ATTRIBUTE("want_return", m_bWantReturn, FALSE)
        BKWIN_UINT_ATTRIBUTE("want_esc", m_bWantEsc, FALSE)
        BKWIN_UINT_ATTRIBUTE("want_tab", m_bWantTab, FALSE)
    BKWIN_DECLARE_ATTRIBUTES_END()

protected:
    CString m_strClass;
    UINT m_nStyle;
    UINT m_nStyleEx;
    HWND m_hRealWnd;

    BOOL m_bWantReturn;     // 是否支持RETURN键
    BOOL m_bWantEsc;        // 是否支持ESC键
    BOOL m_bWantTab;        // 是否支持TAB键
};


#endif