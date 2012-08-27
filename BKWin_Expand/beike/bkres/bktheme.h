#pragma once
#include "bkresutil.h"
#include "bkstringpool.h"
#include "bkskin.h"
#include "../bkwin/bkwndstyle.h"
#include "../bkeffect/bkEffect.h"

class BkTheme
{
public:
    static void SetSkinPath( LPCTSTR lpszPath )
    {
        BkResManager::SetResourcePath( lpszPath );
        _Instance()->m_strResourcePath = lpszPath;
    }

    static void Load( LPCTSTR lpszStringFile,LPCTSTR lpszSkinFile,LPCTSTR lpszStyleFile,LPCTSTR lpszEffectFile )
    {
        BkString::Load( lpszStringFile );
        BkSkin::LoadSkins( lpszSkinFile );
        BkStyle::LoadStyles( lpszStyleFile );
        BkEffect::LoadEffects( lpszEffectFile );

        _Instance()->m_strStringFile = lpszStringFile;
        _Instance()->m_strSkinFile = lpszSkinFile;
        _Instance()->m_strStyleFile = lpszStyleFile;
    }

protected:
    static BkTheme* _Instance()
    {
        static BkTheme instance;
        return &instance;
    }

protected:
    CString m_strResourcePath;
    CString m_strStringFile;
    CString m_strSkinFile;
    CString m_strStyleFile;
};