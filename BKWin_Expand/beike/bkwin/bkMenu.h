#pragma once
#include "bkwndfloating.h"


#define CLASSNAME_MENU				"menu"
#define CLASSNAME_MENU_BODY	        "menu-body"
#define CLASSNAME_MENU_ITEM	        "menu-item"
#define CLASSNAME_MENU_SEPARATOR	"menu-separator"
#define CLASSNAME_MENU_SUB	        "menu-sub"
#define CLASSNAME_MENU_TEMPLATE     "menu-template"

enum
{
    Menu_Item = 1,
    Menu_Separator,
    Menu_Sub,
};


class CBkMenu;
class CBkMenuItem : public CBkDiv
                  , public INotify
{
    BKOBJ_DECLARE_CLASS_NAME(CBkMenuItem, CLASSNAME_MENU_ITEM)

public:
    CBkMenuItem();
    CBkMenuItem(int type);
    virtual ~CBkMenuItem();

    int             GetType();
    CBkMenu*        GetSubMenu() const;
    void            ShowSubMenu(BOOL bShow);
	void			ShowSubMenuStart(BOOL bShow);

    virtual void    SetDisable(BOOL bDisable);
    virtual void    SetChecked(BOOL bCheck);
    virtual int     GetItemWidth();
	virtual BOOL    OnDestroy();
	virtual BOOL    OnMouseLeave();
	void UpdateState(DWORD dwStateAdd, DWORD dwStateRemove);

protected:
    virtual void    OnChildLoad( TiXmlElement* pTiXmlElem,CBkWnd* pChildWindow );

    virtual BOOL    OnLButtonDown(UINT nFlags, CPoint point);
    virtual BOOL    OnLButtonUp(UINT nFlags, CPoint point,BOOL bAct = TRUE);
    virtual BOOL    OnMouseHover();

    static VOID CALLBACK OnTimer_ShowSubMenu( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime );
    static VOID CALLBACK OnTimer_HideSubMenu( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime );

    virtual BOOL    Action(NotifyInfo* pInfo);

protected:
    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_INT_ATTRIBUTE("text-margin-left", m_nTextMarginLeft, FALSE)
        BKWIN_INT_ATTRIBUTE("text-margin-right", m_nTextMarginRight, FALSE)
        BKWIN_INT_ATTRIBUTE("submenu-gap", m_nSubMenuGapX, FALSE)
		BKWIN_INT_ATTRIBUTE("submenu-gap-y", m_nSubMenuGapY, FALSE)
		BKWIN_INT_ATTRIBUTE("type", m_nType, FALSE)
    BKWIN_DECLARE_ATTRIBUTES_END()

private:
    int             m_nType;                // 菜单项类别
    CBkMenu*        m_pSubMenu;             // 子菜单对象

    int             m_nTextMarginLeft;      // 菜单项文字部分距左侧的距离
    int             m_nTextMarginRight;     // 菜单项文字部分距右侧的距离
    int             m_nSubMenuGapX;          // 子菜单与子菜单项的间隔
	int             m_nSubMenuGapY;          // 子菜单与子菜单项的间隔
};

class CBkMenuBody : public CBkDivV
                  , public INotify
{
    BKOBJ_DECLARE_CLASS_NAME(CBkMenuBody, CLASSNAME_MENU_BODY)

public:
    CBkMenuBody();
    virtual ~CBkMenuBody();

    int         GetRealWidth();
    void        HideSubMenus();

    CBkMenu*    GetSubMenu(int nPos) const;
    int         AppendMenu(UINT nTemplateIndex, UINT_PTR nIDNewItem, int type);
	int			DeleteMenu(UINT nIDItem, UINT nFlag);

	void		HideOtherSubmenu(CBkMenuItem* currentItem);

protected:
    virtual BOOL    Action(NotifyInfo* pInfo);

    virtual BOOL    LoadChilds(TiXmlElement* pTiXmlChildElem);

    CBkMenuItem*    _NewItem(int type, TiXmlElement* pTiXmlChildElem);
    CBkMenuItem*    _NewItem(TiXmlElement* pTiXmlChildElem);

private:
    CAtlList<TiXmlElement*> m_lstTemplate;       // 模板列表
};


class CBkMenu : public CBkFloatingWindow
              , public INotify
{
    BKOBJ_DECLARE_CLASS_NAME(CBkMenu, CLASSNAME_MENU)

public:
    CBkMenu();
    virtual ~CBkMenu();

    void        ShowMenu(const CPoint& pt,LPCTSTR szXml);
    void        ShowSubMenu(const CPoint& pt);

    BOOL        CreateMenu(LPCTSTR szXml, HWND hParentWnd);
    BOOL        TrackPopupMenu(const CPoint& pt);
    CBkMenu*    GetSubMenu(int nPos) const;
	CBkMenu*	GetSubMenu(UINT nIDItem, UINT nFlags);
    int         AppendMenu(UINT nTemplateIndex, UINT_PTR nIDNewItem = 0, int type = Menu_Item);
	int 		DeleteMenu(UINT nIDItem, UINT nFlag);

    UINT        CheckMenuItem(UINT nIDCheckItem, UINT nCheck);
    BOOL        CheckMenuRadioItem(UINT nIDFirst, UINT nIDLast, UINT nIDItem, UINT nFlags);
    UINT        EnableMenuItem(UINT nIDEnableItem, UINT nEnable);

    SIZE        GetMenuSize();
    int         GetMenuItemCount() const;
    IBkWnd*     GetMenuItemChild(int nMenuIndex, int nChildIndex);
	CBkMenuItem* GetMenuItem(UINT nPosition, UINT nFlags);

protected:
    virtual BOOL    Action(NotifyInfo* pInfo);
    virtual BOOL    LoadChilds(TiXmlElement* pTiXmlChildElem);
    virtual void    OnShown(BOOL bShow);

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_INT_ATTRIBUTE("margin-top", m_nMarginTop, FALSE)
        BKWIN_INT_ATTRIBUTE("margin-bottom", m_nMarginBottom, FALSE)
        BKWIN_INT_ATTRIBUTE("margin-left", m_nMarginLeft, FALSE)
        BKWIN_INT_ATTRIBUTE("margin-right", m_nMarginRight, FALSE)
        BKWIN_INT_ATTRIBUTE("maxwidth", m_nMaxWidth, FALSE)
    BKWIN_DECLARE_ATTRIBUTES_END()

	BOOL _OnLButtonDown(UINT nFlags, CPoint point);
	BOOL _OnMouseMove(UINT dwKey, CPoint point);
	BOOL _OnMouseHover(UINT_PTR dwKey, CPoint point);
	BOOL _OnMouseLeave();

 	BEGIN_MSG_MAP_EX(CBkMenu)
 		MSG_WM_LBUTTONDOWN(_OnLButtonDown)
		//MSG_WM_MOUSEMOVE(_OnMouseMove)
		//MSG_WM_MOUSEHOVER(_OnMouseHover)
		//MSG_WM_MOUSELEAVE(_OnMouseLeave)
		CHAIN_MSG_MAP(CBkFloatingWindow)
 	END_MSG_MAP()

private:
    int     _GetRealHeight();
    int     _GetRealWidth();
    void    _HideSubMenus();

private:
    CBkMenuBody*    m_pBody;
    int             m_nMarginTop;           // 菜单BODY区域据边框顶部的距离
    int             m_nMarginBottom;        // 菜单BODY区域据边框底部的距离
    int             m_nMarginLeft;          // 菜单BODY区域据边框左侧的距离
    int             m_nMarginRight;         // 菜单BODY区域据边框右侧的距离
    int             m_nMaxWidth;            // 菜单项最大宽度
};

#include "bkMenu.inl"