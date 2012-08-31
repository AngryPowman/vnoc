
inline BOOL CBkTreeNode::Load(TiXmlElement* pTiXmlElem)
{
    if (!__super::Load(pTiXmlElem))
        return FALSE;
    m_pExpandIcon = new CBkImageWnd;
    m_pExpandIcon->SetVisible(FALSE);
    m_pExpandIcon->RegisterNotify(this);
    m_pExpandIcon->SetParent(m_hBkWnd);
    m_pExpandIcon->SetContainer(m_hWndContainer);
	m_pExpandIcon->SetDropTarget(m_pDropTarget);
    m_pExpandIcon->SetRealContainer(m_hWndRealContainer);
    m_pExpandIcon->SetParentsVisible(IsVisible());
    m_pExpandIcon->SetTabControler(m_tabControler);
    m_pExpandIcon->SetTooltipCtrl(m_wndToolTip);
    m_lstWndChild.AddTail(m_pExpandIcon);

    m_pTreeIcon = new CBkImageWnd;
    m_pTreeIcon->RegisterNotify(this);
    m_pTreeIcon->SetParent(m_hBkWnd);
    m_pTreeIcon->SetContainer(m_hWndContainer);
	m_pTreeIcon->SetDropTarget(m_pDropTarget);
    m_pTreeIcon->SetRealContainer(m_hWndRealContainer);
    m_pTreeIcon->SetParentsVisible(IsVisible());
    m_pTreeIcon->SetTabControler(m_tabControler);
    m_pTreeIcon->SetTooltipCtrl(m_wndToolTip);
    m_lstWndChild.AddTail(m_pTreeIcon);

    m_pNodeText = new CBkText;
    m_pNodeText->RegisterNotify(this);
    m_pNodeText->SetParent(m_hBkWnd);
    m_pNodeText->SetContainer(m_hWndContainer);
	m_pNodeText->SetDropTarget(m_pDropTarget);
    m_pNodeText->SetRealContainer(m_hWndRealContainer);
    m_pNodeText->SetParentsVisible(IsVisible());
    m_pNodeText->SetTabControler(m_tabControler);
    m_pNodeText->SetTooltipCtrl(m_wndToolTip);
    m_lstWndChild.AddTail(m_pNodeText);

    return TRUE;
}

inline BOOL CBkTreeNode::Action(NotifyInfo* pInfo)
{
     return TRUE;
}

inline void CBkTreeNode::SetLevel(int iLevel)
{
    m_iLevel = iLevel;
    ArrangeChildren();
}

inline void CBkTreeNode::SetImage(int iImage, int iSelectedImage)
{
    BOOL bOld = m_bHasImage;
    m_bHasImage = -1 == iImage ? FALSE:TRUE;

    m_pTreeIcon->SetSubImage(iImage);
    if (bOld != m_bHasImage)
    {
        ArrangeChildren();
    }
}

inline void CBkTreeNode::SetText(LPCWSTR szText)
{
    m_pNodeText->SetText(szText);
}

inline LPCWSTR CBkTreeNode::GetText()
{
    return m_pNodeText->GetText();
}

inline void CBkTreeNode::ArrangeChildren()
{
    BKDLG_POSITION dlgPos;
    dlgPos.nType           = 2;
    dlgPos.Left.bCenter    = FALSE;
    dlgPos.Left.bMinus     = FALSE;
    dlgPos.Left.nPos       = 0;
    dlgPos.Top.bCenter     = FALSE;
    dlgPos.Top.bMinus      = FALSE;    

    dlgPos.Top.nPos = (m_rcWindow.Height()-m_pExpandIconSkin->GetSkinSize().cy)/2;
    dlgPos.Left.nPos = m_iLevel*m_iIndent;
    m_pExpandIcon->SetPos(&dlgPos, FALSE);
    m_pExpandIcon->SetSkin(m_pExpandIconSkin);
    m_pExpandIcon->SetSubImage(TreeNodeState_Expand);

    if (!m_bHasImage)
    {
        m_pTreeIcon->SetVisible(FALSE);
        //为了使无图标子节点的文字与有图标的父节点的文字对齐
        dlgPos.Left.nPos = m_iLevel*m_iIndent + m_pExpandIconSkin->GetSkinSize().cx + m_iSpaceFromIconToText;
    }
    else
    {
        m_pTreeIcon->SetVisible(TRUE);
        dlgPos.Top.nPos = (m_rcWindow.Height()-m_pTreeIconSkin->GetSkinSize().cy)/2;
        dlgPos.Left.nPos = dlgPos.Left.nPos + m_pExpandIconSkin->GetSkinSize().cx;
        m_pTreeIcon->SetPos(&dlgPos,FALSE);
        m_pTreeIcon->SetSkin(m_pTreeIconSkin);
        dlgPos.Left.nPos = dlgPos.Left.nPos + m_pTreeIconSkin->GetSkinSize().cx + m_iSpaceFromIconToText;
    }
    

    
    CRect rcText;
    m_pNodeText->GetRect(&rcText);
    dlgPos.Top.nPos = (m_rcWindow.Height()-14)/2;
    dlgPos.Right.bMinus = TRUE;
    dlgPos.Right.nPos = 0;
    m_pNodeText->SetPos(&dlgPos,FALSE);
}

inline DWORD CBkTreeNode::GetData()
{
    return m_dwData;
}

inline void CBkTreeNode::SetData(DWORD dwData)
{
    m_dwData = dwData;
}

inline void CBkTreeNode::SetTextColor(COLORREF rcText)
{
    m_pNodeText->SetTextColor(rcText);
}

inline void CBkTreeNode::SetHasChild(BOOL bHasChild)
{
    m_bHasChild = bHasChild;
    if (m_bHasChild)
    {
        m_pExpandIcon->SetVisible(TRUE);
    }
}

inline void CBkTreeNode::SetExpand(BOOL bExpand)
{
    
    if (m_bHasChild)
    {
        m_bExpand = bExpand;
        if (m_bExpand)
        {
            m_pExpandIcon->SetSubImage(TreeNodeState_Expand);
        }
        else
        {
            m_pExpandIcon->SetSubImage(TreeNodeState_Collapse);
        }
    }
    
}

inline void CBkTreeNode::SetSelected(BOOL bSelected)
{
    if (bSelected)
    {
        m_pNodeText->SetBold(TRUE);
        m_pExpandIcon->SetSkin(m_pExpandSelIconSkin);
        ModifyState(BkWndState_PushDown, BkWndState_Normal|BkWndState_Hover);
    }
    else
    {
        m_pNodeText->SetBold(FALSE);
        m_pExpandIcon->SetSkin(m_pExpandIconSkin);
        ModifyState(BkWndState_Normal, BkWndState_PushDown|BkWndState_Hover);
    }
    NotifyInvalidate();
}

inline void CBkTreeNode::SetExpandIconSkin(CBkSkinBase* pSkin)
{
    m_pExpandIconSkin = pSkin;
    m_pExpandIcon->SetSkin(pSkin);
}

inline void CBkTreeNode::SetExpandSelIconSkin(CBkSkinBase* pSkin)
{
    m_pExpandSelIconSkin = pSkin;
}

inline void CBkTreeNode::SetTreeIconSkin(CBkSkinBase* pSkin)
{
    m_pTreeIconSkin = pSkin;
    m_pExpandIcon->SetSkin(pSkin);
}

inline BOOL CBkTreeNode::OnLButtonDown( UINT nFlags, CPoint point )
{
    if (m_bHasChild && m_pExpandIcon->PtInRect(point))
    {
        if (m_pNotify != NULL)
        {
            NotifyInfo info = {BKM_TREE_NODE_EXPAND, point, this};
            m_pNotify->Action(&info);
        }
    }
    else
    {
        if (m_pNotify != NULL)
        {
            NotifyInfo info = {BKM_TREE_NODE_SELECT, point, this};
            m_pNotify->Action(&info);
        }
    }
    //__super::OnLButtonDown(nFlags, point);
    return FALSE;
}

inline void CBkTreeNode::SetIndent(int iIndent)
{
    m_iIndent = iIndent;
    ArrangeChildren();
}

inline BOOL CBkTreeNode::OnMouseHover()
{
    if (BkWndState_PushDown != GetState())
    {
        CBkWnd::OnMouseHover();
        __super::OnMouseHover();
        if (m_pNotify)
        {
            NotifyInfo info = {WM_MOUSEHOVER, CPoint(0, 0), this, 0};
            m_pNotify->Action(&info);
        }
    }
    
    return TRUE;
}

inline BOOL CBkTreeNode::OnMouseLeave()
{
    CBkWnd::OnMouseLeave();
    __super::OnMouseLeave();
    if (m_pNotify)
    {
        NotifyInfo info = {WM_MOUSELEAVE, CPoint(0, 0), this, 0};
        m_pNotify->Action(&info);
    }
    return TRUE;
}

inline BOOL CBkTreeCtrl::Load(TiXmlElement* pTiXmlElem)
{
    if (!CBkWnd::Load(pTiXmlElem))
        return FALSE;

    //OnDestroy();

    m_pNodeXmlTemplete = pTiXmlElem->FirstChildElement()->Clone();
    m_sizeIcon =  m_pSkinExpandIcon->GetSkinSize();

    return TRUE;
}

inline BOOL CBkTreeCtrl::Action(NotifyInfo* pInfo)
{
    BOOL bUpdate = FALSE;
    switch (pInfo->nAction)
    {
    case BKM_TREE_NODE_EXPAND:
        {
            SelectNode(dynamic_cast<IBkTreeNode*>(pInfo->pFrom));
            Expand(dynamic_cast<IBkTreeNode*>(pInfo->pFrom));
            LayoutNodes();
        }
        break;
    case BKM_TREE_NODE_SELECT:
        SelectNode(dynamic_cast<IBkTreeNode*>(pInfo->pFrom));
        break;
    case WM_MOUSEHOVER:
        m_pHoveredNode = dynamic_cast<CBkTreeNode*>(pInfo->pFrom);
        break;
    case WM_MOUSELEAVE:
        m_pHoveredNode = NULL;
    	break;
    default:
        break;
    }

    return TRUE;
}

inline CBkTreeNode* CBkTreeCtrl::CreateEmptyItem()
{
    CBkTreeNode *pNewNode = CBkTreeNode::CheckAndNew(m_pNodeXmlTemplete->Value());
    pNewNode->Load(m_pNodeXmlTemplete->ToElement());
    pNewNode->SetTextColor(m_style.m_crText);
    pNewNode->SetExpandIconSkin(m_pSkinExpandIcon);
    pNewNode->SetExpandSelIconSkin(m_pSkinExpandSelectIcon);
    pNewNode->SetTreeIconSkin(m_pSkinTreeIcon);
    pNewNode->SetParent(m_hBkWnd);
    pNewNode->SetContainer(m_hWndContainer);
	pNewNode->SetDropTarget(m_pDropTarget);
    pNewNode->SetRealContainer(m_hWndRealContainer);
    pNewNode->RegisterNotify(this);
    return pNewNode;
}

inline void CBkTreeCtrl::LayoutNodes()
{
    BKDLG_POSITION dlgPos;
    dlgPos.nType            = 4;
    dlgPos.Left.bCenter     = FALSE;
    dlgPos.Right.bCenter    = FALSE;
    dlgPos.Top.bCenter      = FALSE;
    dlgPos.Bottom.bCenter   = FALSE;

    POSITION pos = m_lstWndChild.GetHeadPosition();

    int nPrevBottom = 0;
    while (pos != NULL)
    {
        BOOL bHasChildren = HasChild(pos);
        CBkTreeNode* pNode = (CBkTreeNode*) m_lstWndChild.GetNext(pos);
        pNode->SetHasChild(bHasChildren);

        if (pNode->IsVisibleDeep())
        {
            dlgPos.Left.nPos    = 0;
            dlgPos.Left.bMinus  = FALSE;
            dlgPos.Right.nPos   = 0;
            dlgPos.Right.bMinus = TRUE;


            dlgPos.Top.bMinus = FALSE;
            dlgPos.Top.nPos = nPrevBottom;

            dlgPos.Bottom.bMinus = FALSE;
            CRect rcNode;
            pNode->GetRect(&rcNode);
            dlgPos.Bottom.nPos = dlgPos.Top.nPos + rcNode.Height();
            nPrevBottom += rcNode.Height() + 1;
            pNode->SetPos(&dlgPos, FALSE);
        }
    }

    NotifyInvalidate();

}

inline void CBkTreeCtrl::GetExpandIconRect(CBkWnd* pNode, CRect &rcExpandIcon)
{
    CRect rcNode;
    pNode->GetRect(&rcNode);
    rcExpandIcon.left   = rcNode.left - m_sizeIcon.cx;
    rcExpandIcon.top    = rcNode.top + (rcNode.Height()-m_sizeIcon.cy)/2;
    rcExpandIcon.right  = rcExpandIcon.left + m_sizeIcon.cx;
    rcExpandIcon.bottom = rcExpandIcon.top + m_sizeIcon.cy;
}

inline BOOL CBkTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
    __super::OnLButtonDown(nFlags, point);
    POSITION pos = m_lstWndChild.GetHeadPosition();

    while (pos != NULL)
    {
        CBkWnd *pBkWndChild = m_lstWndChild.GetNext(pos);

        if (pBkWndChild->IsVisibleDeep())
        {
            CRect rcNode;
            pBkWndChild->GetRect(&rcNode);
            rcNode.left     = m_rcWindow.left;
            rcNode.right    = m_rcWindow.right;
            if (rcNode.PtInRect(point))
            {
                SelectNode(dynamic_cast<IBkTreeNode*>(pBkWndChild));
                LayoutNodes();
                
                break;
            }
        }
    }
    return TRUE;
}

inline BOOL CBkTreeCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    POSITION pos = m_lstWndChild.GetHeadPosition();

    while (pos != NULL)
    {
        CBkWnd *pBkWndChild = m_lstWndChild.GetNext(pos);

        if (pBkWndChild->IsVisibleDeep())
        {
            CRect rcNode;
            pBkWndChild->GetRect(&rcNode);
            rcNode.left     = m_rcWindow.left;
            rcNode.right    = m_rcWindow.right;
            if (rcNode.PtInRect(point))
            {
                SelectNode(dynamic_cast<IBkTreeNode*>(pBkWndChild));
                Expand(dynamic_cast<IBkTreeNode*>(pBkWndChild));
                LayoutNodes();

                break;
            }
        }
    }
    return FALSE;
}

inline BOOL CBkTreeCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    POSITION pos = m_lstWndChild.Find(dynamic_cast<CBkWnd*>(GetSelectedNode()));
    if (VK_UP == nChar)
    {
        CBkWnd * pBkWnd = m_lstWndChild.GetPrev(pos); //要先走一个
        if (NULL != pos)
        {
            pBkWnd = m_lstWndChild.GetPrev(pos);
            SelectNode(dynamic_cast<IBkTreeNode*>(pBkWnd));
        }
    }
    else if (VK_DOWN == nChar)
    {
        CBkWnd * pBkWnd = m_lstWndChild.GetNext(pos); //要先走一个
        if (NULL != pos)
        {
            pBkWnd = m_lstWndChild.GetNext(pos);
            SelectNode(dynamic_cast<IBkTreeNode*>(pBkWnd));
        }
    }
    
    return TRUE;
}

inline BOOL CBkTreeCtrl::HasChild(POSITION pos)
{
    CBkTreeNode * pNode = dynamic_cast<CBkTreeNode*>(m_lstWndChild.GetNext(pos)); //要先走一个GetNext
    if (NULL != pos)
    {
        CBkTreeNode *pBkWndChild = dynamic_cast<CBkTreeNode*>(m_lstWndChild.GetNext(pos));
        if (NULL != pBkWndChild)
        {
            if (pBkWndChild->GetLevel() > pNode->GetLevel())
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }
}


inline BOOL CBkTreeCtrl::HasChild(CBkTreeNode* pNode)
{
    POSITION pos = m_lstWndChild.Find(pNode);
    return HasChild(pos);
}

inline IBkTreeNode* CBkTreeCtrl::GetRootNode()
{
    return BK_TVI_ROOT;
}

inline IBkTreeNode* CBkTreeCtrl::InsertNode(LPCWSTR lpszItem, int iImage, int iSelectedImage, IBkTreeNode* pParent, IBkTreeNode* pInsertAfter)
{
    CBkTreeNode *pNewNode = NULL;

    if (BK_TVI_FIRST == pInsertAfter)
    {
        if (BK_TVI_ROOT == pParent)
        {
            pNewNode = CreateEmptyItem();
            pNewNode->SetLevel(0);
            pNewNode->SetImage(iImage, iSelectedImage);
            pNewNode->SetText(lpszItem);

            m_lstWndChild.AddHead(pNewNode);
        }
        else
        {
            POSITION pos = m_lstWndChild.GetHeadPosition();

            BOOL bFind = FALSE;
            while (pos != NULL)
            {
                CBkTreeNode* pNode = (CBkTreeNode*) m_lstWndChild.GetNext(pos);
                if (pParent == pNode)
                {
                    if (NULL == pos)
                    {
                        pNewNode = CreateEmptyItem();
                        pNewNode->SetLevel(pNode->GetLevel()+1);
                        pNewNode->SetImage(iImage, iSelectedImage);
                        pNewNode->SetText(lpszItem);
                        m_lstWndChild.AddTail(pNewNode);
                    }
                    else
                    {
                        pNewNode = CreateEmptyItem();
                        pNewNode->SetLevel(pNode->GetLevel()+1);
                        pNewNode->SetImage(iImage, iSelectedImage);
                        pNewNode->SetText(lpszItem);

                        m_lstWndChild.InsertAfter(pos, pNewNode);
                    }

                    break;
                }
            }
        }
    }
    else if (BK_TVI_LAST == pInsertAfter)
    {
        if (BK_TVI_ROOT == pParent)
        {
            pNewNode = CreateEmptyItem();
            pNewNode->SetLevel(0);
            pNewNode->SetImage(iImage, iSelectedImage);
            pNewNode->SetText(lpszItem);

            m_lstWndChild.AddTail(pNewNode);
        }
        else
        {
            POSITION pos = m_lstWndChild.GetHeadPosition();

            BOOL bFind = FALSE;
            while (pos != NULL)
            {
                CBkTreeNode* pNode = (CBkTreeNode*) m_lstWndChild.GetNext(pos);
                if (pParent == pNode)
                {
                    if (NULL == pos)
                    {
                        pNewNode = CreateEmptyItem();
                        pNewNode->SetLevel(pNode->GetLevel()+1);
                        pNewNode->SetImage(iImage, iSelectedImage);
                        pNewNode->SetText(lpszItem);
                        m_lstWndChild.AddTail(pNewNode);
                    }
                    else
                    {
                        while(pos != NULL)
                        {
                            CBkTreeNode* pChildNode = (CBkTreeNode*) m_lstWndChild.GetNext(pos);
                            if (NULL == pos)
                            {
                                pNewNode = CreateEmptyItem();
                                pNewNode->SetLevel(pNode->GetLevel()+1);
                                pNewNode->SetImage(iImage, iSelectedImage);
                                pNewNode->SetText(lpszItem);
                                m_lstWndChild.AddTail(pNewNode);
                            }
                            else
                            {
                                if (NULL!=pChildNode && pChildNode->GetLevel()<=pNode->GetLevel())
                                {

                                    CBkTreeNode* pChildNode = (CBkTreeNode*) m_lstWndChild.GetPrev(pos);

                                    pNewNode = CreateEmptyItem();
                                    pNewNode->SetLevel(pNode->GetLevel()+1);
                                    pNewNode->SetImage(iImage, iSelectedImage);
                                    pNewNode->SetText(lpszItem);

                                    m_lstWndChild.InsertAfter(pos, pNewNode);
                                    break;
                                }
                            }
                        }
                    }

                    break;
                }
            }
        }
    }
    else
    {
        POSITION pos = m_lstWndChild.GetHeadPosition();

        BOOL bFind = FALSE;
        while (pos != NULL)
        {
            CBkTreeNode* pNode = (CBkTreeNode*) m_lstWndChild.GetNext(pos);
            if (pInsertAfter == pNode)
            {
                if (NULL == pos)
                {
                    pNewNode = CreateEmptyItem();
                    pNewNode->SetLevel(pNode->GetLevel());
                    pNewNode->SetImage(iImage, iSelectedImage);
                    pNewNode->SetText(lpszItem);
                    m_lstWndChild.AddTail(pNewNode);
                }
                else
                {
                    pNewNode = CreateEmptyItem();
                    pNewNode->SetLevel(pNode->GetLevel());
                    pNewNode->SetImage(iImage, iSelectedImage);
                    pNewNode->SetText(lpszItem);

                    m_lstWndChild.InsertAfter(pos, pNode);
                }

                break;
            }
        }
    }

    if (NULL != pNewNode)
    {
        LayoutNodes();
    }
    
    return pNewNode;
}

inline void CBkTreeCtrl::Expand(IBkTreeNode* _pNode)
{
    if (NULL == _pNode)
    {
        return;
    }
    CBkTreeNode* pNode = dynamic_cast<CBkTreeNode*>(_pNode);
    if (pNode->IsExpand())
    {
        pNode->SetExpand(FALSE);
    }
    else
    {
        pNode->SetExpand(TRUE);
    }
    POSITION pos = m_lstWndChild.Find(pNode);
    m_lstWndChild.GetNext(pos); //要先走一个

    while (pos != NULL)
    {
        CBkTreeNode *pBkWndChild = (CBkTreeNode*) m_lstWndChild.GetNext(pos);
        if (pBkWndChild->GetLevel()<=pNode->GetLevel())
        {
            break;
        }
        else
        {
            if (pNode->IsExpand())
            {
                pBkWndChild->SetVisible(TRUE);

                //本身是折叠状态的要跳过去
                if (!pBkWndChild->IsExpand())
                {
                    int iParentLevel = pBkWndChild->GetLevel();
                    while (pos != NULL)
                    {
                        pBkWndChild = (CBkTreeNode*) m_lstWndChild.GetNext(pos);
                        if (pBkWndChild->GetLevel()>iParentLevel)
                        {
                            pBkWndChild->SetVisible(FALSE);
                        }
                        else
                        {
                            if (NULL != pos)
                            {
                                m_lstWndChild.GetPrev(pos);//修正一下pos，因为会看过一位。
                            }
                            else
                            {
                                pos = m_lstWndChild.GetTailPosition();
                            }
                            break;
                        }
                    }
                }
            }
            else
            {
                pBkWndChild->SetVisible(FALSE);
            }
        }
    }
    NotifyInvalidate();
}

inline void CBkTreeCtrl::SelectNode(IBkTreeNode* pNode)
{
    if (pNode != m_pSelectedNode)
    {
        if (m_pSelectedNode)
        {
            m_pSelectedNode->SetTextColor(m_style.m_crText);
            m_pSelectedNode->SetSelected(FALSE);
        }
        NotifyInfo info;
        if (m_pNotify != NULL)
        {
            info.nAction    = BKM_TREE_SELCHANGING;
            info.pFrom      = this;
            info.pt         = CPoint(0,0);
            info.dwData     = (NULL == m_pSelectedNode) ? -1 : m_pSelectedNode->GetData();
            m_pNotify->Action(&info);
        }
        m_pSelectedNode = dynamic_cast<CBkTreeNode *>(pNode);
        if (m_pNotify != NULL)
        {
            info.nAction    = BKM_TREE_SELCHANGED;
            info.pFrom      = this;
            info.pt         = CPoint(0,0);
            info.dwData     = m_pSelectedNode->GetData();
            m_pNotify->Action(&info);
        }
        m_pSelectedNode->SetTextColor(m_crTextSelected);
        m_pSelectedNode->SetSelected(TRUE);
        NotifyInvalidate();
    }
}

inline IBkTreeNode* CBkTreeCtrl::HitTest(const CPoint& point)
{
    POSITION pos = m_lstWndChild.GetHeadPosition();

    while (pos != NULL)
    {
        CBkWnd *pBkWndChild = m_lstWndChild.GetNext(pos);

        CRect rcNode;
        pBkWndChild->GetRect(&rcNode);
        rcNode.left     = m_rcWindow.left;
        rcNode.right    = m_rcWindow.right;
        if (rcNode.PtInRect(point))
        {
            return (dynamic_cast<IBkTreeNode*>(pBkWndChild));
            break;
        }
    }
    return NULL;
}

inline void CBkTreeCtrl::DeleteAllNodes()
{
    POSITION pos = m_lstWndChild.GetHeadPosition();

    while (pos != NULL)
    {
        CBkWnd *pBkWndChild = m_lstWndChild.GetNext(pos);
        delete pBkWndChild;
    }

    m_lstWndChild.RemoveAll();
    m_pSelectedNode = NULL;
    m_pHoveredNode = NULL;
}

inline void CBkTreeCtrl::DeleteNode(IBkTreeNode* pNode)
{
    if (NULL == pNode)
    {
        return;
    }
    POSITION pos = m_lstWndChild.GetHeadPosition();

    while (pos != NULL)
    {
        CBkWnd *pBkWndChild = m_lstWndChild.GetNext(pos);
        if (dynamic_cast<IBkTreeNode*>(pBkWndChild) == pNode)
        {
            if (m_pSelectedNode == dynamic_cast<CBkTreeNode*>(pBkWndChild))
            {
                m_pSelectedNode = NULL;
            }
            if (m_pHoveredNode == dynamic_cast<CBkTreeNode*>(pBkWndChild))
            {
                m_pHoveredNode = NULL;
            }
            delete pBkWndChild;

            m_lstWndChild.RemoveAt(m_lstWndChild.Find(pBkWndChild));
            break;
        }
    }
}

inline IBkTreeNode* CBkTreeCtrl::GetFirstNode()
{
    return dynamic_cast<IBkTreeNode*>(m_lstWndChild.GetHead());
}

inline IBkTreeNode* CBkTreeCtrl::GetNextNode(IBkTreeNode* pNode)
{
    if (NULL == pNode)
    {
        return NULL;
    }
    POSITION pos = m_lstWndChild.Find(dynamic_cast<CBkWnd*>(pNode));
    if (NULL != pos)
    {
        m_lstWndChild.GetNext(pos); //要先走一个
        if (NULL != pos)
        {
            return dynamic_cast<IBkTreeNode*>(m_lstWndChild.GetNext(pos));
        }
    }

    return NULL;
}

inline IBkTreeNode* CBkTreeCtrl::GetNextSiblingNode(IBkTreeNode* _pNode)
{
    if (NULL == _pNode)
    {
        return NULL;
    }
    CBkTreeNode* pNode = dynamic_cast<CBkTreeNode*>(_pNode);
    POSITION pos = m_lstWndChild.Find(pNode);
    CBkTreeNode *pBkWndChild = NULL;
    if (NULL != pos)
    {
        m_lstWndChild.GetNext(pos); //要先走一个
        
        while (pos != NULL)
        {
            pBkWndChild = (CBkTreeNode*) m_lstWndChild.GetNext(pos);
            if (pBkWndChild->GetLevel()==pNode->GetLevel())
            {
                break;
            }
        }
    }
    if(NULL == pos)
    {
        return NULL;
    }
    return pBkWndChild;
}

inline IBkTreeNode* CBkTreeCtrl::GetChildNode(IBkTreeNode* _pNode)
{
    if (NULL == _pNode)
    {
        return NULL;
    }
    CBkTreeNode* pNode = dynamic_cast<CBkTreeNode*>(_pNode);
    POSITION pos = m_lstWndChild.Find(pNode);
    if (NULL != pos)
    {
        m_lstWndChild.GetNext(pos); //要先走一个
        CBkTreeNode* pBkWndChild = NULL;
        if (NULL != pos)
        {
            pBkWndChild = dynamic_cast<CBkTreeNode*>(m_lstWndChild.GetNext(pos));
            if (pBkWndChild->GetLevel() > pNode->GetLevel())
            {
                return dynamic_cast<IBkTreeNode*>(pBkWndChild);
            }
        }
    }

    return NULL;
}

inline IBkTreeNode* CBkTreeCtrl::GetParentNode(IBkTreeNode* _pNode)
{
    if (NULL == _pNode)
    {
        return NULL;
    }
    CBkTreeNode* pNode = dynamic_cast<CBkTreeNode*>(_pNode);
    CBkTreeNode *pBkWndParent = NULL;
    POSITION pos = m_lstWndChild.Find(pNode);
    if (NULL != pos)
    {
        m_lstWndChild.GetPrev(pos); //要先走一个
        
        while (pos != NULL)
        {
            pBkWndParent = (CBkTreeNode*) m_lstWndChild.GetPrev(pos);
            if (pBkWndParent->GetLevel()<pNode->GetLevel())
            {
                break;
            }
        }
    }
    if (NULL != pBkWndParent)
    {
        return dynamic_cast<IBkTreeNode*>(pBkWndParent);
    }
    return BK_TVI_ROOT;
}

inline IBkTreeNode* CBkTreeCtrl::GetSelectedNode()
{
    return dynamic_cast<IBkTreeNode*>(m_pSelectedNode);
}

inline IBkTreeNode* CBkTreeCtrl::GetHoveredNode()
{
    return m_pHoveredNode;
}

inline HRESULT CBkTreeCtrl::OnDragEnter(IDataObject *pDataObj, POINT pt, DWORD *pdwEffect)
{
    if (NULL == pdwEffect)
    {
        return E_INVALIDARG;
    }
    if (m_rcWindow.PtInRect(pt))
    {
        *pdwEffect = DROPEFFECT_LINK;
    }
    else
    {
        *pdwEffect = DROPEFFECT_NONE;
    }
    return S_OK;
}

inline HRESULT CBkTreeCtrl::OnDragOver(POINT pt, DWORD *pdwEffect)
{
    if (NULL == pdwEffect)
    {
        return E_INVALIDARG;
    }
    *pdwEffect = DROPEFFECT_NONE;
    HRESULT hrs = S_FALSE;
    if (m_rcWindow.PtInRect(pt))
    {
        *pdwEffect = DROPEFFECT_LINK;
        //OnMouseMove(0, pt);
        hrs = S_OK;
    }
    else if (m_pHoveredNode)
    {
        if (m_pHoveredNode != m_pSelectedNode)
        {
            m_pHoveredNode->SetTextColor(m_style.m_crText);
        }
        m_pHoveredNode = NULL;
        NotifyInvalidate();
    }
    return hrs;
}

inline HRESULT CBkTreeCtrl::OnDrop(IDataObject *pDataObj, POINT pt, DWORD *pdwEffect)
{
    if (NULL == pdwEffect)
    {
        return E_INVALIDARG;
    }
    *pdwEffect = DROPEFFECT_NONE;
    HRESULT hrs = S_FALSE;
    if (m_rcWindow.PtInRect(pt))
    {
        OnMouseMove(0, pt);
    }
    else if (m_pHoveredNode)
    {
        if (m_pHoveredNode != m_pSelectedNode)
        {
            m_pHoveredNode->SetTextColor(m_style.m_crText);
        }
        m_pHoveredNode = NULL;
        NotifyInvalidate();
    }

    if (m_pHoveredNode)
    {
        hrs = S_OK;
        NotifyInfo info;
        if (m_pNotify != NULL)
        {
            info.nAction    = BKM_TREE_ACCEPTDROP;
            info.pFrom      = this;
            info.pt         = pt;
            info.dwData     = m_pHoveredNode->GetData();
            if (m_pNotify->Action(&info))
            {
                *pdwEffect = DROPEFFECT_LINK;
            }
        }
    }
    return hrs;
}

inline HRESULT CBkTreeCtrl::OnDragLeave( void)
{
	return S_OK;
}
