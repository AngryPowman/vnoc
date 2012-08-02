#include "stdafx.h"
#include "VNOCSocket.h"

#include "../util/util.h"
#include "../../Message/MessageParser.h"

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
	Util::CAutoCS ac(m_cs);
	length = ((CAsyncSocket*)this)->Receive(m_recvBuffer.AllocAppend(1024),1024);
	if (length == SOCKET_ERROR)
	{
		if (GetLastError() != WSAEWOULDBLOCK)
		{
			Global->Logf(LogFile_Net,_T("Socket错误，错误码:%d"),GetLastError());
			Global->CriticalError(_T("Socket异常，无法工作"));
			return;
		}
	}
	m_recvBuffer.AccomplishAppend(length);
	_TryParse();
}

void CVNOCSocket::OnSend( int nErrorCode )
{
	CSocketImpl::OnSend(nErrorCode);
	int ret = CSocketImpl::Send(m_sendBuffer,m_sendBuffer.GetSize());
	if (ret != SOCKET_ERROR)
	{
		m_sendBuffer.Get(NULL,ret);
		AsyncSelect(FD_READ);
	}
	else
	{
		DWORD le = WSAGetLastError();
	}
}

VOID CVNOCSocket::_TryParse()
{ // 解析buffer，将数据包分离出来
	BYTE* pBuffer;
	DWORD bufSize;
	pBuffer = m_recvBuffer.GetBuffer();
	bufSize = m_recvBuffer.GetSize();
	int checkResult = CMessageParser::Check(pBuffer,bufSize);
	if (checkResult != -1)
	{
		CBuffer buffer;
		buffer.Attach(pBuffer,checkResult);
		IVNOCSocketListener *pListener=NULL;
		pListener = dynamic_cast<IVNOCSocketListener*>(m_listener);
		if (pListener)
		{
			pListener->OnPackReady(buffer);
		}
		buffer.Detach();
		m_recvBuffer.Get(NULL,checkResult);
	}
}

int CVNOCSocket::Send( const void* lpBuf, int nBufLen, int nFlags /*= 0*/ )
{
	m_sendBuffer.Append(lpBuf,nBufLen);
	CAsyncSocket::AsyncSelect(FD_WRITE);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
