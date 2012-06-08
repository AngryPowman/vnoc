#pragma once

#include "IModule.h"
#include "..\..\Message\MsgDef.h"

#include <vector>

interface INetListener
{
	STDMETHOD( OnMessage(const CMessage& msg) = 0);
};

interface INetCenter: public IModule
{
    STDMETHOD( IsServerConnected() = 0);
	STDMETHOD( SendServer(const CMessage &helper) = 0);
	STDMETHOD( Send(LPCTSTR ipv4Addr,DWORD port,const CMessage& netMsg) = 0);
	STDMETHOD( SetListener(MSGTYPE msgType,INetListener *listener) = 0);
	STDMETHOD( RemoveListener(MSGTYPE msgType,INetListener *listener) = 0);
};

class CNetListenerHelper
{
public:
	CNetListenerHelper();
	~CNetListenerHelper();
public:
	void AddFilter(MSGTYPE msgType,INetListener *listener);
	void StartListen();
	void StopListen();
private:
	std::vector<std::pair<MSGTYPE,INetListener*> >	m_vec;
};