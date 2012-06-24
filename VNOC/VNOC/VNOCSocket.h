#pragma once

#include "ISocket.h"
#include "GlobalDefine.h"
#include "../util/CBuffer.h"

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
private:
	ISocketListener *m_listener;
};

class CVNOCSocket: public CSocketImpl
{
public:
	CVNOCSocket(ISocketListener *pListener=NULL);
	virtual ~CVNOCSocket();
	virtual void OnReceive(int nErrorCode);
private:
	CAutoStreamBuffer	m_buffer;
};

