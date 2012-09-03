#include "VNOCSocket.h"
#include "../../../../../Message/MessageParser.h"
#include "../../Global/IGlobal.h"
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
	BYTE* pBuffer;
	DWORD bufSize;
	pBuffer = m_buffer.GetBuffer();
	bufSize = m_buffer.GetSize();
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
		m_buffer.Get(NULL,checkResult);
	}
}

//////////////////////////////////////////////////////////////////////////
