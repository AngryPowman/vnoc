
#pragma  once


#include "MessageParser.h"


#define  CHECKUP_DATALEN(Index,DataLen)   if(Index >= (int)DataLen){return - 1;}

class PackMessage
{
public:

	PackMessage(){}
	~PackMessage(){}

public:

	int Pack(const MSG_AVC* const avc, byte* buf, size_t len);

	int Pack(const MSG_RVC* const rvc, byte* buf, size_t len);

	int Pack(const MSG_ALI* const ali, byte* buf, size_t len);

	int Pack(const MSG_RLI* const rli, byte* buf, size_t len);

	int Pack(const MSG_APS* const asp, byte* buf, size_t len);

	int Pack(const MSG_RPS* const rsp, byte* buf, size_t len);

	int GetMessageLen(const CMessage* const msg);

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
	

	int _Head(const CMessage* const msg_clss,byte* buf, size_t len);
	int _Tail(const CMessage* const msg_clss,byte* buf,int index ,size_t len);

};


