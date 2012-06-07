#include "stdafx.h"
#include "Net.h"
#include "../../Message/PackMessage.h"
#include "../../Message/MessageParser.h"
#include "../util/util.h"

#include <algorithm>

//////////////////////////////////////////////////////////////////////////

CNetCenter::CNetCenter()
{
	ZeroMemory(&m_wsaData,sizeof(m_wsaData));
}

CNetCenter::~CNetCenter()
{

}

HRESULT CNetCenter::Initialize( IModule* UpperFrame/*=NULL*/ )
{
	WSAStartup(MAKEWORD(2,2),&m_wsaData);
	m_serverSocket.Create(0,SOCK_STREAM);
	return S_OK;
}

HRESULT CNetCenter::UnInitialize()
{
	m_serverSocket.Close();
	WSACleanup();
	return S_OK;
}

HRESULT CNetCenter::Run()
{
	m_serverSocket.SetListener(dynamic_cast<ISocketListener*>(this));
	return S_OK;
}

HRESULT CNetCenter::Terminate()
{
	// 关闭所有连接
	return S_OK;
}

HRESULT CNetCenter::ConnectServer( LPCTSTR ipv4Addr,DWORD port )
{
	BOOL bSuccess = m_serverSocket.Connect(ipv4Addr,port);
	if (!bSuccess && m_serverSocket.GetLastError()!=WSAEWOULDBLOCK)
	{
		Global->Logf(LogFile_Net,_T("连接服务器失败.错误码%d\n"),m_serverSocket.GetLastError());
		return E_FAIL;
	}
	Global->Logf(LogFile_Net,_T("连接服务器成功,错误码%d\n"),m_serverSocket.GetLastError());
	return S_OK;
}

HRESULT CNetCenter::IsServerConnected()
{
    return S_OK;
}

HRESULT CNetCenter::SendServer( const CMessage &helper )
{
	PackMessage packer;
	int len = packer.GetMessageLen(&helper);
	CBuffer buffer;
	buffer.Alloc(len);
	packer.Pack(&helper,buffer.GetBuffer(),len);
	m_serverSocket.Send(buffer.GetBuffer(),len);
	return S_OK;
}

HRESULT CNetCenter::SetListener( MSGTYPE msgType,INetListener *listener )
{
	Util::CAutoCS ac(m_cs);
	m_listeners[msgType].push_back(listener);
	return S_OK;
}

HRESULT CNetCenter::RemoveListener( MSGTYPE msgType,INetListener *listener )
{
	Util::CAutoCS ac(m_cs);
	auto si = m_listeners.find(msgType);
	if (si != m_listeners.end())
	{
		auto i = std::find(si->second.begin(),si->second.end(),listener);
		do 
		{
			si->second.erase(i);
			i = std::find(si->second.begin(),si->second.end(),listener);
		} while (i != si->second.end());
	}
	return S_OK;
}

HRESULT CNetCenter::Send( LPCTSTR ipv4Addr,DWORD port,const CMessage& netMsg )
{
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////

void CNetCenter::OnAccept( int nErrorCode,CAsyncSocket* pSock )
{
	Global->Log(LogFile_Net,_T("OnAccept"));
}

void CNetCenter::OnClose( int nErrorCode,CAsyncSocket* pSock )
{
	Global->Log(LogFile_Net,_T("OnClose"));
}

void CNetCenter::OnConnect( int nErrorCode,CAsyncSocket* pSock )
{
	Global->Log(LogFile_Net,_T("OnConnect"));
}

void CNetCenter::OnOutOfBandData( int nErrorCode,CAsyncSocket* pSock )
{
	Global->Log(LogFile_Net,_T("OnOutOfBandData"));
}

void CNetCenter::OnReceive( int nErrorCode,CAsyncSocket* pSock )
{
	Global->PtrAssert(pSock);
	CBuffer buffer;
	buffer.Alloc(1024);
	int length = pSock->Receive(buffer.GetBuffer(),1024);
	Global->Logf(LogFile_Net,_T("OnReceive, size:%d\n"),length);

	CMessageParser parser;
	CMessage *pMsg=NULL;
	pMsg = parser.Parse(buffer.GetBuffer(),length);
	if (pMsg)
	{
		_DispatchMessage(pMsg);
		delete pMsg;
	}
}

void CNetCenter::OnSend( int nErrorCode,CAsyncSocket* pSock )
{
	Global->Log(LogFile_Net,_T("OnSend"));
}

void CNetCenter::_DispatchMessage( const CMessage* pMsg )
{
	Util::CAutoCS ac(m_cs);
	auto i = m_listeners.find((MSGTYPE)pMsg->GetMessageType());
	if ( i != m_listeners.end())
	{
		auto ii = i->second.begin();
		for (; ii!=i->second.end();++ii)
		{
			(*ii)->OnMessage(*pMsg);
		}
	}
}

//////////////////////////////////////////////////////////////////////////

CNetListenerHelper::CNetListenerHelper()
{
}

CNetListenerHelper::~CNetListenerHelper()
{
}

void CNetListenerHelper::AddFilter(MSGTYPE msgType,INetListener *listener)
{
	m_vec.push_back(std::make_pair(msgType,listener));
}

void CNetListenerHelper::StartListen()
{
	INetCenter *pCenter=NULL;
	Global->GetINetCenter(&pCenter);
	Global->PtrAssert(pCenter);

	for(auto i=m_vec.begin(); i!=m_vec.end(); ++i)
	{
		pCenter->SetListener(i->first,i->second);
	}
}

void CNetListenerHelper::StopListen()
{
	INetCenter *pCenter=NULL;
	Global->GetINetCenter(&pCenter);
	Global->PtrAssert(pCenter);

	for(auto i=m_vec.begin(); i!=m_vec.end(); ++i)
	{
		pCenter->RemoveListener(i->first,i->second);
	}
}
