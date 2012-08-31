//////////////////////////////////////////////////////////////////////////
//  Class Name: CBkPageList
// Description: 可翻页列表控件
//              在可视区域显示不下的情况下可进行翻页更换显示内容
//     Creator: YangDa
//     Version: 2011.05.02 - 1.0 - YangDa - Create
//////////////////////////////////////////////////////////////////////////



#pragma once
//#include "bkwndedit.h"

#define CLASSNAME_PAGELISTITEM          "pagelistitem"
#define CLASSNAME_PAGELIST              "pagelist"
#define CLASSNAME_PAGELISTTEMPLATE      "template"

class CBkPageListItem : public CBkDiv
{
    BKOBJ_DECLARE_CLASS_NAME(CBkPageListItem, CLASSNAME_PAGELISTITEM)

public:
    CBkPageListItem()
        :m_editPos("1,1,-1,-1")
    {
    }

    virtual ~CBkPageListItem()
    {
    }

    CStringA GetEditPos()
    {
        return m_editPos;
    }

public:
    virtual BOOL RedrawRegion( CDCHandle& dc, CRgn& rgn, LPRECT lpRcParent )
    {
        if( __super::RedrawRegion(dc, rgn, lpRcParent) )
        {
        }

        return FALSE;
    }


    BOOL OnLButtonDown( UINT nFlags, CPoint point )
    {
        CBkWnd::OnLButtonDown( nFlags,point );
        __super::OnLButtonDown( nFlags,point );

        return TRUE;
    }

    BOOL OnLButtonUp( UINT nFlags, CPoint point ,BOOL bAct = TRUE)
    {
        CBkWnd::OnLButtonUp( nFlags,point ,bAct);
        __super::OnLButtonUp( nFlags,point,bAct );

		if (bAct)
		{
			if( m_pNotify )
			{
				NotifyInfo info = {WM_LBUTTONUP, point, this};
				m_pNotify->Action(&info);
			}
		}

        return FALSE;
    }

    BOOL OnRButtonUp( UINT nFlags, CPoint point ,BOOL bAct = TRUE)
    {
        CBkWnd::OnRButtonUp( nFlags,point,bAct);
        __super::OnRButtonUp( nFlags,point,bAct );

		if (bAct)
		{
			if( m_pNotify )
			{
				NotifyInfo info = {WM_RBUTTONUP, point, this};
				m_pNotify->Action(&info);
			}
		}
        return FALSE;
    }

    BOOL OnMouseMove(UINT nFlags, CPoint point)
    {
        CBkWnd::OnMouseMove( nFlags,point );
        __super::OnMouseMove( nFlags,point );
        return TRUE;
    }

    BOOL OnMouseLeave()
    {
        CBkWnd::OnMouseLeave();
        __super::OnMouseLeave();
        return TRUE;
    }

    BOOL OnDestroy()
    {
        return TRUE;
    }

    BOOL OnPaint( CDCHandle dc )
    {
        //if( m_pSkin )
        //{
        //    m_pSkin->Draw( dc, m_rcWindow,IIF_STATE4(GetState(),0,1,2,3) );
        //}

        __super::OnPaint( dc );
        return TRUE;
    }

protected:
    virtual void OnLoad()
    {

    }

protected:
    CStringA m_editPos;

protected:
    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_STRING_ATTRIBUTE("editpos", m_editPos, FALSE)
        //BKWIN_INT_ATTRIBUTE("fixed", m_bFixed, FALSE)
        //BKWIN_SKIN_ATTRIBUTE("skin", m_pSkin, FALSE)
    BKWIN_DECLARE_ATTRIBUTES_END()
};


class CBkPageList : public CBkDiv
                  , public INotify
{
    BKOBJ_DECLARE_CLASS_NAME(CBkPageList, CLASSNAME_PAGELIST)

public:
    class CBkPageListEdit : public CBkEdit
    {
    public:
        CBkPageList*    m_pHost;
        CString         m_initText;
        BOOL            m_isLostFocusAccept;
        int	            m_itemEditType;
    private:
        BOOL            m_isLostFocusNoHide; //内部使用，提交或取消时的操作
    public:
        CBkPageListEdit() : CBkEdit()
        {
            m_pHost = NULL;
            m_initText= _T("");
            m_isLostFocusNoHide = FALSE;
            m_isLostFocusAccept  = TRUE;
        }

        virtual ~CBkPageListEdit()
        {
        }

        BOOL GetIsLostFocusAccept()
        {
            return m_isLostFocusAccept;
        }

        BOOL CreateEditCtrl( HWND hParent )
        {
            CreateHandle( hParent );
            if( this->IsWindow() )
                return TRUE;
            else
                return FALSE;
        }

        void SetHWndHost( CBkPageList *pHost )
        {
            m_pHost = pHost;
        }

        void SetInitText( CString strInitText )
        {
            m_initText = strInitText;
            if( this->IsWindow() )
            {
                this->SetWindowText( m_initText );
            }
        }

        void CancelEdit()
        {
            this->ShowWindow(SW_HIDE);
            m_isLostFocusNoHide = TRUE;
            //m_pHost->m_isEditing = FALSE;
            if (NULL != m_pHost && m_pHost->GetContainer() != NULL) 
            {
                //m_pHost->SetFocus();
            }
            m_isLostFocusNoHide = FALSE;
        }

        void AcceptEdit()
        {
            this->ShowWindow(SW_HIDE);
            m_isLostFocusNoHide = TRUE;
            //m_pHost->m_isEditing = FALSE;
            if (NULL != m_pHost && m_pHost->GetContainer() != NULL ) 
            {
               // m_pHost->SetFocus();

                CString strText = _T("");
                this->GetWindowText( strText );
                BOOL bContext = FALSE;
                //if (strText != m_initText)
                //    m_pHost->OnFocusEdited(m_itemEditType , bContext);

            }
            m_isLostFocusNoHide = FALSE;
        }
    public:
        BEGIN_MSG_MAP( CBkPageListEdit )
            MSG_WM_KEYDOWN( OnKeyDown )
            MESSAGE_HANDLER( WM_KILLFOCUS , OnWndKillFocus )
            MSG_WM_GETDLGCODE( OnGetDlgCode )
            CHAIN_MSG_MAP(CBkEdit)
        END_MSG_MAP()

    protected:
        LRESULT OnWndKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
        {

            if( this->IsWindowVisible() )
            {
                if( m_isLostFocusAccept )
                    AcceptEdit();//提交操作
                else
                    CancelEdit();//取消操作
            }

            return CBkEdit::OnWndKillFocus(uMsg, wParam, lParam, bHandled); 
        }
        BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
        {
            if( this->IsWindowVisible() )
            {	
                if( nChar == VK_RETURN )
                {
                    AcceptEdit();
                }
                else if( nChar == VK_ESCAPE )
                {
                    CancelEdit();
                }
                else
                {
                    SetMsgHandled(FALSE);
                    return TRUE;
                }
            }
            else
            {
                SetMsgHandled(FALSE);
                return TRUE;
            }
            SetMsgHandled(TRUE);
            return TRUE;
        }

        UINT OnGetDlgCode(LPMSG lpMsg)
        {
            return DLGC_WANTALLKEYS;
        }
    };

public:
    CBkPageList()
        :m_pBtnPrev(NULL)
        ,m_pBtnNext(NULL)
        ,m_pStaticPageCount(NULL)
        ,m_pEdit(NULL)
        ,m_pFocusItem(NULL)
        ,m_pageAlign(AlignBottom)
        ,m_pageWidth(0)
        ,m_pageHeight(0)
        ,m_pageRegionHeight(0)
        ,m_rowHeight(1)
        ,m_curPage(0)
        ,m_pageCount(0)
        ,m_itemCountEachPage(0)
    {
    }

    virtual ~CBkPageList()
    {
    }

    typedef CBkDiv __bkClass;

public:
    // Own Interface
    // 根据模板创建一个Item,templateIndex为模板序号,返回值为新创建的Item
    CBkPageListItem* AppendItemByTemplate( int templateIndex )
    {
        if( templateIndex < 0 || templateIndex >= (int)m_lstTemplate.GetCount() )
            return NULL;

        TiXmlElement* itemTemplate = m_lstTemplate.GetAt( m_lstTemplate.FindIndex(templateIndex) );        
        if( itemTemplate )
        {
            CBkPageListItem *pItem = CBkPageListItem::CheckAndNew(CLASSNAME_TABITEM);
            if( pItem )
            {
                pItem->SetParent(m_hBkWnd);
                pItem->SetContainer(m_hWndContainer);
				pItem->SetDropTarget(m_pDropTarget);
                pItem->Load(itemTemplate);
                OnChildLoad( itemTemplate,pItem );
                m_lstWndChild.AddTail(pItem);

                // 只有添加到当前页时才更新Items的位置
                BOOL isUpdateItems = (m_curPage == m_pageCount);
                _RepositionChilds( isUpdateItems );
                if( isUpdateItems )
                {
                    NotifyInvalidate();
                }

                return pItem;
            }
        }
        return NULL;
    }

    int DeleteItem( int index )
    {
        CBkPageListItem* pItem = GetItem( index );
        if( pItem )
        {
            ClearFocusItem( pItem );
            delete pItem;
            pItem = NULL;

            m_lstWndChild.RemoveAt( m_lstWndChild.FindIndex(index) );

            _ClearLastHover();
            _RepositionChilds();
            NotifyInvalidate();
            return (int)m_lstWndChild.GetCount();
        }
        return -1;
    }

    void EditItem()
    {
        if( !m_pFocusItem )
            return;

        //if( m_pFocusItem->GetHwndHost() != this )
        //    return;

        CRect rcItem;
        m_pFocusItem->GetRect( &rcItem );
        CString  strInitText = _T("");
        BOOL  bContext = TRUE;
        BOOL  bCancel = FALSE;
        int nMaxLength = 0;
        //m_pFocusItem->OnBeforeEdit(rcEdit, nEditItemType , nMaxLength ,CPoint(0,0), rcEdit, strInitText, bContext, bCancel );
        if( bCancel ) 
            return;
        //PostUserPaint( TRUE );
        OnFocusEdit( rcItem );
        return;
    }

    // Base Interface
    virtual BOOL Load(TiXmlElement* pTiXmlElem)
    {
        if (!CBkWnd::Load(pTiXmlElem))
            return FALSE;

        BOOL result = FALSE;
        result = LoadChilds(pTiXmlElem->FirstChildElement());

        _AddPageButtons();
        _AddItemEdit();
        return result;
    }

    virtual CBkWnd* FindChildByPoint(const CPoint& pt)
    {
        CBkWnd* pBkChild = __super::FindChildByPoint( pt );
        if( !pBkChild )
        {
            if( m_pBtnPrev )
            {
                if( m_pBtnPrev->IsVisible() && m_pBtnPrev->PtInRect( pt ) )
                {
                    pBkChild = m_pBtnPrev;
                }
            }

            if( !pBkChild && m_pBtnNext )
            {
                if( m_pBtnNext->IsVisible() && m_pBtnNext->PtInRect( pt ) )
                {
                    pBkChild = m_pBtnNext;
                }
            }
        }
        return pBkChild;
    }

    BOOL Action(NotifyInfo* pInfo)
    {
        // 前翻页
        if( pInfo->pFrom == m_pBtnPrev )
        {
            switch( pInfo->nAction )
            {
            case WM_LBUTTONUP:
                _DoPage( m_curPage-1 );
                break;
            }
        }

        // 后翻页
        if( pInfo->pFrom == m_pBtnNext )
        {
            switch( pInfo->nAction )
            {
            case WM_LBUTTONUP:
                _DoPage( m_curPage+1 );
                break;
            }
        }

        return TRUE;
    }

    virtual BOOL RedrawRegion( CDCHandle& dc, CRgn& rgn, LPRECT lpRcParent )
    {
        if( __super::RedrawRegion(dc, rgn, lpRcParent) )
        {
            if( m_pBtnPrev && m_pBtnPrev->IsVisible(TRUE) )
            {
                m_pBtnPrev->RedrawRegion( dc,rgn,lpRcParent );
            }

            if( m_pBtnNext && m_pBtnNext->IsVisible(TRUE) )
            {
                m_pBtnNext->RedrawRegion( dc,rgn,lpRcParent );
            }

            if( m_pStaticPageCount && m_pStaticPageCount->IsVisible(TRUE) )
            {
                m_pStaticPageCount->RedrawRegion( dc,rgn,lpRcParent );
            }
        }

        return FALSE;
    }

    // Event Handle
    BOOL OnDestroy()
    {
        m_pFocusItem = NULL;

        if( m_pBtnNext )
        {
            delete m_pBtnNext;
            m_pBtnNext = NULL;
        }

        if( m_pBtnPrev )
        {
            delete m_pBtnPrev;
            m_pBtnPrev = NULL;
        }

        if( m_pStaticPageCount )
        {
            delete m_pStaticPageCount;
            m_pStaticPageCount = NULL;
        }

        if( m_pEdit )
        {
            m_pEdit->DestroyWindow();
            delete m_pEdit;
            m_pEdit = NULL;
        }
        return TRUE;
    }

    BOOL OnLButtonDown( UINT nFlags, CPoint point )
    {
        //__super::OnLButtonDown( nFlags,point );

        if (!IsVisible(FALSE) || IsDisabled(FALSE))
        {
            return FALSE;
        }

        POSITION pos = m_lstWndChild.GetHeadPosition();
        while (pos)
        {
            CBkWnd* pBkWndChild = m_lstWndChild.GetNext(pos);
            if( !pBkWndChild->IsVisible() || pBkWndChild->IsDisabled() )
                continue;

            if (pBkWndChild->PtInRect(point))
            {
                if (!pBkWndChild->OnLButtonDown(nFlags, point))
                {
                    m_pFocusItem = (CBkPageListItem*)pBkWndChild;
                    break;
                }
            }
        }

        if( m_pBtnPrev )
        {
            if (m_pBtnPrev->PtInRect(point))
            {
                m_pBtnPrev->OnLButtonDown( nFlags,point );
            }
        }

        if( m_pBtnNext )
        {
            if (m_pBtnNext->PtInRect(point))
            {
                m_pBtnNext->OnLButtonDown( nFlags,point );
            }
        }

        return TRUE;
    }

    BOOL OnLButtonUp( UINT nFlags, CPoint point ,BOOL bAct = TRUE)
    {
        //__super::OnLButtonUp( nFlags,point,bAct );

        EditItem();

        if( m_pBtnPrev )
        {
            if (m_pBtnPrev->PtInRect(point))
            {
                m_pBtnPrev->OnLButtonUp( nFlags,point ,bAct);
            }
        }

        if( m_pBtnNext )
        {
            if (m_pBtnNext->PtInRect(point))
            {
                m_pBtnNext->OnLButtonUp( nFlags,point,bAct );
            }
        }

        return TRUE;
    }

    BOOL OnWindowPosChanged(LPWINDOWPOS lpWndPos)
    {
        CBkWnd::OnWindowPosChanged(lpWndPos);
        _RepositionChilds();
        return TRUE;
    }

    void OnExtraChildMouseMove( CPoint point,CBkWnd* &pHover,CBkWnd* &pLeave )
    {
        if( m_pBtnPrev )
        {
            if (m_pBtnPrev->PtInRect(point))
            {
                pHover = m_pBtnPrev;
            }
            else
            {
                if( m_lastHoverChild == m_pBtnPrev )
                {
                    pLeave = m_pBtnPrev;
                }
            }
        }

        if( m_pBtnNext )
        {
            if (m_pBtnNext->PtInRect(point))
            {
                pHover = m_pBtnNext;
            }
            else
            {
                if( m_lastHoverChild == m_pBtnNext )
                {
                    pLeave = m_pBtnNext;
                }
            }
        }
    }

    void OnExtraChildMouseLeave( CBkWnd* &pLeave )
    {
        if( m_pBtnPrev && m_pBtnPrev == m_lastHoverChild )
        {
            pLeave = m_pBtnPrev;
        }

        if( m_pBtnNext && m_pBtnNext == m_lastHoverChild )
        {
            pLeave = m_pBtnNext;
        }
    }



protected:
    virtual void OnChildLoad( TiXmlElement* pTiXmlElem,CBkWnd* pChildWindow )
    {
        if( pChildWindow )
        {
            pChildWindow->RegisterNotify( this );
        }

        if( pTiXmlElem )
        {
            if( strcmp(CLASSNAME_PAGELISTTEMPLATE,pTiXmlElem->Value())  == 0 )
            {
                TiXmlElement* pTiElement = (TiXmlElement*)pTiXmlElem->Clone();
                m_lstTemplate.AddTail( pTiElement );
            }
        }
    }

    CBkPageListItem* GetItem( int index )
    {
        if( index < 0 || index >= (int)m_lstWndChild.GetCount() )
            return NULL;

        return (CBkPageListItem*)m_lstWndChild.GetAt( m_lstWndChild.FindIndex(index) );
    }

    void ClearFocusItem( CBkPageListItem* curItem )
    {
        if( curItem == NULL )
        {
            m_pFocusItem = NULL;
        }
    }

    void _RepositionChilds( BOOL isUpdateItems = TRUE )
    {
        m_pageCount = _CalculatePageCount();

        _AdjustPagesPosition();
        _SetPagesEnable();
        if( isUpdateItems )
        {
            _AdjustItemsPostion();
        }
    }

    // 计算一共有多少页
    int _CalculatePageCount()
    {
        int height = m_rcWindow.Height() - m_pageRegionHeight;

        int count = 0;
        if( height > 0 && m_rowHeight > 0 )
        {
            m_itemCountEachPage = height / m_rowHeight;
            if( m_itemCountEachPage > 0 )
            {
                count = (int)m_lstWndChild.GetCount()/m_itemCountEachPage;
            }
        }
        return count;
    }

    // 计算第一个可视Item的其实Y坐标
    int _CalculateItemStartYPos()
    {
        int yPos = 0;
        switch( m_pageAlign )
        {
        case AlignTop:
            yPos = m_pageRegionHeight;
            break;
        case AlignBottom:
            break;
        }
        return yPos;
    }

    // 调整翻页按钮区域的显示位置
    void _AdjustPagesPosition()
    {
        if( m_pBtnPrev && m_pBtnNext && m_pStaticPageCount )
        {
            CString strPageCount;
            strPageCount.Format( L"%d/%d",m_curPage+1,m_pageCount+1 );
            m_pStaticPageCount->SetPositionType( SizeX_FitContent|SizeY_FitContent );
            m_pStaticPageCount->SetText( strPageCount );

            CRect rtPageCount;
            m_pStaticPageCount->GetRect( &rtPageCount );

            WINDOWPOS WndPosPrev = { 0, 0, m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height(), SWP_SHOWWINDOW };
            WINDOWPOS WndPosNext = { 0, 0, m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height(), SWP_SHOWWINDOW };
            WINDOWPOS WndPosPageCount = { 0, 0, m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height(), SWP_SHOWWINDOW };

            CStringA strPosPrev,strPosNext,strPosPageCount;
            switch( m_pageAlign )
            {
            case None:
                m_pBtnPrev->SetVisible( FALSE );
                m_pBtnNext->SetVisible( FALSE );
                break;
            case AlignTop:
                {
                    strPosNext.Format( "-0,0,-%d,%d", m_pageWidth, m_pageHeight );
                    strPosPageCount.Format( "-%d,0,-%d,%d", m_pageWidth+rtPageCount.Width(),m_pageWidth,rtPageCount.Height() );
                    strPosPrev.Format( "-%d,0,-%d,%d", m_pageWidth+rtPageCount.Width()+m_pageWidth, m_pageWidth+rtPageCount.Width(), m_pageHeight );

                    WndPosNext.x = m_rcWindow.right - m_pageWidth;
                    WndPosPageCount.x = WndPosNext.x - rtPageCount.Width();
                    WndPosPrev.x = WndPosPageCount.x - m_pageWidth;

                    m_pBtnNext->SetAttribute( "pos",strPosNext,TRUE );
                    m_pBtnNext->OnWindowPosChanged( &WndPosNext );

                    m_pStaticPageCount->SetAttribute( "pos",strPosPageCount,TRUE );
                    m_pStaticPageCount->OnWindowPosChanged( &WndPosPageCount );

                    m_pBtnPrev->SetAttribute( "pos",strPosPrev,TRUE );
                    m_pBtnPrev->OnWindowPosChanged( &WndPosPrev );
                }
                break;
            case AlignBottom:
                {
                    strPosNext.Format( "-0,-%d,-%d,-0", m_pageHeight, m_pageWidth );
                    strPosPageCount.Format( "-%d,-%d,-%d,-0", m_pageWidth+rtPageCount.Width(),rtPageCount.Height(),m_pageWidth );
                    strPosPrev.Format( "-%d,-%d,-%d,-0", m_pageWidth+rtPageCount.Width()+m_pageWidth, m_pageHeight, m_pageWidth+rtPageCount.Width() );

                    WndPosNext.x = m_rcWindow.right - m_pageWidth;
                    WndPosPageCount.x = WndPosNext.x - rtPageCount.Width();
                    WndPosPrev.x = WndPosPageCount.x - m_pageWidth;

                    WndPosPrev.y = WndPosNext.y = m_rcWindow.bottom - m_pageHeight;
                    WndPosPageCount.y = m_rcWindow.bottom - rtPageCount.Height();

                    m_pBtnNext->SetAttribute( "pos",strPosNext,TRUE );
                    m_pBtnNext->OnWindowPosChanged( &WndPosNext );

                    m_pStaticPageCount->SetAttribute( "pos",strPosPageCount,TRUE );
                    m_pStaticPageCount->OnWindowPosChanged( &WndPosPageCount );

                    m_pBtnPrev->SetAttribute( "pos",strPosPrev,TRUE );
                    m_pBtnPrev->OnWindowPosChanged( &WndPosPrev );
                }
                break;
            }
        }
    }

    // 调整可视Item的位置
    void _AdjustItemsPostion()
    {
        int itemYPos = _CalculateItemStartYPos();
        int visibleItemStartIndex = m_itemCountEachPage*m_curPage;
        int index = 0;
        int visibleItemCount = 0;
        POSITION pos = m_lstWndChild.GetHeadPosition();
        while( pos != NULL )
        {
            CBkPageListItem *pBkWndChild = (CBkPageListItem*)m_lstWndChild.GetNext(pos);
            if( pBkWndChild )
            {
                if( index >= visibleItemStartIndex && 
                    visibleItemCount < m_itemCountEachPage )
                {
                    pBkWndChild->SetVisible( TRUE );
                    visibleItemCount ++;
                }
                else
                {
                    pBkWndChild->SetVisible( FALSE );
                }

                CStringA strPos;
                strPos.Format("0,%d,-0,%d", itemYPos+(visibleItemCount-1)*m_rowHeight, itemYPos+visibleItemCount*m_rowHeight );

                WINDOWPOS WndPos = {0};
                WndPos.x = m_rcWindow.left;
                WndPos.y = m_rcWindow.top + itemYPos+(visibleItemCount-1)*m_rowHeight;
                WndPos.cx = m_rcWindow.Width();
                WndPos.cy = m_rcWindow.Height();

                pBkWndChild->SetAttribute( "pos",strPos,TRUE );
                pBkWndChild->OnWindowPosChanged( &WndPos );

                index ++;
            }
        }
    }

    // 设置翻页按钮是否可用
    void _SetPagesEnable()
    {
        if( m_pBtnNext && m_pBtnPrev )
        {
            BOOL bEnableBtnNext = TRUE;
            BOOL bEnableBtnPrev = TRUE;

            if( m_pageCount <= 0 )
            {
                bEnableBtnNext = bEnableBtnPrev = FALSE;
            }
            else if( m_curPage <= 0 )
            {
                bEnableBtnPrev = FALSE;
            }
            else if( m_curPage >= m_pageCount )
            {
                bEnableBtnNext = FALSE;
            }

            m_pBtnPrev->IsDisabled( !bEnableBtnPrev );
            m_pBtnNext->IsDisabled( !bEnableBtnNext );
        }
    }

    // 翻页操作
    void _DoPage( int curPage )
    {
        m_curPage = curPage;
        if( m_curPage < 0 )
        {
            m_curPage = 0;
        }
        else if( m_curPage > m_pageCount )
        {
            m_curPage = m_pageCount;
        }

        _RepositionChilds();
        NotifyInvalidate();
    }

    void OnFocusEdit( const CRect& rtItem )
    {
        if( !m_pFocusItem || !m_pEdit )
        {
            return;
        }

        m_pEdit->SetAttribute( "pos",m_pFocusItem->GetEditPos(),FALSE );
        WINDOWPOS WndPosPrev = { 0, 0, rtItem.left, rtItem.top, rtItem.Width(), rtItem.Height(), SWP_SHOWWINDOW };
        m_pEdit->OnWindowPosChanged( &WndPosPrev );

        m_pEdit->SetSel( 0,-1 );
        m_pEdit->ScrollCaret();
        m_pEdit->ShowWindow( SW_SHOW );
    }

    ////// 回调
    class CTraversalItem
    {
    public:
        BOOL m_bHitted;
        BOOL m_bCancel;
        DWORD m_Data;
        CRect m_rcItem;
        CBkPageListItem* m_item;
    public:
        //缺省构造
        CTraversalItem()
        {
            m_bHitted = FALSE;
            m_bCancel = FALSE;
            m_Data = 0 ;
            m_item = NULL;
        }

        //析构
        ~CTraversalItem()
        {

        }

        //拷贝构造
        CTraversalItem(const CTraversalItem &other)
        {
            m_bHitted = other.m_bHitted;
            m_bCancel = other.m_bCancel;
            m_Data = other.m_Data ;
            m_item = other.m_item;
        }

        // 赋值函数
        CTraversalItem& operator=(const CTraversalItem& other)
        {
            m_bHitted = other.m_bHitted;
            m_bCancel = other.m_bCancel;
            m_Data = other.m_Data ;
            m_item = other.m_item;
            return *this;
        }
    };

    typedef void (CALLBACK* TraversalItemProc)( CTraversalItem *pTraversalItem, CBkPageList* pHost );

    CBkPageListItem* TraversalItem( TraversalItemProc callfun, DWORD dwContext )
    {
        CBkPageListItem* item = NULL;

        POSITION pos = m_lstWndChild.GetHeadPosition();
        while (pos)
        {
            CBkPageListItem* pBkWndChild = (CBkPageListItem*)m_lstWndChild.GetNext(pos);
            if( !pBkWndChild->IsVisible() || pBkWndChild->IsDisabled() )
                continue;

            CTraversalItem traversalItem;
            traversalItem.m_item = pBkWndChild;
            traversalItem.m_Data = dwContext;

            callfun( &traversalItem,this );

            if (traversalItem.m_bHitted)
                item = pBkWndChild;
            if (traversalItem.m_bCancel)
                break;
        }

        return item;
    }

    static  void CALLBACK PaintItem( CTraversalItem *pTraversalItem, CBkPageList *pHost)
    {

    }

    static  void CALLBACK MouseMoveItem( CTraversalItem *pTraversalItem, CBkPageList *pHost)
    {

    }

    static  void CALLBACK LButtonDownItem( CTraversalItem *pTraversalItem, CBkPageList *pHost)
    {

    }

private:
    void _AddPageButtons()
    {
        if( m_pBtnPrev )
        {
            m_pBtnPrev->OnDestroy();
        }

        if( m_pBtnNext )
        {
            m_pBtnNext->OnDestroy();
        }

        if( m_pStaticPageCount )
        {
            m_pStaticPageCount->OnDestroy();
        }

        m_pBtnPrev = CBkImageBtnWnd::CheckAndNew( "imgbtn" );
        m_pBtnPrev->SetParent(m_hBkWnd);
        m_pBtnPrev->SetContainer(m_hWndContainer);
		m_pBtnPrev->SetDropTarget(m_pDropTarget);
        m_pBtnPrev->RegisterNotify( this );
        m_pBtnPrev->SetAttribute( "skin",m_strSkinBtnPrev,TRUE );

        m_pBtnNext = CBkImageBtnWnd::CheckAndNew( "imgbtn" );
        m_pBtnNext->SetParent(m_hBkWnd);
        m_pBtnNext->SetContainer(m_hWndContainer);
		m_pBtnNext->SetDropTarget(m_pDropTarget);
        m_pBtnNext->RegisterNotify( this );
        m_pBtnNext->SetAttribute( "skin",m_strSkinBtnNext,TRUE );

        m_pStaticPageCount = CBkStatic::CheckAndNew( "text" );
        m_pStaticPageCount->SetParent(m_hBkWnd);
        m_pStaticPageCount->SetContainer(m_hWndContainer);
		m_pStaticPageCount->SetDropTarget(m_pDropTarget);
    }

    void _AddItemEdit()
    {
        if( m_pEdit )
        {
            m_pEdit->OnDestroy();
        }

        m_pEdit = new CBkPageListEdit;
        m_pEdit->CreateEditCtrl( m_hWndContainer );
        m_pEdit->SetAttribute( "skin",m_strSkinEdit,TRUE );
        m_pEdit->RegisterNotify( this );
        //m_pEdit->__bkClass::SetParent( m_hBkWnd );
        m_pEdit->SetContainer( m_hWndContainer );
		m_pEdit->SetDropTarget(m_pDropTarget);
        m_pEdit->ShowWindow( SW_HIDE );
    }

protected:
    enum 
    {
        None,
        AlignTop,
        AlignBottom,
    };

    int m_pageAlign;		    // 翻页按钮的排列方式
    int m_pageWidth;            // 翻页按钮的宽度
    int m_pageHeight;           // 翻页按钮的高度
    int m_pageRegionHeight;     // 翻页区域的高度
    int m_rowHeight;            // 每行的行高
    int m_curPage;			    // 当前显示的页号
    int m_pageCount;            // 可显示的页数
    int m_itemCountEachPage;    // 每页可可显示的Item个数

    CBkSkinBase* m_pSkin;
    CStringA m_strSkinEdit;     // Edit的皮肤

    CBkPageListItem* m_pFocusItem;  // 当前选中的Item

    CStringA m_strSkinBtnPrev;
    CStringA m_strSkinBtnNext;
    CBkImageBtnWnd* m_pBtnPrev;     // 前翻按钮
    CBkImageBtnWnd* m_pBtnNext;     // 后翻按钮
    CBkStatic* m_pStaticPageCount;  // 页数文字标签
    CBkPageListEdit* m_pEdit;

    CAtlList<TiXmlElement*> m_lstTemplate;

protected:
    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_ENUM_ATTRIBUTE("pagealign", int, TRUE)
            BKWIN_ENUM_VALUE("top", AlignTop)
            BKWIN_ENUM_VALUE("bottom", AlignBottom)
        BKWIN_ENUM_END(m_pageAlign)
        BKWIN_INT_ATTRIBUTE("cursel", m_curPage, FALSE)
        BKWIN_INT_ATTRIBUTE("pagewidth", m_pageWidth, FALSE)
        BKWIN_INT_ATTRIBUTE("pageheight", m_pageHeight, FALSE)
        BKWIN_INT_ATTRIBUTE("pageregionheight", m_pageRegionHeight, FALSE)
        BKWIN_INT_ATTRIBUTE("rowheight", m_rowHeight, FALSE)
        BKWIN_SKIN_ATTRIBUTE("skin", m_pSkin, FALSE)
        BKWIN_STRING_ATTRIBUTE("prevskin", m_strSkinBtnPrev, FALSE)
        BKWIN_STRING_ATTRIBUTE("nextskin", m_strSkinBtnNext, FALSE)
        BKWIN_STRING_ATTRIBUTE("editskin", m_strSkinEdit, FALSE)
    BKWIN_DECLARE_ATTRIBUTES_END()
};