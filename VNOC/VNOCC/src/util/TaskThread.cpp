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
		m_performance.MileStone(_T("����"));
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
		m_performance.MileStone(_T("��ʼ����"));
		m_bAlive = FALSE;
		m_newTaskAvaliable.Set();
		WaitForSingleObject(m_hThread,10000);
		CloseHandle(m_hThread);
		m_performance.MileStone(_T("�������"));
	}

	VOID CTaskThread::OnThreadQuit()
	{
		m_bAlive = FALSE;	// �����ϵ�����϶��Ѿ���FALSE��
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
				{	// �����������
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
					p->Run();	//ִ������
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