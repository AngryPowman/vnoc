#pragma once
#include "ThreadDefine.h"
#include "queue"
#include "list"
#include "atlstr.h"
#include "util.h"

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
	private:
		DISALLOW_COPY_AND_ASSIGN(CTaskThread);
	};
}