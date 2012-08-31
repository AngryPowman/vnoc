//////////////////////////////////////////////////////////////////////////
//   File Name: BkWnd.h
// Description: BkWindow Definition
//     Creator: Zhang Xiaoxuan
//     Version: 2009.04.28 - 1.0 - Create
//              2011.04.24 - 1.1 - Modify By DongM
//				2011.05.29 - 1.2 - Modify By YangDa
//////////////////////////////////////////////////////////////////////////
#pragma once

#include <atlbase.h>
#include <atlapp.h>
#include <atlctrls.h>
#include "bkwndstyle.h"
#include <bkres/bkstringpool.h>
#include "IBkWnd.h"
#include "bkwndManager.h"
#include "bkTaborder.h"
#include "utility/MemDC.h"
#include "bkDrag.h"

class CBkWnd
    : public CBkObject
    , public IBkWnd
	, public IBkDropContainer
{
public:
    CBkWnd();
    virtual ~CBkWnd();

    static void LockView(BOOL bLock);
    static BOOL IsLockView();

    //////////////////////////////////////////////////////////////////////////
    // interface IBkWnd
    virtual IBkWnd* MakeCopy();
    virtual IBkWnd* GetParent() const;
    virtual IBkWnd* GetItem(UINT uItemID) const; 
    virtual IBkWnd* GetItemByIndex(UINT nIndex);
    virtual void    SetText(LPCWSTR szText);
    virtual LPCWSTR GetText() const;
    virtual UINT    GetCmdID() const;
    virtual void    SetCmdID(UINT nCmdID);
    virtual void    GetPos(BKDLG_POSITION* pPos);
    virtual void    SetPos(BKDLG_POSITION* pPos, BOOL bRepaint);
    virtual void    GetRect(LPRECT lpRc);
	virtual LPRECT  GetRect(); 
    virtual void    Invalidate();
    virtual BOOL    IsChecked();
    virtual void    SetChecked(BOOL bCheck);
    virtual BOOL    IsDisable();
    virtual void    SetDisable(BOOL bDisable);
    virtual BOOL    IsVisible();
    virtual void    SetVisible(BOOL bVisible, BOOL bRedraw = TRUE);
    virtual BOOL    SetSkin(LPCTSTR szSkin);
    virtual BOOL    SetStyle(LPCSTR szStyle);
    virtual HWND    GetRealWnd();
    virtual void    RegisterNotify(INotify* pNotify);
    virtual BOOL    IsDragable();
	virtual BOOL    IsDragInner();
    virtual void    SetDragable(BOOL bDragable);
    virtual void    SetFocus();
    virtual void    KillFocus();
    virtual int     GetTaborder();
    virtual void    SetTaborder(int taborder);
    virtual BOOL    IsAccept();
    virtual void    SetAccept(BOOL bAccept);

    virtual void    SetEffect(IBkEffectObject* pEffect);
    virtual void    AddEffect(IBkEffectObject* pEffect);
    virtual void    RemoveEffect(IBkEffectObject* pEffect);


    //////////////////////////////////////////////////////////////////////////
    // interface Message Event Handle
    virtual BOOL    OnMouseMove(UINT nFlags, CPoint point);
    virtual BOOL    OnMouseHover();
    virtual BOOL    OnMouseLeave();
    virtual BOOL    OnLButtonDown(UINT nFlags, CPoint point);
    virtual BOOL    OnLButtonUp(UINT nFlags, CPoint point,BOOL bAct = TRUE);
    virtual BOOL    OnLButtonDblClk(UINT nFlags, CPoint point);
    virtual BOOL    OnRButtonDown(UINT nFlags, CPoint point);
    virtual BOOL    OnRButtonUp(UINT nFlags, CPoint point,BOOL bAct = TRUE);
    virtual BOOL    OnRButtonDblClk(UINT nFlags, CPoint point);
    virtual BOOL    OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    
    virtual BOOL    OnCreate(LPCREATESTRUCT lpCreateStruct);
    virtual BOOL    OnDestroy();
    virtual BOOL    OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    virtual BOOL    OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
    virtual BOOL    OnNcCalcSize(LPSIZE pSize);
    virtual BOOL    OnPaint(CDCHandle dc);
    virtual void    OnSetCursor();
    virtual void    OnVisibleChanged(BOOL bVisible);
    virtual void    OnDisableChanged(BOOL bDisable);
    virtual void    OnEffectChanged(BOOL bEffect);
    virtual BOOL    OnWindowPosChanged(LPWINDOWPOS lpWndPos);


	//////////////////////////////////////////////////////////////////////////
	//drag
	virtual HRESULT OnDragEnter(IDataObject *pDataObj, POINT pt, DWORD *pdwEffect);
	virtual HRESULT OnDragOver(POINT pt, DWORD *pdwEffect);
	virtual HRESULT OnDrop(IDataObject *pDataObj, POINT pt, DWORD *pdwEffect);
	virtual HRESULT OnDragLeave(void);
	virtual IDropTarget* GetIDropTargetPointer();

    
    //////////////////////////////////////////////////////////////////////////
    // inter method
    virtual BOOL    PtInRect(CPoint point);
    virtual void    Move(LPRECT prect);
    virtual BOOL    Load(TiXmlElement* pTiXmlElem);
    virtual void    CreateHandle(HWND hParent);
    virtual void    SetContainer(HWND hWndContainer);
    virtual void    SetRealContainer(HWND hWndRealContainer);
    virtual BOOL    NeedRedrawWhenStateChange();
    virtual BOOL    RedrawRegion(CDCHandle& dc);
    virtual void    DrawBkgnd(CDCHandle& dc);
    virtual void    DrawFocus(CDCHandle& dc);
    virtual void    DrawDisableShade(CDCHandle& dc);
    virtual BOOL    IsBackground();
    virtual void    SetBackground(BOOL bIsBk);
    virtual void    SetData(DWORD dwData);
    virtual DWORD   GetData();
    virtual BOOL    IsFocus();
    virtual CBkWnd* OnSetFocus( CBkWnd* which );
    virtual void    DeleteThis(HWND hWndAsyn = NULL);
    virtual void    SetDrawPosV(int pos);
    virtual void    SetDrawHeight(int height);

    // automated testing
    virtual CString GetTotalCmdID();

    // as a container
    virtual BOOL    IsContainer();
    virtual CBkWnd* FindChildByCmdID(UINT uCmdID) const;
    virtual CBkWnd* FindChildByPoint(const CPoint& pt, BOOL bBackground);
    virtual void    RepositionChilds();
    virtual void    RepositionChild(CBkWnd *pBkWndChild);
    virtual int     GetIndexByItem(CBkWnd* pBkWnd);
    virtual POSITION GetPosByItem(CBkWnd* pBkWnd);
    virtual POSITION GetPosByIndex(UINT nIndex);

    virtual void    SetDropTarget(IBkDropTarget* pDropTarget);

    virtual void    SetTooltipCtrl( CToolTipCtrl* pTooltip );
    virtual BOOL    PreAddTooltip( CBkWnd* pBkWnd );
    virtual BOOL    PreUpdateTooltip( CBkWnd* pBkWnd );
    virtual BOOL    PreUpdateTooltipRect( CBkWnd* pBkWnd, CRect& rtTooltip );
    void    AddTooltip();
    void    RemoveTooltip();
    void    UpdateTooltipRect();
    void    UpdateTooltipText( const CString& text );

    void    TestMainThread();
    DWORD   GetState(void);
    DWORD   ModifyState(DWORD dwStateAdd, DWORD dwStateRemove);
    HBKWND  GetBkHWnd();
    HBKWND  GetParent();
    void    SetParent(HBKWND hBkWndParent);
    BOOL    IsDisableDeep(BOOL bCheckParent = FALSE);
    BOOL    IsVisibleDeep(BOOL bCheckParent = FALSE);
    void    SetParentsVisible(BOOL bParentsVisible);
    BOOL    IsAcceptParent();
    void    SetAcceptParent(BOOL bAcceptParent);
    void    SetTabControler(IBkTaborder* tabControler);
    void    AddTaborder();

    BOOL    NeedRedrawParent();
    void    NotifyInvalidate();
    void    NotifyInvalidateRect(const CRect& rect);
    HWND    GetContainer();
    HWND    GetRealContainer();
    HRESULT OnAttributePosChange(CStringA& strValue, BOOL bLoading);
    HRESULT OnAttributeVisible(CStringA& strValue, BOOL bLoading);
    void    _RepositionSelf();
    BkStyle* GetStyle();
    int     GetWidth();
    void    SetWidth(int nWidth);
    int     GetHeight();
    void    SetHeight(int nHeight);
protected:
    CBkWnd(const CBkWnd& bkWnd);

    virtual void    _ClearFocus(CBkWnd* focusChild);
    virtual void    _ClearDrag(CBkWnd* dragChild);

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_STYLE_ATTRIBUTE("class", m_style, TRUE)
        BKWIN_INT_ATTRIBUTE("id", m_uCmdID, FALSE)
        BKWIN_COLOR_ATTRIBUTE("crbg", m_style.m_crBg, FALSE)
        BKWIN_COLOR_ATTRIBUTE("crtext", m_style.m_crText, FALSE)
        BKWIN_FONT_ATTRIBUTE("font", m_style.m_ftText, FALSE)
        BKWIN_CUSTOM_ATTRIBUTE("pos", OnAttributePosChange)
        BKWIN_CUSTOM_ATTRIBUTE("visible", OnAttributeVisible)
        BKWIN_INT_ATTRIBUTE("background", m_style.m_bBackground, FALSE)
        BKWIN_INT_ATTRIBUTE("drag", m_bDragable, FALSE)
        BKWIN_INT_ATTRIBUTE("accept_parent", m_bAcceptParent, FALSE)
        BKWIN_INT_ATTRIBUTE("hover_parent", m_bHoverParent, FALSE)
        BKWIN_INT_ATTRIBUTE("taborder", m_taborder, FALSE)
        BKWIN_INT_ATTRIBUTE("accept", m_bAccept, FALSE)
        BKWIN_INT_ATTRIBUTE("focus", m_bDrawFocus, FALSE)
        BKWIN_TSTRING_ATTRIBUTE("tooltip", m_tooltip, FALSE)
        BKWIN_INT_ATTRIBUTE("round", m_style.m_nRound, FALSE)
        BKWIN_INT_ATTRIBUTE("ownDC", m_style.m_bOwnDC, FALSE)
		BKWIN_INT_ATTRIBUTE("drag_inner", m_bDragInner, FALSE)
   BKWIN_DECLARE_ATTRIBUTES_END()

protected:
    static BOOL                 s_bLockView;
    HBKWND                      m_hBkWnd;
    HBKWND                      m_hBkWndParent;
    HWND                        m_hWndContainer;        // 容器窗口
    HWND                        m_hWndRealContainer;    // 容器窗口 - 包含REAL窗口的句柄
    UINT                        m_uCmdID;
    BKDLG_POSITION              m_dlgpos;
    CRect                       m_rcWindow;
    BkStyle                     m_style;
    DWORD                       m_dwState;
    INotify*                    m_pNotify;
    DWORD                       m_dwData;
    BOOL                        m_bFocus;               // 是否处于焦点状态
    BOOL                        m_bAcceptParent;        // 是否接受来自父控件的消息处理
    BOOL                        m_bHoverParent;         // 是否通过父控件来确定控件的HOVER/LEAVE状态(此属性设置后Hover和Leave时间不再单独产生，由父控件控制)
    int                         m_taborder;             // TAB顺序序号
    BOOL                        m_bDrawFocus;           // 是否绘制焦点状态
    BOOL                        m_bAccept;              // 是否是ACCEPT默认处理
    IBkTaborder*                m_tabControler;         // TAB顺序控制器 
    CToolTipCtrl*               m_wndToolTip;           // TOOLTIP控件
    CString                     m_tooltip;              // TOOLTIP文字内容
    BOOL                        m_parentsVisible;       // 控件的所有父控件可视状态
    CMemDC                      m_MemDC;
    BOOL                        m_bNeedRedraw;
    BOOL                        m_bResize;
    int                         m_nDrawPosV;            // 绘制Skin的起始位置，可能会有滚动的处理
    int                         m_nDrawHeight;          // 绘制Skin的高度
    IBkEffectObject*            m_pEffect;
    IBkEffectRenderer*          m_pEffectRender;
#ifdef _DEBUG
    DWORD               m_nMainThreadId;
#endif
    BOOL                m_bDragable;
	IBkDropTarget*      m_pDropTarget;
	BOOL                m_bDragInner;
};

__declspec(selectany) BOOL CBkWnd::s_bLockView = FALSE;

#include "bkwnd.inl"
