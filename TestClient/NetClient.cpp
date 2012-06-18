
#include "NetClient.h"



NetClient::NetClient(BYTE mainVer /* = 2 */,BYTE minorVer /* = 2 */)
{
	WORD socketVer = MAKEWORD(minorVer,mainVer);
	WSADATA wsaData;
	if (::WSAStartup(socketVer,&wsaData) != 0)
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


void NetClient::SetLinkAttribute(u_short Port,LPCTSTR Addr,short _family)
{
	if (Addr != 0||Port != 0||_family != 0)
	{
		m_sockAddr.sin_family = _family;
		m_sockAddr.sin_addr.S_un.S_addr = inet_addr(Addr);
		m_sockAddr.sin_port = ::htons(Port);
	}
}

bool NetClient::CreateSocket(u_short Port,LPCTSTR Addr,int Type,short _family /* = AF_INET */,int Protocol /* = 0 */)
{
	if (m_hSocket != NULL)
	{
		return false;
	}
	this->SetLinkAttribute(Port,Addr,_family);
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