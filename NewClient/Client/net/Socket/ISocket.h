#pragma once
#include "../../util/CBuffer.h"

class CAsyncSocketEx;

interface ISocketListener
{
public:
	virtual void OnAccept	(int nErrorCode,CAsyncSocketEx* pSock)	=0;
	virtual void OnClose	(int nErrorCode,CAsyncSocketEx* pSock)	=0;
	virtual void OnConnect	(int nErrorCode,CAsyncSocketEx* pSock)	=0;
	virtual void OnOutOfBandData	(int nErrorCode,CAsyncSocketEx* pSock)	=0;
	virtual void OnReceive	(int nErrorCode,CAsyncSocketEx* pSock)	=0;
	virtual void OnSend		(int nErrorCode,CAsyncSocketEx* pSock)	=0;
};

interface IVNOCSocketListener : public ISocketListener
{
public:
	virtual void OnPackReady(ConstReferenceBuffer buffer)	=0;
};
