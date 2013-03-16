
#pragma  once


#include "MSG_UNION.h"

//
#define  CHECKUP_DATALEN(Index,DataLen)  if(Index >= (int)DataLen){return - 1;}

class PackMessage
{
public:

	PackMessage(){
		CommandListPos = 0;
	}
	~PackMessage(){}

public:

	int Pack(const CMessage* const msg, byte* buf, size_t len);

	int GetMessageLen(const CMessage* const msg);

private:

	int _Head(const CMessage* const msg_clss,byte* buf, size_t len);

	int _Tail(const CMessage* const msg_clss,byte* buf,int index ,size_t len);

private:

	int _CreateCommandLenList( byte* buf, int in_index);

	void _PushCommandLenList(int len);

	void _CloseCommandLenList();

	int _PushCommandList(const byte* const in_Command, int len);

	int _CreateCommandList(byte* buf,int in_index);

	void _CloseCommandList();

private:

	std::vector<uint> m_CommandLenList;
	std::vector<std::vector<byte> > m_CommandList;
	int  CommandListPos;

};


