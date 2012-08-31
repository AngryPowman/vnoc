
#include "AnimateTimer.h"
#include <process.h>

#define ANIMATE_EXIT_TIMEOUT	50
#define ANIMATE_KEEP_TIMEOUT	60 * 1000

CAnimateTimer::CAnimateTimer()
{
	m_hThread = NULL;
	m_hWnd = NULL;
	m_nInterval = 0;
	m_bExit = FALSE;
	m_nTimerTick = -1;
	m_lParam = 0;
}

CAnimateTimer::~CAnimateTimer()
{
	WaitForEnd(ANIMATE_EXIT_TIMEOUT);
}

BOOL CAnimateTimer::SetTimer(HWND hWnd, int nInterval, int lParam /* = 0 */)
{
	m_hWnd = hWnd;
	m_nInterval = nInterval;
	m_lParam = lParam;

	m_nTimerTick = 0;
	m_bExit = FALSE;

	if (!m_hThread)
	{
		m_hThread = (HANDLE)_beginthreadex(0, 0, ThreadProc, (LPVOID)this, 0, NULL);
		return TRUE;
	}

	return FALSE;
}

BOOL CAnimateTimer::KillTimer()
{
	m_nTimerTick = -1;
	return TRUE;
}

BOOL CAnimateTimer::WaitForEnd(DWORD nTimeout /* = INFINITE */)
{
	m_bExit = TRUE;
	if (m_hThread)
	{
		if (WaitForSingleObject(m_hThread, nTimeout) == WAIT_TIMEOUT)
		{
			TerminateThread(m_hThread, -1);
			CloseHandle(m_hThread);
			m_hThread = NULL;
		}
	}

	return TRUE;
}

BOOL CAnimateTimer::IsRunning(BOOL bOnlyCheckTimer /* = FALSE */)
{
	if (m_hThread)
	{
		if (!bOnlyCheckTimer || m_nTimerTick != -1)
			return TRUE;
	}

	return FALSE;
}

UINT __stdcall CAnimateTimer::ThreadProc(LPVOID lpVoid)
{
	CAnimateTimer* pThis = (CAnimateTimer*)lpVoid;

	while (!pThis->m_bExit)
	{
		if (!pThis->m_nTimerTick)
		{
			while (!pThis->m_bExit && pThis->m_nTimerTick != -1)
			{
				int nLastActual = (int)GetTickCount();
				if (!SendMessage(pThis->m_hWnd, WM_MAIN_ANIMATETIMER, 0, pThis->m_lParam))
					break;
				nLastActual = GetTickCount() - nLastActual;
				if (pThis->m_nInterval > nLastActual)
					Sleep(pThis->m_nInterval - nLastActual);
				else
					Sleep(1);
			}
			if (pThis->m_nTimerTick != -1)
				pThis->m_nTimerTick = GetTickCount();
		}

		if (!pThis->m_nTimerTick)
			pThis->m_nTimerTick = GetTickCount();
		else if (GetTickCount() - pThis->m_nTimerTick >= ANIMATE_KEEP_TIMEOUT)
			break;

		Sleep(50);
	}

	CloseHandle(pThis->m_hThread);
	pThis->m_hThread = NULL;

	return 0;
}