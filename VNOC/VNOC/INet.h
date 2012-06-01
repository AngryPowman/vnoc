#pragma once

#include "IModule.h"
#include "MsgDef.h"

class CNetSendHelper;
class CNetListenerHelper;

interface INetListener
{
public:
	STDMETHOD( OnRecieve(CMessage netMsg) = 0);	// �븴�Ƹýṹ�壬�˳��ú�������ɾ��
	STDMETHOD( OnClose(int reason) = 0);	// reason��δ����
};

interface INetCenter: public IModule
{
	STDMETHOD( ConnectServer(LPCTSTR ipv4Addr,DWORD port) = 0);
	STDMETHOD( IsServerConnected() = 0);
	STDMETHOD( SendServer(const CMessage& netMsg) = 0);
	STDMETHOD( Send(LPCTSTR ipv4Addr,DWORD port,const CMessage& netMsg) = 0);
	STDMETHOD( SetListener(const CNetListenerHelper &helper) = 0);
};
