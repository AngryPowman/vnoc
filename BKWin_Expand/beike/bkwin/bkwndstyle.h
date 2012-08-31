//////////////////////////////////////////////////////////////////////////
//   File Name: BkWndStyle.h
// Description: BkWindow Styles Definition
//     Creator: Zhang Xiaoxuan
//     Version: 2009.04.28 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "bkskin.h"

class BkStyle : public CBkObject
{
    BKOBJ_DECLARE_CLASS_NAME(BkStyle, "style")

public:
    BkStyle()
        : m_uAlign(0)
        , m_nTextAlign(DT_BOTTOM | DT_SINGLELINE)
        , m_crBg(CLR_INVALID)
        , m_crBgHover(CLR_INVALID)
        , m_crBgCheck(CLR_INVALID)
        , m_crText(CLR_INVALID)
        , m_crHoverText(CLR_INVALID)
        , m_crPushdownText(CLR_INVALID)
        , m_crDisabledText(RGB(0xCC, 0xCC, 0xCC))
        , m_alpha(254)
        , m_ftText(NULL)
        , m_ftHover(NULL)
        , m_nMarginX(0)
        , m_nMarginY(0)
        , m_nSpacing(0)
        , m_nLineSpacing(20)
        , m_lpCursorName(IDC_ARROW)
        , m_bBackground(FALSE)
        , m_nRound(0)
        , m_bOwnDC(FALSE)
    {
    }

    enum {
        // Specify by "cursor" attribute
        Cursor_Mask     = 0xF00UL, 
        Cursor_Arrow    = 0x000UL,   // cursor = "arrow"
        Cursor_Hand     = 0x100UL,   // cursor = "hand"
    };

    COLORREF m_crBg;
    COLORREF m_crBgHover;
    COLORREF m_crBgCheck;
    COLORREF m_crText;
    COLORREF m_crHoverText;
    COLORREF m_crPushdownText;
    COLORREF m_crDisabledText;
    UINT m_alpha;
    UINT m_uAlign;
    int m_nTextAlign;
    HFONT m_ftText;
    HFONT m_ftHover;
    int m_nMarginX;
    int m_nMarginY;
    int m_nSpacing;
    int m_nLineSpacing;
    BOOL m_bBackground;
    BOOL m_bOwnDC;
    LPCTSTR m_lpCursorName;
    CStringA m_strSkinName;
    CStringA m_strScrollV;
    CStringA m_strScrollH;
    int m_nRound;

    HFONT GetFont(DWORD dwState)
    {
        if (BkWndState_Hover == (dwState & BkWndState_Hover))
        {
            return m_ftHover;
        }
        return m_ftText;
    }

    // Get class name
    LPCSTR GetName()
    {
        return m_strClassName;
    }

    // Get style object from pool by class name
    static const BkStyle& GetStyle(LPCSTR lpszName)
    {
        CStringA strName = lpszName;
        const __StylePoolClass::CPair *pFindRet = _GetStylePool()->Lookup(strName);

        if (pFindRet)
            return pFindRet->m_value;
        else
            return (*_GetStylePool())[""];
    }

    static BOOL LoadStyles(LPCTSTR szRes)
    {
        CStringA strXml;
        BOOL bRet = FALSE;

        bRet = BkResManager::LoadResource(szRes, strXml);
        if (!bRet)
            return FALSE;
        return LoadStylesXml(strXml);
    }

    static BOOL LoadStylesXml(LPCSTR lpszXml)
    {
        TiXmlDocument xmlDoc;

        _GetStylePool()->RemoveAll();

        (*_GetStylePool())[""];

        xmlDoc.Parse(lpszXml, NULL, TIXML_ENCODING_UTF8);

        if (xmlDoc.Error())
            return FALSE;

        _LoadStylePool(xmlDoc.RootElement());

        return TRUE;
    }

    static size_t GetCount()
    {
        return _GetStylePool()->GetCount();
    }

protected:

    typedef CAtlMap<CStringA, BkStyle> __StylePoolClass;

    CStringA m_strClassName;

    static __StylePoolClass* _GetStylePool()
    {
		static __StylePoolClass instance;
		return &instance;
    }

//     static __StylePoolClass& _GetStylePool()
//     {
//         static __StylePoolClass s_mapStylePool;
// 
//         return s_mapStylePool;
//     }

    // Load style-pool from xml tree
    static void _LoadStylePool(TiXmlElement *pXmlStyleRootElem)
    {
        LPCSTR lpszClassName = NULL;

        if (!pXmlStyleRootElem)
            return;

        if (strcmp(pXmlStyleRootElem->Value(), "style") != 0)
            return;

        for (TiXmlElement* pXmlChild = pXmlStyleRootElem->FirstChildElement("class"); NULL != pXmlChild; pXmlChild = pXmlChild->NextSiblingElement("class"))
        {
            lpszClassName = pXmlChild->Attribute("name");
            if (!lpszClassName)
                continue;

            (*_GetStylePool())[lpszClassName].Load(pXmlChild);
        }
    }

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_STRING_ATTRIBUTE("name", m_strClassName, FALSE)
        BKWIN_STRING_ATTRIBUTE("skin", m_strSkinName, FALSE)
        BKWIN_STRING_ATTRIBUTE("scroll_v", m_strScrollV, FALSE)
        BKWIN_STRING_ATTRIBUTE("scroll_h", m_strScrollH, FALSE)
        BKWIN_HEX_ATTRIBUTE("textmode", m_nTextAlign, FALSE)
        BKWIN_COLOR_ATTRIBUTE("crbg", m_crBg, FALSE)
        BKWIN_COLOR_ATTRIBUTE("crbghover", m_crBgHover, FALSE)
        BKWIN_COLOR_ATTRIBUTE("crbgcheck", m_crBgCheck, FALSE)
        BKWIN_COLOR_ATTRIBUTE("crtext", m_crText, FALSE)
        BKWIN_COLOR_ATTRIBUTE("crhover", m_crHoverText, FALSE)
        BKWIN_COLOR_ATTRIBUTE("crpushdown", m_crPushdownText, FALSE)
        BKWIN_COLOR_ATTRIBUTE("crdisabled", m_crDisabledText, FALSE)
        BKWIN_UINT_ATTRIBUTE("alpha", m_alpha, FALSE)
        BKWIN_FONT_ATTRIBUTE("font", m_ftText, FALSE)
        BKWIN_FONT_ATTRIBUTE("hoverfont", m_ftHover, FALSE)
        BKWIN_INT_ATTRIBUTE("x-margin", m_nMarginX, FALSE)
        BKWIN_INT_ATTRIBUTE("y-margin", m_nMarginY, FALSE)
        BKWIN_INT_ATTRIBUTE("margin", m_nMarginX = m_nMarginY, FALSE) // 这样比较bt，不过.....凑合用吧
        BKWIN_INT_ATTRIBUTE("spacing", m_nSpacing, FALSE)
        BKWIN_INT_ATTRIBUTE("linespacing", m_nLineSpacing, FALSE)
        BKWIN_INT_ATTRIBUTE("background", m_bBackground, FALSE)
        BKWIN_INT_ATTRIBUTE("ownDC", m_bOwnDC, FALSE)
        BKWIN_INT_ATTRIBUTE("round", m_nRound, FALSE)
        BKWIN_ENUM_ATTRIBUTE("cursor", LPCTSTR, FALSE)
            BKWIN_ENUM_VALUE("hand", IDC_HAND)
            BKWIN_ENUM_VALUE("arrow", IDC_ARROW)
        BKWIN_ENUM_END(m_lpCursorName)
    BKWIN_DECLARE_ATTRIBUTES_END()
};