//////////////////////////////////////////////////////////////////////////
//   File Name: bkwndIconView.h
// Description: ͼ����ʽ��ͼView
//     Creator: Yang Da 
//     Version: 2011.05.18 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#pragma once

#define CLASSNAME_ICONVIEW       "icon_view"
#define CLASSNAME_ICONITEM       "icon_item"
#define CLASSNAME_ICONBODY       "icon_body"
#define CLASSNAME_ICONTEMPLATE   "icon_template"

class CBkIconItem : public CBkDiv
{
    BKOBJ_DECLARE_CLASS_NAME(CBkIconItem, CLASSNAME_ICONITEM)

public:
    CBkIconItem();

    virtual void    SetChecked(BOOL bCheck);

protected:
    // CBkWnd virtual method
    virtual BOOL    OnLButtonDblClk(UINT nFlags, CPoint point);
    virtual void    OnMouseHoverChild(CBkWnd* pChild, const CPoint& point);
    virtual void    OnMouseLeaveChild(CBkWnd* pChild, const CPoint& point);
    virtual BOOL    OnPaint(CDCHandle dc);
    virtual CBkWnd* OnSetFocus(CBkWnd* which);
private:
    HBITMAP GetBitmap();

protected:
    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_INT_ATTRIBUTE("customdraw", m_bCustomDraw, FALSE)
    BKWIN_DECLARE_ATTRIBUTES_END()

	BOOL			m_bCustomDraw;
};

class CBkIconBody : public CBkDivZ
                  , public INotify
{
    BKOBJ_DECLARE_CLASS_NAME(CBkIconBody, CLASSNAME_ICONBODY)

public:
    CBkIconBody();
    ~CBkIconBody();

public:
    int     AddItem( BOOL isRedraw = TRUE, BOOL isCalc = TRUE );
    int     AddItemFromTemplate( int templateIndex, BOOL isRedraw = TRUE, BOOL isCalc = TRUE );
    int     InsertItem(int nItem);
	BOOL	DeleteItem(int nItem);
    void    DeleteItems(int nItemBegin, int nItemEnd);
	void	DeleteAllItems();
    IBkWnd* GetItemEx(int nIndex, int nSubItemID);
    BOOL    SetItemData(int nItem, DWORD dwData);
    DWORD   GetItemData(int nItem);
    CRect   GetItemRect(int nItem);
    int     GetSelectItem();
    BOOL    SetSelectItems(const std::vector<int>& nItems);
    BOOL    GetSelectItems(std::vector<int>& nItems);
    int     GetSelectItemsCount();
    int     GetItemCount();
    int     GetVisibleItemsHeight();
	int		HitTest(CPoint point);
	int		FindItem(LIST_FIND_TYPE emType, DWORD dwData);
    int     GetRealItemTop(int nItem);
	void    SetIsInnerDrag(BOOL bIsInnerDgag);

    // ���/ɾ��һ��Item
    BOOL    SelectItem( int nItem, BOOL bSelect );
    // ���ѡ���б�
    void    ClearSelectItems();
    BOOL    IsItemSelected( int nItem );
    BOOL    IsMultSelect();
	BOOL	IsItemVisible(int nItem);

    virtual void    SortChildren(ISortCompare* compare);

public:
    // CBkWnd virtual method
    virtual BOOL    Load(TiXmlElement* pTiXmlElem);
    // INotify interface
    virtual BOOL    Action(NotifyInfo* pInfo);

public:
    virtual BOOL    OnLButtonDown(UINT nFlags, CPoint point);
    virtual BOOL    OnLButtonUp(UINT nFlags, CPoint point,BOOL bAct = TRUE);
    virtual BOOL    OnRButtonDown(UINT nFlags, CPoint point);
    virtual BOOL    OnRButtonUp(UINT nFlags, CPoint point,BOOL bAct = TRUE);
    virtual BOOL    OnMouseLeave();
	virtual BOOL    OnMouseMove(UINT nFlags, CPoint point);
    virtual BOOL    OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    virtual BOOL    OnPaint(CDCHandle dc);
	virtual BOOL	OnWindowPosChanged(LPWINDOWPOS lpWndPos);

protected:
    CBkIconItem*    _NewItem();
    CBkWnd*         _NewItem(TiXmlElement* pTiXmlElem);
    CBkWnd*         _GetItem(int nItem, int nIndex);
    CBkIconItem*    _GetItem(int nItem);
    void            _UpdateItem(const CRect& rcItem);
    void            _ClearSelectItems();
    void            _AddSelectItem(int nItem);


    //////// ѡ�����
    // ѡ����һ��Item
    void SelectSingleItem( int nItem );
    // ѡ����Item
    void SelectMultipleItems( int nEndItem );

private:
    TiXmlNode*              m_pDefault;          // Ĭ�ϵ�ģ��
    CAtlList<TiXmlElement*> m_lstTemplate;       // ģ���б�
    BOOL                    m_bMultSelect;       // �Ƿ�֧�ֶ�ѡ
    std::vector<int>        m_iSelectItems;

    CRect                   m_rcItemsBg;         // ��Items��������RowΪ������λ
    BOOL                    m_bDrawVisibles;     // �Ƿ���ڿ���items����Ʊ���
    int                     m_visibleHeight;     // ���п���items�Ŀ���ʾ�߶�
    int                     m_itemsBgGap;        // Items��Row����������������в���Row�����Ϊ�������Ƶ��������

	BOOL                    m_bIsInnerDgag;
 	int						m_nLastDownItem;    
    BOOL					m_bOldChecked;

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_SKIN_ATTRIBUTE("items_bg_skin", m_pItemsBgSkin, FALSE)
        BKWIN_INT_ATTRIBUTE("items_bg_gap", m_itemsBgGap, FALSE)
        BKWIN_INT_ATTRIBUTE("draw-visibles-bg", m_bDrawVisibles, FALSE)
        BKWIN_INT_ATTRIBUTE("multselect", m_bMultSelect, FALSE)
    BKWIN_DECLARE_ATTRIBUTES_END()

    CBkSkinBase*    m_pItemsBgSkin;     // Items��Row����Ƥ������Items������Ż���RowΪ��λ����
};

class CBkIconView : public CBkDiv
                  , public INotify
{
    BKOBJ_DECLARE_CLASS_NAME(CBkIconView, CLASSNAME_ICONVIEW)

public:
    CBkIconView();
    ~CBkIconView();

    virtual int     AddItem( BOOL isRedraw = TRUE, BOOL isReCalc = TRUE );
    virtual int     AddItemFromTemplate( int templateIndex, BOOL isRedraw = TRUE, BOOL isCalc = TRUE );
    virtual int     InsertItem( int nItem, BOOL isReCalc = FALSE );
	virtual BOOL	DeleteItem(int nItem);
    virtual void    DeleteItems(int nItemBegin, int nItemEnd);
	virtual void	DeleteAllItems();
    virtual IBkWnd* GetItemEx(int nIndex, int nSubItemID);
    virtual BOOL    SetItemData(int nItem, DWORD dwData);
    virtual DWORD   GetItemData(int nItem);
    virtual CRect   GetItemRect(int nItem);
    virtual int     GetItemCount();
    virtual int     GetVisibleItemsHeight();
	virtual int		GetSelectItem();
    virtual BOOL    GetSelectItems(std::vector<int>& nItems);
    virtual int     GetSelectItemsCount();
    virtual BOOL    IsItemSelected(int nItem);
    virtual BOOL    SelectItem(int nItem,BOOL bSelect);
    virtual void    ClearSelectItems();
	virtual BOOL	SelectAll();
	virtual int		HitTest(CPoint point);
	virtual int		FindItem(LIST_FIND_TYPE emType, DWORD dwData);
    virtual void    SortItems( ISortCompare* compare );

    void    UpdateView();
    void    UpdateItemsPos();
    void    ScrollToTop();
    void    ScrollToBottom();
    void    ScrollToItem(int nItem);
	void    SetIsInnerDrag(BOOL bIsInnerDgag);

    ////// �Զ���ר��
    int    GetScrollPos();
    CBkScroll* GetScrollBar();
protected:
    // CBkWnd virtual method
    virtual BOOL    Load(TiXmlElement* pTiXmlElem);
    virtual void    OnChildLoad(TiXmlElement* pTiXmlElem, CBkWnd* pChildWindow);
    virtual CBkWnd* OnSetFocus( CBkWnd* which );
	virtual BOOL	OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    virtual BOOL    OnLButtonUp(UINT nFlags, CPoint point,BOOL bAct=TRUE);
    virtual BOOL    OnMouseLeave();
    virtual BOOL    OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

    virtual BOOL    OnWindowPosChanged(LPWINDOWPOS lpWndPos);

	// IDropContainer members
	virtual HRESULT OnDragEnter(IDataObject *pDataObj, POINT pt, DWORD *pdwEffect);
	virtual HRESULT OnDragOver(POINT pt, DWORD *pdwEffect);
	virtual HRESULT OnDrop(IDataObject *pDataObj, POINT pt, DWORD *pdwEffect);
	virtual HRESULT OnDragLeave( void);

    // INotify interface
    virtual BOOL    Action(NotifyInfo* pInfo);

private:
    void            _CreateScrollbar();
    void            _CalcSize( BOOL isForcePosition = FALSE );
	int				GetLineItemCount();

private:
    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_INT_ATTRIBUTE("scroll-offset", m_nScrollOffset, FALSE)
     BKWIN_DECLARE_ATTRIBUTES_END()

    int             m_nScrollOffset;     // ��������ʼ��λ��

    CBkIconBody*    m_pBody;
    CBkScroll*      m_pVScroll;
};


#include "bkwndIconView.inl"