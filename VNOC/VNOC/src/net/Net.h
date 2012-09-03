#pragma once

#include "INet.h"
#include "socket/VNOCSocket.h"

#include <map>
#include <list>

class CNetCenter : public INetCenter
	,public IVNOCSocketListener
{
public:
	CNetCenter();
	~CNetCenter();
public:
	STDMETHOD( Initialize(IModule* UpperFrame=NULL) );
	STDMETHOD( UnInitialize() );
	STDMETHOD( Run() );
	STDMETHOD( Terminate() );

	STDMETHOD( IsServerConnected() );
	STDMETHOD( SendServer(const CMessage& netMsg) );
	STDMETHOD( Send(LPCTSTR ipv4Addr,DWORD port,const CMessage& netMsg) );
	STDMETHOD( SetListener(MSGTYPE msgType,INetListener *listener) );
	STDMETHOD( RemoveListener(MSGTYPE msgType,INetListener *listener) );

VNOC_Private:
	virtual void OnAccept	(int nErrorCode,CAsyncSocketEx* pSock);
	virtual void OnClose	(int nErrorCode,CAsyncSocketEx* pSock);
	virtual void OnConnect	(int nErrorCode,CAsyncSocketEx* pSock);
	virtual void OnOutOfBandData	(int nErrorCode,CAsyncSocketEx* pSock);
	virtual void OnReceive	(int nErrorCode,CAsyncSocketEx* pSock);
	virtual void OnSend		(int nErrorCode,CAsyncSocketEx* pSock);
	virtual void OnPackReady(ConstReferenceBuffer buffer);
private:
	HRESULT _GetServerAddress();
	HRESULT _ConnectServer();
	void _DispatchMessage(const CMessage* pMsg);
private:
	CString	m_serverIP;
	UINT	m_serverPort;
	BOOL	m_isConnected;

	CVNOCSocket m_serverSocket;
	WSADATA m_wsaData;
	std::map<MSGTYPE,std::list<INetListener*> >	m_listeners;
	ATL::CCriticalSection	m_cs;
};
