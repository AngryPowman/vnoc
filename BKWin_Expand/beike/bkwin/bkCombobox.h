#pragma once
#include "bkwndfloating.h"


#define CLASSNAME_COMBOBOX				"combobox_ex"
#define CLASSNAME_COMBOBOX_DROPDOWN	    "combobox_dropdown"


class CBkComboboxDropDown : public CBkFloatingWindow
                          , public INotify
{
    BKOBJ_DECLARE_CLASS_NAME(CBkComboboxDropDown, CLASSNAME_COMBOBOX_DROPDOWN)

public:
    CBkComboboxDropDown();
    virtual ~CBkComboboxDropDown();

    void ShowDropDown( const CPoint& ptShow, BOOL bShow ,BOOL bActiveFocus=TRUE);

    int     AddString(LPCTSTR szText);
    int     FindString(LPCTSTR szText);
    int     SelectString(LPCTSTR szText);
    int     AddItem();
    int     InsertItem(int nItem);
    BOOL    DeleteItem(int nItem);
    BOOL    DeleteAllItems();
    BOOL    SetItemText(int nItem, LPCTSTR szText);
    LPCWSTR GetItemText(int nItem );
    BOOL    SetItemData(int nItem, DWORD dwData);
    DWORD   GetItemData(int nItem);
    BOOL    SetCurSel(int nItem);
    int     GetCurSel();
    int     GetCount();
    void    SortItems( ISortCompare* compare );
	IBkWnd* GetSubItem(int nItem, int subIndex);

    virtual BOOL Load(TiXmlElement* pTiXmlElem);

protected:

    virtual BOOL LoadChilds(TiXmlElement* pTiXmlChildElem);

    virtual BOOL Action(NotifyInfo* pInfo);

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_INT_ATTRIBUTE("max-height", m_nMaxHeight, FALSE)
    BKWIN_DECLARE_ATTRIBUTES_END()

    BEGIN_MSG_MAP_EX(CBkComboboxDropDown)
        MSG_WM_KEYDOWN(_OnKeyDown)
        MSG_WM_KILLFOCUS(_OnKillFocus)
        MSG_WM_LBUTTONUP(_OnLButtonUp)
        MSG_WM_MOUSEMOVE(_OnMouseMove)
        REFLECT_NOTIFICATIONS_EX()
        CHAIN_MSG_MAP(CBkFloatingWindow)
    END_MSG_MAP()

    BOOL    _OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    void    _OnKillFocus( HWND pNewWnd );
    BOOL    _OnLButtonUp(UINT nFlags, CPoint point);
    BOOL    _OnMouseMove(UINT nFlags, CPoint point);

private:
    void    _CreateList();
    int     _GetRealHeight();

protected:
    CBkList*    m_pList;
    int         m_nMaxHeight;
};

class CBkCombobox : public CBkImageBtnWnd
                  , public INotify
{
	BKOBJ_DECLARE_CLASS_NAME(CBkCombobox, CLASSNAME_COMBOBOX)

    enum
    {
        DropDown = 1,
        DropDownList,
    };

public:
	typedef CBkImageBtnWnd __bkClass;

	CBkCombobox();
	virtual ~CBkCombobox();

    int     AddString(LPCTSTR szText);
    int     FindString(LPCTSTR szText);
    int     SelectString(LPCTSTR szText);
    int     AddItem();
    int     InsertItem(int nItem);
    BOOL    DeleteItem(int nItem);
    BOOL    DeleteAllItems();
    BOOL    SetItemText(int nItem, LPCTSTR szText);
    LPCWSTR GetItemText(int nItem);
    BOOL    SetItemData(int nItem, DWORD dwData);
    DWORD   GetItemData(int nItem);
    BOOL    SetCurSel(int nItem);
    int     GetCurSel();
    int     GetCount();
    void    SortItems( ISortCompare* compare );
    LPCWSTR GetEditText();
    BOOL    LimitText(int nMaxChars);
    BOOL    SetEditSel(int nStartChar, int nEndChar);
    IBkWnd* GetSubItem(int nItem, int subIndex);
	void	ShowList(BOOL nShow);
	void    SetDisable(BOOL bDisable);

protected:
    virtual void    CreateHandle(HWND hParent);
    virtual BOOL    LoadChilds(TiXmlElement* pTiXmlChildElem);
    virtual BOOL    RedrawRegion( CDCHandle& dc );
	virtual CBkWnd* OnSetFocus( CBkWnd* which );
    virtual BOOL    OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

    virtual BOOL    Action(NotifyInfo* pInfo);

protected:

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_ENUM_ATTRIBUTE("type", int, TRUE)
        BKWIN_ENUM_VALUE("dropdown", DropDown)
        BKWIN_ENUM_VALUE("dropdownlist", DropDownList)
        BKWIN_ENUM_END(m_nType)
        BKWIN_INT_ATTRIBUTE("margin_x", m_szMargin.cx, FALSE)
        BKWIN_INT_ATTRIBUTE("margin_y", m_szMargin.cy, FALSE)
        BKWIN_INT_ATTRIBUTE("dropdownbtn_width", m_nWidthBtnDropDown, FALSE)
        BKWIN_INT_ATTRIBUTE("dropdown_height", m_nHeightDropDown, FALSE)
        BKWIN_STRING_ATTRIBUTE("skin_dropdownbtn", m_strSkinBtnDropDown, FALSE)
        BKWIN_STRING_ATTRIBUTE("class_edit", m_strSkinEdit, FALSE)
        BKWIN_STRING_ATTRIBUTE("class_text", m_strSkinText, FALSE)
	BKWIN_DECLARE_ATTRIBUTES_END()

private:
    void _CreateDropDownList();
    void _CreateDropDownButton();
    void _CreateEdit();
    void _CreateText();

protected:
    CBkComboboxDropDown*    m_pDropDown;
    CBkImageBtnWnd*         m_pBtnDropDown;
    CBkEdit*                m_pEdit;
    CBkText*                m_pText;

    int                     m_nType;
    SIZE                    m_szMargin;
    int                     m_nWidthBtnDropDown;
    int                     m_nHeightDropDown;
    CStringA                m_strSkinBtnDropDown;
    CStringA                m_strSkinEdit;
    CStringA                m_strSkinText;
	
};

#include "bkcombobox.inl"