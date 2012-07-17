#pragma once
#include "ThreadDefine.h"
#include "queue"
#include "list"
#include "atlstr.h"
#include "util.h"
#include "Performance.h"

VOID _BackendTaskThreadEntry(LPVOID param);

namespace Thread
{
	typedef std::list<CTask*>	TaskList;

	class CTaskThread
	{
	public:
		CTaskThread(BOOL bStartNow=TRUE);
		~CTaskThread(VOID);

	public:
		VOID Start();
		VOID Pause();
		VOID Resume();
		VOID SetForceQuit(BOOL bForceSuspendThreadWhenDestruct){m_bForceQuit = bForceSuspendThreadWhenDestruct;};
		HANDLE GetThreadHandle() const { return m_hThread; }
		BOOL IsAlive() const { return m_bAlive; }

		// 请保证线程已经被启动！(构造函数参数true，或调用过Start)
		VOID PostEmergentTask(CTask * pTask);
		VOID PostTask(CTask * pTask);
		INT WaitingTaskNumber();

	_private:
		VOID OnThreadQuit();
		VOID ThreadPrepare();
		VOID ThreadMain();
	private:
		Util::CTimedCriticalSection m_taskListLock;
		volatile BOOL m_bAlive;
		volatile BOOL m_bForceQuit;
		volatile BOOL m_bPaused;
		HANDLE m_hThread;
		ATL::CEvent m_newTaskAvaliable;
		TaskList m_taskList;

		Performance::CPerformance m_performance;
	private:
		DISALLOW_COPY_AND_ASSIGN(CTaskThread);
	};
}

//TODO: 这个类应当是CTaskThread的一个扩展，在无任务时处理消息循环，有任务时则先处理任务

namespace Thread
{
	typedef std::list<CTask*>	TaskList;

	class CTaskWinThread
	{
	public:
		CTaskWinThread(BOOL bStartNow=TRUE);
		~CTaskWinThread(VOID);

	public:
		VOID Start();
		VOID Pause();
		VOID Resume();
		VOID SetForceQuit(BOOL bForceSuspendThreadWhenDestruct){m_bForceQuit = bForceSuspendThreadWhenDestruct;};
		HANDLE GetThreadHandle() const { return m_hThread; }
		BOOL IsAlive() const { return m_bAlive; }

		// 请保证线程已经被启动！(构造函数参数true，或调用过Start)
		VOID PostTask(CTask * pTask);
		VOID PostEmergentTask(CTask * pTask);
		INT WaitingTaskNumber();

	_private:
		VOID OnThreadQuit();
		VOID ThreadPrepare();
		VOID ThreadMain();
	private:
		Util::CTimedCriticalSection m_taskListLock;
		volatile BOOL m_bAlive;
		volatile BOOL m_bForceQuit;
		volatile BOOL m_bPaused;
		HANDLE	m_hThread;
		HWND	m_hWnd;
		ATL::CEvent m_newTaskAvaliable;
		TaskList m_taskList;

		Performance::CPerformance m_performance;
	private:
		DISALLOW_COPY_AND_ASSIGN(CTaskWinThread);
	};
}