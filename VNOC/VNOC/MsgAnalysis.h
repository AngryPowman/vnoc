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




int   MsgAnalysis::byteToInt(byte* in_byte)
{
	if (in_byte == NULL)
	{
		return -1;
	}

	if (strcmp((const char*)in_byte,"") == 0)
	{
		return -1;
	}
	int*  lpDataLen = NULL; 
	lpDataLen = (int *)in_byte;
	return (*lpDataLen);
}

int   MsgAnalysis::Analysis(MSG_CLASS* msg_class,byte* lpszData)
{
	byte  szTmpDataLen[4] = {0};
	byte  szTmpObligate[4] = {0};
	byte szTmpVerify[4] = {0};
	byte  szTmpGUID[16]   = {0};
	int    nPos      = 0;
	int    nVer      = 0; //
	if(lpszData == NULL)
	{
		return -1;
	}
	//��ʼ���ƥ��
	if(lpszData[0] != MSG_BEGIN )
	{
	  msg_class->nBegin = false;
	  return -1;
	}
	//ƥ���ǳɹ� 
	msg_class->nBegin = true;

	//��ȡ���峤��           С�˴��
/*
 	for(nPos = 0;nPos < MSG_CLASS_LEN; nPos++)
	{
	  	szTmpData[nPos] = lpszData[nPos + 1];
	}
*/
	//��ȡ���峤��           ��˴��
	for(nPos = 0;nPos < MSG_CLASS_LEN; nPos++)
	{
		szTmpDataLen[nPos] = lpszData[MSG_CLASS_LEN- nPos];
	}

	//ת��
	msg_class->nLen = byteToInt(szTmpDataLen);

	//ȡ�汾��
	msg_class->nVer = (int)lpszData[VER_INDEX];

	//ȡ���
	msg_class->nSerial = (int)lpszData[SER_INDEX];

	//��ȡGUID           ��˴��
	for(nPos = 0;nPos < MSG_CLASS_GUID; nPos++)
	{
		msg_class->nGUID[nPos] = lpszData[(GUID_INDEX)- nPos];
	}
	msg_class->nCommand = (int)lpszData[COM_INDEX];

	//��ȡ������           ��˴��
	for(nPos = 0;nPos < MSG_CLASS_OBL; nPos++)
	{
		szTmpObligate[nPos] = lpszData[OBL_INDEX - nPos];
	}
	msg_class->nObligate = byteToInt(szTmpObligate);

	//��ȡ��������
	msg_class->nCmlConst = (int)lpszData[PAC_INDEX];

	//Ԥ��ռ�
	msg_class->nCmlCommandList = new byte*[msg_class->nCmlConst];

	//�������
	msg_class->nCmlListLen = new byte[MSG_CLASS_PARAM * msg_class->nCmlConst + 1];
	memset(msg_class->nCmlListLen,0,sizeof(byte) * (MSG_CLASS_PARAM * msg_class->nCmlConst) + 1);
	for(nPos = 0;nPos < (int)(MSG_CLASS_PARAM * msg_class->nCmlConst); nPos++)
	{
		msg_class->nCmlListLen[nPos] = lpszData[(PAC_INDEX + (MSG_CLASS_PARAM * msg_class->nCmlConst)) - nPos];
	}

	int* tmpCmlListLen = new int[msg_class->nCmlConst];

	byte tmpComlLen[MSG_CLASS_PARAM + 1] = {0};

	//��������
	//���������ռλ����λ��
	int VerifyPos = msg_class->nCmlConst * MSG_CLASS_PARAM;

	for(int i = 0;i < (int)msg_class->nCmlConst;i++)
	{
		int j = i * 4;
		for (int index = 0; index < MSG_CLASS_PARAM; j++,index++)
		{
			tmpComlLen[index] = msg_class->nCmlListLen[j];
		}
		if (tmpComlLen != NULL)
		{

			tmpCmlListLen[i] = byteToInt(tmpComlLen);
			//ȡ����

			memset(tmpComlLen,0,MSG_CLASS_PARAM + 1);

			msg_class->nCmlCommandList[i] = new byte[tmpCmlListLen[i] + 1];
			memset(msg_class->nCmlCommandList[i],0,tmpCmlListLen[i] + 1);

			for (int index = 0; index < tmpCmlListLen[i]; index++ )
			{
				msg_class->nCmlCommandList[i][index] = lpszData[(PAC_INDEX + (MSG_CLASS_PARAM * msg_class->nCmlConst) + (tmpCmlListLen[i] * msg_class->nCmlConst)) - (tmpCmlListLen[i] * i)  - index];
				VerifyPos++;
			}
		}
	}

	//ȡ��Ч����
	for(nPos = 0;nPos < MSG_CLASS_VERIFY; nPos++)
	{
		szTmpVerify[nPos] = lpszData[(VerifyPos + MSG_CLASS_VERIFY + PAC_INDEX)- nPos];
	}

	msg_class->nVerify = byteToInt(szTmpVerify);

	//�жϱ��β

// 	if (msg_class->nLen != (VerifyPos + MSG_CLASS_VERIFY + PAC_INDEX + MSG_CLASS_END))
// 	{
// 		msg_class->nEnd = false;
// 		if (tmpCmlListLen != NULL)
// 		{
// 			delete [] tmpCmlListLen;
// 			tmpCmlListLen = NULL;
// 		}
// 		return -1;
// 	}

	if (lpszData[VerifyPos + MSG_CLASS_VERIFY + PAC_INDEX + MSG_CLASS_END] != MSG_END)
	{
		msg_class->nEnd = false;
	}
	
	msg_class->nEnd = true;

	if (tmpCmlListLen != NULL)
	{
		delete [] tmpCmlListLen;
		tmpCmlListLen = NULL;
	}
	return 0;
}
