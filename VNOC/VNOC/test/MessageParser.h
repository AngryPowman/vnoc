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

