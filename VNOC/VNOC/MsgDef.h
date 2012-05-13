
#pragma once

#include <stdio.h>

#define MSG_BEGIN    0x55   // 'V' �����Ϣ�Ŀ�ʼ
#define MSG_END		 0x43	// 'C' �����Ϣ�Ľ���

typedef  unsigned char   byte;	
typedef  unsigned int	 uint;

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

class MSG_CLASS
{
public:


	MSG_CLASS()
	{
		memset(nGUID,0,17);
		nCmlListLen  = NULL;
		nCmlCommandList = NULL;
	}

	~MSG_CLASS(){

		if (nCmlCommandList)
		{
			for (int count = 0; (int)nCmlConst < count; count++)
			{
				if (nCmlCommandList[count])
				{
					delete [] nCmlCommandList[count];
					nCmlCommandList[count] = NULL;
				}
			}

			delete nCmlCommandList;
			nCmlCommandList  = NULL;
		}

		if (nCmlListLen != NULL)
		{
			delete[] nCmlListLen;
			nCmlListLen = NULL;
		}

		nCmlConst = NULL;
	}

public:

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
	uint  nCmlConst;			  //��������    �����Ĳ������������͸��ݾ���ָ�����
	uint  nLen;                   //���峤��	   4���ֽڣ�����ָ������ĳ��ȣ����Ը������ݽ�����

};