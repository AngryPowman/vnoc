//////////////////////////////////////////////////////////////////////////
//   File Name: bkwndRadioBox.h
// Description: RadioBox control
//     Creator: DongM
//     Version: 2011.04.24 - 1.0 - Create
//       Usage: <radiobox id="1108" pos="10,50,150,90">
//                  <radio pos="0,0,50,-0">aa</radio>
//                  <radio pos="60,0,110,-0">bb</radio>
//              </radiobox>
/////////////////////////////////////////////////////////////////////////
#pragma once


//////////////////////////////////////////////////////////////////////////
// CBkRadioBox定义
class CBkRadioBox
    : public CBkDiv
    , public IBkRadiobox
    , public INotify
{
    BKOBJ_DECLARE_CLASS_NAME(CBkRadioBox, "radiobox")

public:
    // interface IBkRadiobox
    virtual void    SetSelect(UINT nIndex);
    virtual int     GetSelect();

protected:  
    // interface INotify
    virtual BOOL    Action(NotifyInfo* pInfo);

    // inter 
    virtual BOOL    Load(TiXmlElement* pTiXmlElem);
};


//////////////////////////////////////////////////////////////////////////
// CBkRadio定义
class CBkRadio : public CBkText
{
    BKOBJ_DECLARE_CLASS_NAME(CBkRadio, "radio")

public:
    CBkRadio();
    virtual IBkWnd* MakeCopy();

protected:
    CBkRadio(const CBkRadio& bkRadio);
    virtual BOOL NeedRedrawWhenStateChange();
    virtual BOOL OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
    virtual BOOL OnPaint(CDCHandle dc);
    virtual BOOL OnNcCalcSize(LPSIZE pSize);
    virtual BOOL OnLButtonUp(UINT nFlags, CPoint point,BOOL bAct = TRUE);
    virtual void DrawBkgnd(CDCHandle& dc){}

    enum
    {
        RadioBoxSize = 12,
        RadioBoxSpacing = 2,
    };

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_INT_ATTRIBUTE("size", m_size, FALSE)
        BKWIN_INT_ATTRIBUTE("spacing", m_spacing, FALSE)
    BKWIN_DECLARE_ATTRIBUTES_END()

private:
    UINT _GetDrawState();
    CBkRadioBoxTheme m_theme;

    int m_size;         // Radio尺寸
    int m_spacing;      // Radio与文字的间距
};


//////////////////////////////////////////////////////////////////////////
// CBkRadioBox实现
inline CBkRadio::CBkRadio()
:m_size(RadioBoxSize)
,m_spacing(RadioBoxSpacing)
{
}
inline CBkRadio::CBkRadio(const CBkRadio& bkRadio)
:CBkText(bkRadio)
,m_size(bkRadio.m_size)
,m_spacing(bkRadio.m_spacing)
{}
inline IBkWnd* CBkRadio::MakeCopy()
{
    return new CBkRadio(*this);
}
inline BOOL CBkRadioBox::Load(TiXmlElement* pTiXmlElem)
{
    if (!CBkWnd::Load(pTiXmlElem))
        return FALSE;

    for (TiXmlElement* pXmlChild = pTiXmlElem->FirstChildElement(); NULL != pXmlChild; pXmlChild = pXmlChild->NextSiblingElement())
    {
        CBkRadio *pNewColumn = CBkRadio::CheckAndNew(pXmlChild->Value());
        if (!pNewColumn)
            continue;

        pNewColumn->SetParent(m_hBkWnd);
        pNewColumn->SetContainer(m_hWndContainer);
        pNewColumn->SetRealContainer(m_hWndRealContainer);
        pNewColumn->SetParentsVisible(IsVisible());
        pNewColumn->SetTabControler(m_tabControler);
        pNewColumn->SetTooltipCtrl(m_wndToolTip);
        pNewColumn->Load(pXmlChild);
        pNewColumn->RegisterNotify(this);

        m_lstWndChild.AddTail(pNewColumn);
    }
    return TRUE;
}

inline void CBkRadioBox::SetSelect(UINT nIndex)
{
    int i = 0;
    POSITION pos = m_lstWndChild.GetHeadPosition();
    while (pos != NULL)
    {
        CBkWnd *pBkWndChild = (CBkWnd*)m_lstWndChild.GetNext(pos);
        if (pBkWndChild)
        {
            if (i == nIndex)
            {
                pBkWndChild->ModifyState(BkWndState_Check, 0);
            }
            else
            {
                pBkWndChild->ModifyState(0, BkWndState_Check);
            }
            i++;
        }
    }
	NotifyInvalidate();
}

inline int CBkRadioBox::GetSelect()
{
    int nIndex = -1;
    int i = 0;
    POSITION pos = m_lstWndChild.GetHeadPosition();
    while (pos != NULL)
    {
        CBkWnd *pBkWndChild = (CBkWnd*)m_lstWndChild.GetNext(pos);
        if (pBkWndChild)
        {
            if (pBkWndChild->GetState() & BkWndState_Check)
            {
                nIndex = i;
                break;
            }
            i++;
        }
    }
    return nIndex;
}

inline BOOL CBkRadioBox::Action(NotifyInfo* pInfo)
{
    BOOL bUpdate = FALSE;
    if (pInfo->nAction == WM_LBUTTONUP)
    {
        POSITION pos = m_lstWndChild.GetHeadPosition();
        while (pos != NULL)
        {
            CBkWnd* pBkWndChild = (CBkWnd*)m_lstWndChild.GetNext(pos);
            if (pBkWndChild)
            {
                if (pBkWndChild == pInfo->pFrom)
                {
                    pBkWndChild->ModifyState(BkWndState_Check, 0);
                    bUpdate = TRUE;
                }
                else
                {
                    pBkWndChild->ModifyState(0, BkWndState_Check);
                }
            }
        }
        if (bUpdate)
        {
            Invalidate();
            if (m_pNotify != NULL)
            {
                pInfo->pFrom = this;
                m_pNotify->Action(pInfo);
            }
        }
    }
    return TRUE;
}


//////////////////////////////////////////////////////////////////////////
// CBkRadio实现
inline BOOL CBkRadio::OnPaint(CDCHandle dc)
{
    CRect rcCheckBox(0, 0, m_size, m_size);

    rcCheckBox.MoveToXY(m_rcWindow.left+m_style.m_nMarginX, m_rcWindow.top + (m_rcWindow.Height() - m_size) / 2);

    if (m_style.m_strSkinName.IsEmpty())
    {
        m_theme.OpenTheme(m_hWndContainer);

        if (m_theme.IsValid())
        {
            m_theme.DrawBackground(dc, _GetDrawState(), rcCheckBox);
        }
        else
        {
            dc.DrawFrameControl(rcCheckBox, DFC_BUTTON, _GetDrawState());
        }
    }
    else
    {
        CBkImageSkin* pSkin = dynamic_cast<CBkImageSkin*>(BkSkin::GetSkin(m_style.m_strSkinName));
        if (pSkin)
        {
            DWORD dwIndex=-1;

            if (BkWndState_Disable & m_dwState)
            {
                dwIndex = 4;
            }
            else
            {
                if(BkWndState_Hover & m_dwState)
                {
                    if (IsChecked())
                    {
                        dwIndex = 3;
                    }
                    else
                    {
                        dwIndex = 1;
                    }
                }
                else
                {
                    if (IsChecked())
                    {
                        dwIndex = 2;
                    }
                    else
                    {
                        dwIndex = 0;
                    }
                }
            }

            pSkin->Draw(dc, rcCheckBox, dwIndex);
        }
    }

    m_rcWindow.left += m_size + m_spacing;

    m_style.m_nTextAlign &= ~DT_BOTTOM;
    m_style.m_nTextAlign |= DT_VCENTER | DT_SINGLELINE;

    __super::OnPaint(dc);

    m_rcWindow.left -= m_size + m_spacing;
    return FALSE;
}

inline BOOL CBkRadio::OnNcCalcSize(LPSIZE pSize)
{
    __super::OnNcCalcSize(pSize);
    if (m_dlgpos.nType == 2)
    {
        pSize->cx += m_size + m_spacing;
        pSize->cy = max(pSize->cy, m_size);
    }
    return FALSE;
}

inline UINT CBkRadio::_GetDrawState()
{
    DWORD dwState = GetState();
    UINT uState = m_theme.IsValid() ? 0 : DFCS_BUTTONRADIO | DFCS_TRANSPARENT;

    if (m_theme.IsValid())
    {
        if (dwState & BkWndState_Check)
        {
            if (dwState & BkWndState_Disable)
                uState = RBS_CHECKEDDISABLED;
            else if (dwState & BkWndState_PushDown)
                uState = RBS_CHECKEDPRESSED;
            else if (dwState & BkWndState_Hover)
                uState = RBS_CHECKEDHOT;
            else
                uState = RBS_CHECKEDNORMAL;
        }
        else
        {
            if (dwState & BkWndState_Disable)
                uState = RBS_UNCHECKEDDISABLED;
            else if (dwState & BkWndState_PushDown)
                uState = RBS_UNCHECKEDPRESSED;
            else if (dwState & BkWndState_Hover)
                uState = RBS_UNCHECKEDHOT;
            else
                uState = RBS_UNCHECKEDNORMAL;
        }
    }
    else
    {
        uState = DFCS_BUTTONRADIO | DFCS_TRANSPARENT;
        if (dwState & BkWndState_Disable)
            uState |= DFCS_INACTIVE;
        if (dwState & BkWndState_Hover)
            uState |= DFCS_HOT;
        if (dwState & BkWndState_PushDown)
            uState |= DFCS_PUSHED;
        if (dwState & BkWndState_Check)
            uState |= DFCS_CHECKED;
    }

    return uState;
}

inline BOOL CBkRadio::NeedRedrawWhenStateChange()
{
    return TRUE;
}

inline BOOL CBkRadio::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
    if( nChar == VK_SPACE && IsFocus() )
    {
        if (m_pNotify != NULL)
        {
            if ((GetState() & BkWndState_Check) == 0)
            {
                NotifyInfo ni = {WM_LBUTTONUP, CPoint(0,0), this, GetCmdID()};
                m_pNotify->Action(&ni);
            }
        }
        return FALSE;
    }
    else
    {
        return __super::OnKeyDown( nChar, nRepCnt, nFlags );
    }
    return FALSE;
}

inline BOOL CBkRadio::OnLButtonUp(UINT nFlags, CPoint point,BOOL bAct)
{
	if (bAct)
	{
		if (m_pNotify != NULL)
		{
			if ((GetState() & BkWndState_Check) == 0)
			{
				NotifyInfo ni = {WM_LBUTTONUP, point, this, GetCmdID()};
				m_pNotify->Action(&ni);
			}
		}
	}
    return FALSE;
}