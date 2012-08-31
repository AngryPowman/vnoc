#pragma once

#include "windows.h"

#define WM_MAIN_ANIMATETIMER		(WM_USER + 2133)

class CAnimateTimer
{
public:
	CAnimateTimer();
	~CAnimateTimer();

public:
	BOOL SetTimer(HWND hWnd, int nInterval, int lParam = 0);
	BOOL KillTimer();
	BOOL WaitForEnd(DWORD nTimeout = INFINITE);
	BOOL IsRunning(BOOL bOnlyCheckTimer = FALSE);

protected:
	static UINT __stdcall ThreadProc(LPVOID lpVoid);

private:
	HANDLE	m_hThread;
	BOOL	m_bExit;

	HWND	m_hWnd;
	int		m_nInterval;
	DWORD	m_lParam;
	int		m_nTimerTick;
};
