//////////////////////////////////////////////////////////////////////////
//   File Name: bkwndcmnctrl.h
// Description: BkWindow Definition
//     Creator: Zhang Xiaoxuan
//     Version: 2009.04.28 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "bkwnd.h"
#include "bkimage.h"
#include "bktheme.h"


//////////////////////////////////////////////////////////////////////////
// Memory gdiplus Image Draw
//
// Usage: <memimage mempointer=xx />
//
class CBkMemoryImage : public CBkWnd
{
	BKOBJ_DECLARE_CLASS_NAME(CBkMemoryImage, "memimage")

    enum
    {
        Flip_None = 0,      // 不翻转
        Flip_Horz,          // 水平翻转
        Flip_Vert,          // 竖直翻转
    };

    CBkMemoryImage()
    :m_gdiPlusImage(NULL)
    ,m_nAutoSize(TRUE)
    ,m_nAlpha(256)
    ,m_nFlip(Flip_None)
    {
    }

public:
    void SetImage( Gdiplus::Image* image )
    {
        m_gdiPlusImage = image;
        NotifyInvalidate();
    }

    Gdiplus::Image* GetImage()
    {
        return m_gdiPlusImage;
    }

	BOOL OnPaint(CDCHandle dc)
	{
		if( !m_gdiPlusImage )
        {
			return TRUE;
        }

        CBkPngSkin skinDraw;
        Gdiplus::Graphics g(dc.m_hDC);
        switch( m_nFlip )
        {
        case Flip_Horz:
            m_gdiPlusImage->RotateFlip( RotateNoneFlipX );
            break;
        case Flip_Vert:
            m_gdiPlusImage->RotateFlip( RotateNoneFlipY );
            break;
        }

        int nWidth = m_gdiPlusImage->GetWidth();
        int nHeight = m_gdiPlusImage->GetHeight();
        if( m_nAutoSize )
        {
            nWidth = m_rcWindow.Width();
            nHeight = m_rcWindow.Height();
        }

        if( m_nAlpha >= 0 && m_nAlpha < 256 )
        {
            skinDraw.AlphaImage( g, m_rcWindow.left, m_rcWindow.top, nWidth, nHeight, *m_gdiPlusImage, Gdiplus::REAL((double)m_nAlpha/255) );
        }
        else
        {
           skinDraw.DrawImage( g, *m_gdiPlusImage, m_rcWindow.left, m_rcWindow.top, nWidth, nHeight );
        }

        switch( m_nFlip )
        {
        case Flip_Horz:
            m_gdiPlusImage->RotateFlip( RotateNoneFlipX );
            break;
        case Flip_Vert:
            m_gdiPlusImage->RotateFlip( RotateNoneFlipY );
            break;
        }

        return TRUE;
	}

	HRESULT OnAttributeChange(CStringA& strValue, BOOL bLoading)
	{
		m_gdiPlusImage = (Gdiplus::Image*)IntToPtr(atoi( strValue ));
		//m_hIcon = (HICON)IntToPtr(atoi(strValue));
		return S_OK;
	}

	HRESULT OnDrawImageSize(CStringA& strValue, BOOL bLoading)
	{
		if (FALSE == strValue.IsEmpty())
			m_nAutoSize = StrToIntA(strValue);
		return S_OK;
	}

protected:
	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_CUSTOM_ATTRIBUTE("mempointer", OnAttributeChange)
		//BKWIN_CUSTOM_ATTRIBUTE("autosize",   OnDrawImageSize)
        BKWIN_INT_ATTRIBUTE("autosize", m_nAutoSize, FALSE)
        BKWIN_INT_ATTRIBUTE("alpha", m_nAlpha, FALSE)
        BKWIN_ENUM_ATTRIBUTE("flip", INT, TRUE)
            BKWIN_ENUM_VALUE("none", Flip_None)
            BKWIN_ENUM_VALUE("horz", Flip_Horz)
            BKWIN_ENUM_VALUE("vert", Flip_Vert)
        BKWIN_ENUM_END(m_nFlip)
	BKWIN_DECLARE_ATTRIBUTES_END()

protected:
	Gdiplus::Image*	m_gdiPlusImage;
	int			    m_nAutoSize;            // 是否根据给定的矩形区域自适应大小，默认自适应，0表示根据IMAGE的实际大小绘制
    int             m_nFlip;                // 是否翻转
    int             m_nAlpha;               // 是否透明
};

//////////////////////////////////////////////////////////////////////////
// Image Control
// Use src attribute specify a resource id
// 
// Usage: <img src=xx />
//
class CBkImageWnd : public CBkWnd
                  , public IBkImage
{
	BKOBJ_DECLARE_CLASS_NAME(CBkImageWnd, "img")

public:
	CBkImageWnd()
		: m_nSubImageID(-1)
        , m_nFrameCurrent(0)
		, m_pSkin(NULL)
	{

    }
    virtual IBkWnd* MakeCopy()
    {
        return new CBkImageWnd(*this);
    }

    virtual void CreateHandle(HWND hParent)
    {
        CBkWnd::CreateHandle( hParent );
        if( m_pSkin )
        {
            if( m_pSkin->IsClass("gif") )
            {
                Replay();
            }
        }
    }

    virtual BOOL OnDestroy()
    {
        CBkWnd::OnDestroy();
        ::KillTimer( GetContainer(), (UINT_PTR)this );
        return TRUE;
    }

	BOOL OnPaint(CDCHandle dc)
	{
		if (m_pSkin)
			m_pSkin->Draw(dc, m_rcWindow, m_nSubImageID);
        return FALSE;
	}

    BOOL OnNcCalcSize(LPSIZE pSize)
    {
        __super::OnNcCalcSize(pSize);

        if (m_pSkin)
        {
            if( 2 == m_dlgpos.nType )
            {
                SIZE sizeImage = m_pSkin->GetSkinSize();

                if (sizeImage.cx)
                    pSize->cx = sizeImage.cx;
                if (sizeImage.cy)
                    pSize->cy = sizeImage.cy;
            }
        }

        return FALSE;
    }

    virtual void OnVisibleChanged(BOOL bVisible)
    {
        CBkWnd::OnVisibleChanged( bVisible );
        if( m_pSkin )
        {
            if( m_pSkin->IsClass("gif") )
            {
                bVisible ? Replay() : Stop();
            }
        }
    }

    static VOID CALLBACK OnTimer( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime )
    {
        CBkImageWnd* pImgCtrl = ( CBkImageWnd* )idEvent;
        if( pImgCtrl )
        {
            CBkSkinBase* pSkin = pImgCtrl->GetSkin();
            if( pSkin )
            {
                if( pSkin->IsClass("gif") )
                {
                    ::KillTimer( hwnd, idEvent );
                    CBkGifSkin* pSkinGif = (CBkGifSkin*)(pSkin);
                    ::SetTimer( hwnd, idEvent, pImgCtrl->UpdateFrame(), OnTimer );
                }
            }
        }
    }

    virtual void SetSkin(CBkSkinBase* pSkin)
    {
        m_pSkin = pSkin; 
        NotifyInvalidate();
    }

    virtual CBkSkinBase* GetSkin()
    {
        return m_pSkin;
    }

    virtual void SetSubImage(int iIndex)
    {
        m_nSubImageID = iIndex;
        NotifyInvalidate();
    }

    int UpdateFrame()
    {
        if( m_pSkin )
        {
            if( m_pSkin->IsClass("gif") )
            {
                CBkGifSkin* pSkinGif = (CBkGifSkin*)(m_pSkin);
                if( ++m_nFrameCurrent >= pSkinGif->GetFrameCount() )
                {
                    m_nFrameCurrent = pSkinGif->GetFrameCount();
                }
                m_nFrameCurrent = pSkinGif->UpdateFrame( m_nFrameCurrent );
                NotifyInvalidate();
                return pSkinGif->GetFrameTime(m_nFrameCurrent);
            }
        }
        return 0;
    }

protected:
    CBkImageWnd(const CBkImageWnd& bkImageWnd) : CBkWnd(bkImageWnd)
        , m_nSubImageID(bkImageWnd.m_nSubImageID)
        , m_pSkin(bkImageWnd.m_pSkin)
    {
    }

    void Replay()
    {
        if( m_pSkin )
        {
            if( m_pSkin->IsClass("gif") )
            {
                ::KillTimer( GetContainer(), (UINT_PTR)this );

                CBkGifSkin* pSkinGif = (CBkGifSkin*)(m_pSkin);
                m_nFrameCurrent = pSkinGif->GetFrameFirst();
                pSkinGif->Reset();
                ::SetTimer( GetContainer(), (UINT_PTR)this, pSkinGif->GetFrameTime(m_nFrameCurrent), OnTimer );
            }
        }
    }

    void Stop()
    {
        if( m_pSkin )
        {
            if( m_pSkin->IsClass("gif") )
            {
                ::KillTimer( GetContainer(), (UINT_PTR)this );

                CBkGifSkin* pSkinGif = (CBkGifSkin*)(m_pSkin);
                m_nFrameCurrent = pSkinGif->GetFrameFirst();
                pSkinGif->Reset();
            }
        }
    }

	int m_nSubImageID;
    UINT m_nFrameCurrent;
    
	CBkSkinBase *m_pSkin;

	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_SKIN_ATTRIBUTE("skin", m_pSkin, TRUE)
		BKWIN_INT_ATTRIBUTE("sub", m_nSubImageID, FALSE)
	BKWIN_DECLARE_ATTRIBUTES_END()
};

//////////////////////////////////////////////////////////////////////////
// Line Control
// Simple HTML "HR" tag
//
// Usage: <hr style=solid size=1 crbg=.../>
//
class CBkLine : public CBkWnd
{
	BKOBJ_DECLARE_CLASS_NAME(CBkLine, "hr")

public:

	CBkLine()
		: m_nPenStyle(PS_SOLID)
		, m_nLineSize(1)
	{
    }

	// Do nothing
	BOOL OnPaint(CDCHandle dc)
	{
        Gdiplus::Graphics g(dc.m_hDC);
        Gdiplus::Pen pen( Gdiplus::Color(254, GetRValue(m_style.m_crBg), GetGValue(m_style.m_crBg), GetBValue(m_style.m_crBg)), REAL(m_nLineSize) );
        pen.SetDashStyle((Gdiplus::DashStyle)m_nPenStyle);
        g.DrawLine( &pen, m_rcWindow.left, m_rcWindow.top, m_rcWindow.right, m_rcWindow.top );
        return FALSE;
    }

protected:
	int m_nPenStyle;
	int m_nLineSize;

	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_INT_ATTRIBUTE("size", m_nLineSize, FALSE)
		BKWIN_ENUM_ATTRIBUTE("style", int, FALSE)
        BKWIN_ENUM_VALUE("solid", Gdiplus::DashStyleSolid)             // default
        BKWIN_ENUM_VALUE("dash", Gdiplus::DashStyleDash)               /* -------  */ 
        BKWIN_ENUM_VALUE("dot", Gdiplus::DashStyleDot)                 /* .......  */ 
        BKWIN_ENUM_VALUE("dashdot", Gdiplus::DashStyleDashDot)         /* _._._._  */ 
        BKWIN_ENUM_VALUE("dashdotdot", Gdiplus::DashStyleDashDotDot)   /* _.._.._  */ 
		BKWIN_ENUM_END(m_nPenStyle)
	BKWIN_DECLARE_ATTRIBUTES_END()

};

//////////////////////////////////////////////////////////////////////////
// Check Box
//
// Usage: <check state=1>This is a check-box</check>
//
class CBkCheckBox : public CBkText
{
	BKOBJ_DECLARE_CLASS_NAME(CBkCheckBox, "check")

		enum {
			CheckBoxSize = 13,
			CheckBoxSpacing = 4,
	};

public:

	CBkCheckBox()
		: m_theme()
        ,m_size(CheckBoxSize)
        ,m_spacing(CheckBoxSpacing)
	{
	}

    CBkCheckBox::CBkCheckBox(const CBkCheckBox& bkCheckBox)
        :CBkText(bkCheckBox)
        , m_theme()
        ,m_size(bkCheckBox.m_size)
        ,m_spacing(bkCheckBox.m_spacing)
    {}
    IBkWnd* CBkCheckBox::MakeCopy()
    {
        return new CBkCheckBox(*this);
    }

    BOOL OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
    {
        if( nChar == VK_SPACE && IsFocus() )
        {
            BOOL isCheckLast = IsChecked();

            if (IsChecked())
                ModifyState(0, BkWndState_Check);
            else
                ModifyState(BkWndState_Check, 0);
            NotifyInvalidate();
            if (m_pNotify != NULL)
            {
                DWORD dwData = (DWORD)MAKELONG( isCheckLast, IsChecked() );;
                NotifyInfo info = {BKM_CHECK_CLICK, CPoint(0,0), this, dwData};
                m_pNotify->Action(&info);
            }
            return TRUE;
        }
        else
        {
            return __super::OnKeyDown( nChar, nRepCnt, nFlags );
        }
        return FALSE;
    }

	BOOL OnPaint(CDCHandle dc)
	{
		CRect rcCheckBox;
        rcCheckBox.left = m_rcWindow.left + m_style.m_nMarginX;
		rcCheckBox.right = rcCheckBox.left + m_size;
        rcCheckBox.top = m_rcWindow.top + (m_rcWindow.Height() - m_size)/2;
        rcCheckBox.bottom = rcCheckBox.top + m_size;

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
		m_style.m_nTextAlign |= DT_VCENTER;

		__super::OnPaint(dc);

		m_rcWindow.left -= m_size + m_spacing;
        return FALSE;
	}

    void DrawBkgnd(CDCHandle& dc){}

    BOOL OnNcCalcSize(LPSIZE pSize)
    {
        __super::OnNcCalcSize(pSize);
        if (m_dlgpos.nType == 2)
        {
            pSize->cx += m_size + m_size;
            pSize->cy = max(pSize->cy, m_size);
        }

        return FALSE;
    }

protected:

	CBkCheckBoxTheme m_theme;

	int m_nState;
    int m_size;         // Check尺寸
    int m_spacing;      // Check与文字的间距

	UINT _GetDrawState()
	{
		DWORD dwState = GetState();
		UINT uState = m_theme.IsValid() ? 0 : DFCS_BUTTONCHECK | DFCS_TRANSPARENT;

		if (m_theme.IsValid())
		{
			if (dwState & BkWndState_Check)
			{
				if (dwState & BkWndState_Disable)
					uState = CBS_CHECKEDDISABLED;
				else if (dwState & BkWndState_PushDown)
					uState = CBS_CHECKEDPRESSED;
				else if (dwState & BkWndState_Hover)
					uState = CBS_CHECKEDHOT;
				else
					uState = CBS_CHECKEDNORMAL;
			}
			else
			{
				if (dwState & BkWndState_Disable)
					uState = CBS_UNCHECKEDDISABLED;
				else if (dwState & BkWndState_PushDown)
					uState = CBS_UNCHECKEDPRESSED;
				else if (dwState & BkWndState_Hover)
					uState = CBS_UNCHECKEDHOT;
				else
					uState = CBS_UNCHECKEDNORMAL;
			}
		}
		else
		{
			uState = DFCS_BUTTONCHECK | DFCS_TRANSPARENT;
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

	virtual BOOL NeedRedrawWhenStateChange()
	{
		return TRUE;
	}

	BOOL OnLButtonUp(UINT nFlags, CPoint point ,BOOL bAct = TRUE)
	{
        BOOL isCheckLast = IsChecked();

		if (IsChecked())
			ModifyState(0, BkWndState_Check);
		else
			ModifyState(BkWndState_Check, 0);
        NotifyInvalidate();
		if (bAct)
		{
			if (m_pNotify != NULL)
			{
				DWORD dwData = (DWORD)MAKELONG( isCheckLast, IsChecked() );;
				NotifyInfo info = {BKM_CHECK_CLICK, point, this, dwData};
				m_pNotify->Action(&info);
			}
		}
        return FALSE;
	}

	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_UINT_ATTRIBUTE("check", m_nState, FALSE)
        BKWIN_INT_ATTRIBUTE("size", m_size, FALSE)
        BKWIN_INT_ATTRIBUTE("spacing", m_spacing, FALSE)
	BKWIN_DECLARE_ATTRIBUTES_END()


};

//////////////////////////////////////////////////////////////////////////
// ImageListH Control add by zbl
// Use src attribute specify a resource id
// 
// Usage: <imglisth src=xx />
//
class CBkImageListHWnd : public CBkWnd
{
	BKOBJ_DECLARE_CLASS_NAME(CBkImageListHWnd, "imglisth")
public:
	CBkImageListHWnd()
		: m_nSubImageID(-1)
		, m_pSkin(NULL)
		, m_nImageCount(0)
	{
		m_mapSkin.RemoveAll();
		m_mapSkinSub.RemoveAll();
    }

	virtual BOOL Load(TiXmlElement* pTiXmlElem)
	{
		if (!CBkWnd::Load(pTiXmlElem))
			return FALSE;

		CStringA strSkin, strSkinValue;
		for (int i = 0; i < m_nImageCount; i++)
		{
			strSkin.Format("skin%d", i);
			m_mapSkin[strSkin] = NULL;
			//
			strSkin.Format("sub%d", i);
			m_mapSkinSub[strSkin] = -1;
		}

		for (TiXmlAttribute *pAttrib = pTiXmlElem->FirstAttribute(); NULL != pAttrib; pAttrib = pAttrib->Next())
		{
			strSkin = pAttrib->Name();
			if (0 == strSkin.CompareNoCase("skinlist"))
			{
				MapSkinList(pAttrib->Value());
			}else if (0 == strSkin.CompareNoCase("sublist"))
			{
				MapSkinSubList(pAttrib->Value());
			}

		}

		return TRUE;
	}

	BOOL OnPaint(CDCHandle dc)
	{
		CRect rc = m_rcWindow;
		CStringA strKey = "";
		for (int i = 0; i < m_nImageCount; i++)
		{
			CBkSkinBase* pSkin = NULL;
			strKey.Format("skin%d", i);
			pSkin = m_mapSkin[strKey];
			if (NULL != pSkin)
			{
				strKey.Format("sub%d", i);
				pSkin->Draw(dc, rc, m_mapSkinSub[strKey]);
				SIZE size = pSkin->GetSkinSize();
				rc.OffsetRect(size.cx, 0);
			}
		}
        return FALSE;
    }

    BOOL OnNcCalcSize(LPSIZE pSize)
    {
        __super::OnNcCalcSize(pSize);

        if (m_pSkin)
        {
            SIZE sizeImage;
            sizeImage = m_pSkin->GetSkinSize();

            if (sizeImage.cx)
                pSize->cx = sizeImage.cx;
            if (sizeImage.cy)
                pSize->cy = sizeImage.cy;
        }

        return FALSE;
    }

	void InitSkinList(CStringA SkinList, CSimpleArray<CStringA>& arrList)
	{
		CStringA strList = SkinList, strTmp;
		arrList.RemoveAll();
		strList.Append(",");
		int nIndex = -1;
		nIndex = strList.Find(",");
		while(nIndex >= 0)
		{
			strTmp = strList.Left(nIndex);
			if (strTmp.GetLength() > 0)
				arrList.Add(strTmp);
			strList = strList.Mid(nIndex+1);
			nIndex = strList.Find(",");
		}
	}

	void MapSkinList(CStringA strList)
	{
		CSimpleArray<CStringA> arrList;
		InitSkinList(strList, arrList);
		CStringA strSkinValue;
		for (int i = 0; i < arrList.GetSize(); i++)
		{
			CStringA strSkin = arrList[i];
			int nIndex = strSkin.Find("=");
			if (nIndex > 0)
			{
				strSkin = strSkin.Left(nIndex);
				strSkinValue = arrList[i].Mid(nIndex+1);
				m_mapSkin[strSkin] = BkSkin::GetSkin(strSkinValue);
			}
		}
	}

	void MapSkinSubList(CStringA strList)
	{
		CSimpleArray<CStringA> arrList;
		InitSkinList(strList, arrList);
		CStringA strSkinValue;
		for (int i = 0; i < arrList.GetSize(); i++)
		{
			CStringA strSkin = arrList[i];
			int nIndex = strSkin.Find("=");
			if (nIndex > 0)
			{
				strSkin = strSkin.Left(nIndex);
				strSkinValue = arrList[i].Mid(nIndex+1);
				m_mapSkinSub[strSkin] = StrToIntA(strSkinValue);

			}
		}
	}
	HRESULT OnAttributeSkinListChange(CStringA& strValue, BOOL bLoading)
	{//skin0="img0",skin1="img1",skin2="img2"
		if (FALSE == strValue.IsEmpty())
		{
			m_strSkinList = strValue;
			MapSkinList(m_strSkinList);
		}

		return TRUE;
	}

	HRESULT OnAttributeSubSinListChange(CStringA& strValue, BOOL bLoading)
	{//skin0sub="0",skin1sub="1",skin2sub="2"
		if (FALSE == strValue.IsEmpty())
		{
			m_strSubList = strValue;
			
			MapSkinSubList(m_strSubList);
		}

		return TRUE;
	}

protected:
	int m_nSubImageID;
	CBkSkinBase *m_pSkin;
	//
	int m_nImageCount;
	CStringA m_strSkinList;
	CStringA m_strSubList;
	//CSimpleMap<CStringA, CBkSkinBase*>	m_mapSkin;
	CAtlMap<CStringA, int>				m_mapSkinSub;
	CAtlMap<CStringA, CBkSkinBase*>		m_mapSkin;

	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_SKIN_ATTRIBUTE("skin", m_pSkin, TRUE)
		BKWIN_INT_ATTRIBUTE("sub", m_nSubImageID, FALSE)
		BKWIN_INT_ATTRIBUTE("imagecount", m_nImageCount, FALSE)
		BKWIN_CUSTOM_ATTRIBUTE("skinlist", OnAttributeSkinListChange)
		BKWIN_CUSTOM_ATTRIBUTE("sublist", OnAttributeSubSinListChange)
	BKWIN_DECLARE_ATTRIBUTES_END()
};