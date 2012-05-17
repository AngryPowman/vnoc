/********************************************************************
	created:	2012/05/13
	created:	13:05:2012   00:46
	filename: 	D:\vnoc\trunk\VNOC\VNOC\MsgAnalysis.h
	file path:	D:\vnoc\trunk\VNOC\VNOC\MsgAnalysis.h
	file base:	Message Analysis
	file ext:	h
	author:		Sum.
	
	purpose:	���ݰ������࣬������������(�ͻ���)
*********************************************************************/


#pragma once

#include <iostream>


#define MSG_BEGIN    0x55   // 'V' �����Ϣ�Ŀ�ʼ
#define MSG_END		 0x43	// 'C' �����Ϣ�Ľ���

#define  MSG_CLASS_BEGIN  1
#define  MSG_CLASS_END    1
#define  MSG_CLASS_LEN    4
#define	 MSG_CLASS_VER    1
#define  MSG_CLASS_SERIAL 1
#define  MSG_CLASS_GUID   16
#define  MSG_CLASS_COMMAND 1
#define  MSG_CLASS_OBL    4
#define  MSG_CLASS_PARAMCONST 1
#define  MSG_CLASS_VERIFY 2   //Ч���� δ��Ĭ��Ϊ2���ֽ�
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

	
	bool  nBegin;				  //�����Ϣ�Ŀ�ʼ  ͳһ�ַ�'V"��ASC���� 0x55
	bool  nEnd;					  //�����Ϣ�Ľ���  �̶��ַ�'C����ASC���� 0x43

	byte   nCommand;			  //ָ��			�����ָ�������ע�����ݰ��Ĺ���
	byte*  nCmlListLen;			  //�����б�    4�ֽڣ���Ӧ����N�ĳ���
	byte   nSerial;				  //���       ָ��ı��
	byte   nGUID[17];             //GUID       �����ṩ�û���֤
	byte** nCmlCommandList;       //�����б�    �����Ĳ������������͸��ݾ���ָ�����
	//DWORD dwType;				  //��Ϣ����		


	uint nVerify;                 //Ч���� Ԥ��

	uint  nObligate;              //Ԥ���ռ�
	uint  nVer;					  //�汾��     ��־��VNOCЭ��İ汾
	uint  nCmlCount;			  //��������    �����Ĳ������������͸��ݾ���ָ�����
	uint  nLen;                   //���峤��	   4���ֽڣ�����ָ������ĳ��ȣ����Ը������ݽ�����

private:

	void   Close();
};

