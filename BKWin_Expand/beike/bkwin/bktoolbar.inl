#pragma once

inline CBkToolBar::CBkToolBar()
{
}

inline CBkToolBar::~CBkToolBar()
{}

inline void CBkToolBar::ShowItem(UINT uItemID, BOOL bshow, BOOL bResort)
{
    IBkWnd* pBkbtn = NULL;
    pBkbtn = GetItem(uItemID);
    if (NULL != pBkbtn)
    {
        pBkbtn->SetVisible(bshow);
    }
    if (bResort)
    {
        RepositionChilds();
        NotifyInvalidate();
    }
}

inline void CBkToolBar::EnableItem(UINT uItemID, BOOL bEnable)
{
    CBkImageBtnWnd* pBkbtn = NULL;
    pBkbtn = dynamic_cast<CBkImageBtnWnd*>(GetItem(uItemID));
    if (NULL != pBkbtn)
    {
        pBkbtn->SetDisable(!bEnable);
    }
}

inline BOOL CBkToolBar::Action(NotifyInfo* pInfo)
{
    if (pInfo->nAction == WM_LBUTTONDOWN)
    {
        //int nRow = GetIndexByItem(dynamic_cast<CBkWnd*>(pInfo->pFrom));
        //SetSelectRow(nRow);
    }
    else if (pInfo->nAction == WM_LBUTTONUP)
    {
    }

    return TRUE;
}
/*
inline void ReSort()
{
    POSITION pos = m_lstWndChild.GetHeadPosition();
    IBkWnd* pPreBkWnd = NULL;
    BKDLG_POSITION bkPrePosition;
    while (NULL != pos)
    {
        CBkWnd* pBkWnd = m_lstWndChild.GetAt(pos);
        if (pBkWnd && pBkWnd->IsVisibleDeep())
        {
            BKDLG_POSITION bkPosition;
            pBkWnd->GetPos(&bkPosition);
            int width = bkPosition.Right.nPos - bkPosition.Left.nPos;
            if (NULL != pPreBkWnd)
            {
                bkPosition.Left.nPos = bkPrePosition.Right.nPos + 1;
                bkPosition.Right.nPos = bkPosition.Left.nPos + width;
            }
            else
            {
                bkPosition.Left.nPos = 0;
                bkPosition.Right.nPos = width;
            }
            pBkWnd->SetPos(&bkPosition, TRUE);
            pPreBkWnd = pBkWnd;
            memcpy(&bkPrePosition, &bkPosition, sizeof(BKDLG_POSITION));
        }
        m_lstWndChild.GetNext(pos);
    }
}*/