
#pragma  once


#include "MessageParser.h"


#define  CHECKUP_DATALEN(Index,DataLen)   if(Index > (int)DataLen){return - 1;}

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

};


