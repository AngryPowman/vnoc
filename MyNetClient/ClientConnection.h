#pragma once

#include <WinSock2.h>

namespace VNOC_CLIENT{

enum CONNECT_TYPE
{
	CLIENT_TO_CLIENT,
	CLIENT_TO_SERVER,
};

#define WM_NETMSG WM_USER+1
#define VNOC_SERVER_PORT 9999
#define VNOC_SERVER_ADDR "111.111.111.111"

typedef int ConnectType;

class CClientConnection
{
public:
	CClientConnection();
	CClientConnection(HWND hwnd);
	~CClientConnection(void);

	void Create(/*int ConnectType = CLIENT_TO_SERVER,*/ const char* addr = VNOC_SERVER_ADDR, 
				int port = VNOC_SERVER_PORT , 
				long lEvent = FD_READ);
	int Connect();

	SOCKET GetSocket();
	ConnectType GetConnType();
	sockaddr_in GetAddrOfSockAddr();
	char* GetAddrOfString();

	void ReleaseSocket();

private:
	long m_lEvent;
	bool m_bConnected;
	HWND m_hwnd;
	ConnectType m_nConnectType;
	sockaddr_in m_SockAddr;
	SOCKET m_sSocket;
};

};