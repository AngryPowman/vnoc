#pragma once

#include "ISocket.h"
#include "GlobalDefine.h"
#include "../util/CBuffer.h"

#include <afxsock.h>
#include <atlsync.h>

class CSocketImpl : public CAsyncSocket
{
public:
	CSocketImpl(ISocketListener *pListener=NULL);
	virtual ~CSocketImpl();
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnOutOfBandData(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	void SetListener(ISocketListener *pListener);	//  一个实例只能有一个监听者
protected:
	ISocketListener *m_listener;
};

class CVNOCSocket: public CSocketImpl
{
public:
	CVNOCSocket(ISocketListener *pListener=NULL);
	virtual ~CVNOCSocket();
	virtual void OnReceive(int nErrorCode);
	virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);
	virtual void OnSend(int nErrorCode);
private:
	VOID _TryParse();
private:
	CAutoStreamBuffer	m_sendBuffer;
	CAutoStreamBuffer	m_recvBuffer;
	CCriticalSection	m_cs;
};

