//////////////////////////////////////////////////////////////////////////
//   File Name: bkwndpanel.h
// Description: CBkPanel Definition, Virtual Container of BkWindows
//     Creator: Zhang Xiaoxuan
//     Version: 2009.04.28 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#pragma once

enum {
    TreeNodeState_Expand    = 0,
    TreeNodeState_Collapse  = 1
};

class CBkTreeNode
    : public CBkDiv
    , public INotify
    , public IBkTreeNode
{
public:
	BKOBJ_DECLARE_CLASS_NAME(CBkTreeNode, "treenode")

	CBkTreeNode()
    : m_iLevel(0)
    , m_iIndent(0)
    , m_iSpaceFromIconToText(0)
    , m_bExpand(TRUE)
    , m_dwData(0)
    , m_bHasImage(TRUE)
    {}
    int             GetLevel(){return m_iLevel;}
    void            SetLevel(int iLevel);
    BOOL            IsExpand(){return m_bExpand;}
    void            SetExpand(BOOL bExpand);
    void            ArrangeChildren();
    void            SetHasChild(BOOL bHasChild);
    void            SetExpandIconSkin(CBkSkinBase* pSkin);
    void            SetExpandSelIconSkin(CBkSkinBase* pSkin);
    void            SetTreeIconSkin(CBkSkinBase* pSkin);
    void            SetSelected(BOOL bSelected);
    BOOL            OnMouseHover();
    BOOL            OnMouseLeave();
    

protected:
	int             m_iLevel;
    int             m_iIndent;
    int             m_iSpaceFromIconToText;
    CBkSkinBase*    m_pTreeIconSkin;
    CBkSkinBase*    m_pExpandIconSkin;
    CBkSkinBase*    m_pExpandSelIconSkin;
    BOOL            m_bExpand;
    DWORD           m_dwData;
    BOOL            m_bHasChild;
    CBkImageWnd*    m_pExpandIcon;
    CBkImageWnd*    m_pTreeIcon;
    CBkText*        m_pNodeText;
    BOOL            m_bHasImage;

	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_INT_ATTRIBUTE("level", m_iLevel, FALSE)
        BKWIN_INT_ATTRIBUTE("indent", m_iIndent, FALSE)
        BKWIN_INT_ATTRIBUTE("space", m_iSpaceFromIconToText, FALSE)
	BKWIN_DECLARE_ATTRIBUTES_END()
public:
    virtual void    SetImage(int iImage, int iSelectedImage);
    virtual void    SetText(LPCWSTR szText);
    virtual LPCWSTR GetText();
    virtual DWORD   GetData();
    virtual void    SetData(DWORD dwData);
    virtual void    SetTextColor(COLORREF);
    virtual void    SetIndent(int iIndent);
    virtual BOOL    OnLButtonDown( UINT nFlags, CPoint point );
    virtual BOOL    Load(TiXmlElement* pTiXmlElem);
    virtual BOOL    Action(NotifyInfo* pInfo);
};


class CBkTreeCtrl
	: public CBkDiv
    , public IBkTree
    , public INotify
{
	BKOBJ_DECLARE_CLASS_NAME(CBkTreeCtrl, "treectrl")

public:
	CBkTreeCtrl()
    : m_iIndent(0)
    , m_pSelectedNode(NULL)
    , m_pHoveredNode(NULL)
    , m_pSkinExpandIcon(NULL)
    , m_pSkinNode(NULL)
    {}

protected:  
    CBkTreeNode*    CreateEmptyItem();
	void            LayoutNodes();
    void            GetExpandIconRect(CBkWnd* pNode, CRect &rcExpandIcon);
    BOOL            OnLButtonDown(UINT nFlags, CPoint point);
    BOOL            OnLButtonDblClk(UINT nFlags, CPoint point);
    BOOL            OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    BOOL            HasChild(CBkTreeNode* pNode);
    BOOL            HasChild(POSITION pos);
    virtual BOOL    Load(TiXmlElement* pTiXmlElem);
    virtual BOOL    Action(NotifyInfo* pInfo);

protected:
	int         m_iIndent;
    CSize       m_sizeIcon;
	TiXmlNode   *m_pNodeXmlTemplete;
    CBkTreeNode *m_pSelectedNode; 
    CBkTreeNode *m_pHoveredNode;
    CBkSkinBase *m_pSkinExpandIcon;
    CBkSkinBase *m_pSkinExpandSelectIcon;
    CBkSkinBase *m_pSkinTreeIcon;
    CBkSkinBase *m_pSkinNode;
    COLORREF    m_crTextSelected;

	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_SKIN_ATTRIBUTE("expicon", m_pSkinExpandIcon, FALSE)
        BKWIN_SKIN_ATTRIBUTE("expselicon", m_pSkinExpandSelectIcon, FALSE)
        BKWIN_SKIN_ATTRIBUTE("treeicon", m_pSkinTreeIcon, FALSE)
        BKWIN_COLOR_ATTRIBUTE("crtextsel", m_crTextSelected, TRUE)
	BKWIN_DECLARE_ATTRIBUTES_END()
public:
    virtual IBkTreeNode*    GetRootNode();
    virtual IBkTreeNode*    GetFirstNode();
    virtual IBkTreeNode*    GetNextNode(IBkTreeNode* pNode);
    virtual IBkTreeNode*    GetNextSiblingNode(IBkTreeNode* pNode);
    virtual IBkTreeNode*    GetChildNode(IBkTreeNode* pNode);
    virtual IBkTreeNode*    GetParentNode(IBkTreeNode* pNode);
    virtual IBkTreeNode*    GetSelectedNode();
    virtual IBkTreeNode*    GetHoveredNode();
    virtual void            DeleteAllNodes();
    virtual void            DeleteNode(IBkTreeNode* pNode);
    virtual void            Expand(IBkTreeNode* pNode);
    virtual IBkTreeNode*    HitTest(const CPoint& point);
    virtual void            SelectNode(IBkTreeNode* pNode);
    virtual IBkTreeNode*    InsertNode(LPCWSTR lpszItem, int iImage, int iSelectedImage, IBkTreeNode* pParent, IBkTreeNode* pInsertAfter);

    // IDropContainer members
    virtual HRESULT OnDragEnter(IDataObject *pDataObj, POINT pt, DWORD *pdwEffect);
    virtual HRESULT OnDragOver(POINT pt, DWORD *pdwEffect);
    virtual HRESULT OnDrop(IDataObject *pDataObj, POINT pt, DWORD *pdwEffect);
	virtual HRESULT OnDragLeave(void);   
};

#include "bkwndtree.inl"