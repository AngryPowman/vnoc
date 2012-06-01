#pragma once

#include "INet.h"
#include "VNOCSocket.h"

class CNetCenter
{
public:
	CNetCenter();
	~CNetCenter();
public:
	STDMETHOD( Initialize(IModule* UpperFrame=NULL) );
	STDMETHOD( UnInitialize() );
	STDMETHOD( Run() );
	STDMETHOD( Terminate() );

	STDMETHOD( ConnectServer(LPCTSTR ipv4Addr,DWORD port) );
	STDMETHOD( IsServerConnected() );
	STDMETHOD( SendServer(const CMessage& netMsg) );
	STDMETHOD( Send(LPCTSTR ipv4Addr,DWORD port,const CMessage& netMsg) );
	STDMETHOD( SetListener(const CNetListenerHelper &helper) );
private:
	CSocketImpl m_serverSocket;
};
