/********************************************************************
	created:	2012/01/08
	created:	8:1:2012   6:23
	filename: 	D:\vnoc\trunk\VNOC\MyNetClient\NetClient.h
	file path:	D:\vnoc\trunk\VNOC\MyNetClient
	file base:	NetClient
	file ext:	h
	author:		面粉
	
	purpose:	客户端类，用以连接服务器(客户端)
*********************************************************************/

#pragma once
#include <WinSock2.h>
#pragma comment(lib,"WS2_32")
#include <map>
#include <string>

#include "ClientConnection.h"

namespace VNOC_CLIENT{

class CNetClient
{
public:
	CNetClient();
	explicit CNetClient( HWND hwnd ) :m_hwnd(hwnd){};
	~CNetClient();

	int InitClient();
	LRESULT NetMsgProc(HWND hwnd, UINT uMsg, WPARAM wParam/*套接字*/, LPARAM lParam);
	CClientConnection* CreateConn(int nPort, const char* szAddr, const char* szId, long lEvent);

	int Send( CClientConnection connect,const VOID* pbuf, int nsize );

private:
	HWND m_hwnd;
	WSADATA m_wsaData;
	std::map<CClientConnection*,std::string> m_ConnMap;
};

};