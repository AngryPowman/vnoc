//////////////////////////////////////////////////////////////////////////
//  Class Name: CBkTaborder
// Description: 处理TAB按键焦点顺序和默认控件设定
//     Creator: YangDa
//     Version: 2011.06.05 - 1.0 - YangDa - Create
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "IBkWnd.h"

class CBkTaborder : public IBkTaborder
{
public:
    CBkTaborder( IBkWnd* owner );
    virtual ~CBkTaborder();

public:
    void Add( IBkWnd* ctrl );
    void Clear();
    void SetCurrent( int taborder,BOOL bFocus = FALSE );
    void SetCurrent( IBkWnd* ctrl,BOOL bFocus = FALSE );
    IBkWnd* GetCurrent();
    IBkWnd* SetPrevious();
    IBkWnd* SetNext();
    void SetFocusColor(COLORREF clrFocus);
    COLORREF GetFocusColor();

    void AddAccept( IBkWnd* ctrl );
    void ClearAccept();
    IBkWnd* SetAccept( IBkWnd* ctrl );
    IBkWnd* GetAccept();


protected:
    BOOL IsValid( IBkWnd* ctrl );
    int Find( IBkWnd* ctrl );
    IBkWnd* Find( int taborder );
    IBkWnd* UpdateCurrent( int lastIndex,BOOL bFocus = TRUE );

private:
    int                     m_current;          // 当前的TAB序号
    IBkWnd*                 m_ownerWindow;      // 属于哪个窗口 
    std::vector<IBkWnd*>    m_lstTaborder;      // 设置了TAB顺序的控件列表
    COLORREF                m_clrFocus;         // 焦点框的颜色

    IBkWnd*                 m_accept;           // 当前的ACCEPT控件
    std::vector<IBkWnd*>    m_lstAccept;        // 设置了ACCEPT属性的控件列表
};

#include "bkTaborder.inl"