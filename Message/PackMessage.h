
#pragma  once


#include "MessageParser.h"

//
#define  CHECKUP_DATALEN(Index,DataLen)   if(Index >= (int)DataLen){return - 1;}

class PackMessage
{
public:

	PackMessage(){}
	~PackMessage(){}

public:

	int Pack( const CMessage* const msg, byte* buf, size_t len);

	int GetMessageLen(const CMessage* const msg);

private:
	
	int _Pack( MSG_AVC* const avc, byte* buf, size_t len);

	int _Pack( MSG_RVC* const rvc, byte* buf, size_t len);

	int _Pack( MSG_ALI* const ali, byte* buf, size_t len);

	int _Pack( MSG_RLI* const rli, byte* buf, size_t len);

	int _Pack( MSG_APS* const asp, byte* buf, size_t len);

	int _Pack( MSG_RPS* const rsp, byte* buf, size_t len);

	int _Head( CMessage* const msg_clss,byte* buf, size_t len);
	int _Tail( CMessage* const msg_clss,byte* buf,int index ,size_t len);

};


