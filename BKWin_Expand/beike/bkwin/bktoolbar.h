//////////////////////////////////////////////////////////////////////////
//   File Name: bktoolbar.h
// Description: ToolBar Control
//     Creator: Guyuchuan
//     Version: 2011.05.10 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////
#pragma once

// CBkToolBar∂®“Â
class CBkToolBar
    : public CBkDivH
    , public IBkToolBar
    , public INotify
{
    BKOBJ_DECLARE_CLASS_NAME(CBkToolBar, "toolbar")
public:
    CBkToolBar();
    ~CBkToolBar();
    // interface IBkToolBar
    virtual void ShowItem(UINT uItemID, BOOL bshow, BOOL bResort = FALSE);
    virtual void EnableItem(UINT uItemID, BOOL bEnable);
protected:
    //////////////////////////////////////////////////////////////////////////
    // interface INotify
    virtual BOOL Action(NotifyInfo* pInfo);
};

#include "bktoolbar.inl"