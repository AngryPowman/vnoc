#ifndef _I_BK_WND_H_
#define _I_BK_WND_H_

#include <vector>
#include <algorithm>
#include "bkmsg.h"

interface ATL_NO_VTABLE ISortCompare
{
    virtual BOOL Compare( int left, int right )                         = 0;
};

interface ATL_NO_VTABLE INotify
{
    virtual BOOL Action(NotifyInfo* pInfo)                              = 0;
}; 

interface ATL_NO_VTABLE IBkObj {}                                          ;

interface ATL_NO_VTABLE IBkTaborder : IBkObj
{
    virtual void        Add( IBkWnd* ctrl )                             = 0;
    virtual void        Clear()                                         = 0;
    virtual void        SetCurrent( int taborder,BOOL bFocus = FALSE )  = 0;
    virtual void        SetCurrent( IBkWnd* ctrl,BOOL bFocus = FALSE )  = 0;
    virtual IBkWnd*     GetCurrent()                                    = 0;
    virtual IBkWnd*     SetPrevious()                                   = 0;
    virtual IBkWnd*     SetNext()                                       = 0;
    virtual void        SetFocusColor(COLORREF clrFocus)                = 0;
    virtual COLORREF    GetFocusColor()                                 = 0;
    virtual void        AddAccept( IBkWnd* ctrl )                       = 0;
    virtual void        ClearAccept()                                   = 0;
    virtual IBkWnd*     SetAccept( IBkWnd* ctrl )                       = 0;
    virtual IBkWnd*     GetAccept()                                     = 0;
}; 

interface ATL_NO_VTABLE IBkEffectObject : IBkObj
{
    virtual void Init()                                                 = 0;
    virtual void Release()                                              = 0;
    virtual void Play()                                                 = 0;
    virtual void Stop()                                                 = 0;
    virtual void Update(HWND hWnd, CDCHandle& dc)                       = 0;
    virtual BOOL IsUpdate()                                             = 0;
};

interface ATL_NO_VTABLE IBkEffectRenderer : IBkObj
{
    virtual void    AddEffect(IBkEffectObject* obj)                                         = 0;
    virtual void    RemoveEffect(IBkEffectObject* obj, BOOL bDestroy = FALSE)               = 0;
    virtual void    PlayEffect(IBkEffectObject* obj)                                        = 0;
    virtual void    StopEffect(IBkEffectObject* obj)                                        = 0;
    virtual BOOL    IsEffects()                                                             = 0;
    virtual void    RenderEffectObjects(HWND hWnd, CDCHandle& dc, const CRect& rc)          = 0;
    virtual void    RenderEffectObjects(HWND hWnd, CDCHandle& dc)                           = 0;
};

interface ATL_NO_VTABLE IBkWnd : IBkObj
{
    virtual IBkWnd*     MakeCopy()                                      = 0;
    virtual IBkWnd*     GetParent() const                               = 0;
    virtual IBkWnd*     GetItem(UINT uItemID) const                     = 0;
    virtual IBkWnd*     GetItemByIndex(UINT nIndex)                     = 0;
    virtual void        SetText(LPCTSTR szText)                         = 0;
    virtual LPCTSTR     GetText() const                                 = 0;
    virtual UINT        GetCmdID() const                                = 0;
    virtual void        SetCmdID(UINT nCmdID)                           = 0;
    virtual void        GetPos(BKDLG_POSITION* pPos)                    = 0;
    virtual void        SetPos(BKDLG_POSITION* pPos, BOOL bRepaint)     = 0;
    virtual void        GetRect(LPRECT lpRc)                            = 0;
	virtual LPRECT      GetRect()                                       = 0;
    virtual void        Invalidate()                                    = 0;
    virtual BOOL        IsChecked()                                     = 0;
    virtual void        SetChecked(BOOL bCheck)                         = 0;
    virtual BOOL        IsDisable()                                     = 0;
    virtual void        SetDisable(BOOL bDisable)                       = 0;
    virtual BOOL        IsVisible()                                     = 0;
    virtual void        SetVisible(BOOL bVisible, BOOL bRedraw = TRUE)  = 0;
    virtual BOOL        SetSkin(LPCTSTR szSkin)                         = 0;
    virtual BOOL        SetStyle(LPCSTR szStyle)                        = 0;
    virtual HWND        GetRealWnd()                                    = 0;
    virtual void        RegisterNotify(INotify* pNotify)                = 0;
    virtual BOOL        IsVisibleDeep(BOOL bCheckParent = FALSE)        = 0;
    virtual BOOL        IsDragable()                                    = 0;
    virtual void        SetDragable(BOOL bDragable)                     = 0;
    virtual void        SetFocus()                                      = 0;
    virtual void        KillFocus()                                     = 0;
    virtual int         GetTaborder()                                   = 0;
    virtual void        SetTaborder(int taborder)                       = 0;
    virtual BOOL        IsAccept()                                      = 0;
    virtual void        SetAccept(BOOL bAccept)                         = 0;
    virtual void        SetEffect(IBkEffectObject* pEffect)             = 0;
};

interface ATL_NO_VTABLE IBkProgress : IBkObj
{
    virtual void        SetValue(DOUBLE dValue)                 = 0;
    virtual double      GetValue()                              = 0;
    virtual void        SetRange(UINT uMin, UINT uMax)          = 0;
    virtual void        SetShowPercent(BOOL bShow)              = 0;
};

interface ATL_NO_VTABLE IBkRadiobox : IBkObj
{
    virtual void        SetSelect(UINT nIndex)                  = 0;
    virtual int         GetSelect()                             = 0;
};

interface ATL_NO_VTABLE IBkList : IBkObj
{
    virtual int         AddRow()                                    = 0;
    virtual int         InsertRow(int nRow)                         = 0;
    virtual BOOL        SetItemText(int nRow, int nCol, int nIndex, LPCWSTR szText) = 0;
    virtual LPCWSTR     GetItemText(int nRow, int nCol, int nIndex) = 0;
    virtual BOOL        SetRowData(int nRow, DWORD dwData)          = 0;
    virtual DWORD       GetRowData(int nRow)                        = 0;
    virtual void        ClearSelectRow()                            = 0;
    virtual BOOL        SelectRow(int nRow, BOOL bSelect=TRUE)      = 0;
    virtual BOOL        IsRowSelected(int nRow)                     = 0;
    virtual int         GetSelectRow()                              = 0;
    virtual BOOL        SetSelectRows(const std::vector<int>& nRows)= 0;
    virtual BOOL        GetSelectRows(std::vector<int>& nRows)      = 0;
    virtual void        SelectAll()                                 = 0;
    virtual BOOL        DeleteRow(int nRow)                         = 0;
    virtual BOOL        DeleteAllRows()                             = 0;
    virtual int         GetRowCount()                               = 0;
    virtual int         GetSelectRowCount()                         = 0;
    virtual IBkWnd*     GetItem(int nRow, int nCol, int nIndex)     = 0;
    virtual BOOL        UpdateRowPosition(int nOldPos, int nNewPos) = 0;
    virtual int         GetRealHeight()                             = 0;
    virtual int         GetColumnCount()                            = 0;
    virtual IBkWnd*     GetHeadItem(int nCol, int index)            = 0;
    virtual BOOL        BoldRow(int nRow, BOOL bBold)               = 0;
    virtual BOOL        SetColumnWidth(int nCol, int iWidth)        = 0;
    virtual void        SortRows( ISortCompare* compare )           = 0;
    virtual BOOL        MoveRow(int sourceIdx, int targetIdx)       = 0;
};

interface ATL_NO_VTABLE IBkEditList : IBkObj
{
    virtual void        EditItem( int itemIndex )                   = 0;
};

interface ATL_NO_VTABLE IBkView : IBkObj
{
    virtual int         AddItem()                                   = 0;
    virtual int         InsertItem(int nItem)                       = 0;
    virtual IBkWnd*     GetItem(int nItem, int nIndex)              = 0;
    virtual BOOL        SetItemData(int nItem, DWORD dwData)        = 0;
    virtual DWORD       GetItemData(int nItem)                      = 0;
    virtual BOOL        SetItemText(int nItem, int nIndex, LPCWSTR szText) = 0;
    virtual LPCWSTR     GetItemText(int nItem, int nIndex)          =0;
    virtual BOOL        IsCheck(int nItem)                          = 0;
    virtual BOOL        SetCheck(int nItem, BOOL bCheck)            = 0;
};

interface ATL_NO_VTABLE IBkImage : IBkObj
{
    virtual void        SetSubImage(int iIndex)                 = 0;
};

interface ATL_NO_VTABLE IBkTreeNode : IBkObj
{
    virtual LPCWSTR    GetText()                                   = 0;
    virtual void       SetText(LPCWSTR szText)                     = 0;
    virtual DWORD      GetData()                                   = 0;
    virtual void       SetData(DWORD dwData)                       = 0;
    virtual void       SetImage(int iImage, int iSelectedImage)    = 0;
};

interface ATL_NO_VTABLE IBkSplitter : IBkObj
{
	virtual void ExpandLeft() = 0;
	virtual void ExpandRight() = 0;
};

class CBkTreeNode;
#define BK_TVI_ROOT                ((IBkTreeNode*)(ULONG_PTR)0xFFFFFFFF)
#define BK_TVI_FIRST               ((IBkTreeNode*)(ULONG_PTR)0xFFFFFFFE)
#define BK_TVI_LAST                ((IBkTreeNode*)(ULONG_PTR)0xFFFFFFFD)
interface ATL_NO_VTABLE IBkTree : IBkObj
{
    virtual IBkTreeNode*    GetRootNode()                                    = 0;
    virtual IBkTreeNode*    GetFirstNode()                                   = 0;
    virtual IBkTreeNode*    GetNextNode(IBkTreeNode* pNode)                  = 0;
    virtual IBkTreeNode*    GetNextSiblingNode(IBkTreeNode* pNode)           = 0;
    virtual IBkTreeNode*    GetChildNode(IBkTreeNode* pNode)                 = 0;
    virtual IBkTreeNode*    GetParentNode(IBkTreeNode* pNode)                = 0;
    virtual IBkTreeNode*    GetSelectedNode()                                = 0;
    virtual IBkTreeNode*    GetHoveredNode()                                 = 0;
    virtual void            DeleteAllNodes()                                 = 0;
    virtual void            DeleteNode(IBkTreeNode* pNode)                   = 0;
    virtual void            Expand(IBkTreeNode* pNode)                       = 0;
    virtual IBkTreeNode*    HitTest(const CPoint& point)                     = 0;
    virtual void            SelectNode(IBkTreeNode* pNode)                   = 0;
    virtual IBkTreeNode*    InsertNode(LPCWSTR lpszItem, int iImage, int iSelectedImage, IBkTreeNode* pParent, IBkTreeNode* pInsertAfter) = 0;
};

interface ATL_NO_VTABLE IBkWindowPaintHook : IBkObj
{
    virtual BOOL NotifyPaint(IBkWnd* pWnd, HDC hDC, const RECT& rcWin, BOOL bBeforePaint) = 0;
};

interface ATL_NO_VTABLE IBkToolBar : IBkObj
{
    virtual void ShowItem(UINT uItemID, BOOL bshow, BOOL bResort = FALSE) = 0;
    virtual void EnableItem(UINT uItemID, BOOL bEnable) = 0;
};

interface ATL_NO_VTABLE IBkIcon : IBkObj
{
    virtual void SetSrc(LPCWSTR szSrc)              = 0;
    virtual void Attach(HICON hIcon, BOOL bManaged) = 0;
};

//// Tab Control
interface ATL_NO_VTABLE IBkTabItem : IBkObj
{
    virtual IBkWnd*             GetPanel()                                  = 0;
    virtual DWORD               GetData()                                   = 0;
    virtual void                SetData(DWORD dwData)                       = 0;
    virtual IBkWnd*             GetItem(UINT uItemID) const                 = 0;
};

interface ATL_NO_VTABLE IBkTabControl : IBkObj
{
    virtual IBkTabItem*         AddTab( UINT templateID )                   = 0;
    virtual IBkTabItem*         GetTab( int index )                         = 0;
    virtual IBkTabItem*         GetCurSelItem()                             = 0;
	virtual int					GetCurSel()                                 = 0;
    virtual int                 SetCurSel( int index )                      = 0;
    virtual int                 SetLastSel()                                = 0;
    virtual int                 CloseTab( int index )                       = 0;
    virtual int                 CloseCurSel()                               = 0;
    virtual int                 GetTabCount()                               = 0;
};

interface ATL_NO_VTABLE IBkDropContainer : IBkObj
{
	virtual HRESULT OnDragEnter(IDataObject *pDataObj, POINT pt, DWORD *pdwEffect) = 0;
	virtual HRESULT OnDragOver(POINT pt, DWORD *pdwEffect) = 0;
	virtual HRESULT OnDrop(IDataObject *pDataObj, POINT pt, DWORD *pdwEffect) = 0;
	virtual HRESULT OnDragLeave(void) = 0;  
};

interface ATL_NO_VTABLE IBkDropTarget : IBkObj
{
	virtual HRESULT		Register(HWND hWnd) = 0;
	virtual HRESULT		DoDragDrop(BkDragData* bkDragDropData)       = 0;
	virtual void        PreDragDrop(CPoint ptDown) = 0;
	virtual void        UnPreDragDrop() = 0;
	virtual FORMATETC*  GetFormatetc() = 0;
	virtual vStrings&   GetDragQueryFile() = 0;
	virtual CPoint      GetLeavePoint() = 0;
	virtual CPoint      GetLBDownPoint()=0;
	virtual BkDragData* GetDragInfo() = 0;
};

#endif  // _I_BK_WND_H_
