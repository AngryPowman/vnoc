inline CBkEffectRenderer::CBkEffectRenderer()
{
}

inline CBkEffectRenderer::~CBkEffectRenderer()
{
    ClearEffects();
}

inline CBkEffectRenderer* CBkEffectRenderer::Instance()
{
    static CBkEffectRenderer instance;
    return &instance;
}

inline void CBkEffectRenderer::AddEffect(IBkEffectObject* obj)
{
    EffectObjListIter iter = find( m_lstEffectObjs.begin(), m_lstEffectObjs.end(), obj);
    if (iter == m_lstEffectObjs.end())
    {
        m_lstEffectObjs.push_back(obj);
    }
}

inline void CBkEffectRenderer::RemoveEffect(IBkEffectObject* obj, BOOL bDestroy/* = FALSE*/)
{
    EffectObjListIter iter = find( m_lstEffectObjs.begin(), m_lstEffectObjs.end(), obj);
    if (iter != m_lstEffectObjs.end())
    {
        m_lstEffectObjs.erase(iter);
        if (bDestroy)
        {
            delete (*iter);
            (*iter) = NULL;
        }
    }
}

inline void CBkEffectRenderer::ClearEffects()
{
    for (EffectObjListIter iter = m_lstEffectObjs.begin();
         iter != m_lstEffectObjs.end();
         ++iter)
    {
        IBkEffectObject* pEffectObj = (*iter);
        if (pEffectObj)
        {
            delete pEffectObj;
            pEffectObj = NULL;
        }
    }
    m_lstEffectObjs.clear();
}

inline void CBkEffectRenderer::PlayEffect(IBkEffectObject* obj)
{
    if (obj)
    {
        obj->Play();
    }
}

inline void CBkEffectRenderer::StopEffect(IBkEffectObject* obj)
{
    if (obj)
    {
        obj->Stop();
    }
}

inline BOOL CBkEffectRenderer::IsEffects()
{
    return (m_lstEffectObjs.size()>0)?TRUE:FALSE;
}

inline void CBkEffectRenderer::RenderEffectObjects(HWND hWnd, CDCHandle& dc, const CRect& rc)
{
    //Gdiplus::Rect rtClip(rc.left, rc.top, rc.Width(), rc.Height() );
    //Gdiplus::Graphics g(dc.m_hDC);
    //Gdiplus::Region reg;
    //g.GetClip(&reg);
    //g.SetClip(rtClip);

    CRgn rgn;
    rgn.CreateRectRgnIndirect(rc);
    CRgn rgn_old;
    dc.GetClipRgn(rgn_old);
    dc.SelectClipRgn(rgn);

    for (EffectObjListIter iter = m_lstEffectObjs.begin();
        iter != m_lstEffectObjs.end();
        ++iter)
    {
        IBkEffectObject* pEffectObj = (*iter);
        if (pEffectObj)
        {
            pEffectObj->Update(hWnd, dc);
        }
    }
    dc.SelectClipRgn(rgn_old);
    //g.SetClip(&reg);
}

inline void CBkEffectRenderer::RenderEffectObjects(HWND hWnd, CDCHandle& dc)
{
    for (EffectObjListIter iter = m_lstEffectObjs.begin();
        iter != m_lstEffectObjs.end();
        ++iter)
    {
        IBkEffectObject* pEffectObj = (*iter);
        if (pEffectObj)
        {
            pEffectObj->Update(hWnd, dc);
        }
    }
}

