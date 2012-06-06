#pragma once

#include "INet.h"
#include "VNOCSocket.h"

#include <map>
#include <list>

class CNetCenter : public INetCenter
	,public ISocketListener
{
public:
	CNetCenter();
	~CNetCenter();
public:
	STDMETHOD( Initialize(IModule* UpperFrame=NULL) );
	STDMETHOD( UnInitialize() );
	STDMETHOD( Run() );
	STDMETHOD( Terminate() );

	STDMETHOD( ConnectServer(LPCTSTR ipv4Addr,DWORD port) );
	STDMETHOD( IsServerConnected() );
	STDMETHOD( SendServer(const CMessage& netMsg) );
	STDMETHOD( Send(LPCTSTR ipv4Addr,DWORD port,const CMessage& netMsg) );
	STDMETHOD( SetListener(MSGTYPE msgType,INetListener *listener) );
	STDMETHOD( RemoveListener(MSGTYPE msgType,INetListener *listener) );

VNOC_Private:
	virtual void OnAccept	(int nErrorCode,CAsyncSocket* pSock);
	virtual void OnClose	(int nErrorCode,CAsyncSocket* pSock);
	virtual void OnConnect	(int nErrorCode,CAsyncSocket* pSock);
	virtual void OnOutOfBandData	(int nErrorCode,CAsyncSocket* pSock);
	virtual void OnReceive	(int nErrorCode,CAsyncSocket* pSock);
	virtual void OnSend		(int nErrorCode,CAsyncSocket* pSock);
private:
	void _DispatchMessage(const CMessage* pMsg);
private:
	CSocketImpl m_serverSocket;
	WSADATA m_wsaData;
	std::map<MSGTYPE,std::list<INetListener*> >	m_listeners;
	ATL::CCriticalSection	m_cs;
};
