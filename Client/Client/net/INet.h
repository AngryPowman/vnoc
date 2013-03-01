#pragma once
#include <vector>
#include "IModule.h"
#include "../../../NMessage/MessageUnion.h"
#include "../../../NMessage/CMessage.h"
using namespace VNOC::Message;


interface INetListener
{
	virtual ResultCode OnNetMessage(const CMessage &msg)=0;
};

interface INetCenter: public IModule
{
    virtual ResultCode IsServerConnected()=0;
	virtual ResultCode SendServer(const CMessage &helper)=0;
	virtual ResultCode Send(LPCTSTR ipv4Addr,DWORD port,const CMessage& netMsg)=0;
	virtual ResultCode SetListener(VMsg msgType,INetListener *listener)=0;
	virtual ResultCode RemoveListener(VMsg msgType,INetListener *listener)=0;
	virtual ResultCode MockReceive(const CMessage *mockMsg)=0;
};

class CNetListenerHelper
{
public:
	CNetListenerHelper();
	~CNetListenerHelper();
public:
	void AddFilter(VMsg msgType,INetListener *listener);
	void StartListen();
	void StopListen();
private:
	std::vector<std::pair<VMsg,INetListener*> >	m_vec;
};