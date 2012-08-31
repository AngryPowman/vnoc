#pragma once

#include "bkEffectObject.h"

#include <vector>
#include <algorithm>
using namespace std;

//////////////////////////////////////////////////////////////////////////
//   File Name: BkEffectRenderer.h
// Description: 动态特效渲染器
//              用于进行动态效果的绘制
//     Creator: YangDa
//     Version: 2011.09.07 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

class CBkEffectRenderer : public IBkEffectRenderer
{
public:
    CBkEffectRenderer();
    virtual ~CBkEffectRenderer();

    static CBkEffectRenderer* Instance();

    void    AddEffect(IBkEffectObject* obj);
    void    RemoveEffect(IBkEffectObject* obj, BOOL bDestroy = FALSE);
    void    ClearEffects();

    void    PlayEffect(IBkEffectObject* obj);
    void    StopEffect(IBkEffectObject* obj);
    BOOL    IsEffects();
    void    RenderEffectObjects(HWND hWnd, CDCHandle& dc, const CRect& rc);
    void    RenderEffectObjects(HWND hWnd, CDCHandle& dc);

protected:
    typedef vector<IBkEffectObject*>            EffectObjList;
    typedef vector<IBkEffectObject*>::iterator  EffectObjListIter;

    EffectObjList    m_lstEffectObjs;
};

#include "bkEffectRenderer.inl"