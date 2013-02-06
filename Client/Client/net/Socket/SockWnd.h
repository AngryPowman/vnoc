#include <Windows.h>
#include <atlsync.h>
#include <map>
#include "../../util/TaskThread.h"

interface ISocketWinListener
{
public:
	;
};

class CSocketWnd
{
public:
	CSocketWnd();
	~CSocketWnd();
public:
	BOOL RegisterSocket(SOCKET sock,ISocketWinListener* listener);
	VOID RemoveSocket(SOCKET sock);
private:
	void _InitThread();
	void _AddSocket(SOCKET sock);
private:
	typedef std::map<SOCKET,ISocketWinListener*>	SocketListenerMap;
	ATL::CCriticalSection m_cs;
	Thread::CTaskWinThread m_thread;
	SocketListenerMap	m_listeners;
};