//////////////////////////////////////////////////////////////////////////
//  Class Name: CBkDlgView
// Description: Dialog View, Real Container of BkWindow
//     Creator: ZhangXiaoxuan
//     Version: 2010.3.12 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <atlconv.h>
#include "bkwndpanel.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// BkWinManager
// CBkDialogViewTraits
// CBkDialogViewImpl

//////////////////////////////////////////////////////////////////////////
// BkWinManager

class BkWinManager
{
public:
    BkWinManager()
    {
        if (NULL == ms_mangager)
            ms_mangager = this;

        ::InitializeCriticalSection(&m_lockRepaint);
		::InitializeCriticalSection(&m_lockMapActive);
    }

    virtual ~BkWinManager()
    {
        if (this == ms_mangager)
            ms_mangager = NULL;

        ::DeleteCriticalSection(&m_lockRepaint);
		::DeleteCriticalSection(&m_lockMapActive);
   }

    static HWND SetActive(HWND hWnd)
    {
        HWND hWndLastActive = NULL;

        if (ms_mangager)
        {
            return ms_mangager->_SetActive(hWnd);
        }

        return hWndLastActive;
    }

    static HWND GetActive()
    {
        if (ms_mangager)
			return ms_mangager->_GetActive();

        return NULL;
    }

    static void EnterPaintLock()
    {
        if (ms_mangager)
            ::EnterCriticalSection(&(ms_mangager->m_lockRepaint));
    }

    static void LeavePaintLock()
    {
        if (ms_mangager)
            ::LeaveCriticalSection(&(ms_mangager->m_lockRepaint));
    }

protected:
	HWND _SetActive(HWND hWnd)
	{
		::EnterCriticalSection(&m_lockMapActive);

		HWND	hWndLastActive	= NULL;
		DWORD	nCurTID			= ::GetCurrentThreadId();

		CAtlMap<DWORD,HWND>::CPair*	 pairx = m_mapHwnd.Lookup( nCurTID );
		if (pairx)
		{
			hWndLastActive = pairx->m_value;
		}
		m_mapHwnd.SetAt(nCurTID,hWnd);

		::LeaveCriticalSection(&m_lockMapActive);

		return hWndLastActive;
	}

	HWND _GetActive()
	{
		::EnterCriticalSection(&m_lockMapActive);

		HWND	hWndAct = NULL;
		CAtlMap<DWORD,HWND>::CPair*	 pairx = m_mapHwnd.Lookup( ::GetCurrentThreadId() );
		if (pairx)
		{
			hWndAct = pairx->m_value;
		}
		::LeaveCriticalSection(&m_lockMapActive);
		return hWndAct;
	}

protected:
    static BkWinManager*	ms_mangager;

	CRITICAL_SECTION		m_lockMapActive;
	CAtlMap<DWORD,HWND>		m_mapHwnd;
    CRITICAL_SECTION		m_lockRepaint;
};

__declspec(selectany) BkWinManager* BkWinManager::ms_mangager = NULL;
