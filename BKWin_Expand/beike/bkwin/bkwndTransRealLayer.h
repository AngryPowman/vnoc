//////////////////////////////////////////////////////////////////////////
//  Class Name: CBkTransRealLayer
// Description: 透明窗口容纳REAL控件的透明表现层
//              此层仅为处理透明窗口效果时,为使REAL控件也能被正确绘制处理
//     Creator: YangDa
//     Version: 2011.08.09 - 1.0 - YangDa - Create
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "bkres/bkModule.h"
#include "bkdlgview.h"
#include "bkwnddiv.h"
#include "IBkWnd.h"

typedef CWinTraits<WS_POPUP|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,WS_EX_LAYERED|WS_EX_NOACTIVATE> CBkTransRealLayerTraits;

class CBkTransRealLayer : public ATL::CWindowImpl<CBkTransRealLayer, ATL::CWindow, CBkTransRealLayerTraits>
                        , public CMessageFilter
                        , public CBkWnd
{
    BKOBJ_DECLARE_CLASS_NAME(CBkTransRealLayer, "translayer")

public:
    CBkTransRealLayer();
    CBkTransRealLayer( COLORREF clrTrans, BOOL bActive = FALSE );
    virtual ~CBkTransRealLayer();

    HWND CreateTransLayer(HWND hParent);

protected:
    typedef ATL::CWindowImpl<CBkTransRealLayer, ATL::CWindow, CBkTransRealLayerTraits>  __baseClass;

protected:
    BEGIN_MSG_MAP_EX(CBkTransRealLayer)
        MSG_WM_CREATE(_OnCreate)
        MSG_WM_CTLCOLORDLG(_OnCtlColorDlg)
        MSG_WM_ERASEBKGND(_OnEraseBkgnd)
        MSG_WM_DESTROY(_OnDestroy)
        MSG_WM_KILLFOCUS(_OnKillFocus)
		MESSAGE_HANDLER(WM_SYSKEYDOWN, _OnSysKeyDown)
		MSG_WM_SYSCOMMAND(_OnSysCommand)
        //MSG_WM_PAINT(_OnPaint)
        REFLECT_NOTIFICATIONS_EX()
    END_MSG_MAP()

    int     _OnCreate(LPCREATESTRUCT lpCreateStruct);
    HBRUSH  _OnCtlColorDlg(CDCHandle dc, CWindow wnd);
    LRESULT _OnCtlColorDlg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    BOOL    _OnDestroy();
    BOOL    _OnEraseBkgnd(CDCHandle dc);
    void    _OnKillFocus( HWND pNewWnd );
    BOOL    _OnPaint(HDC hdc);
    LRESULT _OnSysKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void	_OnSysCommand(UINT nID, CPoint point);
    virtual BOOL    PreTranslateMessage(MSG* pMsg);

private:
    HWND _CreateWindow(HWND hWndParent);

protected:
    HWND        m_hWndParent;
    COLORREF    m_clrTrans;
    HBRUSH      m_brush;
    BOOL        m_bAllowActive;     // 是否允许被激活
};

#include "bkwndTransRealLayer.inl"