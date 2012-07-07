#pragma once
#include "../util/CBuffer.h"

class CAsyncSocket;

interface ISocketListener
{
public:
	virtual void OnAccept	(int nErrorCode,CAsyncSocket* pSock)	=0;
	virtual void OnClose	(int nErrorCode,CAsyncSocket* pSock)	=0;
	virtual void OnConnect	(int nErrorCode,CAsyncSocket* pSock)	=0;
	virtual void OnOutOfBandData	(int nErrorCode,CAsyncSocket* pSock)	=0;
	virtual void OnReceive	(int nErrorCode,CAsyncSocket* pSock)	=0;
	virtual void OnSend		(int nErrorCode,CAsyncSocket* pSock)	=0;
};

interface IVNOCSocketListener : public ISocketListener
{
public:
	virtual void OnPackReady(ConstReferenceBuffer buffer)	=0;
};
