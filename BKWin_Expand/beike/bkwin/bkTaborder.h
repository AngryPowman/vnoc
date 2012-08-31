//////////////////////////////////////////////////////////////////////////
//  Class Name: CBkTaborder
// Description: ����TAB��������˳���Ĭ�Ͽؼ��趨
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
    int                     m_current;          // ��ǰ��TAB���
    IBkWnd*                 m_ownerWindow;      // �����ĸ����� 
    std::vector<IBkWnd*>    m_lstTaborder;      // ������TAB˳��Ŀؼ��б�
    COLORREF                m_clrFocus;         // ��������ɫ

    IBkWnd*                 m_accept;           // ��ǰ��ACCEPT�ؼ�
    std::vector<IBkWnd*>    m_lstAccept;        // ������ACCEPT���ԵĿؼ��б�
};

#include "bkTaborder.inl"