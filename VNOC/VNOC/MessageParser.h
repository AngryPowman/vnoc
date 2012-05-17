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

#include <iostream>


#define MSG_BEGIN    0x55   // 'V' 标记消息的开始
#define MSG_END		 0x43	// 'C' 标记消息的结束

#define  MSG_CLASS_BEGIN  1
#define  MSG_CLASS_END    1
#define  MSG_CLASS_LEN    4
#define	 MSG_CLASS_VER    1
#define  MSG_CLASS_SERIAL 1
#define  MSG_CLASS_GUID   16
#define  MSG_CLASS_COMMAND 1
#define  MSG_CLASS_OBL    4
#define  MSG_CLASS_PARAMCONST 1
#define  MSG_CLASS_VERIFY 2   //效验码 未定默认为2个字节
#define  MSG_CLASS_PARAM  4

#define  VER_INDEX    MSG_CLASS_LEN + MSG_CLASS_VER
#define	 SER_INDEX	  MSG_CLASS_LEN + MSG_CLASS_VER + MSG_CLASS_SERIAL
#define  GUID_INDEX   MSG_CLASS_LEN + MSG_CLASS_VER + MSG_CLASS_SERIAL + MSG_CLASS_GUID
#define  COM_INDEX	  MSG_CLASS_LEN + MSG_CLASS_VER + MSG_CLASS_SERIAL + MSG_CLASS_COMMAND + MSG_CLASS_GUID
#define	 OBL_INDEX	  MSG_CLASS_LEN + MSG_CLASS_VER + MSG_CLASS_SERIAL + MSG_CLASS_GUID + MSG_CLASS_COMMAND + MSG_CLASS_OBL
#define  PAC_INDEX    MSG_CLASS_LEN + MSG_CLASS_VER + MSG_CLASS_SERIAL + MSG_CLASS_GUID + MSG_CLASS_COMMAND + MSG_CLASS_OBL + MSG_CLASS_PARAMCONST

typedef  unsigned char byte;	
typedef  unsigned int  uint;

class MessageParser
{
public:

	MessageParser(){
		memset(nGUID,0,17);
		nCmlListLen  = NULL;
		nCmlCommandList = NULL;
	}

	int Parse(byte* lpszData);

	~MessageParser(){

	}

protected:

	bool returnBeginTab(){
		return nBegin;
	} 

	bool returnEndTab(){
		return nBegin;
	}
	
	byte returnCommand(){
		return nCommand;
	}

	byte* returnCmlListLen(){
		return  nCmlListLen;
	}

	byte returnSerial(){
		return nSerial;
	}

	byte* returnGUID(){
		return nGUID;
	}

	byte** returnCmlCommandList(){
		return nCmlCommandList;
	}

	uint returnVerify(){
		return nVerify;
	}

	uint returnObligate(){
		return nObligate;
	}

	uint returnVersion(){
		return nVer;
	}
	
	uint returnCmlCount(){
		return nCmlCount;
	}

	uint returnDataLen(){
		return nLen;
	}

	uint   byteToInt(byte* in_byte);

private:

	
	bool  nBegin;				  //标记消息的开始  统一字符'V"的ASCⅡ码 0x55
	bool  nEnd;					  //标记消息的结束  固定字符'C‘的ASCⅡ码 0x43

	byte   nCommand;			  //指令			具体的指令，用来标注此数据包的功能
	byte*  nCmlListLen;			  //参数列表    4字节，对应参数N的长度
	byte   nSerial;				  //序号       指令的编号
	byte   nGUID[17];             //GUID       用来提供用户验证
	byte** nCmlCommandList;       //参数列表    编码后的参数，具体类型根据具体指令决定
	//DWORD dwType;				  //消息类型		


	uint nVerify;                 //效验码 预订

	uint  nObligate;              //预留空间
	uint  nVer;					  //版本号     标志本VNOC协议的版本
	uint  nCmlCount;			  //参数数量    编码后的参数，具体类型根据具体指令决定
	uint  nLen;                   //包体长度	   4个字节，用于指定包体的长度（可以辅助数据解析）

private:

	void   Close();
};

