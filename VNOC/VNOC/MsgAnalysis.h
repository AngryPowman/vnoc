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

#include "MsgDef.h"

class MsgAnalysis
{
public:

	MsgAnalysis(){}
	MsgAnalysis(MSG_CLASS* msg_class,byte* lpszData){
		Analysis(msg_class,lpszData);
	}

	int Analysis(MSG_CLASS* msg_class,byte* lpszData);

	~MsgAnalysis(){}

private:

	int   byteToInt(byte* in_byte);

};
