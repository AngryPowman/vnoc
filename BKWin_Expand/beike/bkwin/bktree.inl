#pragma once

//////////////////////////////////////////////////////////////////////////////////////////////////
inline void	CTreeNode::SetParentNode(CTreeNode* parent)
{
	m_pParent = parent;
}

inline CTreeNode* CTreeNode::GetParentNode()
{
	return m_pParent;
}

inline void CTreeNode::SetTree(CBkTree* pTree)
{
	m_pTree = pTree;
}

inline void CTreeNode::SetBody(CTreeBody* pBody)
{
	m_pBody = pBody;
}

inline void CTreeNode::SetIndent(int indent)
{
	m_iIndent = indent;
}

inline int CTreeNode::GetIndent()
{	
	return m_iIndent;
}

inline IBkWnd* CTreeNode::GetItemIBkWnd(UINT uItemID)
{
	CBkWnd* pDiv = dynamic_cast<CBkWnd*>(GetItemByIndex(0));
	if (pDiv)
	{
		return pDiv->GetItem(uItemID);
	}
	return NULL;
}

inline void CTreeNode::SetText(const CStringW& str, int index)
{
	CBkWnd* pDiv = dynamic_cast<CBkWnd*>(GetItemByIndex(0));

	CBkText* pText = dynamic_cast<CBkText*>(pDiv->GetItemByIndex(index));
	if (pText)
	{
		pText->SetText(str.GetString());
		pText->UpdateTooltipText(str);
	}
}

inline void	CTreeNode::RemoveTip(int index)
{
	CBkWnd* pDiv = dynamic_cast<CBkWnd*>(GetItemByIndex(0));

	CBkText* pText = dynamic_cast<CBkText*>(pDiv->GetItemByIndex(index));
	if (pText)
	{
		//pText->RemoveTooltip();
		pText->UpdateTooltipText(L"");
	}
}

inline void	 CTreeNode::SetTextColor(COLORREF crText)
{
	if (m_pText)
	{
		m_pText->SetTextColor(crText);
	}
}

inline void CTreeNode::SetSelected(bool set)
{
	m_bIsSelected = set;
}

inline bool	CTreeNode::IsExpand()
{
	return m_bIsExpand;
}

inline bool CTreeNode::IsHitExpandIcon(const CPoint& pt)
{
	return m_pExpandImage->PtInRect(pt);
}

inline int CTreeNode::GetChildCount()
{
	return m_Children.size();
}

inline CTreeNode* CTreeNode::GetChild(int index)
{
	assert(index < m_Children.size() && index >= 0);
	return m_Children[index];
}

inline CTreeNode* CTreeNode::CreateChild(bool update)
{
	CTreeNode* childNode = m_pBody->NewNode();
	childNode->SetParentNode(this);
	childNode->SetIndent(m_iIndent + 1);
	
	if (m_bIsExpand)
	{
		if (m_Children.empty())
		{
			m_pBody->InsertNode(childNode, m_pParent, update);
		}
		else
		{
			m_pBody->InsertNode(childNode, m_Children[m_Children.size() - 1], update);
		}

		childNode->SetInBody(true);
	}

	m_Children.push_back(childNode);
	m_pExpandImage->SetVisible(TRUE);

	m_pTree->UpdateScroll();

	return childNode;
}

inline void CTreeNode::Expand()
{
	if (m_bIsExpand)
	{
		return;
	}
	
	m_pExpandImage->SetSubImage(0);
	InsertChildrenToBody();
	m_bIsExpand = true;

	m_pTree->UpdateScroll();
}

inline void CTreeNode::Collapse()
{
	if (!m_bIsExpand)
	{
		return;
	}

	CTreeNode* pSelectedNode = m_pBody->GetSelectedNode();
	if (pSelectedNode)
	{
		CTreeNode* parent = pSelectedNode->GetParentNode();
		while (parent)
		{
			if (parent == this)
			{
				m_pBody->SelectNode(this);
				break;
			}
			parent = parent->GetParentNode();
		}
	}

	m_pExpandImage->SetSubImage(1);
	RemoveChildrenFromBody();
	m_bIsExpand = false;

	m_pTree->UpdateScroll(true);
}

inline void CTreeNode::InsertChildrenToBody()
{
	CTreeNode* insertPos = this;
	for (ChildList::iterator iter = m_Children.begin();
		iter != m_Children.end();
		++iter)
	{
		CTreeNode* node = *iter;

		m_pBody->InsertNode(node, insertPos);
		node->SetInBody(true);

		if (node->IsExpand() && node->GetChildCount() > 0)
		{
			node->InsertChildrenToBody();
			insertPos = node->GetChild(node->GetChildCount() - 1);
		}
		else
		{
			insertPos = node;
		}
	}
}

inline void CTreeNode::RemoveChildrenFromBody()
{
	for (ChildList::iterator iter = m_Children.begin();
		iter != m_Children.end();
		++iter)
	{
		CTreeNode* node = *iter;	
		if (node->IsExpand() && node->GetChildCount() > 0)
		{
			node->RemoveChildrenFromBody();
		}
		m_pBody->DeleteNode(node);
		node->SetInBody(false);
	}

	m_pBody->RepositionChilds();
	m_pBody->Invalidate();
}

inline int CTreeNode::GetNodeHeight()
{
	int height = GetHeight();

	if (m_bIsExpand)
	{
		for (ChildList::iterator node = m_Children.begin();
			node != m_Children.end();
			++node)
		{
			height += (*node)->GetNodeHeight();
		}
	}

	return height;
}

inline BOOL	CTreeNode::Action(NotifyInfo* pInfo)
{
	return TRUE;
}

inline CBkWnd* CTreeNode::OnSetFocus( CBkWnd* which )
{
    m_bFocus = TRUE;
    __super::OnSetFocus( this );
    return this;
}

inline BOOL CTreeNode::OnMouseHover()
{
	CBkWnd::OnMouseHover();
	__super::OnMouseHover();

	SetTextColor(m_pBody->m_crTextHover);

	return TRUE;
}

inline BOOL CTreeNode::OnMouseLeave()
{
	CBkWnd::OnMouseLeave();
	__super::OnMouseLeave();

	if (!m_bIsSelected)
	{
		SetTextColor(m_pBody->m_crTextNormal);
	}

	return TRUE;
}

inline void CTreeNode::OnLButtonHit(CPoint point)
{
	if (m_pExpandImage->PtInRect(point))
	{
		if (m_bIsExpand)
		{
			Collapse();
		}
		else
		{
			Expand();
		}
	}
	else
	{
		m_pBody->SelectNode(this);
	}
}

inline void	CTreeNode::Clear(void (*ReleaseDataFunPtr)(DWORD dwData))
{
	_ClearFocus(m_focusChild);
    _ClearLastHover();
	if (ReleaseDataFunPtr)
	{
		ReleaseDataFunPtr(GetData());
	}
	
	for (ChildList::iterator node = m_Children.begin();
		node != m_Children.end();
		++node)
	{
		(*node)->Clear();
		m_pBody->DestroyNode(*node);
	}

	m_Children.clear();
}

inline void CTreeNode::Traverse(void (*TraverseFunPtr)(CTreeNode* pNode))
{
	TraverseFunPtr(this);

	for (ChildList::iterator node = m_Children.begin();
		 node != m_Children.end();
		 ++node)
	{
		(*node)->Traverse(TraverseFunPtr);
	}
}

inline CTreeNode* CTreeNode::FindNode(bool (*CompFunPtr)(CTreeNode* pNode))
{
	if (CompFunPtr(this)) 
	{
		return this;
	}

	for (ChildList::iterator node = m_Children.begin();
		node != m_Children.end();
		++node)
	{
		CTreeNode* result = (*node)->FindNode(CompFunPtr);
		if (result) 
		{
			return result;
		}
	}

	return NULL;
}

inline void CTreeNode::ResetTip(int textIndex)
{
	CBkWnd* pDiv = dynamic_cast<CBkWnd*>(GetItemByIndex(0));

	CBkText* pText = dynamic_cast<CBkText*>(pDiv->GetItemByIndex(textIndex));
	if (pText)
	{
		pText->RemoveTooltip();
		pText->UpdateTooltipText(pText->GetText());
	}
}

inline void CTreeNode::SetInBody(bool set)
{
	m_bIsInBody = set;
}

inline  bool CTreeNode::IsInBody()
{
	return m_bIsInBody;
}


inline void CTreeNode::OnSelect()
{
	if (m_pNotify)
	{
		NotifyInfo info = {BKM_TREE_NODE_SELECT, {0,0}, 0, 0};
		m_pNotify->Action(&info);
	}
}

inline BOOL CTreeNode::Load(TiXmlElement* pTiXmlElem)
{
	__super::Load(pTiXmlElem);

	CBkWnd* pDiv = dynamic_cast<CBkWnd*>(GetItemByIndex(0));
	m_pExpandImage = dynamic_cast<CBkImageWnd*>(pDiv->GetItemByIndex(1));
	assert(m_pExpandImage);

	m_pText = dynamic_cast<CBkText*>(pDiv->GetItemByIndex(0));

	m_pExpandImage->SetVisible(FALSE);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

inline void CTreeBody::SetTree(CBkTree* pTree)
{
	m_pTree = pTree;
}

inline BOOL CTreeBody::Load(TiXmlElement* pTiXmlElem)
{
	if (!CBkWnd::Load(pTiXmlElem))
		return FALSE;

	if (pTiXmlElem->FirstChildElement() != NULL)
	{
		m_pNodeTemplate = pTiXmlElem->FirstChildElement()->Clone();
	}
	return TRUE;
}

inline CTreeNode* CTreeBody::NewNode()
{
	CTreeNode* pNewNode = CTreeNode::CheckAndNew(m_pNodeTemplate->Value());
	if (pNewNode != NULL)
	{
		pNewNode->RegisterNotify(this);
		pNewNode->SetParent(m_hBkWnd);
		pNewNode->SetContainer(m_hWndContainer);
		pNewNode->SetDropTarget(m_pDropTarget);
		pNewNode->SetRealContainer(m_hWndRealContainer);
		pNewNode->SetParentsVisible(IsVisible());
		pNewNode->SetTabControler(m_tabControler);
		pNewNode->SetTooltipCtrl(m_wndToolTip);
		pNewNode->Load(m_pNodeTemplate->ToElement());
	}

	pNewNode->SetTree(m_pTree);
	pNewNode->SetBody(this);
	pNewNode->SetTextColor(m_crTextNormal);

	
	return pNewNode;
}

inline CTreeNode* CTreeBody::CreateNode()
{
	CTreeNode* pNewNode = NewNode();
	pNewNode->SetIndent(0);
	pNewNode->SetParentNode(NULL);

	m_RootNodes.push_back(pNewNode);

	m_lstWndChild.AddTail(pNewNode);
	//RepositionChild(pNewNode);

	return pNewNode;
}

inline void	 CTreeBody::DeleteNode(CTreeNode* node)
{
	POSITION pos = m_lstWndChild.Find(node);
	m_lstWndChild.RemoveAt(pos);
}

inline void CTreeBody::InsertNode(CTreeNode* pNewNode, CTreeNode* pAfter, bool update)
{
	POSITION insertPos = m_lstWndChild.Find(pAfter);
	m_lstWndChild.InsertAfter(insertPos, pNewNode);

	CBkWnd* pNodeContent = dynamic_cast<CBkWnd*>(pNewNode->GetItemByIndex(0));
	BKDLG_POSITION nodeContentPos;
	pNodeContent->GetPos(&nodeContentPos);
	nodeContentPos.Left.nPos = m_iIndentWidth * pNewNode->GetIndent();
	pNodeContent->SetPos(&nodeContentPos, FALSE);
	//pNewNode->Invalidate();

	if (update)
	{
		RepositionChilds();
		Invalidate();
	}
}

inline void CTreeBody::SelectNode(CTreeNode* pSelectedNode, BOOL bNotify/* = TRUE*/)
{
	m_pSelectedNode = pSelectedNode;

	POSITION pos = m_lstWndChild.GetHeadPosition();
	while (pos)
	{
		CBkWnd* pBkWnd = dynamic_cast<CBkWnd*>(m_lstWndChild.GetNext(pos));
		CTreeNode* pNode = dynamic_cast<CTreeNode*>(pBkWnd);
		if(!pNode)
			continue;

		if (pNode == pSelectedNode)
		{
			pNode->SetSelected(true);
			pNode->SetChecked(TRUE);
			pNode->SetTextColor(m_crTextSelected);
		}
		else
		{
			pNode->SetChecked(FALSE);
			pNode->SetSelected(false);
			pNode->SetTextColor(m_crTextNormal);
		}
	}

	if (bNotify)
	{
		m_pTree->OnNodeSelect(m_pSelectedNode);
	}

	m_pTree->UpdateScroll();
}

inline CTreeNode* CTreeBody::GetSelectedNode()
{
	return m_pSelectedNode;
}

inline CTreeNode* CTreeBody::GetHitNode(const CPoint& pt)
{
	POSITION pos = m_lstWndChild.GetHeadPosition();
	while (pos)
	{
		CBkWnd* pBkWndChild = m_lstWndChild.GetNext(pos);
		if( !pBkWndChild->IsVisible() )
			continue;

		if (pBkWndChild->PtInRect(pt))
		{
			return (CTreeNode*)pBkWndChild;
		}
	}

	return NULL;
}

inline void CTreeBody::DestroyNode(CTreeNode* node)
{
//	m_RootNodes.push_back(node);
	delete node;
}

inline void CTreeBody::Clear(void (*ReleaseDataFunPtr)(DWORD dwData))
{
	_ClearFocus(m_focusChild);
    _ClearLastHover();

	for (NodeList::iterator node = m_RootNodes.begin();
		node != m_RootNodes.end();
		++node)
	{
		(*node)->Clear(ReleaseDataFunPtr);
		DestroyNode(*node);
	}

	m_RootNodes.clear();

	m_lstWndChild.RemoveAll();

    SetShowTop(0);
    NotifyInvalidate();
}

inline void CTreeBody::Update()
{
	RepositionChilds();
	NotifyInvalidate();
}

inline int CTreeBody::GetRealHeight()
{
	int height = 0;
	for (NodeList::iterator node = m_RootNodes.begin();
		node != m_RootNodes.end();
		++node)
	{
		height += (*node)->GetNodeHeight();
	}

	return height;
}

inline void CTreeBody::Traverse(void (*TraverseFunPtr)(CTreeNode* pNode))
{
	for (NodeList::iterator node = m_RootNodes.begin();
		node != m_RootNodes.end();
		++node)
	{
		(*node)->Traverse(TraverseFunPtr);
	}
}

inline CTreeNode* CTreeBody::FindNode(bool (*CompFunPtr)(CTreeNode* pNode))
{
	for (NodeList::iterator node = m_RootNodes.begin();
		node != m_RootNodes.end();
		++node)
	{
		CTreeNode* result = (*node)->FindNode(CompFunPtr);
		if (result) 
		{
			return result;
		}
	}
	return NULL;
}

inline void CTreeBody::ClearDeadNodes()
{
// 	for (DeadNodeList::iterator it = m_DeadNodes.begin();
// 		 it != m_DeadNodes.end();
// 		 ++it)
// 	{
// 		delete (*it);
// 	}

	m_DeadNodes.clear();
}

inline bool CTreeBody::IsEmpty()
{
	return m_RootNodes.empty();
}

inline int CTreeBody::GetRealNodeTop(int nNode)
{
    int index = 0;
    int iScrollPos = 0;
    POSITION pos = m_lstWndChild.GetHeadPosition();
    while (pos != NULL)
    {
        if (index++ == nNode)
        {
            break;
        }

        CBkWnd *pBkWndChild = (CBkWnd*)m_lstWndChild.GetNext(pos);
        if (pBkWndChild)
        {
            iScrollPos += pBkWndChild->GetHeight();
        }
    }
    return iScrollPos;
}

inline int CTreeBody::GetNodeIndex(CTreeNode* pNode)
{
    CBkWnd* pBkWnd = dynamic_cast<CBkWnd*>(pNode);
    if (pBkWnd)
    {
        return GetIndexByItem(pBkWnd);
    }
    return NULL;
}

inline int CTreeBody::GetNodeCount()
{
    return (int)m_lstWndChild.GetCount();
}

inline BOOL CTreeBody::IsNodeVisible(int nNode)
{
    CBkTreeNode* pBkNode = dynamic_cast<CBkTreeNode*>(GetItemByIndex(nNode));
    if (pBkNode)
        return pBkNode->IsVisible();
    return FALSE;
}

inline BOOL	CTreeBody::Action(NotifyInfo* pInfo)
{
	return TRUE;
}

inline BOOL CTreeBody::OnLButtonDown(UINT nFlags, CPoint point)
{
	POSITION pos = m_lstWndChild.GetHeadPosition();
	while (pos)
	{
		CTreeNode* pNode = dynamic_cast<CTreeNode*>(m_lstWndChild.GetNext(pos));
		if(!pNode || !pNode->IsVisible())
			continue;

		if (pNode->PtInRect(point))
		{
			pNode->OnLButtonHit(point);
			break;
		}
	}

	__super::OnLButtonDown(nFlags, point);

	return FALSE;
}

inline BOOL CTreeBody::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	POSITION pos = m_lstWndChild.GetHeadPosition();
	while (pos)
	{
		CTreeNode* pNode = dynamic_cast<CTreeNode*>(m_lstWndChild.GetNext(pos));
		if(!pNode || !pNode->IsVisible())
			continue;

		if (pNode->PtInRect(point))
		{
			SelectNode(pNode);
			if (pNode->IsExpand())
			{
				pNode->Collapse();
			}
			else
			{
				pNode->Expand();
			}
			break;		
		}
	}

	__super::OnLButtonDblClk(nFlags, point);

	return FALSE;
}

inline BOOL CTreeBody::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	int iRealHeight = GetRealHeight();
	CRect rcWin(m_rcWindow);
	if (iRealHeight < rcWin.Height())
	{
		return FALSE;
	}

	int iPer = (m_rcWindow.bottom - m_rcWindow.top) / 10;

	int iShowTop = m_iShowTop;
	if (zDelta < 0)
	{
		if (iShowTop + iPer + rcWin.Height() < iRealHeight)
		{
			iShowTop += iPer;
		}
		else
		{
			iShowTop = iRealHeight - rcWin.Height();
		}
	}
	else
	{
		if (iShowTop > iPer)
		{
			iShowTop -= iPer;
		}
		else
		{
			iShowTop = 0;
		}
	}
	SetShowTop(iShowTop);
	if (m_pNotify != NULL)
	{
		NotifyInfo info = {BKM_LIST_MOUSEWHEEL, CPoint(0, 0), this, m_iShowTop};
		m_pNotify->Action(&info);
	}

	m_pTree->UpdateScroll();

	return FALSE;
}

inline BOOL CTreeBody::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    BOOL bRet = __super::OnKeyDown(nChar, nRepCnt, nFlags);        

    CTreeNode* pSelNode = GetSelectedNode();
    if (pSelNode)
    {
        if (nChar == VK_UP || nChar == VK_DOWN)
        {
            CBkWnd* pBkWnd = dynamic_cast<CBkWnd*>(pSelNode);
            if (pBkWnd)
            {
                int count = (int)m_lstWndChild.GetCount();
                int index = GetIndexByItem(pBkWnd);
                if (index >= 0 && index < count)
                {
                    if (nChar == VK_UP && index - 1 >= 0)
                    {
                        CTreeNode* pPreNode = dynamic_cast<CTreeNode*>(GetItemByIndex(index-1));
                        if (pPreNode)
                        {
                            SelectNode(pPreNode, FALSE);
                        }
                    }
                    else if (nChar == VK_DOWN && index + 1 < count)
                    {
                        CTreeNode* pNextNode = dynamic_cast<CTreeNode*>(GetItemByIndex(index+1));
                        if (pNextNode)
                        {
                            SelectNode(pNextNode, FALSE);
                        }
                    }
                }
            }
        }
        else if(nChar == VK_RETURN)
        {
            SelectNode(pSelNode);
        }
		else if (nChar == VK_LEFT) 
		{
			pSelNode->Collapse();
		}
		else if (nChar == VK_RIGHT) 
		{
			pSelNode->Expand();
		}
    }

    return bRet;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
inline void CBkTree::Update()
{
	UpdateScroll();
	m_pTreeBody->Update();
	m_pTreeBody->Invalidate();
	Invalidate();
}


inline CTreeNode* CBkTree::CreateNode()
{
	CTreeNode* newNode = m_pTreeBody->CreateNode();
	//UpdateScroll();
	return newNode;
}

inline void CBkTree::ClearNodes(void (*ReleaseDataFunPtr)(DWORD dwData))
{
    _ClearFocus(m_focusChild);
    _ClearLastHover();
    if (m_pTreeBody)
    {
        m_pTreeBody->ClearDeadNodes();
        m_pTreeBody->Clear(ReleaseDataFunPtr);
    }
    if (m_pVScroll)
    {
        m_pVScroll->SetValue(0);
    }
    UpdateScroll();
}

inline void CBkTree::UpdateScroll(bool reposition/* = false*/)
{
	CRect rcBody;
	m_pTreeBody->GetRect(&rcBody);
	int iBodyHeight = rcBody.Height();

	int iRealHeight = m_pTreeBody->GetRealHeight();

	BOOL bRepositionAll = (iRealHeight > iBodyHeight) != m_pVScroll->NeedShow();
	if (iRealHeight > iBodyHeight || m_pVScroll->NeedShow())
	{
		m_pVScroll->SetMax(iRealHeight);
		m_pVScroll->SetPage(iBodyHeight);

		if (bRepositionAll)
		{
			BKDLG_POSITION pos;
			m_pTreeBody->GetPos(&pos);
			pos.Right.bMinus = TRUE;
			pos.Right.nPos = (iRealHeight > iBodyHeight) ? m_pVScroll->GetWidth() : pos.Left.nPos;
			m_pTreeBody->SetPos(&pos, FALSE);

			pos.Left.bMinus = TRUE;
			pos.Left.nPos = m_pVScroll->GetWidth();
			pos.Right.bMinus = TRUE;
			pos.Right.nPos = 0;
			if (m_pHScroll->NeedShow())
			{
				pos.Bottom.bMinus = TRUE;
				pos.Bottom.nPos = m_pHScroll->GetHeight();
			}
			m_pVScroll->SetPos(&pos, FALSE);
			m_pVScroll->SetVisible(TRUE);
		}
		else
		{
			m_pVScroll->NotifyInvalidate();
		}

		if (iRealHeight <= iBodyHeight) 
		{
			m_pTreeBody->SetShowTop(0);
			m_pVScroll->SetValue(0);
		}
	}

	if (bRepositionAll || reposition)
	{
		RepositionChilds();
		NotifyInvalidate();
		m_pTreeBody->Invalidate();
	}
}

inline BOOL CBkTree::OnDestroy()
{
	ClearNodes();
	_ClearFocus(this);
    _ClearLastHover();
	return TRUE;
}

inline BOOL CBkTree::LoadChilds(TiXmlElement* pTiXmlChildElem)
{
	BOOL bVisible = IsVisibleDeep(TRUE);
	for (TiXmlElement* pXmlChild = pTiXmlChildElem; NULL != pXmlChild; pXmlChild = pXmlChild->NextSiblingElement())
	{
		CString strNode = pXmlChild->Value();
		CBkWnd* pBkWnd = NULL;
		if (strNode == CTreeBody::GetClassName())
		{
			m_pTreeBody = new CTreeBody();
			m_pTreeBody->RegisterNotify(this);
			m_pTreeBody->SetTree(this);
			pBkWnd = m_pTreeBody;
		}

		if (pBkWnd != NULL)
		{
			pBkWnd->RegisterNotify(this);
			pBkWnd->SetParent(m_hBkWnd);
			pBkWnd->SetContainer(m_hWndContainer);
			pBkWnd->SetDropTarget(m_pDropTarget);
			pBkWnd->SetRealContainer(m_hWndRealContainer);
			pBkWnd->SetParentsVisible(IsVisible());
			pBkWnd->SetTabControler(m_tabControler);
			pBkWnd->SetTooltipCtrl(m_wndToolTip);
			pBkWnd->Load(pXmlChild);
			m_lstWndChild.AddTail(pBkWnd);
		}
	}

	m_pVScroll = new CBkScroll();
	m_pVScroll->RegisterNotify(this);
	m_pVScroll->SetParent(m_hBkWnd);
	m_pVScroll->SetContainer(m_hWndContainer);
	m_pVScroll->SetDropTarget(m_pDropTarget);
	m_pVScroll->SetRealContainer(m_hWndRealContainer);
	m_pVScroll->SetVisible(FALSE);
	m_pVScroll->SetSkin(m_vScrollSkin);
	m_lstWndChild.AddTail(m_pVScroll);

	m_pHScroll = new CBkScroll();
	m_pHScroll->RegisterNotify(this);
	m_pHScroll->SetType(CBkScroll::STE_H);
	m_pHScroll->SetSkin(m_hScrollSkin);
	m_pHScroll->SetParent(m_hBkWnd);
	m_pHScroll->SetContainer(m_hWndContainer);
	m_pVScroll->SetDropTarget(m_pDropTarget);
	m_pHScroll->SetRealContainer(m_hWndRealContainer);
	m_pHScroll->SetVisible(FALSE);
	m_lstWndChild.AddTail(m_pHScroll);

	assert(m_pTreeBody != NULL);
	return TRUE;
}

inline BOOL	CBkTree::Action(NotifyInfo* pInfo)
{
	if (pInfo->nAction == BKM_LIST_MOUSEWHEEL)
	{
		m_pVScroll->SetValue(pInfo->dwData);
	}
	else if (pInfo->nAction == BKM_LIST_SCROLL_V)
	{
		m_pTreeBody->SetShowTop(pInfo->dwData);
	}
	return TRUE;
}

inline void CBkTree::OnNodeSelect(CTreeNode* node)
{
	if (m_pNotify)
	{
		NotifyInfo info = {BKM_TREE_NODE_SELECT, {0,0}, this, node->GetData()};
		m_pNotify->Action(&info);
	}
}

inline void CBkTree::Traverse(void (*TraverseFunPtr)(CTreeNode* pNode))
{
	m_pTreeBody->Traverse(TraverseFunPtr);
}

inline CTreeNode* CBkTree::FindNode(bool (*CompFunPtr)(CTreeNode* pNode))
{
	return m_pTreeBody->FindNode(CompFunPtr);
}

inline void CBkTree::SelectNode(CTreeNode* pNode)
{
	CTreeNode* parent = pNode->GetParentNode();
	while (parent) 
	{
		if (!parent->IsExpand()) 
		{
			parent->Expand();
		}

		parent = parent->GetParentNode();
	}

	m_pTreeBody->SelectNode(pNode, FALSE);
}

inline CTreeNode* CBkTree::GetSelectedNode()
{
	return m_pTreeBody->GetSelectedNode();
}

inline bool CBkTree::IsEmpty()
{
	return m_pTreeBody->IsEmpty();
}

inline CBkWnd* CBkTree::OnSetFocus( CBkWnd* which )
{
    m_bFocus = TRUE;
    __super::OnSetFocus( this );
    return this;
}

inline void CBkTree::ScrollToTop()
{
    if( m_pVScroll && m_pVScroll->IsVisible() )
    {
        m_pVScroll->Scroll( SB_TOP, 0 );
    }
}

inline void CBkTree::ScrollToBottom()
{
    if( m_pVScroll && m_pVScroll->IsVisible() )
    {
        m_pVScroll->Scroll( SB_BOTTOM, 0 );
    }
}

inline void CBkTree::ScrollLineUp()
{
    if( m_pVScroll && m_pVScroll->IsVisible() )
    {
        m_pVScroll->Scroll( SB_LINEUP, 0 );
    }
}

inline void CBkTree::ScrollLineDown()
{
    if( m_pVScroll && m_pVScroll->IsVisible() )
    {
        m_pVScroll->Scroll( SB_LINEDOWN, 0 );
    }
}

inline void CBkTree::ScrollToNode(int nNode)
{
    if( m_pVScroll && m_pVScroll->IsVisibleDeep(FALSE) && !m_pTreeBody->IsNodeVisible(nNode) )
    {
        int iScrollPos = m_pTreeBody->GetRealNodeTop(nNode);
        m_pVScroll->Scroll( SB_THUMBPOSITION, iScrollPos );
    }
}

inline BOOL CBkTree::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    BOOL bRet = __super::OnKeyDown(nChar, nRepCnt, nFlags); 

    if (m_pTreeBody)
    {
        CTreeNode* pSelNode = m_pTreeBody->GetSelectedNode();
        if (pSelNode)
        {
            int index = m_pTreeBody->GetNodeIndex(pSelNode);
            if (index >= 0)
            {
                //ScrollToNode(index);

                CRect rcBody;
                m_pTreeBody->GetRect(&rcBody);

                CRect rcNode;
                pSelNode->GetRect(&rcNode);

                if (nChar == VK_UP)
                {
                    if (!pSelNode->IsVisible() || rcNode.top < rcBody.top)
                    {
                        ScrollLineUp();
                    }
                }
                else if (nChar == VK_DOWN)
                {
                    if (!pSelNode->IsVisible() || rcNode.bottom > rcBody.bottom)
                    {
                        ScrollLineDown();
                    }
                }
            }
        }
    }

    return bRet;
}