/***********************************************************************
    filename:   bktree.h
    created:    2011/11/16
    author:     ZhaoHao

    purpose:	Define a new tree control that supports multi hierarchy.
*************************************************************************/
#pragma once

#include <vector>

class CBkTree;
class CTreeBody;

class CTreeNode : public CBkDiv
				   , public INotify
{
    BKOBJ_DECLARE_CLASS_NAME(CTreeNode, "tree_node")

public:
	CTreeNode() : 
		m_pTree(NULL),
		m_pBody(NULL),
		m_bIsExpand(false),
		m_iIndent(0),
		m_pExpandImage(NULL),
		m_pText(NULL),
		m_bIsSelected(false),
		m_bIsInBody(false)
	{}

	void		SetParentNode(CTreeNode* parent);
	CTreeNode*	GetParentNode();
	void		SetTree(CBkTree* pTree);
	void		SetBody(CTreeBody* pBody);
	void		SetIndent(int indent);
	int			GetIndent();
	void		SetText(const CStringW& str, int index);
	IBkWnd*     GetItemIBkWnd(UINT uItemID);
	void		SetTextColor(COLORREF crText);
	void		SetSelected(bool set);
	bool		IsExpand();
	bool		IsHitExpandIcon(const CPoint& pt);
	int			GetChildCount();
	CTreeNode*	GetChild(int index);
	CTreeNode* 	CreateChild(bool update = true);
	void		Expand();
	void		Collapse();
	int			GetNodeHeight();
	void		InsertChildrenToBody();
	void		RemoveChildrenFromBody();
	void		OnLButtonHit(CPoint point);
	void		Clear(void (*ReleaseDataFunPtr)(DWORD dwData) = NULL);
	void		SetInBody(bool set);
	bool		IsInBody();

	void		Traverse(void (*TraverseFunPtr)(CTreeNode* pNode));
	void		ResetTip(int textIndex);
	void		RemoveTip(int textIndex);

	void		OnSelect();

	CTreeNode*	FindNode(bool (*CompFunPtr)(CTreeNode* pNode));
	
	virtual BOOL	Load(TiXmlElement* pTiXmlElem);
	virtual BOOL	Action(NotifyInfo* pInfo);
    virtual CBkWnd* OnSetFocus( CBkWnd* which );
	virtual BOOL    OnMouseHover();
	virtual BOOL	OnMouseLeave();

private:
	CBkTree*	m_pTree;
	CTreeBody*	m_pBody;

	bool		m_bIsInBody;
	bool		m_bIsExpand;
	bool		m_bIsSelected;

	typedef std::vector<CTreeNode*> ChildList;
	ChildList	m_Children;
	CTreeNode*	m_pParent;

	int				m_iIndent;
	CBkImageWnd*	m_pExpandImage;
	CBkText*		m_pText;
};	

class CTreeBody : public CBkDivV
                  , public INotify
{
    BKOBJ_DECLARE_CLASS_NAME(CTreeBody, "tree_body")

public:
	CTreeBody() : 
		m_pTree(NULL),
		m_pNodeTemplate(NULL),
		m_iIndentWidth(0),
		m_crTextSelected(0),
		m_crTextNormal(0),
		m_crTextHover(0),
		m_pSelectedNode(NULL)
	{}
	~CTreeBody()  
	{
	}

	bool		IsEmpty();
	void		SetTree(CBkTree* pTree);
	CTreeNode*	NewNode();
	CTreeNode*	CreateNode();
	void		DeleteNode(CTreeNode* pNode);
	void		InsertNode(CTreeNode* pNewNode, CTreeNode* pAfter, bool update = true);
	void		SelectNode(CTreeNode* pSelectedNode, BOOL bNotify = TRUE);
	CTreeNode*	GetSelectedNode();
	void		Clear(void (*ReleaseDataFunPtr)(DWORD dwData) = NULL);
	void		Update();
	int			GetRealHeight();
    int         GetRealNodeTop(int nNode);
    int         GetNodeIndex(CTreeNode* pNode);
    int         GetNodeCount();
    BOOL        IsNodeVisible(int nNode);
	void		Traverse(void (*TraverseFunPtr)(CTreeNode* pNode));
	void		ClearDeadNodes();
	CTreeNode*	FindNode(bool (*CompFunPtr)(CTreeNode* pNode));
	
	virtual BOOL	Load(TiXmlElement* pTiXmlElem);
	virtual BOOL	Action(NotifyInfo* pInfo);
	virtual BOOL    OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL	OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual BOOL    OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    virtual BOOL	OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

private:
	CTreeNode*	GetHitNode(const CPoint& pt);
	void		DestroyNode(CTreeNode* node);

	CBkTree*	m_pTree;
	TiXmlNode*  m_pNodeTemplate;          // 默认的Node模板
	int			m_iIndentWidth;
	COLORREF	m_crTextSelected;
	COLORREF	m_crTextNormal;
	COLORREF	m_crTextHover;

	typedef std::vector<CTreeNode*> NodeList;
	NodeList	m_RootNodes;

	CTreeNode*	m_pSelectedNode;

	// dead pool
	typedef std::vector<CTreeNode*> DeadNodeList;
	DeadNodeList m_DeadNodes;

	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_INT_ATTRIBUTE("Indent", m_iIndentWidth, FALSE)
		BKWIN_COLOR_ATTRIBUTE("crtextsel", m_crTextSelected, FALSE)
		BKWIN_COLOR_ATTRIBUTE("crtextnormal", m_crTextNormal, FALSE)
		BKWIN_COLOR_ATTRIBUTE("crtexthover", m_crTextHover, FALSE)
	BKWIN_DECLARE_ATTRIBUTES_END()

	friend class CTreeNode;
};


//////////////////////////////////////////////////////////////////////////
// CBkTree定义
class CBkTree
    : public CBkDiv
    , public INotify
{
    BKOBJ_DECLARE_CLASS_NAME(CBkTree, "tree")

public:
	CBkTree() : 
		m_pTreeBody(NULL),
		m_pHScroll(NULL),
		m_pVScroll(NULL)
	{}
	~CBkTree()
	{
	}

	bool		IsEmpty();
	void		Update();
	CTreeNode*	CreateNode();
	void		ClearNodes(void (*ReleaseDataFunPtr)(DWORD dwData) = NULL);
	void		UpdateScroll(bool reposition = false);
	void		OnNodeSelect(CTreeNode* node);
	void		Traverse(void (*TraverseFunPtr)(CTreeNode* pNode));
	CTreeNode*	GetSelectedNode();
	CTreeNode*	FindNode(bool (*CompFunPtr)(CTreeNode* pNode));
	void		SelectNode(CTreeNode* pNode);

    void        ScrollToTop();
    void        ScrollToBottom();
    void        ScrollLineUp();
    void        ScrollLineDown();
    void        ScrollToNode(int nNode);

	virtual BOOL    OnDestroy();
	virtual BOOL    LoadChilds(TiXmlElement* pTiXmlChildElem);
	virtual BOOL	Action(NotifyInfo* pInfo);
    virtual CBkWnd* OnSetFocus( CBkWnd* which );

    virtual BOOL	OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

protected:
    CTreeBody*	m_pTreeBody;
    CBkScroll*	m_pHScroll;
    CBkScroll*	m_pVScroll;
	CStringA	m_vScrollSkin; 
	CStringA	m_hScrollSkin;

	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_STRING_ATTRIBUTE("VScroll", m_vScrollSkin, FALSE)
		BKWIN_STRING_ATTRIBUTE("HScroll", m_hScrollSkin, FALSE)
	BKWIN_DECLARE_ATTRIBUTES_END()
};

#include "bktree.inl"