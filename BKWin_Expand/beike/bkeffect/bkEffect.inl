#include "bkEffectMove.h"
#include "bkEffectTurnPage.h"
#include "bkEffectTurnPage2.h"
#include "bkEffectScroll.h"
#include "bkEffectSprite.h"
#include "bkEffectRealMove.h"
#include "bkEffectPositionFrame.h"

inline BkEffect::BkEffect()
:m_nEffectID(0)
{

}

inline BkEffect::~BkEffect()
{
    _ClearEffects();
}

inline BkEffect* BkEffect::Instance()
{
    static BkEffect instance;
    return &instance;
}

inline CBkEffectObject* BkEffect::GetEffect(LPCSTR lpszEffectName)
{
    __BkEffectPool::CPair *pairRet = Instance()->m_mapPool.Lookup(lpszEffectName);

    if (pairRet)
        return pairRet->m_value;
    else
        return NULL;
}

inline CBkEffectObject* BkEffect::GetEffect(UINT nEffectID)
{
    POSITION pos = Instance()->m_mapPool.GetStartPosition();
    while (pos)
    {
        __BkEffectPool::CPair *pairRet = Instance()->m_mapPool.GetNext(pos);
        if (pairRet)
        {
            if (pairRet->m_value && pairRet->m_value->GetID() == nEffectID)
            {
                return pairRet->m_value;
            }
        }
    }
    return NULL;
}

inline void BkEffect::PlayEffect(LPCSTR lpszEffectName, EffectActionInfo info)
{
    CBkEffectObject* pEffect = GetEffect(lpszEffectName);
    if (pEffect)
    {
        pEffect->Init(info);
        pEffect->Play();
    }
}

inline void BkEffect::StopEffect(LPCSTR lpszEffectName)
{
    CBkEffectObject* pEffect = GetEffect(lpszEffectName);
    if (pEffect)
    {
        pEffect->Stop();
    }
}

inline void BkEffect::StopEffects()
{
    Instance()->_StopEffects();
}

inline BOOL BkEffect::LoadEffects(LPCTSTR szRes)
{
    Instance()->_ClearEffects();

    CStringA strXml;
    BOOL bRet = BkResManager::LoadResource(szRes, strXml);
    if (!bRet)
        return FALSE;

    TiXmlDocument xmlDoc;
    xmlDoc.Parse(strXml, NULL, TIXML_ENCODING_UTF8);
    if (xmlDoc.Error())
        return FALSE;

    Instance()->_LoadEffects(xmlDoc.RootElement());
    return TRUE;
}

inline void BkEffect::_LoadEffects(TiXmlElement *pXmlEffectRootElem)
{
    LPCSTR lpszEffectName = NULL, lpszTypeName = NULL;

    if (!pXmlEffectRootElem)
        return;

    if (strcmp(pXmlEffectRootElem->Value(), "effects") != 0)
        return;

    for (TiXmlElement* pXmlChild = pXmlEffectRootElem->FirstChildElement(); NULL != pXmlChild; pXmlChild = pXmlChild->NextSiblingElement())
    {
        lpszEffectName = pXmlChild->Attribute("name");
        lpszTypeName = pXmlChild->Value();
        if (!lpszEffectName || !lpszTypeName)
            continue;

        CBkEffectObject* pEffect = _CreateEffectByName(lpszTypeName);
        if (!pEffect)
            continue;

        pEffect->SetID(++m_nEffectID);
        pEffect->Load(pXmlChild);
        pEffect->Init();
        m_mapPool[lpszEffectName] = pEffect;
    }
}

inline void BkEffect::_StopEffects()
{
    POSITION pos = m_mapPool.GetStartPosition();

    while (pos)
    {
        CBkEffectObject* pSkin = m_mapPool.GetNextValue(pos);
        pSkin->Stop();
    }
}

inline void BkEffect::_ClearEffects()
{
    POSITION pos = m_mapPool.GetStartPosition();

    while (pos)
    {
        IBkEffectObject* pSkin = m_mapPool.GetNextValue(pos);
        pSkin->Release();
        delete pSkin;
    }

    m_mapPool.RemoveAll();
    m_nEffectID = 0;
}

inline CBkEffectObject* BkEffect::_CreateEffectByName(LPCSTR lpszName)
{
    CBkEffectObject* pNewEffect = NULL;

    pNewEffect = CBkEffectMove::CheckAndNew(lpszName);
    if (pNewEffect)
        return pNewEffect;

    pNewEffect = CBkEffectTurnPage::CheckAndNew(lpszName);
    if (pNewEffect)
        return pNewEffect;

    pNewEffect = CBkEffectTurnPage2::CheckAndNew(lpszName);
    if (pNewEffect)
        return pNewEffect;

    pNewEffect = CBkEffectScroll::CheckAndNew(lpszName);
    if (pNewEffect)
        return pNewEffect;

    pNewEffect = CBkEffectSprite::CheckAndNew(lpszName);
    if (pNewEffect)
        return pNewEffect;
	pNewEffect = CBkEffectRealMove::CheckAndNew(lpszName);
	if (pNewEffect)
		return pNewEffect;
	pNewEffect = CBkEffectPositionFrame::CheckAndNew(lpszName);
	if (pNewEffect)
		return pNewEffect;

    return NULL;
}