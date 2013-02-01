#pragma once

#include "IModule.h"
#include "../../../Message/MSG_UNION.h"

#include <vector>

interface INetListener
{
	virtual ResultCode OnNetMessage(const CMessage& msg)=0;
};

interface INetCenter: public IModule
{
    virtual ResultCode IsServerConnected()=0;
	virtual ResultCode SendServer(const CMessage &helper)=0;
	virtual ResultCode Send(LPCTSTR ipv4Addr,DWORD port,const CMessage& netMsg)=0;
	virtual ResultCode SetListener(MSGTYPE msgType,INetListener *listener)=0;
	virtual ResultCode RemoveListener(MSGTYPE msgType,INetListener *listener)=0;
	virtual ResultCode MockReceive(const CMessage *mockMsg)=0;
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