#include "stdafx.h"
#include "NetClient.h"



NetClient::NetClient(BYTE mainVer /* = 2 */,BYTE minorVer /* = 2 */)
{
	WORD socketVer = MAKEWORD(minorVer,mainVer);
	if (::WSAStartup(socketVer,&m_wsaData) != 0)
	{
		//³õÊ¼»¯socketÊ§°Ü 
		return;
	}
}


NetClient::~NetClient()
{
	::WSACleanup();
	if (m_hSocket != NULL)
	{
		this->CloseSocket();
		m_hSocket = NULL;
	}
}


bool NetClient::CreateSocket(int Type,short _family /* = AF_INET */,int Protocol /* = 0 */)
{
	if (m_hSocket != NULL)
	{
		return false;
	}

	if (_family != 0)
	{
		m_sockAddr.sin_family = _family;
	}


	m_hSocket = ::socket(AF_INET,Type,Protocol);
	if (m_hSocket != INVALID_SOCKET)
	{
		return true;
	}
	return false;
}

bool NetClient::CloseSocket()
{
	if (::closesocket(m_hSocket)!= INVALID_SOCKET)
	{
		return true;
	}
	return false;
}


bool NetClient::Build(LPCTSTR Addr,u_short Port)
{
	if (Addr == NULL)
	{
		return false;
	}
	m_sockAddr.sin_addr.S_un.S_addr = inet_addr(Addr);
	m_sockAddr.sin_port = ::htons(Port);

	if (::connect(m_hSocket,(LPSOCKADDR)&m_sockAddr,sizeof(m_sockAddr)) == -1)
	{
		return false;
	}

	return true;
}

bool NetClient::Result(LPTSTR lpszbuffer,int nlen,int flag /* = 0 */)
{
	if (!lpszbuffer||!m_hSocket)
	{
		return false;
	}

	int nRecv = ::recv(m_hSocket,lpszbuffer,nlen,flag);
	if (nRecv > 0)
	{
		return true;
	}
	return false;
}

bool NetClient::Send(LPTSTR lpszBuffer,int flag /* = 0 */)
{
	if (!lpszBuffer||!m_hSocket)
	{
		return false;
	}

	int nRecv = ::send(m_hSocket,lpszBuffer,lstrlen(lpszBuffer),flag);
	if (nRecv > 0)
	{
		return true;
	}
	return false;
}