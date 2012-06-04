/********************************************************************
	created:	2012/05/13
	created:	13:05:2012   00:46
	filename: 	D:\vnoc\trunk\VNOC\VNOC\PackMessage.h
	file path:	D:\vnoc\trunk\VNOC\VNOC\PackMessage.h
	file base:	Message Pack
	file ext:	h
	author:		Sum.
	
	purpose:	消息打包(客户端)
*********************************************************************/
#pragma  once


#include "MessageParser.h"


#define  CHECKUP_DATALEN(Index,DataLen)   if(Index >= (int)DataLen){return - 1;}

class PackMessage
{
public:

	PackMessage(){}
	~PackMessage(){}

public:

	int Pack(MSG_AVC* avc, byte* buf, size_t len);

	int Pack(MSG_RVC* rvc, byte* buf, size_t len);

	int Pack(MSG_ALI* ali, byte* buf, size_t len);

	int Pack(MSG_RLI* rli, byte* buf, size_t len);

	int Pack(MSG_APS* asp, byte* buf, size_t len);

	int Pack(MSG_RPS* rsp, byte* buf, size_t len);

	int GetMessageLen(CMessage* msg);

// 	int GetMessageLen(MSG_AVC* avc);
// 
// 	int GetMessageLen(MSG_RVC* rvc);
// 
// 	int GetMessageLen(MSG_ALI* ali);
// 
// 	int GetMessageLen(MSG_RLI* rli);
// 
// 	int GetMessageLen(MSG_APS* asp);
// 
// 	int GetMessageLen(MSG_RPS* rsp);

private:
	

	int _Head(CMessage* msg_clss,byte* buf, size_t len);
	int _Tail(CMessage* msg_clss,byte* buf,int index ,size_t len);

};


