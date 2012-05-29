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
	return S_OK;
}

HRESULT CNetCenter::Terminate()
{
	return S_OK;
}

HRESULT CNetCenter::ConnectServer( LPCTSTR ipv4Addr,DWORD port )
{
	m_serverSocket.Connect(ipv4Addr,port);
	return S_OK;
}

HRESULT CNetCenter::IsServerConnected()
{
	return S_OK;
}

HRESULT CNetCenter::SendServer( const CMessage& netMsg )
{
	return S_OK;
}

HRESULT CNetCenter::SetListener( const CNetListenerHelper &helper )
{
	return S_OK;
}
