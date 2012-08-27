#pragma once
#include "bkEffectObject.h"

//////////////////////////////////////////////////////////////////////////
//   File Name: BkEffect.h
// Description: 动态特效管理器
//              用于读取管理动态特效
//     Creator: YangDa
//     Version: 2011.09.12 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

class BkEffect
{
private:
    BkEffect();

public:
    virtual ~BkEffect();

    static BkEffect*           Instance();

    static CBkEffectObject*    GetEffect(LPCSTR lpszEffectName);
    static CBkEffectObject*    GetEffect(UINT nEffectID);
    static void                PlayEffect(LPCSTR lpszEffectName, EffectActionInfo info);
    static void                StopEffect(LPCSTR lpszEffectName);
    static void                StopEffects();
    static BOOL                LoadEffects(LPCTSTR szRes);

protected:
    void                _LoadEffects(TiXmlElement *pXmlEffectRootElem);
    void                _StopEffects();
    void                _ClearEffects();
    CBkEffectObject*    _CreateEffectByName(LPCSTR lpszName);

    typedef CAtlMap<CStringA, CBkEffectObject*> __BkEffectPool;

    __BkEffectPool    m_mapPool;
    UINT              m_nEffectID;
};

#include "bkEffect.inl"