//////////////////////////////////////////////////////////////////////////
//   File Name: bkwndFactory.h
// Description: CBkPanel Definition, Virtual Container of BkWindows
//     Creator: DongM
//     Version: 2011.04.24 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////
#ifndef _BK_WND_FACTORY_H_
#define _BK_WND_FACTORY_H_

#include "bkDrag.h"
#include "bkwndText.h"
#include "bkwndcmnctrl.h"
#include "bkwndedit.h"
#include "bkwnddiv.h"
#include "bkwndrealctrl.h"
#include "bkwndBtn.h"
#include "bkwndProgress.h"
#include "bkwndScroll.h"
#include "bkwndtree.h"
#include "bkwndRadioBox.h"
#include "bkwndList.h"
#include "bkwndIconView.h"
#include "bkwndReportView.h"
#include "bktabctrl.h"
#include "bktoolbar.h"
#include "bkwndEditList.h"
#include "bkwndhotkey.h"
#include "bkwndCombobox.h"
#include "bkwndIcon.h"
#include "bkwndScroll.h"
#include "bkHotKeyCtrl.h"
#include "bkCombobox.h"
#include "bkWndSplitter.h"
#include "bktree.h"

static CBkWnd* _CreateBkWindowByName(LPCSTR lpszName)
{
    CBkWnd *pNewWindow = NULL;

	pNewWindow = CBkTree::CheckAndNew(lpszName);
	if (pNewWindow)
		return pNewWindow; 

    pNewWindow = CBkTreeCtrl::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow; 

    pNewWindow = CBkRealCtrl::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow; 

    pNewWindow = CBkList::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow; 

    pNewWindow = CBkReportView::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;     

    pNewWindow = CBkDiv::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;   

    pNewWindow = CBkDivV::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;   

    pNewWindow = CBkDivH::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

    pNewWindow = CBkFile::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

    pNewWindow = CBkList::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

    pNewWindow = CBkText::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

    pNewWindow = CBkButton::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

    pNewWindow = CBkSplitterButton::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

    pNewWindow = CBkImageWnd::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

    pNewWindow = CBkProgress::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

    pNewWindow = CBkImageBtnWnd::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

    pNewWindow = CBkLine::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

    pNewWindow = CBkCheckBox::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

    pNewWindow = CBkIcon::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

    pNewWindow = CBkRadioBox::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

    pNewWindow = CBkLink::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

    pNewWindow = CBkMemoryImage::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

    pNewWindow = CBkImageListHWnd::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

    pNewWindow = CBkEdit::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

    pNewWindow = CBkTabItem::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

    pNewWindow = CBkTabControl::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

    pNewWindow = CBkToolBar::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

    pNewWindow = CBkEditList::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

    pNewWindow = CBkIconView::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

    pNewWindow = CBkHotkey::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

    pNewWindow = CBkWndCombobox::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

    pNewWindow = CBkDropDownBtn::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

    pNewWindow = CBkScroll::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

	pNewWindow = CBkHotKeyCtrl::CheckAndNew(lpszName);
     if (pNewWindow)
		 return pNewWindow;

     pNewWindow = CBkCombobox::CheckAndNew(lpszName);
     if (pNewWindow)
         return pNewWindow; 
     
     pNewWindow = CBkSplitter::CheckAndNew(lpszName);
     if (pNewWindow)
         return pNewWindow;

    return NULL;
}

#endif
