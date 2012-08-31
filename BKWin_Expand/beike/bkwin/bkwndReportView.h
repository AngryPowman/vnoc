//////////////////////////////////////////////////////////////////////////
//   File Name: bkwndView.h
// Description: View Control
//     Creator: DongM
//     Version: 2011.04.08 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////////
// CBkListBody定义

class CBkViewItem
    : public CBkDiv
{
    BKOBJ_DECLARE_CLASS_NAME(CBkViewItem, "view_item")

public:
    CBkViewItem();

    virtual BOOL    IsChecked();
    virtual void    SetChecked(BOOL bCheck);

protected:
    virtual void    OnChildLoad(TiXmlElement* pTiXmlElem, CBkWnd* pChildWindow);
    virtual BOOL    OnLButtonUp(UINT nFlags, CPoint point,BOOL bAct = TRUE);
    virtual BOOL    NeedRedrawWhenStateChange();

private:
    CBkCheckBox*    m_pCheck;
};

class CBkReportViewBody
    : public CBkDivZ
    , public INotify
{
    BKOBJ_DECLARE_CLASS_NAME(CBkViewItem, "view_body")

public:
    CBkReportViewBody();
    ~CBkReportViewBody();

    int     AddItem();
    int     InsertItem(int nItem);
    IBkWnd* GetItem(int nItem, int nIndex);
    BOOL    SetItemData(int nItem, DWORD dwData);
    DWORD   GetItemData(int nItem);
    BOOL    SetItemText(int nItem, int nIndex, LPCWSTR szText);
    LPCWSTR GetItemText(int nItem, int nIndex);
    BOOL    IsCheck(int nItem);
    BOOL    SetCheck(int nItem, BOOL bCheck);
    int     _GetRealHeight();

protected:
    // CBkWnd virtual method
    virtual BOOL    Load(TiXmlElement* pTiXmlElem);
    virtual BOOL    OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    // INotify interface
    virtual BOOL    Action(NotifyInfo* pInfo);

private:
    CBkViewItem*    _NewItem();
    CBkWnd*         _GetItem(int nItem, int nIndex);

private:
    TiXmlNode*  m_pDefault;          // 默认的模板
};

class CBkReportView
    : public CBkDiv
    , public IBkView
    , public INotify
{
    BKOBJ_DECLARE_CLASS_NAME(CBkReportView, "view")

public:
    CBkReportView();

    virtual int     AddItem();
    virtual int     InsertItem(int nItem);
    virtual IBkWnd* GetItem(int nItem, int nIndex);
    virtual BOOL    SetItemData(int nItem, DWORD dwData);
    virtual DWORD   GetItemData(int nItem);
    virtual BOOL    SetItemText(int nItem, int nIndex, LPCWSTR szText);
    virtual LPCWSTR GetItemText(int nItem, int nIndex);
    virtual BOOL    IsCheck(int nItem);
    virtual BOOL    SetCheck(int nItem, BOOL bCheck);

protected:
    // CBkWnd virtual method
    virtual BOOL    LoadChilds(TiXmlElement* pTiXmlChildElem);
    virtual BOOL    OnWindowPosChanged(LPWINDOWPOS lpWndPos);
    // INotify interface
    virtual BOOL    Action(NotifyInfo* pInfo);

private:
    void            _CalcSize();

protected:
    CBkReportViewBody*  m_pBody;
    CBkScroll*          m_pVScroll;
};


#include "bkwndReportView.inl"