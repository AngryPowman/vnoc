#pragma once

#define NOLAYERS
#include "AsyncSocketEx.h"
#include "ISocket.h"
#include "../../Global/GlobalDefine.h"
#include "../../util/CBuffer.h"

#include <atlsync.h>

class CSocketImpl : public CAsyncSocketEx
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
private:
	VOID _TryParse();
private:
	CAutoStreamBuffer	m_buffer;
	CCriticalSection	m_cs;
};

