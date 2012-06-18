#include "NetClient.h"

namespace VNOC_CLIENT{


CNetClient::~CNetClient(void)
{
}

int CNetClient::InitClient()
{
	return WSAStartup(MAKEWORD(2,2),&m_wsaData);
}

int CNetClient::Send( CClientConnection connect,const VOID* pbuf, int size )
{
	int nSentSum = 0;//总发送字节
	int nSent = 0;//当前发送字节
	char* p;
	p = (char*)malloc(size);
	memcpy(p,pbuf,size);

	if (connect.GetConnType() == CLIENT_TO_SERVER)
	{
		while(nSentSum < size)
		{
			nSent = ::send(connect.GetSocket(),p + nSentSum, size-nSentSum, 0);
			if(nSent < 0)
			{
				return nSent;
			}
			else
			{
				nSentSum += nSent;
			}
		}
	}
	else
	{
		sockaddr_in add = connect.GetAddrOfSockAddr();
		while(nSentSum < size)
		{
			nSent = ::sendto(connect.GetSocket(),p + nSentSum, size-nSentSum,0,(sockaddr*)&(add),sizeof(add));
			if(nSent < 0)
			{
				return nSent;
			}
			else
			{
				nSentSum += nSent;	
			}
		}
	}
	
	return nSentSum;
}

LRESULT CNetClient::NetMsgProc( HWND hwnd, UINT uMsg, WPARAM wParam/*套接字*/, LPARAM lParam )
{

	return 0;
}

CClientConnection* CNetClient::CreateConn( int nPort, const char* szAddr, const char* szId, long lEvent )
{
	CClientConnection* pConn = new CClientConnection(m_hwnd);
	pConn->Create(szAddr,nPort,lEvent);

	std::pair<CClientConnection*,std::string> pair = std::make_pair(pConn,szId);
	m_ConnMap.insert(pair);

	return pConn;
}



}//namespace VNOC_CLIENT