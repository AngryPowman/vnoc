#include "stdafx.h"
#include "Net.h"

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
	m_serverSocket.Send(NULL,0);		//? 发啥啊。。。
	return S_OK;
}

HRESULT CNetCenter::SetListener( const CNetListenerHelper &helper )
{
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
	Global->Log(LogFile_Net,_T("OnReceive"));
}

void CNetCenter::OnSend( int nErrorCode,CAsyncSocket* pSock )
{

}
