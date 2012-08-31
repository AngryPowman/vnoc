
//////////////////////////////////////////////////////////////////////////
//  Class Name: CBkEdit
// Description: 编辑框控件(窗口控件)
//     Creator: YangDa
//     Version: 2011.04.15 - 1.0 - YangDa - Create
//              2011.04.18 - 1.1 - YangDa - 自绘.基础文本功能
//				2011.04.21 - 1.2 - YangDa - 调整真窗口位置和显示操作接口
//				2011.05.31 - 1.3 - YangDa - 添加默认文字支持
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "bkwndrealctrl.h"
#include "bkmsg.h"

class CBkEdit : public CWindowImpl< CBkEdit,WTL::CEdit /*, CWinTraitsOR< ES_AUTOHSCROLL >*/ >
			  , public CBkRealCtrl
{
	BKOBJ_DECLARE_CLASS_NAME(CBkEdit, "edit")
	DECLARE_WND_SUPERCLASS(_T("BKEdit") , WC_EDIT );

	typedef CWindowImpl< CBkEdit,WTL::CEdit /*, CWinTraitsOR<  ES_AUTOHSCROLL >*/ > __baseClass;
	typedef CBkRealCtrl __bkClass;

public:
    CBkEdit()
        :m_bReadOnly(FALSE)
        ,m_bNumber(FALSE)
        ,m_bMultiline(FALSE)
        ,m_bWantReturn(FALSE)
        ,m_bVScroll(FALSE)
        ,m_bWrap(FALSE)
        ,m_bPassword(FALSE)
        ,m_bSelect(TRUE)
        ,m_bRight(FALSE)
        ,m_maxLength(0)
        ,m_bShadow(FALSE)
        ,m_bMouseHover(NULL)
        ,m_brushBg(NULL)
        ,m_cursorIBeam(NULL)
        ,m_bShowDefaultText(FALSE)
    {
    }

	virtual ~CBkEdit()
	{
		if (m_brushBg)
		{
			DeleteObject( m_brushBg );
			m_brushBg = NULL;
		}
	}

public:
	virtual void SetText(LPCTSTR szText)
	{
        m_bShowDefaultText = FALSE;
		m_strInnerText = szText;
		SetWindowText( szText );
	}

    virtual LPCTSTR GetText()
    {
        GetWindowText( m_strInnerText );
        return m_strInnerText;
    }

    BOOL GetIsDefaultText()
    {
        return m_bShowDefaultText;
    }

    void SetDefaultText(LPCTSTR szText)
    {
        m_bShowDefaultText = TRUE;
        m_strDefaultText = szText;
        SetWindowText( szText );
    }

    ////// Base Interface
	virtual void CreateHandle( HWND hParent )
	{
        if( m_hWnd )
        {
            ::DestroyWindow(m_hWnd);
        }

        if( m_hWndRealContainer )
        {
            hParent = m_hWndRealContainer;
        }

        CBkRealCtrl::CreateHandle( hParent );

		DWORD dwStyle = _ComposeStyle();
		m_hRealWnd = Create( hParent,m_rcWindow,NULL,dwStyle );

		if( m_maxLength > 0 )
		{
			SetLimitText( m_maxLength );
		}

		SetFont( BkFontPool::GetFont(BKF_DEFAULTFONT) );

        if( !m_strInnerText.IsEmpty() )
        {
            SetWindowText( m_strInnerText );
        }

        if( !m_strDefaultText.IsEmpty() )
        {
            SetDefaultText( m_strDefaultText );
        }
	}


    BOOL Load( TiXmlElement* pTiXmlElem )
    {
        if (__super::Load(pTiXmlElem))
        {
            m_strInnerText = CA2T(pTiXmlElem->GetText(), CP_UTF8);
            //CreateHandle( GetContainer() );

            if( !m_strInnerText.IsEmpty() )
            {
                SetWindowText( m_strInnerText );
            }

            if( !m_strDefaultText.IsEmpty() )
            {
                SetDefaultText( m_strDefaultText );
            }
        }
        return TRUE;
    }

    virtual void SetFocus()
    {
        __bkClass::SetFocus();
    }

    virtual void KillFocus()
    {
        __bkClass::KillFocus();
    }

protected:
    BOOL SubclassWindow( HWND hWnd )
    {
        CWindow( hWnd ).ModifyStyle( WS_BORDER,  0, 0 );	
        CWindow( hWnd ).ModifyStyleEx( WS_EX_CLIENTEDGE,  0, 0 );	
        return __baseClass::SubclassWindow(hWnd);
    }

    virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
    {
        switch( nChar )
        {
        case VK_RETURN:
            {
                if( m_bMultiline || m_bWantReturn )
                {
                    return FALSE;
                }

                if( m_pNotify )
                {
                    NotifyInfo info = { BKM_EDIT_PRESSENTER, CPoint(0, 0), this, 0 };
                    m_pNotify->Action( &info );
                }
            }
            break;
        case VK_ESCAPE:
            {
                if( m_pNotify )
                {
                    NotifyInfo info = { BKM_EDIT_PRESSESC, CPoint(0, 0), this, 0 };
                    m_pNotify->Action( &info );
                }
            }
            break;
        case 0x41: //A
            if( GetKeyState(VK_CONTROL) & 0x8000 )	// CTRL+A
            {
                SetSel( 0,-1 );
            }
            break;
        default:
            break;
        }

        return TRUE;
    }

protected:
    DWORD _ComposeStyle()
    {
        DWORD dwStyle = WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|ES_AUTOHSCROLL;

		if( m_bReadOnly )
			dwStyle |= ES_READONLY;
		if( m_bNumber )
			dwStyle |= ES_NUMBER;
		if( m_bMultiline )
			dwStyle |= ES_MULTILINE;
        if( m_bVScroll )
            dwStyle |= WS_VSCROLL;
        if( m_bPassword )
            dwStyle |= ES_PASSWORD;
        if( m_bWrap )
            dwStyle &= ~ES_AUTOHSCROLL;
        if( m_bRight )
            dwStyle |= ES_RIGHT;

		return dwStyle;
	}

    void _GetColors( COLORREF &clrBack,COLORREF &clrBorder,COLORREF &clrText,BOOL isOtherColor = FALSE )
    {
        CPoint ptCursor;
        GetCursorPos( &ptCursor );
        ::ScreenToClient(m_hWnd, &ptCursor );
        CRect rcC;
        GetClientRect( &rcC );

        CBkSkinEdit* pSkin = (CBkSkinEdit*)BkSkin::GetSkin(m_style.m_strSkinName);
        if( pSkin )
        {
            BOOL bHover = rcC.PtInRect(ptCursor);
            BOOL bReadOnly = (__baseClass::GetStyle() & ES_READONLY) || m_bReadOnly;
            BOOL bDisable = !IsWindowEnabled();

            DWORD dwState = BkWndState_Normal;
            if( isOtherColor ) { dwState |= BkWndState_Check; }
            else if( bReadOnly ) { dwState |= BkWndState_PushDown; }
            else if( bDisable ) { dwState |= BkWndState_Disable; }
            else if( bHover ) { dwState |= BkWndState_Hover; }

            pSkin->GetColors( clrBack,clrBorder,clrText,dwState );
        }
    }

public:
	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_INT_ATTRIBUTE("readonly", m_bReadOnly, FALSE)
		BKWIN_INT_ATTRIBUTE("number", m_bNumber, FALSE)
		BKWIN_INT_ATTRIBUTE("multiline", m_bMultiline, FALSE)
        BKWIN_INT_ATTRIBUTE("wantreturn", m_bWantReturn, FALSE)
        BKWIN_INT_ATTRIBUTE("vscroll", m_bVScroll, FALSE)
        BKWIN_INT_ATTRIBUTE("password", m_bPassword, FALSE)
        BKWIN_INT_ATTRIBUTE("wrap", m_bWrap, FALSE)
        BKWIN_INT_ATTRIBUTE("right", m_bRight, FALSE)
		BKWIN_UINT_ATTRIBUTE("maxlength", m_maxLength, FALSE)
		BKWIN_INT_ATTRIBUTE("shadow", m_bShadow, FALSE)
        BKWIN_TSTRING_ATTRIBUTE("default_text", m_strDefaultText, FALSE)
	BKWIN_DECLARE_ATTRIBUTES_END()

	BEGIN_MSG_MAP( CBkEdit )
		//MESSAGE_HANDLER( WM_CHAR, OnChar )
		MESSAGE_HANDLER( WM_CREATE, OnCreate )
        MESSAGE_HANDLER( WM_ERASEBKGND, OnEraseBkgnd )
        MESSAGE_HANDLER( WM_GETDLGCODE, OnGetDlgCode )
		MESSAGE_HANDLER( WM_LBUTTONDOWN, OnLButtonDown )
        MESSAGE_HANDLER( WM_LBUTTONDBLCLK, OnLButtonDBClick )
        MESSAGE_HANDLER( WM_KEYDOWN, OnKeyDown )
        MESSAGE_HANDLER( WM_KEYUP, OnKeyUp )
		MESSAGE_HANDLER( WM_MOUSELEAVE, OnMouseLeave )
		MESSAGE_HANDLER( WM_MOUSEMOVE, OnMouseMove )
		MESSAGE_HANDLER( WM_NCCALCSIZE, OnNcCalcSize )
		MESSAGE_HANDLER( WM_NCPAINT , OnNcPaint )
        MESSAGE_HANDLER( WM_SETFOCUS, OnWndSetFocus )
		MESSAGE_HANDLER( WM_KILLFOCUS, OnWndKillFocus )
		MESSAGE_HANDLER( OCM_CTLCOLOREDIT, OnCtlColorEdit )
		MESSAGE_HANDLER( OCM_CTLCOLORSTATIC, OnCtlColorEdit )
		MESSAGE_HANDLER(WM_SYSKEYDOWN, OnSysKeyDown)
		REFLECTED_COMMAND_CODE_HANDLER( EN_CHANGE,OnTextChanged )
		//DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()
    
	LRESULT OnSysKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{ 
		bHandled  = FALSE;
		//禁用alt + F4
		if (wParam == VK_F4)
		{
            bHandled = TRUE;
			return FALSE;
		}
		return TRUE;
	}
	LRESULT OnChar( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
	{
		return FALSE;
	}

	LRESULT OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
	{
		CREATESTRUCT* lpCreate = (CREATESTRUCT*)lParam;

		lpCreate->style &= ~WS_BORDER;
		lpCreate->dwExStyle &= ~WS_EX_CLIENTEDGE;

		return DefWindowProc( uMsg,wParam,lParam );
		return FALSE;
	}

    LRESULT OnEraseBkgnd( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
    {
        bHandled = FALSE;
        return TRUE;
    }

    LRESULT OnGetDlgCode(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if( VK_TAB == wParam && GetIsWantTab()  )
        {
            return DLGC_WANTARROWS | DLGC_WANTALLKEYS | DLGC_HASSETSEL | DLGC_WANTCHARS;
        }

        if( VK_RETURN == wParam && GetIsWantReturn() )
        {
            return DLGC_WANTARROWS | DLGC_WANTALLKEYS | DLGC_HASSETSEL | DLGC_WANTCHARS;
        }

        if( VK_ESCAPE == wParam && GetIsWantEsc()  )
        {
            return DLGC_WANTARROWS | DLGC_WANTALLKEYS | DLGC_HASSETSEL | DLGC_WANTCHARS;
        }

        return 0;
    }

	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
	{
		bHandled = FALSE;
		return TRUE;
	}

    LRESULT OnLButtonDBClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
    {
        bHandled = FALSE;
        return TRUE;
    }

    LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
    {
        bHandled = FALSE;
        OnKeyDown( (TCHAR)wParam, (UINT)lParam & 0xFFFF, (UINT)((lParam & 0xFFFF0000) >> 16) );

        return FALSE;
    }

    LRESULT OnKeyUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
    {
        if (m_pNotify != NULL)
        {
            NotifyInfo ni = {WM_KEYUP, CPoint(0,0), this};
            m_pNotify->Action(&ni);
        }
        bHandled = FALSE;
        return FALSE;
    }

	LRESULT OnMouseLeave( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
	{
        SetCursor( LoadCursor( NULL,IDC_ARROW ) );

		m_bMouseHover = FALSE;
		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME);
		bHandled = FALSE;
		return 1L;
	}

	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
	{
		CPoint ptCursor;
		GetCursorPos( &ptCursor );
		::ScreenToClient( m_hWnd, &ptCursor );

        SetCursor( LoadCursor( NULL,IDC_IBEAM ) );

		CRect rcC;
		GetClientRect(&rcC);
    
		bHandled = FALSE;
        
        if( !m_bSelect )
        {
            bHandled = TRUE;
        }

		if( m_bMouseHover )
		{
			return 1L;
		}

		m_bMouseHover = TRUE;
		TRACKMOUSEEVENT csTME;
		csTME.cbSize = sizeof(csTME);
		csTME.dwFlags = TME_LEAVE;
		csTME.hwndTrack = m_hWnd;
		::_TrackMouseEvent( &csTME );
		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME);
		return 1L;
	}

	LRESULT OnNcCalcSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
	{
		BOOL bCalcValidRects = ( BOOL )wParam;
		if ( !bCalcValidRects )
		{
			bHandled = FALSE;
			return 0L;
		}

		CRect rc;
		GetWindowRect( rc );
		LPNCCALCSIZE_PARAMS lpncsp = ( LPNCCALCSIZE_PARAMS )lParam;

        int nXGap = m_bShadow?2:(m_style.m_nMarginX?m_style.m_nMarginX:1);
		int nYGap = m_bShadow?2:(m_style.m_nMarginY?m_style.m_nMarginY:1);

		lpncsp->rgrc[0].left += nXGap;
		lpncsp->rgrc[0].right -= nXGap;
		lpncsp->rgrc[0].top += nYGap ;
		lpncsp->rgrc[0].bottom -= nYGap;

		WINDOWPOS* lwp = lpncsp->lppos;
		if( lwp )
		{
			lwp->flags |= SWP_NOREDRAW;
		}
		bHandled = FALSE;
		return 0L;
	}

	LRESULT OnNcPaint( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
	{
		CRect rcWindow , rcClient;
		GetWindowRect( &rcWindow );
		GetClientRect( &rcClient );
		::MapWindowPoints( NULL,m_hWnd,(LPPOINT)&rcWindow,2 );
		rcClient.OffsetRect( -rcWindow.left,-rcWindow.top );
		rcWindow.OffsetRect( -rcWindow.left,-rcWindow.top );

		CWindowDC dc( m_hWnd );
		int nRet = dc.ExcludeClipRect( rcClient );
		if( nRet == ERROR || nRet == NULLREGION ) return 0L;

		COLORREF clrBack,clrBorder,clrText;
		_GetColors( clrBack,clrBorder,clrText );

		COLORREF clrSelct,clrShadow,clrDefault;
		_GetColors( clrSelct,clrShadow,clrDefault,TRUE );

		CRect rcBorder = rcWindow;
		//rcBorder.DeflateRect( 1,1 );
        HBRUSH brushBK = ::CreateSolidBrush( clrBack );
		HBRUSH brushBorder = ::CreateSolidBrush( clrBorder );
        dc.FillRect( &rcBorder,brushBK );
		dc.FrameRect( &rcBorder,brushBorder );
		::DeleteObject( brushBorder );
        ::DeleteObject( brushBK );

		//if( m_bShadow )
		//{
		//	CRect rcShadow = rcWindow;
		//	HBRUSH brushShadow = ::CreateSolidBrush( clrShadow );
		//	dc.FrameRect( &rcShadow,brushShadow );
		//	::DeleteObject( brushShadow );
		//}

		bHandled = FALSE;
		return 0L;
	}

	LRESULT OnWndSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
	{
		bHandled = FALSE;

        if( !m_bReadOnly )
        {
            if( m_bShowDefaultText )
            {
                m_bShowDefaultText = FALSE;
                SetWindowText( L"" );
            }
        }

        m_bFocus = TRUE;
        OnSetFocus( this );

        if( m_pNotify )
        {
            NotifyInfo info = { WM_SETFOCUS, CPoint(0, 0), this, 0 };
            m_pNotify->Action( &info );
        }

		return 0;
	}

	LRESULT OnWndKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
	{
		bHandled = FALSE;

        KillFocus();
        m_bFocus = FALSE;

        CString strText;
        GetWindowText( strText );
        if( !m_bReadOnly )
        {
            if( !m_bShowDefaultText )
            {
                if( strText.IsEmpty() )
                {
                    m_bShowDefaultText = TRUE;
                    SetWindowText( m_strDefaultText );
                }
            }
        }

        if( m_pNotify )
        {
            NotifyInfo info = { WM_KILLFOCUS, CPoint(0, 0), this, wParam };
            m_pNotify->Action( &info );
        }

		return 0;
	}

	LRESULT OnCtlColorEdit(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
	{
		HBRUSH hBrush = NULL;
		CDCHandle dc( (HDC)wParam );
		COLORREF clrBack,clrBorder,clrText;
		_GetColors( clrBack ,clrBorder , clrText );

        if( m_bShowDefaultText )
        {
            COLORREF clrSelct,clrShadow,clrDefault;
            _GetColors( clrSelct,clrShadow,clrDefault,TRUE );
            clrText = clrDefault;
        }

		if( OCM_CTLCOLOREDIT == uMsg || OCM_CTLCOLORSTATIC == uMsg )
		{
			COLORREF rgbText = clrText;
			dc.SetTextColor( clrText );
			dc.SetBkColor( clrBack );
			dc.SetBkMode( OPAQUE );
			if( m_brushBg )
			{
				::DeleteObject( m_brushBg );
				m_brushBg = NULL;
			}	
			m_brushBg = ::CreateSolidBrush( clrBack );
			bHandled = TRUE;
			return (LRESULT)m_brushBg;
		}
		else
		{
			bHandled = FALSE;
		}
		return (LRESULT)hBrush;
	}

	LRESULT OnTextChanged( UINT uMsg, int nCtlID, HWND hWnd, BOOL& bHandled )
	{
		bHandled = FALSE;

        CString strText;
        GetWindowText( strText );

        if( !m_bShowDefaultText )
        {
            if( m_strInnerText != strText )
            {
                if( m_pNotify )
                {
                    NotifyInfo info = { BKM_EDIT_TEXTCHANGE, CPoint(0, 0), this, 0 };
                    m_pNotify->Action( &info );
                }
            }
        }

		return FALSE;
	}

protected:
	BOOL	m_bReadOnly;		// 是否只读
	BOOL	m_bNumber;			// 是否仅能表示数字
	BOOL	m_bMultiline;		// 是否为多行
    BOOL    m_bWantReturn;      // 多行时是否支持回车换行,不支持则回车确认
    BOOL    m_bVScroll;         // 是否有垂直滚动条
    BOOL    m_bPassword;        // 是否按照密码形式显示
    BOOL    m_bWrap;            // 是否自动换行
    BOOL    m_bSelect;          // 是否能选择文字
    BOOL    m_bRight;           // 是否靠右排列文字
	UINT	m_maxLength;		// 最大文字个数

	BOOL	m_bShadow;			// 是否显示阴影

    BOOL    m_bShowDefaultText; // 是否正在显示默认文字


	BOOL	m_bMouseHover;		// Hover标志
	HBRUSH	m_brushBg;			// 背景画刷
    HCURSOR m_cursorIBeam;      // I文字鼠标指针
    CString m_strInnerText;     // 内部字符串
    CString m_strDefaultText;   // 默认文字
};