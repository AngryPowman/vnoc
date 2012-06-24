#include "stdafx.h"
#include <afxsock.h>
#include "VNOCSocket.h"

CSocketImpl::CSocketImpl(ISocketListener *pListener) : m_listener(pListener)
{
}

CSocketImpl::~CSocketImpl()
{
}

void CSocketImpl::OnAccept(int nErrorCode)
{
	if (m_listener)
	{
		m_listener->OnAccept(nErrorCode,this);
	}
	CAsyncSocket::OnAccept(nErrorCode);
}

void CSocketImpl::OnClose(int nErrorCode)
{
	if (m_listener)
	{
		m_listener->OnClose(nErrorCode,this);
	}
	CAsyncSocket::OnClose(nErrorCode);
}

void CSocketImpl::OnConnect(int nErrorCode)
{
	if (m_listener)
	{
		m_listener->OnConnect(nErrorCode,this);
	}
	CAsyncSocket::OnConnect(nErrorCode);
}

void CSocketImpl::OnOutOfBandData(int nErrorCode)
{
	if (m_listener)
	{
		m_listener->OnOutOfBandData(nErrorCode,this);
	}
	CAsyncSocket::OnOutOfBandData(nErrorCode);
}

void CSocketImpl::OnReceive(int nErrorCode)
{
	if (m_listener)
	{
		m_listener->OnReceive(nErrorCode,this);
	}
	CAsyncSocket::OnReceive(nErrorCode);
}

void CSocketImpl::OnSend(int nErrorCode)
{
	if (m_listener)
	{
		m_listener->OnSend(nErrorCode,this);
	}
	CAsyncSocket::OnSend(nErrorCode);
}

void CSocketImpl::SetListener( ISocketListener *pListener )
{
	m_listener = pListener;
}

//////////////////////////////////////////////////////////////////////////

CVNOCSocket::CVNOCSocket( ISocketListener *pListener/*=NULL*/ )
	:CSocketImpl(pListener)
{

}

CVNOCSocket::~CVNOCSocket()
{
}

void CVNOCSocket::OnReceive( int nErrorCode )
{
	int length = 0;
	length = ((CAsyncSocket*)this)->Receive(m_buffer.AllocAppend(1024),1024);
	m_buffer.AccomplishAppend(length);
}

//////////////////////////////////////////////////////////////////////////
