#include "ClientConnection.h"

namespace VNOC_CLIENT{

CClientConnection::CClientConnection()
{
	;
}

CClientConnection::CClientConnection(HWND hwnd)
{
	m_hwnd = hwnd;

	m_sSocket = 0;//socket(AF_INET,ConnectType,0);
	m_nConnectType = 0;
	m_bConnected = false;

	m_SockAddr.sin_family = AF_INET;
	m_SockAddr.sin_addr.s_addr = inet_addr("0,0,0,0");
	m_SockAddr.sin_port = htons(0);
}



CClientConnection::~CClientConnection(void)
{
	ReleaseSocket();
}

void CClientConnection::Create( /*int ConnectType = CLIENT_TO_SERVER,*/ 
								const char* addr /*= VNOC_SERVER_ADDR*/, 
								int port /*= VNOC_SERVER_PORT */, 
								long lEvent /*= FD_READ */)
{
	//m_sSocket = socket(AF_INET,ConnectType,0);
	m_nConnectType = CLIENT_TO_SERVER;//ConnectType;
	m_lEvent = lEvent;
	m_SockAddr.sin_family = AF_INET;
	m_SockAddr.sin_addr.s_addr = inet_addr(addr);
	m_SockAddr.sin_port = htons(port);
}

int CClientConnection::Connect()
{
	//连接客户端用UDP，连接服务器端用TCP;
	/*if (m_nConnectType == CLIENT_TO_SERVER)
	{
		m_sSocket = socket(AF_INET, SOCK_STREAM, 0);
	}
	else
	{
		m_sSocket = socket(AF_INET, SOCK_DGRAM, 0);
	}*/

	m_sSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	WSAAsyncSelect(m_sSocket,m_hwnd,WM_NETMSG,m_lEvent);
	int nRet = ::connect(m_sSocket,(LPSOCKADDR)&m_SockAddr, sizeof(m_SockAddr));
	if (nRet == 0)
	{
		m_bConnected = true;
	}
	
	return nRet;
}

SOCKET CClientConnection::GetSocket()
{
	return m_sSocket;
}

void CClientConnection::ReleaseSocket()         //关闭服务器
{
	m_bConnected=false;
	closesocket(m_sSocket);
}

sockaddr_in CClientConnection::GetAddrOfSockAddr()
{
	return m_SockAddr;
}

char* CClientConnection::GetAddrOfString()
{
	return inet_ntoa(m_SockAddr.sin_addr);
}

ConnectType CClientConnection::GetConnType()
{
	return m_nConnectType;
}


}