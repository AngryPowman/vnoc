#include "AsyncSocket.h"
#include <atlbase.h>
#pragma comment(lib,"ws2_32.lib")

#define VERIFY(_i)	ATLASSERT(_i)

void CAsyncSocket::OnReceive( int nErrorCode )
{

}

void CAsyncSocket::OnSend( int nErrorCode )
{

}

void CAsyncSocket::OnOutOfBandData( int nErrorCode )
{

}

void CAsyncSocket::OnConnect( int nErrorCode )
{

}

void CAsyncSocket::OnAccept( int nErrorCode )
{

}

void CAsyncSocket::OnClose( int nErrorCode )
{

}

CAsyncSocket::CAsyncSocket()
{
	m_hSocket = INVALID_SOCKET;
}

BOOL CAsyncSocket::Create( UINT nSocketPort /*= 0*/, int nSocketType/*=SOCK_STREAM*/, long lEvent /*= FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE*/, LPCTSTR lpszSocketAddress /*= NULL*/ )
{
	if (Socket(nSocketType, lEvent))
	{
		if (Bind(nSocketPort,lpszSocketAddress))
			return TRUE;
		int nResult = GetLastError();
		Close();
		WSASetLastError(nResult);
	}
	return FALSE;
}

BOOL CAsyncSocket::Attach( SOCKET hSocket, long lEvent /*= FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE*/ )
{
	ATLASSERT(hSocket != INVALID_SOCKET); 

	if (hSocket == INVALID_SOCKET)
	{
		WSASetLastError (WSA_INVALID_HANDLE);
		return FALSE;
	}

	m_hSocket = hSocket;
	CAsyncSocket::AttachHandle(hSocket, this);

	return AsyncSelect(lEvent);
}

SOCKET CAsyncSocket::Detach()
{
	SOCKET hSocket = m_hSocket;
	if (AsyncSelect(0))
	{
		CAsyncSocket::KillSocket(hSocket, this);
		m_hSocket = INVALID_SOCKET;
		return hSocket;
	}
	return INVALID_SOCKET;
}

BOOL CAsyncSocket::GetPeerName( CString& rPeerAddress, UINT& rPeerPort )
{
	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));

	int nSockAddrLen = sizeof(sockAddr);
	BOOL bResult = GetPeerName((SOCKADDR*)&sockAddr, &nSockAddrLen);
	if (bResult)
	{
		rPeerPort = ntohs(sockAddr.sin_port);
		rPeerAddress = inet_ntoa(sockAddr.sin_addr);
	}
	return bResult;
}

BOOL CAsyncSocket::GetPeerName( SOCKADDR* lpSockAddr, int* lpSockAddrLen )
{
	return (SOCKET_ERROR != getpeername(m_hSocket, lpSockAddr, lpSockAddrLen)); 
}

BOOL CAsyncSocket::GetSockName( CString& rSocketAddress, UINT& rSocketPort )
{
	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));

	int nSockAddrLen = sizeof(sockAddr);
	BOOL bResult = GetSockName((SOCKADDR*)&sockAddr, &nSockAddrLen);
	if (bResult)
	{
		rSocketPort = ntohs(sockAddr.sin_port);
		rSocketAddress = inet_ntoa(sockAddr.sin_addr);
	}
	return bResult;
}

BOOL CAsyncSocket::GetSockName( SOCKADDR* lpSockAddr, int* lpSockAddrLen )
{
	return (SOCKET_ERROR != getsockname(m_hSocket, lpSockAddr, lpSockAddrLen));
}

BOOL CAsyncSocket::SetSockOpt( int nOptionName, const void* lpOptionValue, int nOptionLen, int nLevel /*= SOL_SOCKET*/ )
{
	return (SOCKET_ERROR != setsockopt(m_hSocket, nLevel, nOptionName, (LPCSTR)lpOptionValue, nOptionLen));
}

BOOL CAsyncSocket::GetSockOpt( int nOptionName, void* lpOptionValue, int* lpOptionLen, int nLevel /*= SOL_SOCKET*/ )
{
	return (SOCKET_ERROR != getsockopt(m_hSocket, nLevel, nOptionName, (LPSTR)lpOptionValue, lpOptionLen));
}

CAsyncSocket* PASCAL CAsyncSocket::FromHandle( SOCKET hSocket )
{
	return CAsyncSocket::LookupHandle(hSocket, FALSE);
}

int PASCAL CAsyncSocket::GetLastError()
{
	return WSAGetLastError(); // <ÈýÔÂÓê>
}

BOOL CAsyncSocket::Accept( CAsyncSocket& rConnectedSocket, SOCKADDR* lpSockAddr /*= NULL*/, int* lpSockAddrLen /*= NULL*/ )
{
	ATLASSERT(rConnectedSocket.m_hSocket == INVALID_SOCKET);
	ATLASSERT(CAsyncSocket::FromHandle(INVALID_SOCKET) == NULL);

	CAsyncSocket::AttachHandle(INVALID_SOCKET, &rConnectedSocket);
	if (CAsyncSocket::FromHandle(INVALID_SOCKET) == NULL)
	{
		// AttachHandle Call has failed
		return FALSE;
	}

	SOCKET hTemp = accept(m_hSocket, lpSockAddr, lpSockAddrLen);	

	if (hTemp == INVALID_SOCKET)
	{
		DWORD dwProblem = GetLastError();
		CAsyncSocket::DetachHandle(rConnectedSocket.m_hSocket, FALSE);
		rConnectedSocket.m_hSocket = INVALID_SOCKET;
		SetLastError(dwProblem);
	}
	else if (CAsyncSocket::FromHandle(INVALID_SOCKET) != NULL)
	{
		rConnectedSocket.m_hSocket = hTemp;
		CAsyncSocket::DetachHandle(INVALID_SOCKET, FALSE);
		CAsyncSocket::AttachHandle(hTemp, &rConnectedSocket);
	}

	return (hTemp != INVALID_SOCKET);
}

BOOL CAsyncSocket::Bind( UINT nSocketPort, LPCTSTR lpszSocketAddress /*= NULL*/ )
{
	USES_CONVERSION_EX;

	SOCKADDR_IN sockAddr;
	memset(&sockAddr,0,sizeof(sockAddr));

	LPSTR lpszAscii;
	if (lpszSocketAddress != NULL)
	{
		lpszAscii = T2A_EX((LPTSTR)lpszSocketAddress, _ATL_SAFE_ALLOCA_DEF_THRESHOLD);
		if (lpszAscii == NULL)
		{
			// OUT OF MEMORY
			WSASetLastError(ERROR_NOT_ENOUGH_MEMORY);
			return FALSE;
		}
	}
	else
	{
		lpszAscii = NULL;
	}

	sockAddr.sin_family = AF_INET;

	if (lpszAscii == NULL)
		sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	else
	{
		DWORD lResult = inet_addr(lpszAscii);
		if (lResult == INADDR_NONE)
		{
			WSASetLastError(WSAEINVAL);
			return FALSE;
		}
		sockAddr.sin_addr.s_addr = lResult;
	}

	sockAddr.sin_port = htons((u_short)nSocketPort);

	return Bind((SOCKADDR*)&sockAddr, sizeof(sockAddr));
}

BOOL CAsyncSocket::Bind( const SOCKADDR* lpSockAddr, int nSockAddrLen )
{
	return (SOCKET_ERROR != bind(m_hSocket, lpSockAddr, nSockAddrLen));
}

void CAsyncSocket::Close()
{
	if (m_hSocket != INVALID_SOCKET)
	{
		VERIFY(SOCKET_ERROR != closesocket(m_hSocket));
		CAsyncSocket::KillSocket(m_hSocket, this);
		m_hSocket = INVALID_SOCKET;
	}
}

BOOL CAsyncSocket::Connect( LPCTSTR lpszHostAddress, UINT nHostPort )
{
	USES_CONVERSION_EX;

	ATLASSERT(lpszHostAddress != NULL);

	if (lpszHostAddress == NULL)
	{
		WSASetLastError (WSAEINVAL);
		return FALSE;
	}

	SOCKADDR_IN sockAddr;
	memset(&sockAddr,0,sizeof(sockAddr));

	LPSTR lpszAscii = T2A_EX((LPTSTR)lpszHostAddress, _ATL_SAFE_ALLOCA_DEF_THRESHOLD);
	if (lpszAscii == NULL)
	{
		WSASetLastError(ERROR_NOT_ENOUGH_MEMORY);
		return FALSE;
	}

	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = inet_addr(lpszAscii);

	if (sockAddr.sin_addr.s_addr == INADDR_NONE)
	{
		LPHOSTENT lphost;
		lphost = gethostbyname(lpszAscii);
		if (lphost != NULL)
			sockAddr.sin_addr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
		else
		{
			WSASetLastError(WSAEINVAL);
			return FALSE;
		}
	}

	sockAddr.sin_port = htons((u_short)nHostPort);

	return Connect((SOCKADDR*)&sockAddr, sizeof(sockAddr));
}

BOOL CAsyncSocket::Connect( const SOCKADDR* lpSockAddr, int nSockAddrLen )
{
	return ConnectHelper(lpSockAddr, nSockAddrLen);
}

BOOL CAsyncSocket::IOCtl( long lCommand, DWORD* lpArgument )
{
	return (SOCKET_ERROR != ioctlsocket(m_hSocket, lCommand, lpArgument));
}

BOOL CAsyncSocket::Listen( int nConnectionBacklog/*=5*/ )
{
	return (SOCKET_ERROR != listen(m_hSocket, nConnectionBacklog));
}

int CAsyncSocket::Receive( void* lpBuf, int nBufLen, int nFlags /*= 0*/ )
{
	return recv(m_hSocket, (LPSTR)lpBuf, nBufLen, nFlags);
}

int CAsyncSocket::ReceiveFrom( void* lpBuf, int nBufLen, CString& rSocketAddress, UINT& rSocketPort, int nFlags /*= 0*/ )
{
	SOCKADDR_IN sockAddr;

	memset(&sockAddr, 0, sizeof(sockAddr));

	int nSockAddrLen = sizeof(sockAddr);
	int nResult = ReceiveFrom(lpBuf, nBufLen, (SOCKADDR*)&sockAddr, &nSockAddrLen, nFlags);
	if(nResult != SOCKET_ERROR)
	{
		rSocketPort = ntohs(sockAddr.sin_port);
		rSocketAddress = inet_ntoa(sockAddr.sin_addr);
	}
	return nResult;
}

int CAsyncSocket::ReceiveFrom( void* lpBuf, int nBufLen, SOCKADDR* lpSockAddr, int* lpSockAddrLen, int nFlags /*= 0*/ )
{
	return ReceiveFromHelper(lpBuf, nBufLen, lpSockAddr, lpSockAddrLen, nFlags);
}

BOOL CAsyncSocket::ShutDown( int nHow /*= sends*/ )
{
	return (SOCKET_ERROR != shutdown(m_hSocket,nHow));
}

int CAsyncSocket::Send( const void* lpBuf, int nBufLen, int nFlags /*= 0*/ )
{
	return send(m_hSocket, (LPSTR)lpBuf, nBufLen, nFlags);
}

int CAsyncSocket::SendTo( const void* lpBuf, int nBufLen, UINT nHostPort, LPCTSTR lpszHostAddress /*= NULL*/, int nFlags /*= 0*/ )
{
	USES_CONVERSION_EX;

	SOCKADDR_IN sockAddr;

	memset(&sockAddr,0,sizeof(sockAddr));

	LPSTR lpszAscii;
	if (lpszHostAddress != NULL)
	{
		lpszAscii = T2A_EX((LPTSTR)lpszHostAddress, _ATL_SAFE_ALLOCA_DEF_THRESHOLD);
		if (lpszAscii == NULL)
		{
			// OUT OF MEMORY
			WSASetLastError(ERROR_NOT_ENOUGH_MEMORY);
			return FALSE;
		}	
	}
	else
	{
		lpszAscii = NULL;
	}

	sockAddr.sin_family = AF_INET;

	if (lpszAscii == NULL)
		sockAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	else
	{
		sockAddr.sin_addr.s_addr = inet_addr(lpszAscii);
		if (sockAddr.sin_addr.s_addr == INADDR_NONE)
		{
			LPHOSTENT lphost;
			lphost = gethostbyname(lpszAscii);
			if (lphost != NULL)
				sockAddr.sin_addr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
			else
			{
				WSASetLastError(WSAEINVAL);
				return SOCKET_ERROR;
			}
		}
	}

	sockAddr.sin_port = htons((u_short)nHostPort);

	return SendTo(lpBuf, nBufLen, (SOCKADDR*)&sockAddr, sizeof(sockAddr), nFlags);
}

int CAsyncSocket::SendTo( const void* lpBuf, int nBufLen, const SOCKADDR* lpSockAddr, int nSockAddrLen, int nFlags /*= 0*/ )
{
	return SendToHelper(lpBuf, nBufLen, lpSockAddr, nSockAddrLen, nFlags);
}

BOOL CAsyncSocket::AsyncSelect( long lEvent /*= FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE*/ )
{
	ATLASSERT(m_hSocket != INVALID_SOCKET);
	return TRUE;
// 	_AFX_SOCK_THREAD_STATE* pState = _afxSockThreadState;
// 	ATLASSERT(pState->m_hSocketWindow != NULL);
// 
// 	return WSAAsyncSelect(m_hSocket, pState->m_hSocketWindow,
// 		WM_SOCKET_NOTIFY, lEvent) != SOCKET_ERROR;
}

CAsyncSocket::~CAsyncSocket()
{

}

CAsyncSocket* PASCAL CAsyncSocket::LookupHandle( SOCKET hSocket, BOOL bDead /*= FALSE*/ )
{
// 	CAsyncSocket* pSocket;
// 	_AFX_SOCK_THREAD_STATE* pState = _afxSockThreadState;
// 	if (!bDead)
// 	{
// 		pSocket = (CAsyncSocket*)
// 			pState->m_pmapSocketHandle->GetValueAt((void*)hSocket);
// 		if (pSocket != NULL)
// 			return pSocket;
// 	}
// 	else
// 	{
// 		pSocket = (CAsyncSocket*)
// 			pState->m_pmapDeadSockets->GetValueAt((void*)hSocket);
// 		if (pSocket != NULL)
// 			return pSocket;
// 	}
	return NULL;
}

void PASCAL CAsyncSocket::AttachHandle( SOCKET hSocket, CAsyncSocket* pSocket, BOOL bDead /*= FALSE*/ )
{
// 	_AFX_SOCK_THREAD_STATE* pState = _afxSockThreadState;
// 
// //	TRY 
// 	{
// 		if (!bDead)
// 		{
// 			ATLASSERT(CAsyncSocket::LookupHandle(hSocket, bDead) == NULL);
// 			if (pState->m_pmapSocketHandle->IsEmpty())
// 			{
// 				ATLASSERT(pState->m_pmapDeadSockets->IsEmpty());
// 				ATLASSERT(pState->m_hSocketWindow == NULL);
// 
// 				CSocketWnd* pWnd = new CSocketWnd;
// 				pWnd->m_hWnd = NULL;
// 
// 				if (!pWnd->CreateEx(0, AfxRegisterWndClass(0),
// 					_T("Socket Notification Sink"),
// 					WS_OVERLAPPED, 0, 0, 0, 0, NULL, NULL))
// 				{
// 					TRACE(traceSocket, 0, "Warning: unable to create socket notify window!\n");
// 					delete pWnd;
// 				}
// 
// 				ATLASSERT(pWnd->m_hWnd != NULL);
// 				ATLASSERT(CWnd::FromHandlePermanent(pWnd->m_hWnd) == pWnd);
// 				pState->m_hSocketWindow = pWnd->m_hWnd;
// 			}
// 			pState->m_pmapSocketHandle->SetAt((void*)hSocket, pSocket);
// 		}
// 		else
// 		{
// 			void* pvCount;
// 			INT_PTR nCount;
// 			if (pState->m_pmapDeadSockets->Lookup((void*)hSocket, pvCount))
// 			{
// 				nCount = (INT_PTR)pvCount;
// 				nCount++;
// 			}
// 			else
// 				nCount = 1;
// 			pState->m_pmapDeadSockets->SetAt((void*)hSocket, (void*)nCount);
// 		}
// 	}
// 	CATCH_ALL (e) 
// 	{ 
// 	} 
// 	END_CATCH_ALL
}

void PASCAL CAsyncSocket::DetachHandle( SOCKET hSocket, BOOL bDead /*= FALSE*/ )
{
// 	ATLASSERT(CAsyncSocket::LookupHandle(hSocket, bDead) != NULL);
// 
// 	_AFX_SOCK_THREAD_STATE* pState = _afxSockThreadState;
// 	if (!bDead)
// 	{
// 		pState->m_pmapSocketHandle->RemoveKey((void*)hSocket);
// 		if (pState->m_pmapSocketHandle->IsEmpty())
// 		{
// 			ATLASSERT(pState->m_hSocketWindow != NULL);
// 			CWnd* pWnd =
// 				CWnd::FromHandlePermanent(pState->m_hSocketWindow);
// 			ASSERT_VALID(pWnd);
// 
// 			if (pWnd != NULL)
// 			{
// 				pWnd->DestroyWindow();
// 				delete pWnd;
// 			}
// 
// 			pState->m_hSocketWindow = NULL;
// 
// 			pState->m_pmapDeadSockets->RemoveAll();
// 
// 			while (!pState->m_plistSocketNotifications->IsEmpty())
// 				delete pState->m_plistSocketNotifications->RemoveHead();
// 		}
// 	}
// 	else
// 	{
// 		void* pvCount;
// 		INT_PTR nCount;
// 		if (pState->m_pmapDeadSockets->Lookup((void*)hSocket, pvCount))
// 		{
// 			nCount = (INT_PTR)pvCount;
// 			nCount--;
// 			if (nCount == 0)
// 				pState->m_pmapDeadSockets->RemoveKey((void*)hSocket);
// 			else
// 				pState->m_pmapDeadSockets->SetAt((void*)hSocket, (void*)nCount);
// 		}
// 	}
}

void PASCAL CAsyncSocket::KillSocket( SOCKET hSocket, CAsyncSocket* pSocket )
{
// 	ATLASSERT(CAsyncSocket::LookupHandle(hSocket, FALSE) != NULL);
// 
// 	_AFX_SOCK_THREAD_STATE* pState = _afxSockThreadState;
// 
// 	CAsyncSocket::DetachHandle(hSocket, FALSE);
// 	if (pState->m_hSocketWindow != NULL)
// 	{
// 		::PostMessage(pState->m_hSocketWindow, WM_SOCKET_DEAD,
// 			(WPARAM)hSocket, 0L);
// 		CAsyncSocket::AttachHandle(hSocket, pSocket, TRUE);
// 	}
}

void PASCAL CAsyncSocket::DoCallBack( WPARAM wParam, LPARAM lParam )
{
	if (wParam == 0 && lParam == 0)
		return;

	// Has the socket be closed - lookup in dead handle list
	CAsyncSocket* pSocket = CAsyncSocket::LookupHandle((SOCKET)wParam, TRUE);

	// If yes ignore message
	if (pSocket != NULL)
		return;

	pSocket = CAsyncSocket::LookupHandle((SOCKET)wParam, FALSE);
	if (pSocket == NULL)
	{
		// Must be in the middle of an Accept call
		pSocket = CAsyncSocket::LookupHandle(INVALID_SOCKET, FALSE);
		ATLASSERT(pSocket != NULL);

		if(pSocket == NULL)
			return;

		pSocket->m_hSocket = (SOCKET)wParam;
		CAsyncSocket::DetachHandle(INVALID_SOCKET, FALSE);
		CAsyncSocket::AttachHandle(pSocket->m_hSocket, pSocket, FALSE);
	}

	int nErrorCode = WSAGETSELECTERROR(lParam);
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_READ:
		{
			fd_set fds;
			int nReady;
			timeval timeout;

			timeout.tv_sec = 0;
			timeout.tv_usec = 0;

			FD_ZERO(&fds);
			FD_SET(pSocket->m_hSocket, &fds);
			nReady = select(0, &fds, NULL, NULL, &timeout);
			if (nReady == SOCKET_ERROR)
				nErrorCode = WSAGetLastError();
			if ((nReady == 1) || (nErrorCode != 0))
				pSocket->OnReceive(nErrorCode);
		}
		break;
	case FD_WRITE:
		pSocket->OnSend(nErrorCode);
		break;
	case FD_OOB:
		pSocket->OnOutOfBandData(nErrorCode);
		break;
	case FD_ACCEPT:
		pSocket->OnAccept(nErrorCode);
		break;
	case FD_CONNECT:
		pSocket->OnConnect(nErrorCode);
		break;
	case FD_CLOSE:
		pSocket->OnClose(nErrorCode);
		break;
	}
}

BOOL CAsyncSocket::Socket( int nSocketType/*=SOCK_STREAM*/, long lEvent /*= FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE*/, int nProtocolType /*= 0*/, int nAddressFormat /*= PF_INET*/ )
{
	ATLASSERT(m_hSocket == INVALID_SOCKET);

	m_hSocket = socket(nAddressFormat,nSocketType,nProtocolType);
	if (m_hSocket != INVALID_SOCKET)
	{
		CAsyncSocket::AttachHandle(m_hSocket, this, FALSE);
		return AsyncSelect(lEvent);
	}
	return FALSE;
}
BOOL CAsyncSocket::ConnectHelper( const SOCKADDR* lpSockAddr, int nSockAddrLen )
{
	return connect(m_hSocket, lpSockAddr, nSockAddrLen) != SOCKET_ERROR;
}

int CAsyncSocket::ReceiveFromHelper( void* lpBuf, int nBufLen, SOCKADDR* lpSockAddr, int* lpSockAddrLen, int nFlags )
{
	return recvfrom(m_hSocket, (LPSTR)lpBuf, nBufLen, nFlags, lpSockAddr, lpSockAddrLen);
}

int CAsyncSocket::SendToHelper( const void* lpBuf, int nBufLen, const SOCKADDR* lpSockAddr, int nSockAddrLen, int nFlags )
{
	return sendto(m_hSocket, (LPSTR)lpBuf, nBufLen, nFlags, lpSockAddr, nSockAddrLen);
}
