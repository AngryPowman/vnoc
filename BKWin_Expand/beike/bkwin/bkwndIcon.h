//////////////////////////////////////////////////////////////////////////
//   File Name: bkIcon.h
// Description: Icon Mangaer
//     Creator: DongM
//     Version: 2011.06.21 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "bkwnd.h"
class BkIconPool
{
public:
    typedef CAtlMap<CStringA, HICON> _TypeIconPool;
    static HICON Get(LPCSTR szRes)
    {
        HICON hIcon = NULL;

        _TypeIconPool::CPair* pPairRet = _Instance()->m_mapIcon.Lookup(szRes);
        if (NULL == pPairRet)
        {
            hIcon = _Instance()->GetIconFromFile(szRes);
            if (hIcon)
                _Instance()->m_mapIcon[szRes] = hIcon;
        }
        else
            hIcon = pPairRet->m_value;

        return hIcon;
    }
    ~BkIconPool(){}

private:
    static BkIconPool* _Instance()
    {
		static BkIconPool instance;
		return &instance;
    }

    HICON GetIconFromFile(LPCSTR szRes)
    {
        HICON hIcon = NULL;
        if (::PathFileExistsA(szRes))
        {
            SHFILEINFOA fi = {0};
            if (::SHGetFileInfoA(szRes, 0, &fi, sizeof(fi), SHGFI_ICON | SHGFI_LARGEICON |SHGFI_DISPLAYNAME))
            {
                hIcon = fi.hIcon;
            }
        }
        return hIcon;
    }

    _TypeIconPool       m_mapIcon;
    BkIconPool() {}
};

class CBkIcon
    : public CBkWnd
    , public IBkIcon
{
    BKOBJ_DECLARE_CLASS_NAME(CBkIcon, "icon")
public:
    CBkIcon();
    ~CBkIcon();

    virtual IBkWnd* MakeCopy();
    virtual BOOL OnPaint(CDCHandle dc);
    virtual BOOL OnNcCalcSize(LPSIZE pSize);
    virtual void SetSrc(LPCWSTR szSrc);
    virtual void Attach(HICON hIcon, BOOL bManaged);

protected:
    CBkIcon(const CBkIcon& bkIcon);
    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_ICON_ATTRIBUTE("src", m_hIcon, FALSE)
        BKWIN_INT_ATTRIBUTE("size", m_nSize, FALSE)
    BKWIN_DECLARE_ATTRIBUTES_END()

private:
    void FreeIcon();

protected:
    HICON   m_hIcon;
    BOOL    m_bManaged;
    int     m_nSize;
};

inline CBkIcon::CBkIcon()
{
    m_bManaged = FALSE;
    m_hIcon = NULL;
    m_nSize = 16;
}

inline CBkIcon::CBkIcon(const CBkIcon& bkIcon) : CBkWnd(bkIcon)
{
    m_bManaged = bkIcon.m_bManaged;
    m_hIcon = bkIcon.m_hIcon;
    m_nSize = bkIcon.m_nSize;
}

inline CBkIcon::~CBkIcon()
{
    FreeIcon();
}

inline IBkWnd* CBkIcon::MakeCopy()
{
    return new CBkIcon(*this);
}

inline BOOL CBkIcon::OnPaint(CDCHandle dc)
{
    if (NULL != m_hIcon)
    {
        CIconHandle ico(m_hIcon);
        ico.DrawIconEx(dc, m_rcWindow.TopLeft(), m_rcWindow.Size());
    }

    return FALSE;
}

inline BOOL CBkIcon::OnNcCalcSize(LPSIZE pSize)
{
    pSize->cx = m_nSize;
    pSize->cy = m_nSize;
    return FALSE;
}

inline void CBkIcon::SetSrc(LPCWSTR szSrc)
{
    FreeIcon();
    m_bManaged = FALSE;
    m_hIcon = BkIconPool::Get(CT2A(szSrc));
    NotifyInvalidate();
}

inline void CBkIcon::Attach(HICON hIcon, BOOL bManaged)
{
    FreeIcon();
    m_bManaged = bManaged;
    m_hIcon = hIcon;
    NotifyInvalidate();
}

inline void CBkIcon::FreeIcon()
{
    if (m_bManaged)
    {
        ::DestroyIcon(m_hIcon);
        m_hIcon = NULL;
    }
}