#include "TaskThread.h"

VOID _BackendTaskThreadEntry( LPVOID param )
{
	Thread::CTaskThread* p=static_cast<Thread::CTaskThread*>(param);
	if (p)
	{
		p->ThreadPrepare();
		p->ThreadMain();
	}
	p->OnThreadQuit();
}


namespace Thread
{
	CTaskThread::CTaskThread( BOOL bStartNow ) : m_taskListLock(3000)
	{
		m_bAlive = FALSE;
		m_bForceQuit = FALSE;
		m_bPaused = TRUE;
		m_hThread = 0;
		m_newTaskAvaliable.Create(NULL,FALSE,FALSE,NULL);
		if (bStartNow)
		{
			Start();
		}
		m_performance.MileStone(_T("构造"));
	}

	VOID CTaskThread::Start()
	{
		if (!IsAlive())
		{
			_beginthread(_BackendTaskThreadEntry,0,this);
		}
	}

	CTaskThread::~CTaskThread( VOID )
	{
		m_performance.MileStone(_T("开始析构"));
		m_bAlive = FALSE;
		m_newTaskAvaliable.Set();
		WaitForSingleObject(m_hThread,10000);
		CloseHandle(m_hThread);
		m_performance.MileStone(_T("析构完成"));
	}

	VOID CTaskThread::OnThreadQuit()
	{
		m_bAlive = FALSE;	// 理论上到这里肯定已经是FALSE了
	}

	VOID CTaskThread::PostEmergentTask( CTask * pTask )
	{
		Util::CAutoTimedCS Guard(m_taskListLock);
		if (Guard.IsTimedOut())
		{
			return;
		}
		m_taskList.push_front(pTask);
		m_newTaskAvaliable.Set();
	}

	VOID CTaskThread::PostTask( Thread::CTask * pTask )
	{
		Util::CAutoTimedCS Guard(m_taskListLock);
		if (Guard.IsTimedOut())
		{
			return;
		}
		m_taskList.push_back(pTask);
		m_newTaskAvaliable.Set();
	}

	VOID CTaskThread::ThreadPrepare()
	{
		m_hThread = GetCurrentThread();
		m_bAlive = TRUE;
		m_bPaused = FALSE;
	}

	VOID CTaskThread::ThreadMain()
	{
		BOOL bQuit;
		Thread::CTask *p;
		TaskList::iterator i;
		do 
		{
			bQuit = FALSE;
			while(!bQuit)
			{
				p = NULL;
				m_taskListLock.Enter();
				i = m_taskList.begin();
				if (i == m_taskList.end())
				{	// 若任务处理光了
					bQuit = TRUE;
				}
				else
				{
					p = *i;
					m_taskList.pop_front();
				}
				m_taskListLock.Leave();
				if (p)
				{
					p->Run();	//执行任务
					delete p;
				}
			}
			WaitForSingleObject(m_newTaskAvaliable,INFINITE);
			while (m_bPaused)
			{
				WaitForSingleObject(m_newTaskAvaliable,INFINITE);
			}
			if (!m_bAlive)
			{
				if (m_bForceQuit)
				{
					break;
				}
			}
		} while (1);
	}

	INT CTaskThread::WaitingTaskNumber()
	{
		return static_cast<INT>(m_taskList.size());
	}

	VOID CTaskThread::Pause()
	{
		m_bPaused = TRUE;
	}

	VOID CTaskThread::Resume()
	{
		m_bPaused = FALSE;
		m_newTaskAvaliable.Set();
	}



}

//////////////////////////////////////////////////////////////////////////

namespace Thread
{
	VOID _BackendTaskWinThreadEntry( LPVOID param )
	{
		Thread::CTaskWinThread* p=static_cast<Thread::CTaskWinThread*>(param);
		if (p)
		{
			p->ThreadPrepare();
			p->ThreadMain();
		}
		p->OnThreadQuit();
	}

	CTaskWinThread::CTaskWinThread( BOOL bStartNow/*=TRUE*/ ) : m_taskListLock(3000)
	{
		m_bAlive = FALSE;
		m_bForceQuit = FALSE;
		m_bPaused = TRUE;
		m_hThread = 0;
		m_newTaskAvaliable.Create(NULL,FALSE,FALSE,NULL);
		if (bStartNow)
		{
			Start();
		}
		m_performance.MileStone(_T("构造"));
	}

	CTaskWinThread::~CTaskWinThread( VOID )
	{
		m_performance.MileStone(_T("开始析构"));
		m_bAlive = FALSE;
		m_newTaskAvaliable.Set();
		WaitForSingleObject(m_hThread,10000);
		CloseHandle(m_hThread);
		m_performance.MileStone(_T("析构完成"));
	}

	VOID CTaskWinThread::Start()
	{
		if (!IsAlive())
		{
			_beginthread(_BackendTaskThreadEntry,0,this);
		}
	}

	VOID CTaskWinThread::Pause()
	{
		m_bPaused = TRUE;
	}

	VOID CTaskWinThread::Resume()
	{
		m_bPaused = FALSE;
		m_newTaskAvaliable.Set();
	}

	VOID CTaskWinThread::PostTask( CTask * pTask )
	{
		Util::CAutoTimedCS Guard(m_taskListLock);
		if (Guard.IsTimedOut())
		{
			return;
		}
		m_taskList.push_back(pTask);
		m_newTaskAvaliable.Set();
	}

	INT CTaskWinThread::WaitingTaskNumber()
	{
		return static_cast<INT>(m_taskList.size());
	}

	VOID CTaskWinThread::OnThreadQuit()
	{
		m_bAlive = FALSE;	// 理论上到这里肯定已经是FALSE了
	}

	VOID CTaskWinThread::ThreadPrepare()
	{
		m_hThread = GetCurrentThread();
		m_bAlive = TRUE;
		m_bPaused = FALSE;
	}

	VOID CTaskWinThread::PostEmergentTask( CTask * pTask )
	{
		Util::CAutoTimedCS Guard(m_taskListLock);
		if (Guard.IsTimedOut())
		{
			return;
		}
		m_taskList.push_front(pTask);
		m_newTaskAvaliable.Set();
	}

	VOID CTaskWinThread::ThreadMain()
	{
		m_hWnd = CreateWindow(_T(""),_T(""),WS_OVERLAPPED,
			0,0,0,0,HWND_MESSAGE,0,0,0);
		if (m_hWnd == INVALID_HANDLE_VALUE)
		{
			ATLASSERT(FALSE && "Create SocketWindow Fail");
		}
		MessageBox(NULL,_T("ddd"),0,0);
	}

}
