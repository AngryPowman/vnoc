#pragma once

#include "IModule.h"
#include "MsgDef.h"

class CNetSendHelper;
class CNetListenerHelper;

interface INetListener
{
public:
	STDMETHOD( OnRecieve(CMessage netMsg) = 0);	// 请复制该结构体，退出该函数将被删除
	STDMETHOD( OnClose(int reason) = 0);	// reason还未定义
};

interface INetCenter: public IModule
{
	STDMETHOD( ConnectServer(LPCTSTR ipv4Addr,DWORD port) = 0);
	STDMETHOD( IsServerConnected() = 0);
	STDMETHOD( SendServer(const CMessage& netMsg) = 0);
	STDMETHOD( Send(LPCTSTR ipv4Addr,DWORD port,const CMessage& netMsg) = 0);
	STDMETHOD( SetListener(const CNetListenerHelper &helper) = 0);
};
