#include "stdafx.h"
#include "Net.h"

//////////////////////////////////////////////////////////////////////////

CNetCenter::CNetCenter()
{

}

CNetCenter::~CNetCenter()
{

}

HRESULT CNetCenter::Initialize( IModule* UpperFrame/*=NULL*/ )
{
	return S_OK;
}

HRESULT CNetCenter::UnInitialize()
{
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
	m_serverSocket.Connect(_T("192.168.0.1"),2508);
	return S_OK;
}

HRESULT CNetCenter::IsServerConnected()
{
    return S_OK;
}

HRESULT CNetCenter::SendServer( const CMessage &helper )
{
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
