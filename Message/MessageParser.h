

#pragma once

#include "MsgDef.h"


class CMessageParser
{
public:
	
	CMessageParser(){
		m_Instruct = 0;
	}

	~CMessageParser(){

	}

public:
	//解析
	CMessage* Parse(byte* lpszData,size_t len);


private:

	CMessage* m_MessageStr;           //

	byte   m_Instruct;			  //指令			具体的指令，用来标注此数据包的功能

	static int _Checking(CMessage* _MessageType,byte* lpszDate,size_t len);

	int _Head(CMessage* _Messsage,byte* lpszData,size_t len);

	int _Body(CMessage* _Messsage,byte* lpszData,size_t len);

	int _Tail(CMessage* _Messsage,byte* lpszData,int ParamLen,size_t len);

	CMessage* _MessageType();
	int  _GetMessageType();

	int _CheckUpLen(int Index,size_t len)
	{
		if (Index > (int)len)
		{
			return -1;
		}
		return 0;
	}

};

