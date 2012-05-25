/********************************************************************
	created:	2012/05/13
	created:	13:05:2012   00:46
	filename: 	D:\vnoc\trunk\VNOC\VNOC\MsgAnalysis.h
	file path:	D:\vnoc\trunk\VNOC\VNOC\MsgAnalysis.h
	file base:	Message Analysis
	file ext:	h
	author:		Sum.
	
	purpose:	数据包解析类，分析传输数据(客户端)
*********************************************************************/


#pragma once

#include "msgdef.h"

typedef unsigned short ushort;

int PackMessage(MSG_AVC* avc, byte* buf, size_t len);

int PackMessage(MSG_RVC* rvc, byte* buf, size_t len);

int PackMessage(MSG_ALI* ali, byte* buf, size_t len);

int PackMessage(MSG_RLI* rli, byte* buf, size_t len);

int PackMessage(MSG_APS* asp, byte* buf, size_t len);

int PackMessage(MSG_RPS* rsp, byte* buf, size_t len);

// 短整型大小端互换
#define BigLittleSwap16(A)        ((((ushort)(A) & 0xff00) >> 8) | \
	(((ushort)(A) & 0x00ff) << 8))

// 长整型大小端互换
#define BigLittleSwap32(A)        ((((uint)(A) & 0xff000000) >> 24) | \
	(((uint)(A) & 0x00ff0000) >> 8) | \
	(((uint)(A) & 0x0000ff00) << 8) | \
	(((uint)(A) & 0x000000ff) << 24))

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
	CMessage* Parse(byte* lpszData);


private:

	CMessage* m_MessageStr;           //

	byte   m_Instruct;			  //指令			具体的指令，用来标注此数据包的功能

	void _Head(CMessage* _Messsage,byte* lpszData);

	int _Body(CMessage* _Messsage,byte* lpszData);

	void _Tail(CMessage* _Messsage,byte* lpszData,int ParamLen);

	CMessage* _MessageType();
	int  _GetMessageType();

};

