#include "SockWnd.h"
#include "../../util/util.h"

CSocketWnd::CSocketWnd()
{
	_InitThread();
}

CSocketWnd::~CSocketWnd()
{

}

void CSocketWnd::_InitThread()
{
	m_thread.Start();
}

BOOL CSocketWnd::RegisterSocket( SOCKET sock,ISocketWinListener* listener )
{
	Util::CAutoCS ac(m_cs);
	m_listeners[sock] = listener;
	return TRUE;
}

VOID CSocketWnd::RemoveSocket( SOCKET sock )
{
	Util::CAutoCS ac(m_cs);
	auto i = m_listeners.find(sock);
	if (i != m_listeners.end())
	{
		m_listeners.erase(i);
	}
}

void CSocketWnd::_AddSocket( SOCKET sock )
{
}
