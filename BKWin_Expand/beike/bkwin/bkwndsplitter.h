//////////////////////////////////////////////////////////////////////////
//   File Name: bkwndSplitter.h
// Description: splitter Control
//     Creator: xu hailong
//     Version: 2011.08.20 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#include "bkwnddiv.h"
#include "bkwndbtn.h"
#include "bkwndfloating.h"

#pragma once


//////////////////////////////////////////////////////////////////////////
// CBkSplitter¶¨Òå
class CBkSplitter
	: public CBkDiv
	, public INotify
	, public IBkSplitter
{
	BKOBJ_DECLARE_CLASS_NAME(CBkSplitter, "splitter")

public:
	CBkSplitter();
	virtual void ExpandLeft();
	virtual void ExpandRight();

protected:
    //virtual BOOL    Load(TiXmlElement* pTiXmlElem);
    virtual BOOL    LoadChilds(TiXmlElement* pTiXmlChildElem);
    //virtual BOOL    OnLButtonDown(UINT nFlags, CPoint point);
    //virtual BOOL    OnLButtonUp(UINT nFlags, CPoint point);
    //virtual BOOL    OnMouseMove(UINT nFlags, CPoint point);
    //virtual void    OnSetCursor();
    virtual BOOL    Action(NotifyInfo* pInfo);

protected:
    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_UINT_ATTRIBUTE("left_min", m_leftmin, FALSE)
        BKWIN_UINT_ATTRIBUTE("right_min", m_rightmin, FALSE)
        BKWIN_UINT_ATTRIBUTE("left_zero_enable", m_leftzero_enable, FALSE)
        BKWIN_UINT_ATTRIBUTE("right_zero_enable", m_rightzero_enable, FALSE)
    BKWIN_DECLARE_ATTRIBUTES_END()

private:
	void MoveFloat(RECT const& rc, bool brepaint = false)
	{
		::MoveWindow(m_pFloatButton->GetContainer(),rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top,brepaint?TRUE:FALSE);
	}
	void MoveSplit(RECT const& rc, bool brepaint = false)
	{
		::MoveWindow(m_pButton->GetContainer(),rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top,brepaint?TRUE:FALSE);
	}
	//CBkSplitterButton* m_pLineButton;
	CBkSplitterButton* m_pButton;
	//CBkFloatingWindow* m_pFloatWnd;
	CBkFloatingWindow* m_pFloatButton;
	CBkDiv* m_pLeftDiv;
	CBkDiv* m_pRightDiv;
	bool m_bstarted;
	POINT m_ptlast;
	HCURSOR m_hOldCursor;
	unsigned int m_leftmin;
	unsigned int m_rightmin;
	unsigned int m_leftzero_enable;
	unsigned int m_rightzero_enable;
};

inline CBkSplitter::CBkSplitter()
{
	m_bstarted = false;
	m_pLeftDiv = 0;
	m_pButton = 0;
	m_pRightDiv = 0;
	m_pFloatButton = 0;
	//m_pFloatWnd = 0;
	m_hOldCursor = 0;
	m_leftmin = 0;
	m_rightmin = 0;
	m_leftzero_enable = 1;
	m_rightzero_enable = 1;
}

inline void CBkSplitter::ExpandLeft()
{
	BKDLG_POSITION pos;

	m_pRightDiv->GetPos(&pos);
	pos.Left.nPos = pos.Right.nPos;
	m_pRightDiv->SetPos(&pos,TRUE);

	m_pButton->GetPos(&pos);
	pos.Left.nPos = GetWidth()-m_pButton->GetWidth();
	pos.Right.nPos = GetWidth();
	m_pButton->SetPos(&pos,TRUE);

	m_pLeftDiv->GetPos(&pos);
	pos.Right.nPos = GetWidth();
	m_pLeftDiv->SetPos(&pos,TRUE);
}

inline void CBkSplitter::ExpandRight()
{
	BKDLG_POSITION pos;

	m_pLeftDiv->GetPos(&pos);
	pos.Right.nPos = pos.Left.nPos;
	m_pLeftDiv->SetPos(&pos,TRUE);

	m_pButton->GetPos(&pos);
	pos.Right.nPos -= pos.Left.nPos;
	pos.Left.nPos -= pos.Left.nPos;
	m_pButton->SetPos(&pos,TRUE);

	m_pRightDiv->GetPos(&pos);
	pos.Left.nPos = m_pButton->GetWidth();
	m_pRightDiv->SetPos(&pos,TRUE);
}

static void printpt(int x, int y,int z=0,char const* str="")
{
	char buffer [128] = {0};
	sprintf (buffer,"%s[%d,%d,%d]",str,x,y,z);
	OutputDebugStringA(buffer);
}
static void printpt(POINT pt,char const* str="")
{
	printpt(pt.x,pt.y,0,str);
}

inline BOOL CBkSplitter::Action(NotifyInfo* pInfo)
{
	if (pInfo->nAction == WM_LBUTTONDOWN)
	{
		m_bstarted = true;
		m_ptlast = pInfo->pt;

		//BKDLG_POSITION pos;
		//m_pButton->GetPos(&pos);
		//m_pLineButton->SetPos(&pos,FALSE);
		//m_pLineButton->SetVisible(TRUE);

		//RECT pos;
		//m_pButton->GetRect(&pos);

		//pos.left = pos.top = 0;
		//pos.right = pos.bottom = 30;
		//MoveFloat(pos);
		//m_pFloatButton->SetVisible(TRUE);
	}
	else if (pInfo->nAction == WM_MOUSEMOVE)
	{
		if (!m_bstarted) return TRUE;

		int xdelta = pInfo->pt.x - m_ptlast.x;

		//RECT oldpos;
		//m_pFloatButton->GetRect(&oldpos);
		//RECT pos = oldpos;
		//pos.left += xdelta;
		//pos.right += xdelta;

		//do {
		//	if (pos.left <= 0)
		//	{
		//		if (oldpos.left > 0)
		//		{
		//			pos.right -= pos.left;
		//			pos.left = 0;
		//			m_ptlast.x = (pos.right+pos.left)/2;
		//			MoveFloat(pos);
		//		}
		//		break;
		//	}
		//	else if (pos.right >= GetWidth())
		//	{
		//		if (oldpos.right<GetWidth())
		//		{
		//			pos.left = GetWidth()-m_pFloatButton->GetWidth();
		//			pos.right = GetWidth();
		//			m_ptlast.x = (pos.right+pos.left)/2;
		//			MoveFloat(pos);
		//		}
		//		break;
		//	}
		//	m_ptlast = pInfo->pt;
		//	MoveFloat(pos,true);
		//}while(0);

		BKDLG_POSITION oldpos;
		m_pButton->GetPos(&oldpos);
		BKDLG_POSITION pos = oldpos;
		pos.Left.nPos += xdelta;
		pos.Right.nPos += xdelta;

		do {
			printpt(pInfo->pt,"\n*");
			printpt(oldpos.Left.nPos,pos.Left.nPos,xdelta,"\t*");
			if (pos.Left.nPos <= 0)
			{
				printpt(pos.Left.nPos,0,0,"\t<");
				printpt(m_ptlast,"\tlast=");
				if (oldpos.Left.nPos>0)
				{
					pos.Right.nPos -= pos.Left.nPos;
					pos.Left.nPos = 0;
					m_ptlast.x = (pos.Right.nPos+pos.Left.nPos)/2;
					printpt(m_ptlast,"\tone-last=");
				}
				else break;
			}
			else if (pos.Right.nPos >= GetWidth())
			{
				if (oldpos.Right.nPos<GetWidth())
				{
					pos.Left.nPos = GetWidth()-m_pButton->GetWidth();
					pos.Right.nPos = GetWidth();
					m_ptlast.x = (pos.Right.nPos+pos.Left.nPos)/2;
				}
				else break;
			}
			else
			{
				m_ptlast = pInfo->pt;
			}

			BKDLG_POSITION pos2;

			m_pLeftDiv->GetPos(&pos2);
			pos2.Right.nPos = pos.Left.nPos;
			m_pLeftDiv->SetPos(&pos2,TRUE);

			m_pRightDiv->GetPos(&pos2);
			pos2.Left.nPos = pos.Right.nPos;
			m_pRightDiv->SetPos(&pos2,TRUE);

			m_pButton->SetPos(&pos,TRUE);
		} while (0);
	}
	else if (pInfo->nAction == WM_LBUTTONUP)
	{
		do {
			if (!m_bstarted) break;
			m_bstarted = false;
			break;
			m_ptlast = pInfo->pt;
			BKDLG_POSITION pos;

			BKDLG_POSITION pos2;

			m_pLeftDiv->GetPos(&pos2);
			pos2.Right.nPos = pos.Left.nPos;
			m_pLeftDiv->SetPos(&pos2,TRUE);

			m_pRightDiv->GetPos(&pos2);
			pos2.Left.nPos = pos.Right.nPos;
			m_pRightDiv->SetPos(&pos2,TRUE);
		}while(0);
	}
	else if (pInfo->nAction == WM_KEYDOWN)
	{
		do {
			if (pInfo->dwData != VK_ESCAPE)
				break;
			if (!m_bstarted)
				break;
			m_bstarted = false;
			m_pFloatButton->SetVisible(FALSE);
		    HCURSOR hCur = ::LoadCursor(NULL, IDC_ARROW);
		    ::SetCursor(hCur);
		}while(0);
	}
	return TRUE;
}

inline BOOL CBkSplitter::LoadChilds(TiXmlElement* pTiXmlChildElem)
{
    BOOL bVisible = IsVisibleDeep(TRUE);
    for (TiXmlElement* pXmlChild = pTiXmlChildElem; NULL != pXmlChild; pXmlChild = pXmlChild->NextSiblingElement())
    {
        CString strNode = pXmlChild->Value();
        CBkWnd* pBkWnd = NULL;
        if (strNode == CBkDiv::GetClassName())
        {
            CBkDiv* div = new CBkDiv();
			if (!m_pLeftDiv)
			{
				m_pLeftDiv = div;
				m_pLeftDiv->RegisterNotify(this);
			}
			else if(!m_pRightDiv)
			{
				m_pRightDiv = div;
				m_pRightDiv->RegisterNotify(this);
			}

            pBkWnd = div;

			if (pBkWnd)
			{
				pBkWnd->SetParent(m_hBkWnd);
				pBkWnd->SetContainer(m_hWndContainer);
				pBkWnd->SetRealContainer(m_hWndRealContainer);
				pBkWnd->SetParentsVisible(IsVisible());
				pBkWnd->SetTabControler(m_tabControler);
				pBkWnd->SetTooltipCtrl(m_wndToolTip);
				pBkWnd->Load(pXmlChild);
			}
        }
		//else if (strNode == CBkFloatingWindow::GetClassName())
		//{
		//	if (!m_pFloatButton)
		//	{
		//		pBkWnd = m_pFloatButton = new CBkFloatingWindow();

		//		//pBkWnd->SetVisible(FALSE);

		//		pBkWnd->SetParent(m_hBkWnd);
		//		pBkWnd->SetContainer(m_hWndContainer);
		//		pBkWnd->SetRealContainer(m_hWndRealContainer);
		//		pBkWnd->SetParentsVisible(IsVisible());
		//		pBkWnd->SetTabControler(m_tabControler);
		//		pBkWnd->SetTooltipCtrl(m_wndToolTip);
		//		pBkWnd->Load(pXmlChild);
		//		continue;
		//	}
		//}
		else if (strNode == CBkSplitterButton::GetClassName())
		{
			if (!m_pButton)
			{
				pBkWnd = m_pButton = new CBkSplitterButton();
				//pBkWnd->SetAttribute(L"class",L"combobox_dropdown_item",true);
				//pBkWnd->SetAttribute(L"crbg",L"000000",true);
				m_pButton = static_cast<CBkSplitterButton*>(pBkWnd);
				pBkWnd->RegisterNotify(this);

				pBkWnd->SetParent(m_hBkWnd);
				pBkWnd->SetContainer(m_hWndContainer);
				pBkWnd->SetRealContainer(m_hWndRealContainer);
				pBkWnd->SetParentsVisible(IsVisible());
				pBkWnd->SetTabControler(m_tabControler);
				pBkWnd->SetTooltipCtrl(m_wndToolTip);
				pBkWnd->Load(pXmlChild);

				//m_lstWndChild.AddTail(pBkWnd);

				//pBkWnd = m_pFloatButton = static_cast<CBkSplitterButton*>(m_pButton->MakeCopy());
				//pBkWnd->SetVisible(FALSE);
				//m_pFloatButton = static_cast<CBkSplitterButton*>(pBkWnd);

				//pBkWnd->SetParent(m_hBkWnd);
				//pBkWnd->SetContainer(m_hWndContainer);
				//pBkWnd->SetRealContainer(m_hWndRealContainer);
				//pBkWnd->SetParentsVisible(IsVisible());
				//pBkWnd->SetTabControler(m_tabControler);
				//pBkWnd->SetTooltipCtrl(m_wndToolTip);
				//pBkWnd->Load(pXmlChild);
			}
		}
		assert (pBkWnd);
		if (pBkWnd) m_lstWndChild.AddTail(pBkWnd);
    }

    //assert(m_pFloatButton != NULL);

	BKDLG_POSITION posbutton;
	m_pButton->GetPos(&posbutton);

	BKDLG_POSITION rc;
	m_pLeftDiv->GetPos(&rc);
	rc.Right.nPos = posbutton.Left.nPos;
	m_pLeftDiv->SetPos(&rc,FALSE);

	m_pRightDiv->GetPos(&rc);
	rc.Left.nPos = posbutton.Right.nPos;
	m_pRightDiv->SetPos(&rc,FALSE);
    return TRUE;
}
