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
	BOOL m_bFixed;			    // tab�Ƿ�̶�
    BOOL m_bPushExtraShow;      // �Ƿ�԰��µ�tab���ж������ʾ������������TRUE��Push��tabҲ�ᴦ��hover�����һ�ʹ�õ�����skin����

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

    // ����ģ�崴��һ��TAB,templateIndexΪģ�����,����ֵΪ�´�����TAB
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
    // ��ȡtab��Ĭ�ϳߴ�.���з�ʽΪTopʱ�ǿ��,Leftʱ�Ǹ߶�
    int _GetTabDefaultSize();
    // ��ȡһ������ĳߴ�.���з�ʽΪTopʱ�ǿ��,Leftʱ�Ǹ߶�
    int _GetRectSize( const CRect& rc );
    // ����Tabs����ĳߴ�
    int _CalculateTabsSize( int frontSize,int backendSize );
    // �������ʺϵ�tab�ߴ�.�ܹ�ǡ������������tabs,С������ӽ���С�ߴ��tab�ߴ�
    int _CaluculateOptimumTabSize( int tabsSize );
    // �������ʾTabs����ʼλ��
    int _CaluculateTabsStartPositon( BOOL isCanContainAll );
    // �����Ƿ���ʾ��ҳ��ť
    void _SetPagesVisible( BOOL isVisible );
    // ������ҳ��ť����ʾλ�ò����¿���Tabs����ߴ�
    void _AdjustPageButtonsPosition( int& tabssize );
    // ������ǰ��ʾ��Tab��Panel��λ��
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
		None = 0,			    // û��
		AllFront,			    // ȫ����ǰ��
		AllBackend,			    // ȫ���ں��
		BothSides,			    // �ֱ�������
	};

	int m_tabAlign;			    // tab�����з�ʽ
	int m_pageAlign;		    // ��ҳ��ť�����з�ʽ
	int m_tabWidth;			    // tab��Ĭ�ϱ�׼���
    int m_tabHeight;            // tab��Ĭ�ϱ�׼�߶�
	int m_tabMinSize;		    // tab����С�ߴ�
    int m_tabMarginX;           // tab�ı߾�X - ����tab���
    int m_tabMarginY;           // tab�ı߾�Y - ����tab���
    int m_tabPanelGap;          // tab��Panel�ļ�������ֵ
    int m_tabStartPos;          // tab����ʼ����λ��
    int m_backendMargin;        // ��������ƫ��ֵ(���������������λ�ú���ƫ�ƴ�λ��)
	int m_backendMinSize;	    // ���������С�ߴ�
    int m_pageWidth;            // ��ҳ��ť�Ŀ��
    int m_pageHeight;           // ��ҳ��ť�ĸ߶�
	int m_curPage;			    // ��ǰѡ�е�tab
    BOOL m_isBackendAutoHide;   // �Ƿ��������Զ�����.��ΪTRUE,�����������tabʱ����ʾ�������

	CBkSkinBase* m_pSkin;

	int m_tabAverageSize;	    // tabƽ���ߴ�
    int m_lastFixedTabIndex;    // ���һ���̶�Tab�����
	int m_frontSize;		    // ǰ������ߴ�
	int m_backendSize;		    // �������ߴ�
	CBkDiv* m_pDlgFront;        // ǰ������
	CBkDiv* m_pDlgBackend;      // �������

    CStringA m_strSkinBtnPrev;
    CStringA m_strSkinBtnNext;
    CBkImageBtnWnd* m_pBtnPrev; // ǰ����ť
    CBkImageBtnWnd* m_pBtnNext; // �󷭰�ť

    CBkDiv* m_panelTab;         // ��ǰѡ��Tab����ʾPanel
    BOOL    m_bFullScreen;      // �Ƿ�������ȫ����״̬

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