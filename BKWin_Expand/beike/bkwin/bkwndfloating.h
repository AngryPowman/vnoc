//////////////////////////////////////////////////////////////////////////
//  Class Name: CBkFloatingWindow
// Description: 基础浮动窗口
//     Creator: YangDa
//     Version: 2011.05.04 - 1.0 - YangDa - Create
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "bkres/bkModule.h"
#include "bkdlgview.h"
#include "bkwnddiv.h"
#include "bkwndTransRealLayer.h"

#define BORDER_WIDTH   3

typedef CWinTraits<WS_POPUP|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,WS_EX_TOOLWINDOW> CBkFloatingTraits;

class CBkFloatingWindow : public ATL::CWindowImpl<CBkFloatingWindow, ATL::CWindow, CBkFloatingTraits>
                        , public CMessageFilter
                        , public CBkDiv
{
    BKOBJ_DECLARE_CLASS_NAME(CBkFloatingWindow, "floating")

public:
    CBkFloatingWindow();
    virtual ~CBkFloatingWindow();

protected:
    typedef ATL::CWindowImpl<CBkFloatingWindow, ATL::CWindow, CBkFloatingTraits>  __baseClass;
    typedef CBkFloatingWindow __thisClass;
    typedef CBkDiv __bkClass;

public:
    // virtual method of CBkWnd 
    virtual IBkWnd* GetItem(UINT uItemID) const;
    virtual void    ShowWindow(BOOL bShow);

    // virtual method of CBkDialog
    virtual void OnInitDialog();
    virtual void OnFreeDialog();

    HWND Create(TiXmlElement* pTiXmlElem, HWND hWndParent = ::GetActiveWindow());
    HWND Create(LPCTSTR szXml, HWND hWndParent = ::GetActiveWindow());
    void Show( LPCTSTR szXml, BOOL bShow = TRUE, HWND hOwner = ::GetActiveWindow() );
    void Move( POINT ptPos, BOOL bShow = TRUE );
    void Hide();
    void Close();

protected:
    virtual BOOL PreTranslateMessage(MSG* pMsg);

    void _Clear();
    BOOL _LoadXml(LPCTSTR szRes);
    HWND _CreateWindow(HWND hWndParent);
    void _InvalidateRect(CRect& rcInvalidate);
    HBITMAP _CreateCompatibleBitmap(CRect& rcClient);

    virtual void    CreateHandle( HWND hParent );
    virtual void    SetFocus();
    virtual void    KillFocus();
    virtual CBkWnd* OnSetFocus( CBkWnd* which );
    virtual void    OnChildLoad(TiXmlElement* pTiXmlElem, CBkWnd* pChildWindow);
    virtual BOOL    OnWindowPosChanged(LPWINDOWPOS lpWndPos);
    virtual void    OnFinalMessage(HWND hWnd);
    virtual void    OnShown(BOOL bShow);

protected:
    BEGIN_MSG_MAP_EX(CBkFloatingWindow)
        MESSAGE_RANGE_HANDLER_EX(WM_MOUSEFIRST, WM_MOUSELAST, _OnToolTipEvent)
        //MSG_WM_ACTIVATE(_OnActivate)
        MSG_WM_CREATE(_OnCreate)
        MSG_WM_DESTROY(_OnDestroy)
        MSG_WM_ERASEBKGND(_OnEraseBkgnd)
        MSG_WM_KEYDOWN(_OnKeyDown)
        MSG_WM_KILLFOCUS(_OnKillFocus)
        MSG_WM_LBUTTONDOWN(_OnLButtonDown)
        MSG_WM_LBUTTONUP(_OnLButtonUp)
        MSG_WM_RBUTTONDOWN(_OnRButtonDown)
        MSG_WM_RBUTTONUP(_OnRButtonUp)
        MSG_WM_LBUTTONDBLCLK(_OnLButtonDblClk)
        MSG_WM_MOUSEMOVE(_OnMouseMove)
        MSG_WM_MOUSEWHEEL(_OnMouseWheel)
        MSG_WM_MOVE(_OnMove)
        MSG_WM_NCACTIVATE(_OnNcActivate)
        MSG_WM_NCCALCSIZE(_OnNcCalcSize)
        MSG_WM_NCHITTEST(_OnNcHitTest)
        MSG_WM_PAINT(_OnPaint)
        MSG_WM_SETCURSOR(_OnSetCursor)
        MSG_WM_SHOWWINDOW(_OnShowWindow)
        MSG_WM_SIZE(_OnSize)
		MSG_WM_SYSCOMMAND(_OnSysCommand)
        NOTIFY_CODE_HANDLER_EX(BKINM_INVALIDATERECT, _OnBKINMInvalidateRect)
		NOTIFY_CODE_HANDLER_EX(BKINM_UPDATE_DRAGITEM, _OnBKINMUpdateDragItem)
        REFLECT_NOTIFICATIONS_EX()
    END_MSG_MAP()

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_STRING_ATTRIBUTE("title", m_strCaption, FALSE)
        BKWIN_INT_ATTRIBUTE("tabdefault", m_tabDefault, FALSE)
        BKWIN_COLOR_ATTRIBUTE("focus-color", m_clrFocus, FALSE)
        BKWIN_UINT_ATTRIBUTE("alpha", m_alpha, FALSE)
        BKWIN_UINT_ATTRIBUTE("layer-trans", m_bLayerTrans, FALSE)
        BKWIN_UINT_ATTRIBUTE("layer-active", m_bLayerActive, FALSE)
        BKWIN_COLOR_ATTRIBUTE("layer-trans-color", m_clrTransRealLayer, FALSE)
    BKWIN_DECLARE_ATTRIBUTES_END()

    void _OnActivate(UINT nState, BOOL bMinimized, HWND wndOther);
    int _OnCreate(LPCREATESTRUCT lpCreateStruct);
    BOOL _OnDestroy();
    BOOL _OnEraseBkgnd(CDCHandle dc);
    BOOL _OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    void _OnKillFocus( HWND pNewWnd );
    BOOL _OnRButtonDown(UINT nFlags, CPoint point);
    BOOL _OnRButtonUp(UINT nFlags, CPoint point);
    BOOL _OnLButtonDown(UINT nFlags, CPoint point);
    BOOL _OnLButtonUp(UINT nFlags, CPoint point);
    BOOL _OnLButtonDblClk(UINT nFlags, CPoint point);
    BOOL _OnMouseMove(UINT nFlags, CPoint point);
    BOOL _OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    void _OnMove(CPoint ptPos);
    BOOL _OnNcActivate(BOOL bActive);
    BOOL _OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam);
    UINT _OnNcHitTest( CPoint point );
    BOOL _OnPaint(HDC hdc);
    BOOL _OnSetCursor(CWindow /*wnd*/, UINT nHitTest, UINT message);
    void _OnShowWindow(BOOL bShow, UINT nStatus);
    void _OnSize(UINT nType, CSize size);
	void _OnSysCommand(UINT nID, CPoint point);
    LRESULT _OnToolTipEvent(UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT _OnBKINMInvalidateRect(LPNMHDR pnmh);
	LRESULT _OnBKINMUpdateDragItem(LPNMHDR pnmh);

protected:
    HWND                m_hWndParent;
    BOOL                m_bChild;
    BOOL                m_bCaptureSetted;
    BOOL                m_bActive;
    CString             m_strCaption;
    BOOL                m_bAutoClose;           // 是否自动关闭销毁,当设置为TRUE时,失去焦点关闭销毁窗口
    BOOL                m_bAutoHide;            // 是否自动隐藏,当设置为TRUE时,失去焦点隐藏显示窗口
    int                 m_tabDefault;           // 默认TAB控件
    COLORREF            m_clrFocus;             // 焦点框的颜色
    UINT                m_alpha;                // 透明度,有效范围在0-255之间.若想同时处理REAL控件的透明,需要同时设定m_bLayerTrans为TRUE

    BOOL                m_bLayerTrans;          // 是否使用层次透明处理
    BOOL                m_bLayerActive;         // 是否可以激活透明层次
    COLORREF            m_clrTransRealLayer;    // 层次透明颜色,被设定为此颜色值的部分会被透明掉
    CBkTransRealLayer*  m_pTransRealLayer;      // 层次透明层,处理REAL的透明

    DWORD               m_dwPrePaintTime;       // 上一次绘制时间
    CMemDC              m_dcDlg;
    BOOL                m_bInvalid;

	CBkWnd*             m_pDragWnd;
};

#include "bkwndfloating.inl"