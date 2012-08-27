//////////////////////////////////////////////////////////////////////////
// CBkWnd Handle Manager
//////////////////////////////////////////////////////////////////////////
#ifndef _BK_WND_MANAGER_H_
#define _BK_WND_MANAGER_H_
#pragma once

class BkWnds
{
public:
    static CBkWnd*  GetWindow(HBKWND hBkWnd);
    static HBKWND   NewWindow(CBkWnd *pBkWnd);
    static BOOL     DestroyWindow(HBKWND hBkWnd);
    static size_t   GetCount();

private:
    BkWnds();
    ~BkWnds();
    static BkWnds* _Instance();

    typedef CAtlMap<HBKWND, CBkWnd *> _BkWndHandlePool;

    CRITICAL_SECTION        ms_lockWndMap;
    HBKWND                  m_hNextWnd;
    _BkWndHandlePool        m_mapPool;
};


//////////////////////////////////////////////////////////////////////////
// implement
inline BkWnds::BkWnds() : m_hNextWnd(NULL)
{
    ::InitializeCriticalSection(&ms_lockWndMap);
}

inline BkWnds::~BkWnds()
{
    ::DeleteCriticalSection(&ms_lockWndMap);
}

inline CBkWnd* BkWnds::GetWindow(HBKWND hBkWnd)
{
    ::EnterCriticalSection(&_Instance()->ms_lockWndMap);
    const _BkWndHandlePool::CPair *pairRet = _Instance()->m_mapPool.Lookup(hBkWnd);
    ::LeaveCriticalSection(&_Instance()->ms_lockWndMap);
    if (pairRet)
    {
        return pairRet->m_value;
    }
    else
        return NULL;
}

inline HBKWND BkWnds::NewWindow(CBkWnd *pBkWnd)
{
    if (pBkWnd)
    {
        _Instance();
        ::EnterCriticalSection(&_Instance()->ms_lockWndMap);
        HBKWND hBkWndNext = ++ _Instance()->m_hNextWnd;
        _Instance()->m_mapPool[hBkWndNext] = pBkWnd;
        ::LeaveCriticalSection(&_Instance()->ms_lockWndMap);
        return hBkWndNext;
    }
    else
        return NULL;
}

inline BOOL BkWnds::DestroyWindow(HBKWND hBkWnd)
{
    ::EnterCriticalSection(&_Instance()->ms_lockWndMap);
    BOOL bRet = (BOOL)_Instance()->m_mapPool.RemoveKey(hBkWnd);
    ::LeaveCriticalSection(&_Instance()->ms_lockWndMap);
    return bRet;
}

inline size_t BkWnds::GetCount()
{
    ::EnterCriticalSection(&_Instance()->ms_lockWndMap);
    size_t nRet = _Instance()->m_mapPool.GetCount();
    ::LeaveCriticalSection(&_Instance()->ms_lockWndMap);
    return nRet;
}

inline BkWnds* BkWnds::_Instance()
{
	static BkWnds instance;
	return &instance;
}

#endif // _BK_WND_MANAGER_H_
