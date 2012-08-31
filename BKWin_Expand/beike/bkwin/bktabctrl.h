#pragma once

#define CLASSNAME_TABITEMPANEL	"tabitempanel"
#define CLASSNAME_TABITEM       "tabitem"
#define CLASSNAME_TABCTRL       "tabcontrol"
#define CLASSNAME_TABTEMPLATE   "template"

class CBkTabItemPanel : public CBkDiv
{
	BKOBJ_DECLARE_CLASS_NAME(CBkTabItemPanel, CLASSNAME_TABITEMPANEL)

public:
	CBkTabItemPanel();
	virtual ~CBkTabItemPanel();

public:
	CBkDiv* GetTabItem();
	BOOL SetTabItem(CBkDiv* pTabItem);
protected:
	CBkWnd* OnSetFocus(CBkWnd* which);
	BOOL OnDestroy();
	// IDropContainer members
	virtual HRESULT OnDragEnter(IDataObject *pDataObj, POINT pt, DWORD *pdwEffect);
	virtual HRESULT OnDragOver(POINT pt, DWORD *pdwEffect);
	virtual HRESULT OnDrop(IDataObject *pDataObj, POINT pt, DWORD *pdwEffect);
	virtual HRESULT OnDragLeave( void);

protected:
	CBkDiv* m_pTablItem;

};

class CBkTabItem : public CBkDiv
{
	BKOBJ_DECLARE_CLASS_NAME(CBkTabItem, CLASSNAME_TABITEM)

public:
    typedef CBkDiv __bkClass;

    CBkTabItem();
    virtual ~CBkTabItem();

public:
    CBkDiv* GetPanel();
    void SetPanel( CBkDiv* pTabPanel );
    void ShowTabPanel( BOOL isVisible );

	BOOL isFixed();

    void UpdateState( DWORD dwStateAdd, DWORD dwStateRemove );

    //////////////////////////////////////////////////////////////////////////
    // Base interface    
    virtual void SetContainer(HWND hWndContainer);
    virtual void SetRealContainer(HWND hWndRealContainer);
    virtual CBkWnd* FindChildByPoint(const CPoint& pt, BOOL bBackground);
    virtual CBkWnd* FindChildByCmdID(UINT uCmdID) const;
	virtual void    SetDropTarget(IBkDropTarget* pDropTarget);

    //////////////////////////////////////////////////////////////////////////
    // interface Message Event Handle
    BOOL OnLButtonDown( UINT nFlags, CPoint point );
    BOOL OnLButtonUp( UINT nFlags, CPoint point, BOOL bAct = TRUE );
	BOOL OnRButtonUp( UINT nFlags, CPoint point, BOOL bAct = TRUE);
	BOOL OnLButtonDblClk( UINT nFlags, CPoint point );
    BOOL OnMouseMove(UINT nFlags, CPoint point);
    BOOL OnMouseHover();
    BOOL OnMouseLeave();
    //BOOL OnDestroy();
	BOOL OnPaint( CDCHandle dc );
	BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    CBkWnd* OnSetFocus( CBkWnd* which );

protected:
	BOOL m_bFixed;			    // tab是否固定
    BOOL m_bPushExtraShow;      // 是否对按下的tab进行额外的显示，此属性设置TRUE后，Push的tab也会处理hover，并且会使用单独的skin绘制

    CBkDiv*         m_panel;
	CBkSkinBase*    m_pSkin;

protected:
	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_INT_ATTRIBUTE("fixed", m_bFixed, FALSE)
        BKWIN_INT_ATTRIBUTE("push_extra", m_bPushExtraShow, FALSE)
		BKWIN_SKIN_ATTRIBUTE("skin", m_pSkin, FALSE)
	BKWIN_DECLARE_ATTRIBUTES_END()
};

class CBkTabControl : public CBkDiv
                    , public INotify
{
	BKOBJ_DECLARE_CLASS_NAME(CBkTabControl, CLASSNAME_TABCTRL)

public:
    typedef CBkDiv __bkClass;

	CBkTabControl();
	virtual ~CBkTabControl();

public:
    //////////////////////////////////////////////////////////////////////////
    // interface IBkTabControl

    // 根据模板创建一个TAB,templateIndex为模板序号,返回值为新创建的TAB
    CBkTabItem* AppendItemByTemplate( int templateIndex );
    CBkTabItem* InsertItemByTemplate( int templateIndex, int tabIndex );
    void MoveHeadToTail(int count);
	void MoveIndexToHead(int index);
    CBkTabItem* GetTab( int index );
    CBkDiv* GetTabPanel( UINT tabID );
    CBkTabItem* GetCurSelItem();
	int GetCurSel();
    int SetCurSel( int index );
    int SetCurSelByID( UINT uItemID );
    int SetLastSel();
    int CloseTab( int index );
    int CloseCurSel();
    int GetTabCount();
    int GetVisibleTabCount();
    BOOL IsVisibleTab(int index);
    BOOL IsFullScreen();
    void FullScreen(BOOL bFullScreen);
    void ShowTab( int index, BOOL bShow );

    // Base Interface
    virtual void CreateHandle(HWND hParent);
	virtual BOOL Load(TiXmlElement* pTiXmlElem);
    virtual CBkWnd* FindChildByCmdID(UINT uCmdID) const;
    virtual CBkWnd* FindChildByPoint(const CPoint& pt, BOOL bBackground);
    BOOL Action(NotifyInfo* pInfo);
	virtual BOOL RedrawRegion( CDCHandle& dc );

    // Own Virtual Function
    virtual void OnCancelTabSel( int index );
    virtual void OnTabSel( int index );

    // Event Handle
    BOOL OnDestroy();
	BOOL OnWindowPosChanged(LPWINDOWPOS lpWndPos);

protected:
    virtual BOOL LoadChilds(TiXmlElement* pTiXmlChildElem);

    void _ClearAllSel();
    int _SetCurSel( int index );
    // 获取tab的默认尺寸.排列方式为Top时是宽度,Left时是高度
    int _GetTabDefaultSize();
    // 获取一个区域的尺寸.排列方式为Top时是宽度,Left时是高度
    int _GetRectSize( const CRect& rc );
    // 计算Tabs区域的尺寸
    int _CalculateTabsSize( int frontSize,int backendSize );
    // 计算最适合的tab尺寸.能够恰好容纳整数个tabs,小于且最接近最小尺寸的tab尺寸
    int _CaluculateOptimumTabSize( int tabsSize );
    // 计算可显示Tabs区域开始位置
    int _CaluculateTabsStartPositon( BOOL isCanContainAll );
    // 设置是否显示翻页按钮
    void _SetPagesVisible( BOOL isVisible );
    // 调整翻页按钮的显示位置并更新可视Tabs区域尺寸
    void _AdjustPageButtonsPosition( int& tabssize );
    // 调整当前显示的Tab绑定Panel的位置
    void _AdjustTabPanelPosition( CBkDiv* tabPanel );
	void _RepositionChilds();

private:
    void _AddPageButtons();

    void _LoadFront(TiXmlElement* pTiXmlElem,CBkDiv* front);
    void _LoadBackend(TiXmlElement* pTiXmlElem,CBkDiv* backend);
    void _LoadTabPanel(TiXmlElement* pTiXmlElem,CBkTabItem* tab);

    void _AddHideTab( CBkWnd* tab );
    void _RemoveHideTab( CBkWnd* tab );
    void _ClearHideTabs();
    BOOL _IsHideTab( CBkWnd* tab );

protected:
	enum 
	{
		AlignTop,
		AlignLeft,
	};

	enum
	{
		None = 0,			    // 没有
		AllFront,			    // 全部在前端
		AllBackend,			    // 全部在后端
		BothSides,			    // 分别在两侧
	};

	int m_tabAlign;			    // tab的排列方式
	int m_pageAlign;		    // 翻页按钮的排列方式
	int m_tabWidth;			    // tab的默认标准宽度
    int m_tabHeight;            // tab的默认标准高度
	int m_tabMinSize;		    // tab的最小尺寸
    int m_tabMarginX;           // tab的边距X - 两个tab间距
    int m_tabMarginY;           // tab的边距Y - 两个tab间距
    int m_tabPanelGap;          // tab和Panel的间距间距调整值
    int m_tabStartPos;          // tab的起始计算位置
    int m_backendMargin;        // 后端区域的偏移值(当计算完后端区域的位置后，再偏移此位置)
	int m_backendMinSize;	    // 后端区域最小尺寸
    int m_pageWidth;            // 翻页按钮的宽度
    int m_pageHeight;           // 翻页按钮的高度
	int m_curPage;			    // 当前选中的tab
    BOOL m_isBackendAutoHide;   // 是否后端区域自动隐藏.若为TRUE,则可容纳所有tab时不显示后端区域

	CBkSkinBase* m_pSkin;

	int m_tabAverageSize;	    // tab平均尺寸
    int m_lastFixedTabIndex;    // 最后一个固定Tab的序号
	int m_frontSize;		    // 前端区域尺寸
	int m_backendSize;		    // 后端区域尺寸
	CBkDiv* m_pDlgFront;        // 前端区域
	CBkDiv* m_pDlgBackend;      // 后端区域

    CStringA m_strSkinBtnPrev;
    CStringA m_strSkinBtnNext;
    CBkImageBtnWnd* m_pBtnPrev; // 前翻按钮
    CBkImageBtnWnd* m_pBtnNext; // 后翻按钮

    CBkDiv* m_panelTab;         // 当前选中Tab的显示Panel
    BOOL    m_bFullScreen;      // 是否正处于全屏显状态

    CAtlList<CBkWnd*> m_lstTabs;
	CAtlList<CBkWnd*> m_lstHideTabs;
    CAtlList<TiXmlElement*> m_lstTemplate;

protected:
	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_ENUM_ATTRIBUTE("tabalign", int, TRUE)
			BKWIN_ENUM_VALUE("top", AlignTop)
			BKWIN_ENUM_VALUE("left", AlignLeft)
		BKWIN_ENUM_END(m_tabAlign)
		BKWIN_ENUM_ATTRIBUTE("pagealign", int, TRUE)
			BKWIN_ENUM_VALUE("none", None)
			BKWIN_ENUM_VALUE("allfront", AllFront)
			BKWIN_ENUM_VALUE("allbackend", AllBackend)
			BKWIN_ENUM_VALUE("bothsides", BothSides)
		BKWIN_ENUM_END(m_pageAlign)
		BKWIN_INT_ATTRIBUTE("tabwidth", m_tabWidth, FALSE)
        BKWIN_INT_ATTRIBUTE("tabheight", m_tabHeight, FALSE)
		BKWIN_INT_ATTRIBUTE("tabminsize", m_tabMinSize, FALSE)
        BKWIN_INT_ATTRIBUTE("tabmarginx", m_tabMarginX, FALSE)
        BKWIN_INT_ATTRIBUTE("tabmarginy", m_tabMarginY, FALSE)
        BKWIN_INT_ATTRIBUTE("tabpanelgap", m_tabPanelGap, FALSE)
        BKWIN_INT_ATTRIBUTE("tabstartpos", m_tabStartPos, FALSE)
		//BKWIN_INT_ATTRIBUTE("backendminsize", m_backendMinSize, FALSE)
		BKWIN_INT_ATTRIBUTE("cursel", m_curPage, FALSE)
		BKWIN_SKIN_ATTRIBUTE("skin", m_pSkin, FALSE)
        BKWIN_STRING_ATTRIBUTE("prevskin", m_strSkinBtnPrev, FALSE)
        BKWIN_STRING_ATTRIBUTE("nextskin", m_strSkinBtnNext, FALSE)
        BKWIN_INT_ATTRIBUTE("pagewidth", m_pageWidth, FALSE)
        BKWIN_INT_ATTRIBUTE("pageheight", m_pageHeight, FALSE)
	BKWIN_DECLARE_ATTRIBUTES_END()
};

#include "bktabctrl.inl"