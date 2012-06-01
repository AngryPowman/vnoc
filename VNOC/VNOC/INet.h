#pragma once

#include "IModule.h"
#include "MsgDef.h"

class CNetListenerHelper;

interface INetCenter: public IModule
{
	STDMETHOD( ConnectServer(LPCTSTR ipv4Addr,DWORD port) = 0);
    STDMETHOD( IsServerConnected() );
	STDMETHOD( SendServer(const CMessage &helper) = 0);
	STDMETHOD( Send(LPCTSTR ipv4Addr,DWORD port,const CMessage& netMsg) = 0);
	STDMETHOD( SetListener(const CNetListenerHelper &helper) = 0);
};
