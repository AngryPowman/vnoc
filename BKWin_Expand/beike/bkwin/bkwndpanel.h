//////////////////////////////////////////////////////////////////////////
//   File Name: bkwndpanel.h
// Description: CBkPanel Definition, Virtual Container of BkWindows
//     Creator: Zhang Xiaoxuan
//     Version: 2009.04.28 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "bkwnd.h"

class CBkPanel : public CBkWnd
{
    BKOBJ_DECLARE_CLASS_NAME(CBkPanel, "pan")

public:
    CBkPanel();

    //////////////////////////////////////////////////////////////////////////
    // interface IBkWnd
    virtual IBkWnd* GetItem(UINT uItemID) const;
    virtual IBkWnd* GetItemByIndex(UINT nIndex);

    //////////////////////////////////////////////////////////////////////////
    // interface CBkWnd
    virtual BOOL    Load(TiXmlElement* pTiXmlElem);
    virtual BOOL    LoadChilds(TiXmlElement* pTiXmlChildElem);
    virtual BOOL    RedrawRegion(CDCHandle& dc);
    virtual BOOL    IsContainer();
    virtual CBkWnd* FindChildByCmdID(UINT uCmdID) const;
    virtual CBkWnd* FindChildByPoint(const CPoint& pt, BOOL bBackground);
    virtual void    SetContainer(HWND hWndContainer);
    virtual void    SetRealContainer(HWND hWndRealContainer);
    virtual void    OnChildLoad(TiXmlElement* pTiXmlElem, CBkWnd* pChildWindow);
    virtual int     GetIndexByItem(CBkWnd* pBkWnd);
    virtual POSITION GetPosByItem(CBkWnd* pBkWnd);
    virtual POSITION GetPosByIndex(UINT nIndex);
    virtual void    SortChild(CBkWnd* pBkWnd,ISortCompare* compare);
    virtual void    SortChildren(ISortCompare* compare);

	//////////////////////////////////////////////////////////////////////////
	virtual void    SetDropTarget(IBkDropTarget* pDropTarget);

	//drag IDropContainer
	virtual HRESULT OnDragEnter(IDataObject *pDataObj, POINT pt, DWORD *pdwEffect);
	virtual HRESULT OnDragOver(POINT pt, DWORD *pdwEffect);
	virtual HRESULT OnDrop(IDataObject *pDataObj, POINT pt, DWORD *pdwEffect);
	virtual HRESULT OnDragLeave(void);

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
    virtual BOOL    OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

    virtual BOOL    OnDestroy();
    virtual void    OnSetCursor();
    virtual void    OnVisibleChanged(BOOL bVisible);
    virtual void    OnDisableChanged(BOOL bDisable);
    virtual void    OnEffectChanged(BOOL bEffect);
    virtual void    DrawDisableShade(CDCHandle& dc);
	virtual void	_ClearFocus(CBkWnd* focusChild);
    virtual void    _ClearDrag(CBkWnd* dragChild);

    virtual void    OnExtraChildMouseMove(CPoint point,CBkWnd* &pHover,CBkWnd* &pLeave );
    virtual void    OnExtraChildMouseLeave( CBkWnd* &pLeave );
    virtual void    OnMouseHoverChild(CBkWnd* pChild, const CPoint& point);
    virtual void    OnMouseLeaveChild(CBkWnd* pChild, const CPoint& point);
    virtual int	    _HitTest(CPoint point);
    virtual int     _GetItemCount();
    virtual CBkWnd* _GetItemByIndex(int nIndex);

    virtual CBkWnd* _GetHoverChild();

	void SetScaleX(float x);
	void SetScaleY(float y);
	void SetNeedScale(int set) { m_iNeedScale = set; }

protected:
    CBkPanel(const CBkPanel& bkPanel);

    void    _ClearLastHover();
    void    _Sort( int indexStart, int indexEnd, ISortCompare* compare );
    CBkWnd* _GetDragWnd();
    void    _SetDragWnd(CBkWnd* pBkWnd);
	HBITMAP _CreateCompatibleBitmap(int width, int height);

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_INT_ATTRIBUTE("click-done-sub", m_bClickDnoeSub, FALSE)
		BKWIN_INT_ATTRIBUTE("need-scale", m_iNeedScale, FALSE)
		BKWIN_FLOAT_ATTRIBUTE("scale-x", m_fScaleX, FALSE)
		BKWIN_FLOAT_ATTRIBUTE("scale-y", m_fScaleY, FALSE)
    BKWIN_DECLARE_ATTRIBUTES_END()

    CAtlList<CBkWnd *>  m_lstWndChild;
    CBkWnd*             m_lastHoverChild;
    CBkWnd*             m_focusChild;           // 获得焦点的控件
    BOOL                m_bUpdateFocus;         // 是否更新焦点控件
    BOOL                m_bClickDnoeSub;        // 是否当按钮等原子基础子控件点击后就表示完成了本容器控件的电机操作
	float				m_fScaleX;
	float				m_fScaleY;
	int					m_iNeedScale;
};

#include "bkwndpanel.inl"