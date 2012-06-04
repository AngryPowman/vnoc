#pragma once

#include "INet.h"
#include "VNOCSocket.h"

class CNetCenter : public ISocketListener
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
	STDMETHOD( SetListener(const CNetListenerHelper &helper) );

VNOC_Private:
	virtual void OnAccept	(int nErrorCode,CAsyncSocket* pSock);
	virtual void OnClose	(int nErrorCode,CAsyncSocket* pSock);
	virtual void OnConnect	(int nErrorCode,CAsyncSocket* pSock);
	virtual void OnOutOfBandData	(int nErrorCode,CAsyncSocket* pSock);
	virtual void OnReceive	(int nErrorCode,CAsyncSocket* pSock);
	virtual void OnSend		(int nErrorCode,CAsyncSocket* pSock);
private:
	CSocketImpl m_serverSocket;
};
