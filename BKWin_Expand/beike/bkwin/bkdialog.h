//////////////////////////////////////////////////////////////////////////
//  Class Name: CBkDialog
// Description: 基础普通窗口
//     Creator: YangDa
//     Version: 2011.04.15 - 1.0 - YangDa - Create
//              2011.04.18 - 1.1 - YangDa - 事件响应.基于XML读取创建功能
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "bkres/bkModule.h"
#include "bkdlgview.h"
#include "bkwnddiv.h"

#define BORDER_WIDTH   3
#define CORNER_WIDTH   9


typedef CWinTraits<WS_VISIBLE | WS_CLIPCHILDREN | WS_TABSTOP, 0> CBkDialogTraits;
typedef CWinTraits<WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_TABSTOP, 0> CBkChildDialogTraits;

class CBkDialog
    : public ATL::CWindowImpl<CBkDialog, ATL::CWindow, CBkDialogTraits>
    , public CMessageFilter
    , public CBkDiv
{
public:

    DECLARE_WND_CLASS(_T("BKDialog"))

    static BOOL _IsWinXPAndLater();
    //static ATL::CWndClassInfo& GetWndClassInfo();
    CBkDialog();
    ~CBkDialog();

    // virtual method of CBkWnd 
    virtual IBkWnd* GetItem(UINT uItemID) const;
    virtual void    ShowWindow(BOOL bShow);
    virtual HWND    GetRealWnd();

    // virtual method of CBkDialog
    virtual void OnInitDialog();
    virtual void OnFreeDialog();

    HWND Create(LPCTSTR szXml, HWND hWndParent = ::GetActiveWindow());
    void Show( LPCTSTR szXml, HWND hOwner = ::GetActiveWindow() );
    void Hide();
    void Move( POINT ptPos, BOOL bShow = TRUE );
    void Close();
    UINT DoModal(LPCTSTR szXml, HWND hWndParent = ::GetActiveWindow());
    void EndModal(UINT uRetCode);

	HWND GetSafeOwner(HWND hParent, HWND* pWndTop);
	HWND PreModal();
	void PostModal();
	UINT DoModal2(LPCTSTR szXml, HWND hWndParent = ::GetActiveWindow());

	void SetWndStyle(DWORD dwStyle);

public:
	static BOOL CloseTopModalHwnd();
	static std::vector<HWND>& GetAllModalHwnd();

protected:
    typedef ATL::CWindowImpl<CBkDialog, ATL::CWindow, CBkDialogTraits> __baseClass;
    typedef CBkDiv __bkClass;

    BEGIN_MSG_MAP_EX(CBkDialog)
        MESSAGE_RANGE_HANDLER_EX(WM_MOUSEFIRST, WM_MOUSELAST, _OnToolTipEvent)
        //MSG_WM_ACTIVATE(_OnActivate)
        MSG_WM_CLOSE(_OnClose)
        MSG_WM_DESTROY(_OnDestroy)
        //MSG_WM_ENABLE(_OnEnable)
        MSG_WM_ERASEBKGND(_OnEraseBkgnd)
        MSG_WM_GETMINMAXINFO(_OnGetMinMaxInfo)
        MSG_WM_KEYDOWN(_OnKeyDown)
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
        MSG_WM_NCDESTROY(_OnNcDestroy)
        MSG_WM_NCHITTEST(_OnNcHitTest)
        MSG_WM_NCLBUTTONDBLCLK(_OnNcLButtonDblClk)
        MSG_WM_PAINT(_OnPaint)
        MSG_WM_SETCURSOR(_OnSetCursor)
        MSG_WM_SHOWWINDOW(_OnShowWindow)
        MSG_WM_SIZE(_OnSize)
        MESSAGE_HANDLER_EX(WM_SIZING, _OnSizing)
        MESSAGE_HANDLER(BKINM_DELETE_THIS, _OnDeleteThis)
        MESSAGE_HANDLER(WM_MAIN_ANIMATETIMER, _OnEffectAnimateTimer)
		NOTIFY_CODE_HANDLER_EX(BKINM_INVALIDATERECT, _OnBKINMInvalidateRect)
        NOTIFY_CODE_HANDLER_EX(BKINM_UPDATE_DRAGITEM, _OnBKINMUpdateDragItem)
        REFLECT_NOTIFICATIONS_EX()
    END_MSG_MAP()

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_INT_ATTRIBUTE("appwin", m_bAppWin, FALSE)
        BKWIN_TSTRING_ATTRIBUTE("title", m_strCaption, FALSE)
        BKWIN_INT_ATTRIBUTE("resizable", m_bResizable, FALSE)
        BKWIN_INT_ATTRIBUTE("min_width", m_minSize.cx, FALSE)
        BKWIN_INT_ATTRIBUTE("min_height", m_minSize.cy, FALSE)
        BKWIN_INT_ATTRIBUTE("start_x", m_startPos.cx, FALSE)
        BKWIN_INT_ATTRIBUTE("start_y", m_startPos.cy, FALSE)
        BKWIN_INT_ATTRIBUTE("tabdefault", m_tabDefault, FALSE)
        BKWIN_COLOR_ATTRIBUTE("focus-color", m_clrFocus, FALSE)
        BKWIN_UINT_ATTRIBUTE("alpha", m_alpha, FALSE)
        BKWIN_UINT_ATTRIBUTE("layer-trans", m_bLayerTrans, FALSE)
        BKWIN_UINT_ATTRIBUTE("layer-active", m_bLayerActive, FALSE)
        BKWIN_COLOR_ATTRIBUTE("layer-trans-color", m_clrTransRealLayer, FALSE)
    BKWIN_DECLARE_ATTRIBUTES_END()

    virtual void    CreateHandle( HWND hParent );
    virtual void    SetFocus();
    virtual void    KillFocus();
    virtual CBkWnd* OnSetFocus( CBkWnd* which );
    virtual BOOL    OnWindowPosChanged(LPWINDOWPOS lpWndPos);
    virtual void    OnFinalMessage(HWND hWnd);
    virtual void    OnShown(BOOL bShow);
    virtual BOOL    RedrawRegion(CDCHandle& dc);
    virtual BOOL    PreTranslateMessage(MSG* pMsg);

    void _Clear();
    BOOL _LoadXml(LPCTSTR szRes);
    void _ModalMessageLoop();
    HWND _CreateWindow(HWND hWndParent);
    void _InvalidateRect(CRect& rcInvalidate);
    void _ThrowFocusChild(LPPOINT pt);
    HBITMAP _CreateCompatibleBitmap(CRect& rcClient);

    void _OnActivate(UINT nState, BOOL bMinimized, HWND wndOther);
    void _OnClose();
    BOOL _OnDestroy();
    void _OnEnable(BOOL bEnable);
    BOOL _OnEraseBkgnd(CDCHandle dc);
    void _OnGetMinMaxInfo(LPMINMAXINFO lpMMI);
    BOOL _OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
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
    void _OnNcDestroy();
    UINT _OnNcHitTest( CPoint point );
    BOOL _OnNcLButtonDblClk(UINT nFlags, CPoint point);
    BOOL _OnPaint(HDC dc);
    BOOL _OnSetCursor(CWindow /*wnd*/, UINT nHitTest, UINT message);
    void _OnShowWindow(BOOL bShow, UINT nStatus);
    void _OnSize(UINT nType, CSize size);
    LRESULT _OnSizing(UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT _OnToolTipEvent(UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT _OnBKINMInvalidateRect(LPNMHDR pnmh);
    LRESULT _OnBKINMUpdateDragItem(LPNMHDR pnmh);
    LRESULT _OnDeleteThis(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT _OnEffectAnimateTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual HRESULT OnDragEnter(IDataObject *pDataObj, POINT pt, DWORD *pdwEffect);
	virtual HRESULT OnDragOver(POINT pt, DWORD *pdwEffect);
	virtual HRESULT OnDrop(IDataObject *pDataObj, POINT pt, DWORD *pdwEffect);
	virtual HRESULT OnDragLeave(void);

private:
    void DrawDisableScreen(CDCHandle& dc);

protected:
    HWND                m_hWndParent;
	HWND                m_hWndTop;
    BOOL                m_bAppWin;
    BOOL                m_bCaptureSetted;
    UINT                m_uRetCode;
    BOOL                m_bExitModalLoop;
    BOOL                m_bActive;
    BOOL                m_bResizable;
    CString             m_strCaption;
    CSize               m_minSize;
    CSize               m_startPos;             // 窗口起始显示位置
    CBkWnd*             m_pDragWnd;
    int                 m_tabDefault;           // 默认TAB控件
    COLORREF            m_clrFocus;             // 焦点框的颜色
    UINT                m_alpha;
    BOOL                m_bLayerTrans;          // 是否使用层次透明处理
    BOOL                m_bLayerActive;         // 是否可以激活透明层次
    COLORREF            m_clrTransRealLayer;    // 层次透明颜色,被设定为此颜色值的部分会被透明掉
    CBkTransRealLayer*  m_pTransRealLayer;      // 层次透明层,处理REAL的透明
    DWORD               m_dwPrePaintTime;       // 上一次绘制时间
    CMemDC              m_dcDlg;
    BOOL                m_bInvalid;

	DWORD               m_dwWndStyle;
};

#include "bkdialog.inl"