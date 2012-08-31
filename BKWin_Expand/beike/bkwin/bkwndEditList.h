//////////////////////////////////////////////////////////////////////////
//   File Name: bkwndEditList.h
// Description: 可编辑列表控件
//     Creator: YangDa
//     Version: 2011.05.11 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#pragma once

class CBkEditList : public CBkList
                  , public IBkEditList
{
    BKOBJ_DECLARE_CLASS_NAME(CBkEditList, "edit_list")

public:
    class CBkListEdit : public CBkEdit
    {
    public:
        CBkEditList*    m_pHost;
        CString         m_initText;
        BOOL            m_isLostFocusAccept;    // 是否失去焦点后提交内容
        int	            m_itemEditType;
    private:
        BOOL            m_isLostFocusNoHide;    //内部使用，提交或取消时的操作
    public:
        CBkListEdit() : CBkEdit()
        {
            m_pHost = NULL;
            m_initText= _T("");
            m_isLostFocusNoHide = FALSE;
            m_isLostFocusAccept  = TRUE;
        }

        virtual ~CBkListEdit()
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

        void SetHWndHost( CBkEditList *pHost )
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
            if( m_pHost && m_pHost->GetIsEditing() )
            {
                this->ShowWindow(SW_HIDE);
                m_isLostFocusNoHide = TRUE;
				m_pHost->OnFocusEdited(FALSE);
                if (NULL != m_pHost && m_pHost->GetContainer() != NULL) 
                {
                    m_pHost->SetFocus();
                }
                m_isLostFocusNoHide = FALSE;
            }
        }

        void AcceptEdit()
        {
            if( m_pHost && m_pHost->GetIsEditing() )
            {
                this->ShowWindow(SW_HIDE);
                m_isLostFocusNoHide = TRUE;
                if (NULL != m_pHost && m_pHost->GetContainer() != NULL ) 
                {
                    m_pHost->SetFocus();

                    CString strText = _T("");
                    this->GetWindowText( strText );
                    BOOL bContext = FALSE;
                    if( m_initText.IsEmpty() || strText != m_initText )
                    {
                        m_pHost->OnFocusEdited(TRUE);
                    }
					else
					{
						m_pHost->OnFocusEdited(FALSE);
					}
                }

                m_isLostFocusNoHide = FALSE;
            }
        }

        BOOL IsLostFocusAccept()
        {
            return m_isLostFocusAccept;
        }

    public:
        BEGIN_MSG_MAP( CBkListEdit )
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
    CBkEditList()
        :m_pEdit(NULL)
        ,m_editPos("1,1,-1,-1")
        ,m_nEditing(-1)
    {

    }

    ~CBkEditList()
    {

    }

    typedef CBkDiv __bkClass;

public:
    CStringA GetEditPos()
    {
        return m_editPos;
    }

    BOOL GetIsEditing()
    {
        return m_nEditing != -1;
    }

    void EditItem( int itemIndex )
    {
        ScrollToRow(itemIndex);

        CRect rcItem = m_pListBody->GetRowRect( itemIndex );

        if( rcItem.Width() <= 0 || rcItem.Height() <= 0 )
        {
            CRect rcRow = m_pListBody->GetRowRect(0);
            m_pListBody->GetRect( &rcItem );
            rcItem.bottom = rcItem.top + rcRow.Height();
            m_pVScroll->SetValue(0);
        }

        CString  strInitText = _T("");
        BOOL  bContext = TRUE;
        BOOL  bCancel = FALSE;
        int nMaxLength = 0;

        BKM_EDITLIST_ARG editListArg;
        editListArg.itemIndex = itemIndex;

        if( m_pNotify )
        {
            NotifyInfo info = { BKM_EDITLIST_PREEDIT, CPoint(0, 0), this, (DWORD)&editListArg };
            m_pNotify->Action( &info );
        }

        if( editListArg.isCancel ) 
            return;

        OnFocusEdit( rcItem, editListArg );
        return;
    }

    //// Base Interface
    virtual void KillFocus()
    {
        if( GetIsEditing() && m_pEdit )
        {
            m_pEdit->IsLostFocusAccept() ? m_pEdit->AcceptEdit() : m_pEdit->CancelEdit();
        }
        __super::KillFocus();
    }
	inline void OnVisibleChanged(BOOL bVisible){
		if ( !bVisible )
		{
			KillFocus() ;
		}
	}

public:
    virtual BOOL Load(TiXmlElement* pTiXmlElem)
    {
        BOOL result = FALSE;
        result = __super::Load( pTiXmlElem );

        _AddItemEdit();
        return result;
    }


protected:
    virtual CBkWnd* OnSetFocus( CBkWnd* which )
    {
        __super::OnSetFocus( this );
        //m_focusChild = which;
        return this;
    }

	inline BOOL Action(NotifyInfo* pInfo)
	{
		if (pInfo->nAction == BKM_LIST_SELCHANGED)
		{
			if( GetIsEditing() && m_pEdit )
			{
				m_pEdit->IsLostFocusAccept() ? m_pEdit->AcceptEdit() : m_pEdit->CancelEdit();
			}
		}

		return __super::Action(pInfo);
	}

	BOOL OnLButtonDown(UINT nFlags, CPoint point)
	{
		if( GetIsEditing() && m_pEdit )
		{
			m_pEdit->IsLostFocusAccept() ? m_pEdit->AcceptEdit() : m_pEdit->CancelEdit();
		}

		return __super::OnLButtonDown(nFlags, point);
	}

	BOOL OnRButtonDown(UINT nFlags, CPoint point)
	{
		if( GetIsEditing() && m_pEdit )
		{
			m_pEdit->IsLostFocusAccept() ? m_pEdit->AcceptEdit() : m_pEdit->CancelEdit();
		}

		return __super::OnRButtonDown(nFlags, point);	
	}

    BOOL OnFocusEdit( const CRect& rtItem, BKM_EDITLIST_ARG arg )
    {
        if( !m_pEdit )
        {
            return FALSE;
        }

        m_pEdit->SetAttribute( "pos",GetEditPos(),FALSE );
        RepositionChild(m_pEdit, rtItem);
        //WINDOWPOS WndPosPrev = { 0, 0, rtItem.left, rtItem.top, rtItem.Width(), rtItem.Height(), SWP_SHOWWINDOW };
        //m_pEdit->OnWindowPosChanged( &WndPosPrev );

        m_pEdit->ShowWindow( SW_SHOW );
        m_pEdit->SetText( arg.editText );
        m_pEdit->SetSel( 0,-1 );
        m_pEdit->ScrollCaret();
        m_pEdit->CBkWnd::SetFocus();

		m_nEditing = arg.itemIndex;
		return TRUE;
    }

    void OnFocusEdited(BOOL bAccept)
    {
        if( !m_pEdit )
        {
            return;
        }

		int nEdit = m_nEditing;
		m_nEditing = -1;
		if (bAccept)
		{
			BKM_EDITLIST_ARG editListArg;
			m_pEdit->GetWindowText( editListArg.editText );
			editListArg.itemIndex = nEdit;

			if( m_pNotify )
			{
				NotifyInfo info = { BKM_EDITLIST_AFTEREDIT, CPoint(0, 0), this, (DWORD)&editListArg };
				m_pNotify->Action( &info );
			}
		}
    }

	virtual BOOL OnDestroy()
	{
		BOOL bRet = __super::OnDestroy();
		if (m_pEdit)
		{
			m_pEdit->OnDestroy();

			delete m_pEdit;
			m_pEdit = NULL;
		}
		return bRet;
	}

private:
    void _AddItemEdit()
    {
        if( m_pEdit )
        {
            m_pEdit->OnDestroy();
        }

        m_pEdit = new CBkListEdit;
        HWND hParent = GetRealContainer()?GetRealContainer():GetContainer();
        m_pEdit->CreateEditCtrl( hParent );
        m_pEdit->SetHWndHost( this );
        m_pEdit->SetAttribute( "class",m_strSkinEdit,TRUE );
        m_pEdit->RegisterNotify( this );
        m_pEdit->CBkWnd::SetParent( m_hBkWnd );
        m_pEdit->SetContainer( m_hWndContainer );
        m_pEdit->SetRealContainer( m_hWndRealContainer );
        m_pEdit->SetParentsVisible(IsVisible());
        m_pEdit->SetTabControler(m_tabControler);
        m_pEdit->SetTooltipCtrl(m_wndToolTip);
        m_pEdit->ShowWindow( SW_HIDE );
    }

protected:
    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_STRING_ATTRIBUTE("editclass", m_strSkinEdit, FALSE)
        BKWIN_STRING_ATTRIBUTE("editpos", m_editPos, FALSE)
    BKWIN_DECLARE_ATTRIBUTES_END()

protected:
    CBkListEdit* m_pEdit;

    CStringA m_strSkinEdit;     // Edit的皮肤
    CStringA m_editPos;         // Edit的位置

	int  m_nEditing;
};