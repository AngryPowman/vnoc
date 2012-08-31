
inline CBkTaborder::CBkTaborder( IBkWnd* owner )
:m_current(-1)
,m_ownerWindow(owner)
,m_accept(NULL)
,m_clrFocus(RGB(0,0,255))
{

}

inline CBkTaborder::~CBkTaborder()
{

}

inline void CBkTaborder::Add( IBkWnd* ctrl )
{
    if( ctrl->GetTaborder() <= 0 )
    {
        return;
    }

    if( Find(ctrl) > 0 )
    {
        return;
    }

    AddAccept( ctrl );

    std::vector<IBkWnd*>::iterator iter;
    for( iter = m_lstTaborder.begin( ); 
         iter != m_lstTaborder.end( ); 
         ++iter )
    {
        IBkWnd* child = *iter;
        if( child->GetTaborder() > ctrl->GetTaborder() )
        {
            break;
        }
    }
    m_lstTaborder.insert( iter, ctrl );
}

inline void CBkTaborder::Clear()
{
    m_lstTaborder.clear();
    m_current = -1;
    ClearAccept();
}

inline void CBkTaborder::SetCurrent( int taborder,BOOL bFocus/* = FALSE*/ )
{
    IBkWnd* ctrl = Find( taborder );
    if( ctrl )
    {
        SetCurrent( ctrl, bFocus );
    }
}

inline void CBkTaborder::SetCurrent( IBkWnd* ctrl,BOOL bFocus/* = FALSE*/ )
{
    int i = Find( ctrl );
    if( i >= 0 )
    {
        int last = m_current;
        m_current = i;
        UpdateCurrent( last, bFocus );
    }
}

inline IBkWnd* CBkTaborder::GetCurrent()
{
    if( m_current >= 0 && m_current < (int)m_lstTaborder.size() )
    {
        return m_lstTaborder[m_current];
    }
    return NULL;
}

inline IBkWnd* CBkTaborder::SetPrevious()
{
    if( (int)m_lstTaborder.size() <= 0 )
    {
        return NULL;
    }

    int start = m_current;
    int last = m_current;

    if( m_current < 0 ) //没有选择时
    {
        start = (int)m_lstTaborder.size() - 1;
        if( IsValid(m_lstTaborder[start]) )
        {
            m_current = start;
            return UpdateCurrent( last );
        }
    }

    int i = start;
    do 
    {
        if( --i < 0 )
        {
            i = (int)m_lstTaborder.size() - 1;
        }

        // 循环搜索了一遍，仍没有合适的下一个TAB，设置回start
        if( i == start )
        {
            return UpdateCurrent( last );
        }

    } while ( !IsValid(m_lstTaborder[i]) );
    
    m_current = i;
    return UpdateCurrent( last );
}

inline IBkWnd* CBkTaborder::SetNext()
{
    if( (int)m_lstTaborder.size() <= 0 )
    {
        return NULL;
    }

    int start = m_current;
    int last = m_current;

    // 没有选择任何控件时
    if( m_current < 0 )
    {
        start = 0;
        if( IsValid(m_lstTaborder[start]) )
        {
            m_current = start;
            return UpdateCurrent( last );
        }
    }

    int i = start;
    do 
    {
        if( ++i >= (int)m_lstTaborder.size() )
        {
            i = 0;
        }

        // 循环搜索了一遍，仍没有合适的下一个TAB，设置回start
        if( i == start )
        {
            return UpdateCurrent( last );
        }

    } while( !IsValid(m_lstTaborder[i]) );

    m_current = i;
    return UpdateCurrent( last );
}

inline void CBkTaborder::SetFocusColor(COLORREF clrFocus)
{
    m_clrFocus = clrFocus;
}

inline COLORREF CBkTaborder::GetFocusColor()
{
    return m_clrFocus;
}

inline void CBkTaborder::AddAccept( IBkWnd* ctrl )
{
    if( !ctrl->IsAccept() )
    {
        return;
    }

    std::vector<IBkWnd*>::iterator iter;
    iter = std::find( m_lstAccept.begin(),m_lstAccept.end(), ctrl );
    if( iter != m_lstAccept.end() )
    {
        return;
    }

    for( iter = m_lstAccept.begin(); 
         iter != m_lstAccept.end( ); 
         ++iter )
    {
        IBkWnd* child = *iter;
        if( child->GetTaborder() > ctrl->GetTaborder() )
        {
            break;
        }
    }

    m_lstAccept.insert( iter, ctrl );
}

inline void CBkTaborder::ClearAccept()
{
    m_lstAccept.clear();
    m_accept = NULL;
}

inline IBkWnd* CBkTaborder::SetAccept( IBkWnd* ctrl )
{
    if( ctrl->IsAccept() )
    {
        m_accept = ctrl;
    }
    else if( (int)m_lstAccept.size() > 0 )
    {
        m_accept = m_lstAccept[0];
    }
    else
    {
        m_accept = NULL;
    }
    return m_accept;
}

inline IBkWnd* CBkTaborder::GetAccept()
{
    return m_accept;
}

inline BOOL CBkTaborder::IsValid( IBkWnd* ctrl )
{
    ATLASSERT( ctrl );
    ATLASSERT( ctrl->GetTaborder() > 0 );

    if( ctrl->IsDisable() || !ctrl->IsVisible() )
        return FALSE;

    return TRUE;
}

inline int CBkTaborder::Find( IBkWnd* ctrl )
{
    std::vector<IBkWnd*>::iterator iter;
    int i = 0;
    for( iter = m_lstTaborder.begin( ); 
         iter != m_lstTaborder.end( ); 
         ++iter, i++ )
    {
        IBkWnd* child = *iter;
        if( child == ctrl )
        {
            return i;
        }
    }
    return -1;
}

inline IBkWnd* CBkTaborder::Find( int taborder )
{
    std::vector<IBkWnd*>::iterator iter;
    for( iter = m_lstTaborder.begin( ); 
        iter != m_lstTaborder.end( ); 
        ++iter )
    {
        IBkWnd* ctrl = *iter;
        if( ctrl->GetTaborder() == taborder )
        {
            return ctrl;
        }
    }
    return NULL;
}

inline IBkWnd* CBkTaborder::UpdateCurrent( int lastIndex,BOOL bFocus/* = TRUE*/ )
{
    if( m_current < 0 || m_current >= (int)m_lstTaborder.size() )
        return NULL;
    
    IBkWnd* ctrl = m_lstTaborder[m_current];

    if( !IsValid(ctrl) )
        return NULL;

    if( bFocus )
    {
        ctrl->SetFocus();
    }
    return ctrl;
}