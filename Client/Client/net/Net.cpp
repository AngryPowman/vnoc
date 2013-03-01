#include "stdafx.h"
#include "Net.h"
#include "../../../NMessage/Message2Pack.h"
#include "../../../NMessage/Message2Parser.h"
#include "../../../NMessage/ParserMessageXML.h"

#include <algorithm>

//////////////////////////////////////////////////////////////////////////

CNetCenter::CNetCenter()
{
	ZeroMemory(&m_wsaData,sizeof(m_wsaData));
}

CNetCenter::~CNetCenter()
{

}

ResultCode CNetCenter::Initialize( IModule* UpperFrame/*=NULL*/ )
{
	m_isConnected = FALSE;
	WSAStartup(MAKEWORD(2,2),&m_wsaData);
	m_serverSocket.Create(0,SOCK_STREAM);
	return Result_Success;
}

ResultCode CNetCenter::UnInitialize()
{
	m_serverSocket.Close();
	WSACleanup();
	return Result_Success;
}

ResultCode CNetCenter::Run()
{
	m_serverSocket.SetListener(dynamic_cast<ISocketListener*>(this));
	return Result_Success;
}

ResultCode CNetCenter::Terminate()
{
	// 关闭所有连接
	return Result_Success;
}

ResultCode CNetCenter::_GetServerAddress()
{
	CConfig cfg(_T("VNOC/Net/ServerAddress"));
	cfg.Get();
	ConfigNode node = cfg.GetNode();

	m_serverPort = 0;

	if ( node.GetAttribute(_T("ServerIP"),m_serverIP)
		&& node.GetAttribute(_T("ServerPort"),m_serverPort))
	{
		if (!m_serverIP.IsEmpty())
		{
			return Result_Success;
		}
	}
	Global->CriticalError(_T("配置文件损坏，无法获取服务器地址."));
	return Result_Fail;
}

ResultCode CNetCenter::_ConnectServer()
{
	if (m_serverIP.IsEmpty())
	{
		_GetServerAddress();
	}
	if (m_serverIP.IsEmpty())
	{
		return Result_InvalidParam;
	}
	BOOL bSuccess = m_serverSocket.Connect(m_serverIP,m_serverPort);
	if (!bSuccess && m_serverSocket.GetLastError()!=WSAEWOULDBLOCK)
	{
		Global->Logf(LogFile_Net,_T("连接服务器失败.错误码%d\n"),m_serverSocket.GetLastError());
		return Result_Fail;
	}
	Global->Logf(LogFile_Net,_T("连接服务器成功,错误码%d\n"),m_serverSocket.GetLastError());
	m_isConnected = TRUE;
	return Result_Success;
}

ResultCode CNetCenter::IsServerConnected()
{
    return m_isConnected? Result_Success: Result_Fail;
}

ResultCode CNetCenter::SendServer( const CMessage &helper )
{
	if (!m_isConnected)
	{
		_ConnectServer();
	}
	CMessage2Pack packer;
    int len;
	packer.GetPackSize(&helper, len);
	CBufferMessage buffer;
	buffer.Alloc(len);
	packer.PackMessage(&helper, buffer);
	m_serverSocket.Send(buffer.GetBuffer(),len);
	return Result_Success;
}

ResultCode CNetCenter::SetListener( VMsg msgType,INetListener *listener )
{
	Util::CAutoCS ac(m_cs);
	m_listeners[msgType].push_back(listener);
	return Result_Success;
}

ResultCode CNetCenter::RemoveListener( VMsg msgType,INetListener *listener )
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
	return Result_Success;
}

ResultCode CNetCenter::Send( LPCTSTR ipv4Addr,DWORD port,const CMessage& netMsg )
{
	return Result_Success;
}

//////////////////////////////////////////////////////////////////////////

void CNetCenter::OnAccept( int nErrorCode,CAsyncSocketEx* pSock )
{
	Global->Log(LogFile_Net,_T("OnAccept"));
}

void CNetCenter::OnClose( int nErrorCode,CAsyncSocketEx* pSock )
{
	Global->Log(LogFile_Net,_T("OnClose"));
}

void CNetCenter::OnConnect( int nErrorCode,CAsyncSocketEx* pSock )
{
	Global->Log(LogFile_Net,_T("OnConnect"));
}

void CNetCenter::OnOutOfBandData( int nErrorCode,CAsyncSocketEx* pSock )
{
	Global->Log(LogFile_Net,_T("OnOutOfBandData"));
}

void CNetCenter::OnReceive( int nErrorCode,CAsyncSocketEx* pSock )
{
	Global->PtrAssert(pSock);
	CBufferMessage buffer;
	buffer.Alloc(1024);
	int length = pSock->Receive(buffer.GetBuffer(),1024);
	Global->Logf(LogFile_Net,_T("OnReceive, size:%d\n"),length);

	CMessage2Parser parser;
	CMessage msg(parser.GetMsgType(buffer));

	if (parser.Parser(&msg, buffer) == MsgStatus_Ok)
	{
		_DispatchMessage(msg);
	}
}

void CNetCenter::OnSend( int nErrorCode,CAsyncSocketEx* pSock )
{
	Global->Log(LogFile_Net,_T("OnSend"));
}

void CNetCenter::_DispatchMessage( const CMessage &msg )
{
	Util::CAutoCS ac(m_cs);
    auto i = m_listeners.find(VMsg(msg.MsgId()));
	if ( i != m_listeners.end())
	{
		auto ii = i->second.begin();
		for (; ii!=i->second.end();++ii)
		{
			(*ii)->OnNetMessage(msg);
		}
	}
}

void CNetCenter::OnPackReady(const CBufferMessage &buffer )
{
	Global->Logf(LogFile_Net,_T("OnPackReady, size:%d\n"),buffer.GetSize());
	CMessage2Parser parser;
	CMessage msg(parser.GetMsgType(buffer));
	if(parser.Parser(&msg, buffer) == MsgStatus_Ok)
	{
		_DispatchMessage(msg);
	}
}

ResultCode CNetCenter::MockReceive( const CMessage *mockMsg )
{
	if (mockMsg)
	{
		_DispatchMessage(*mockMsg);
	}
	return Result_Success;
}

//////////////////////////////////////////////////////////////////////////

CNetListenerHelper::CNetListenerHelper()
{
}

CNetListenerHelper::~CNetListenerHelper()
{
}

void CNetListenerHelper::AddFilter(VMsg msgType,INetListener *listener)
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
