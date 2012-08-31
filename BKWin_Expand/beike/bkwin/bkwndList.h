//////////////////////////////////////////////////////////////////////////
//   File Name: bkwndList.h
// Description: List Control
//     Creator: DongM
//     Version: 2011.04.08 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#pragma once

enum LIST_FIND_TYPE
{
	LIST_FIND_TEXT,
	LIST_FIND_DATA
};

//////////////////////////////////////////////////////////////////////////
// CBkListHead定义
class CBkListHead : public CBkDivH
{
    BKOBJ_DECLARE_CLASS_NAME(CBkListHead, "list_head")

public:
    CBkListHead();
    IBkWnd* GetItem(int nCol, int nIndex);
    BOOL    SetColumnWidth(int nCol, int nWidth);

protected:
    // CBkWnd virtual method
    virtual BOOL    OnLButtonDown(UINT nFlags, CPoint point);
    virtual BOOL    OnLButtonUp(UINT nFlags, CPoint point,BOOL bAct = TRUE);
    virtual BOOL    OnMouseMove(UINT nFlags, CPoint point);
    virtual void    OnSetCursor();
    virtual BOOL    RedrawRegion(CDCHandle& dc);
    virtual void    RepositionChilds();
    virtual BOOL    LoadChilds(TiXmlElement* pTiXmlChildElem);

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_SKIN_ATTRIBUTE("sort_arrow", m_pSkinArrow, FALSE)
        BKWIN_INT_ATTRIBUTE("allow-drag", m_bAllowDrag, FALSE)
    BKWIN_DECLARE_ATTRIBUTES_END()

private:
    void RepositionArrow();

private:
    CBkWnd*         m_pSortCol;
    CBkSkinBase*    m_pSkinArrow;
    CRect           m_rcArrow;
    int             m_nDragSrcWidth;
    BOOL            m_bCanDrag;
    BOOL            m_bAllowDrag;               // 是否允许进行拖动表头功能
    POINT           m_ptPre;
    BOOL            m_bDrag;
    int             m_nDragCol;
    int             m_nDragMinWidth;
    POINT           m_ptDown;
    HCURSOR         m_hCursor;
};

//////////////////////////////////////////////////////////////////////////
// CBkListCol定义
class CBkListCol : public CBkDiv
{
    BKOBJ_DECLARE_CLASS_NAME(CBkListCol, "list_col")

public:
    CBkListCol();
    CBkListCol(const CBkListCol& other);

    IBkWnd* MakeCopy();
    int     GetMinWidth();

protected:
    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_INT_ATTRIBUTE("min_width", m_iMinWidth, FALSE)
        BKWIN_DECLARE_ATTRIBUTES_END()

private:
    int m_iMinWidth;
};

//////////////////////////////////////////////////////////////////////////
// CBkListRow定义
class CBkListRow : public CBkDivH
{
    BKOBJ_DECLARE_CLASS_NAME(CBkListRow, "list_row")

public:
    CBkListRow();

    void    Bold(BOOL bBold);
    BOOL    SetColumnWidth(int nCol, int nWidth);
    HBITMAP GetBitmap();
	LPCTSTR GetText() const;

protected:
    // CBkWnd virtual method
    virtual BOOL    NeedRedrawWhenStateChange();
    virtual BOOL    OnMouseHover();
    virtual BOOL    OnMouseLeave();
    virtual void    OnMouseHoverChild(CBkWnd* pChild, const CPoint& point);
    virtual void    OnMouseLeaveChild(CBkWnd* pChild, const CPoint& point);

private:
    void BoldPanel(CBkPanel* pBkPanel, BOOL bBold);
};

//////////////////////////////////////////////////////////////////////////
// CBkListBody定义
class CBkListBody : public CBkDivV
                  , public INotify
{
    BKOBJ_DECLARE_CLASS_NAME(CBkListHead, "list_body")

public:
    CBkListBody();
    ~CBkListBody();

    int     AddRow();
    int     InsertRow(int nRow);
    BOOL    SetItemText(int nRow, int nCol, int nIndex, LPCTSTR szText);
    LPCWSTR GetItemText(int nRow, int nCol, int nIndex);
    BOOL    SetRowData(int nRow, DWORD dwData);
    DWORD   GetRowData(int nRow);
    CRect   GetRowRect(int nRow);
    void    ClearSelectRow();
    BOOL    SelectRow(int nRow, BOOL bSelect=TRUE);
    int     GetSelectRow();
    BOOL    SetSelectRows(const std::vector<int>& nRows);
    BOOL    GetSelectRows(std::vector<int>& nRows);
    void    SelectAll();
    BOOL    DeleteRow(int nRow);
    BOOL    DeleteAllRows();
    int     GetRowCount();
    int     GetSelectRowCount();
    IBkWnd* GetItem(int nRow, int nCol, int nIndex);
    BOOL    UpdateRowPosition(int nOldPos, int nNewPos);
    void    SetShowLeft(int iLeft);
    BOOL    BoldRow(int nRow, BOOL bBold);
    BOOL    IsRowSelected(int nRow);
    int     OnDrop(POINT pt);
	int     HitTest(CPoint point);
	int		FindRow(LIST_FIND_TYPE emType, DWORD dwData);
    BOOL    SetColumnWidth(int nCol, int nWidth);
    BOOL    MoveRow(int sourceIdx, int targetIdx);
	BOOL	IsMultSelect();
	BOOL	IsNavigator();
	BOOL	IsRowVisible(int nRow);
	int		GetRealRowTop(int nRow);

    void    SortChildren(ISortCompare* compare);
	BOOL	OnDropState(UINT nFlags, CPoint point);
	BOOL	OnResetState(int nRow);

public:
    // CBkWnd virtual method
    virtual BOOL    Load(TiXmlElement* pTiXmlElem);
	virtual BOOL    OnWindowPosChanged(LPWINDOWPOS lpWndPos);
    virtual BOOL    OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    virtual BOOL    OnMouseMove(UINT nFlags, CPoint point);
    virtual BOOL    OnMouseLeave();
    virtual void    OnMouseLeaveChild(CBkWnd* pChild, const CPoint& point);
    virtual BOOL    OnLButtonDown(UINT nFlags, CPoint point);
    virtual BOOL    OnLButtonUp(UINT nFlags, CPoint point,BOOL bAct = TRUE);
    virtual BOOL    OnLButtonDblClk(UINT nFlags, CPoint point);
    virtual BOOL    OnRButtonDown(UINT nFlags, CPoint point);
    virtual BOOL    OnRButtonUp(UINT nFlags, CPoint point,BOOL bAct = TRUE);
    virtual BOOL    OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    virtual CBkWnd* OnSetFocus( CBkWnd* which );
    // INotify interface
    virtual BOOL    Action(NotifyInfo* pInfo);

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_INT_ATTRIBUTE("multselect", m_bMultSelect, FALSE)
		BKWIN_INT_ATTRIBUTE("navigator", m_bNavigator, FALSE)
    BKWIN_DECLARE_ATTRIBUTES_END()

private:
    CBkListRow* _NewRow();
    CBkListRow* _GetRow(int nRow);
    CBkWnd*     _GetItem(int nRow, int nCol, int nIndex);
    void        _UpdateSelectRow(int nRow, BOOL bAdd);

private:
    TiXmlNode*  m_pDefaultRow;          // 默认的行模板
    std::vector<int> m_iSelectRows;
    std::vector<int> m_iColWidths;
    int         m_nDownRow;
    BOOL        m_bDownRowChecked;
    CPoint      m_lDownPt;
    BOOL        m_bMultSelect;       // 是否支持多选
	BOOL		m_bNavigator;		 // 导航栏模式，不丢失选择
};


//////////////////////////////////////////////////////////////////////////
// CBkList定义
class CBkList
    : public CBkDiv
    , public IBkList
    , public INotify
{
    BKOBJ_DECLARE_CLASS_NAME(CBkList, "list")

public:
    CBkList();

    // IDropContainer members
    virtual HRESULT OnDragEnter(IDataObject *pDataObj, POINT pt, DWORD *pdwEffect);
    virtual HRESULT OnDragOver(POINT pt, DWORD *pdwEffect);
    virtual HRESULT OnDrop(IDataObject *pDataObj, POINT pt, DWORD *pdwEffect);
	virtual HRESULT OnDragLeave( void);
    //////////////////////////////////////////////////////////////////////////
    // interface IBkList
    virtual int     AddRow();
    virtual int     InsertRow(int nRow);
    virtual BOOL    SetItemText(int nRow, int nCol, int nIndex, LPCTSTR szText);
    virtual LPCWSTR GetItemText(int nRow, int nCol, int nIndex);
    virtual BOOL    SetRowData(int nRow, DWORD dwData);
    virtual DWORD   GetRowData(int nRow);
	virtual CRect   GetRowRect(int nRow);
    virtual void    ClearSelectRow();
    virtual BOOL    SelectRow(int nRow, BOOL bSelect = TRUE);
    virtual int     GetSelectRow();
    virtual BOOL    SetSelectRows(const std::vector<int>& nRows);
    virtual BOOL    GetSelectRows(std::vector<int>& nRows);
    virtual void    SelectAll();
    virtual BOOL    DeleteRow(int nRow);
    virtual BOOL    DeleteAllRows();
    virtual int     GetRowCount();
    virtual int     GetSelectRowCount();
    virtual IBkWnd* GetItem(int nRow, int nCol, int nIndex);
    virtual BOOL    UpdateRowPosition(int nOldPos, int nNewPos);
    virtual int     GetRealHeight();
    virtual int     GetColumnCount();
    virtual IBkWnd* GetHeadItem(int nCol, int index);
    virtual BOOL    BoldRow(int nRow, BOOL bBold);
    virtual BOOL    IsRowSelected(int nRow);
	virtual int		HitTest(CPoint point);
	virtual int		FindRow(LIST_FIND_TYPE emType, DWORD dwData);
    virtual BOOL    SetColumnWidth(int nCol, int nWidth);
    virtual void    SortRows( ISortCompare* compare );
    virtual BOOL    MoveRow(int sourceIdx, int targetIdx);

    void    ScrollToTop();
    void    ScrollToBottom();
    void    ScrollToRow(int nRow);

protected:
    // CBkWnd virtual method
    virtual BOOL    LoadChilds(TiXmlElement* pTiXmlChildElem);
    virtual BOOL    OnWindowPosChanged(LPWINDOWPOS lpWndPos);
    virtual BOOL    OnLButtonDblClk(UINT nFlags, CPoint point);
    virtual BOOL    OnLButtonUp(UINT nFlags, CPoint point,BOOL bAct = TRUE);
	virtual BOOL    OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual CBkWnd*	OnSetFocus( CBkWnd* which );
    virtual BOOL    OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

    // INotify interface
    virtual BOOL    Action(NotifyInfo* pInfo);

private:
    void            _CalcSize();

protected:
    CBkListHead* m_pListHead;
    CBkListBody* m_pListBody;
    CBkScroll*   m_pHScroll;
    CBkScroll*   m_pVScroll;
};

#include "bkwndList.inl"