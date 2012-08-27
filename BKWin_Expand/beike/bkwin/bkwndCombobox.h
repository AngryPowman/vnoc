//////////////////////////////////////////////////////////////////////////
//  Class Name: CBkCombobox
// Description: COMBOBOX控件(窗口控件)
//     Creator: YangDa
//     Version: 2011.05.26 - 1.0 - YangDa - Create
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "bkwndrealctrl.h"
#include "bkmsg.h"
#include "bkres/bkModule.h"

class CBkWndCombobox : public CWindowImpl< CBkWndCombobox, WTL::CComboBox  >
                     , public CBkRealCtrl
{
    BKOBJ_DECLARE_CLASS_NAME(CBkWndCombobox, "combobox")
    DECLARE_WND_SUPERCLASS(_T("BKCOMBOBOX") , WC_COMBOBOX );

    typedef CWindowImpl< CBkWndCombobox, WTL::CComboBox  > __baseClass;
    typedef CBkRealCtrl __bkClass;

public:
    CBkWndCombobox()
        :m_wndEditCtrl( this , 1 )
        ,m_bEditable(FALSE) 
    {

    }

    virtual ~CBkWndCombobox()
    {
    }

    void SetIsEditable( BOOL isEditable )
    {
        m_bEditable = isEditable;
        if( isEditable )
        {
            ModifyStyle( CBS_DROPDOWNLIST,CBS_DROPDOWN );
        }
        else
        {
            ModifyStyle( CBS_DROPDOWN,CBS_DROPDOWNLIST );
        }
    }

    BOOL GetIsEditable()
    {
        return m_bEditable;
    }

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

        // 处理能够响应键盘事件
        HWND hEditControl = GetWindow( GW_CHILD );
        if( hEditControl != NULL )
        {
            m_wndEditCtrl.SubclassWindow( hEditControl );
        }

        SetFont( BkFontPool::GetFont(BKF_DEFAULTFONT) );
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
    DWORD _ComposeStyle()
    {
        DWORD dwStyle = WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|CBS_NOINTEGRALHEIGHT|CBS_AUTOHSCROLL;

        dwStyle |=  m_bEditable ? CBS_DROPDOWN : CBS_DROPDOWNLIST;

        return dwStyle;
    }

public:
    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_INT_ATTRIBUTE("editable", m_bEditable, FALSE)
    BKWIN_DECLARE_ATTRIBUTES_END()

    BEGIN_MSG_MAP( CBkWndCombobox )
        MESSAGE_HANDLER( WM_GETDLGCODE, OnGetDlgCode )
        MESSAGE_HANDLER( WM_SETFOCUS, OnWndSetFocus )
        MESSAGE_HANDLER( WM_KILLFOCUS, OnWndKillFocus )

        REFLECTED_COMMAND_CODE_HANDLER( CBN_SELCHANGE,OnSelChanged )
        REFLECTED_COMMAND_CODE_HANDLER( CBN_EDITCHANGE, OnEditChanged )

        ALT_MSG_MAP(1)
        MESSAGE_HANDLER( WM_KEYDOWN, OnEditKeyDown )
        MESSAGE_HANDLER( WM_SETFOCUS, OnEditSetFocus )
        MESSAGE_HANDLER( WM_KILLFOCUS, OnEditKillFocus )
        DEFAULT_REFLECTION_HANDLER()
    END_MSG_MAP()

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

    LRESULT OnWndSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
    {
        bHandled = FALSE;
        m_bFocus = TRUE;
        OnSetFocus( this );
        return 0;
    }

    LRESULT OnWndKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
    {
        bHandled = FALSE;
        KillFocus();
        m_bFocus = FALSE;
        return 0;
    }

    LRESULT OnSelChanged( UINT uMsg, int nCtlID, HWND hWnd, BOOL& bHandled )
    {
        bHandled = FALSE;

        if( m_pNotify )
        {
            int indexCurSel = GetCurSel();
            NotifyInfo info = { BKM_COMBOBOX_SELCHANGED, CPoint(0, 0), this, indexCurSel };
            m_pNotify->Action( &info );
        }

        return FALSE;
    }
    LRESULT OnEditChanged( UINT uMsg, int nCtlID, HWND hWnd, BOOL& bHandled )
    {
        bHandled = FALSE;
        if( m_pNotify )
        {
            NotifyInfo info = { BKM_COMBOBOX_EDITCHANGED, CPoint(0, 0), this, -1 };
            m_pNotify->Action( &info );
        }
        return 0;
    }

    LRESULT OnEditKeyDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled  )
    {
        bHandled = FALSE;
        switch( wParam )
        {
        case VK_RETURN:
            if( m_pNotify && GetIsEditable() && GetIsWantReturn() )
            {
                NotifyInfo info = { BKM_COMBOBOX_EDITENTER, CPoint(0, 0), this, -1 };
                m_pNotify->Action( &info );
            }
            break;
        }
        return 0;
    }

    LRESULT OnEditSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
    {
        bHandled = FALSE;
        m_bFocus = TRUE;
        OnSetFocus( this );
        return 0;
    }

    LRESULT OnEditKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
    {
        bHandled = FALSE;
        KillFocus();
        m_bFocus = FALSE;
        return 0;
    }

protected:
    BOOL m_bEditable; 
    CContainedWindowT< CEdit > m_wndEditCtrl;
};
