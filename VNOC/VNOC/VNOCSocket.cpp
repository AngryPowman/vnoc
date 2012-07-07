#include "stdafx.h"
#include "VNOCSocket.h"

#include "../util/util.h"

#define VNOCProtocol_
#define VNOCProtocol_HeadFlag	'V'
#define VNOCProtocol_HeadFlagSize	1
#define VNOCProtocol_VersionSize	1
#define VNOCProtocol_SeqNumSize		2
#define VNOCProtocol_HeadLeadSize	\
	VNOCProtocol_HeadFlagSize+ \
	VNOCProtocol_VersionSize+ \
	VNOCProtocol_SeqNumSize
#define VNOCProtocol_BodyLengthParamSize	4
#define VNOCProtocol_TokenSize		16
#define VNOCProtocol_CommandSize	1
#define VNOCProtocol_ReserveSize	4
#define VNOCProtocol_ParamCountSize	1
#define VNOCProtocol_HeadParamSize	\
	VNOCProtocol_TokenSize+ \
	VNOCProtocol_CommandSize+ \
	VNOCProtocol_ReserveSize+ \
	VNOCProtocol_ParamCountSize
#define VNOCProtocol_VerifyCodeSize	4
#define VNOCProtocol_TailFlag		'C'
#define VNOCProtocol_TailFlagSize	1
#define VNOCProtocol_PackTailSize	VNOCProtocol_VerifyCodeSize+VNOCProtocol_TailFlagSize

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
	length = ((CAsyncSocket*)this)->Receive(m_buffer.AllocAppend(1024),1024);
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
	DWORD startPos=0;
	while(startPos<bufSize && *pBuffer!=VNOCProtocol_HeadFlag)
	{
		++startPos;
	}
	if (startPos)
	{
		m_buffer.Get(NULL,startPos);	// 跳过不可识别数据
		Global->Logf(LogFile_Net,_T("出现了不可识别字符,长度为%d字节"),startPos);
		_TryParse();	// 理论上只会递归一次
		return;
	}
	if (bufSize > VNOCProtocol_HeadLeadSize+VNOCProtocol_BodyLengthParamSize)	// 检查长度是否足够
	{
		DWORD packSize=0;
		packSize = *((DWORD*)pBuffer + 1);
		if (bufSize >= packSize)
		{
			IVNOCSocketListener *pListener=NULL;
			pListener = dynamic_cast<IVNOCSocketListener*>(m_listener);
			if (pListener)
			{
				CBuffer buffer;
				buffer.Attach(pBuffer,packSize);
				pListener->OnPackReady(buffer);
				buffer.Detach();
			}
			m_buffer.Get(NULL,packSize);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
