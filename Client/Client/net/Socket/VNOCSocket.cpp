#include "stdafx.h"
#include "VNOCSocket.h"
#include "../../../../NMessage/Message2Parser.h"
#include "../../../../NMessage/BufferMessage.h"
#include "../../util/util.h"

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
	CAsyncSocketEx::OnAccept(nErrorCode);
}

void CSocketImpl::OnClose(int nErrorCode)
{
	if (m_listener)
	{
		m_listener->OnClose(nErrorCode,this);
	}
	CAsyncSocketEx::OnClose(nErrorCode);
}

void CSocketImpl::OnConnect(int nErrorCode)
{
	if (m_listener)
	{
		m_listener->OnConnect(nErrorCode,this);
	}
	CAsyncSocketEx::OnConnect(nErrorCode);
}

void CSocketImpl::OnOutOfBandData(int nErrorCode)
{
	if (m_listener)
	{
		m_listener->OnOutOfBandData(nErrorCode,this);
	}
	//CAsyncSocketEx::OnOutOfBandData(nErrorCode);
}

void CSocketImpl::OnReceive(int nErrorCode)
{
	if (m_listener)
	{
		m_listener->OnReceive(nErrorCode,this);
	}
	CAsyncSocketEx::OnReceive(nErrorCode);
}

void CSocketImpl::OnSend(int nErrorCode)
{
	if (m_listener)
	{
		m_listener->OnSend(nErrorCode,this);
	}
	CAsyncSocketEx::OnSend(nErrorCode);
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
	length = ((CAsyncSocketEx*)this)->Receive(m_buffer.AllocAppend(1024),1024);
	if (FAILED(length))
	{
		Global->Logf(LogFile_Net,_T("Socket错误，错误码:%d"),GetLastError());
		Global->CriticalError(_T("Socket异常，无法工作"));
		return;
	}
	m_buffer.AccomplishAppend(length);
	_TryParse();
}

VOID CVNOCSocket::_TryParse()
{ // 解析buffer，将数据包分离出来
    CBufferMessage buffer;
    buffer.Attach(m_buffer.GetBuffer(), 
        CMessage2Parser::GetMessageLen(m_buffer.GetBuffer(), m_buffer.GetSize()));
    CMessage2Parser parser;
	if(parser.IsVaild(buffer))
	{
		IVNOCSocketListener *pListener=NULL;
		pListener = dynamic_cast<IVNOCSocketListener*>(m_listener);
		if (pListener)
		{
			pListener->OnPackReady(buffer);
		}
		m_buffer.Get(NULL, buffer.GetSize());
	}
    buffer.Detach();
}

//////////////////////////////////////////////////////////////////////////
