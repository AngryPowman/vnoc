
#ifndef MET_CLIENT
#define MET_CLIENT

#include <WinSock2.h>

#pragma comment(lib,"WS2_32")


#define VNOC_SOC_S   0;
#define VNOC_SOC_F  -1;

class NetClient
{
public:
	NetClient(BYTE mainVer = 2,BYTE minorVer = 2);
	virtual ~NetClient();
	
	bool CreateSocket(u_short Port,LPCTSTR Addr,int Type,short _family = AF_INET,int Protocol = 0);
	bool CloseSocket();

/*
	int  Monitor();
    int  ShutMonitor();

	int  Message();*/


public:

	virtual void SetLinkAttribute(u_short Port,LPCTSTR Addr,short _family);

	sockaddr_in m_sockAddr;

/*private:*/

	SOCKET m_hSocket;
};

#endif