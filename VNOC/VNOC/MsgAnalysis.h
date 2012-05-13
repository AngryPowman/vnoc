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
	//开始标记匹配
	if(lpszData[0] != MSG_BEGIN )
	{
	  msg_class->nBegin = false;
	  return -1;
	}
	//匹配标记成功 
	msg_class->nBegin = true;

	//获取包体长度           小端存放
/*
 	for(nPos = 0;nPos < MSG_CLASS_LEN; nPos++)
	{
	  	szTmpData[nPos] = lpszData[nPos + 1];
	}
*/
	//获取包体长度           大端存放
	for(nPos = 0;nPos < MSG_CLASS_LEN; nPos++)
	{
		szTmpDataLen[nPos] = lpszData[MSG_CLASS_LEN- nPos];
	}

	//转换
	msg_class->nLen = byteToInt(szTmpDataLen);

	//取版本号
	msg_class->nVer = (int)lpszData[VER_INDEX];

	//取序号
	msg_class->nSerial = (int)lpszData[SER_INDEX];

	//获取GUID           大端存放
	for(nPos = 0;nPos < MSG_CLASS_GUID; nPos++)
	{
		msg_class->nGUID[nPos] = lpszData[(GUID_INDEX)- nPos];
	}
	msg_class->nCommand = (int)lpszData[COM_INDEX];

	//获取保留字           大端存放
	for(nPos = 0;nPos < MSG_CLASS_OBL; nPos++)
	{
		szTmpObligate[nPos] = lpszData[OBL_INDEX - nPos];
	}
	msg_class->nObligate = byteToInt(szTmpObligate);

	//获取参数个数
	msg_class->nCmlConst = (int)lpszData[PAC_INDEX];

	//预设空间
	msg_class->nCmlCommandList = new byte*[msg_class->nCmlConst];

	//处理参数
	msg_class->nCmlListLen = new byte[MSG_CLASS_PARAM * msg_class->nCmlConst + 1];
	memset(msg_class->nCmlListLen,0,sizeof(byte) * (MSG_CLASS_PARAM * msg_class->nCmlConst) + 1);
	for(nPos = 0;nPos < (int)(MSG_CLASS_PARAM * msg_class->nCmlConst); nPos++)
	{
		msg_class->nCmlListLen[nPos] = lpszData[(PAC_INDEX + (MSG_CLASS_PARAM * msg_class->nCmlConst)) - nPos];
	}

	int* tmpCmlListLen = new int[msg_class->nCmlConst];

	byte tmpComlLen[MSG_CLASS_PARAM + 1] = {0};

	//参数处理
	//计算参数所占位数及位置
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
			//取参数

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

	//取出效验码
	for(nPos = 0;nPos < MSG_CLASS_VERIFY; nPos++)
	{
		szTmpVerify[nPos] = lpszData[(VerifyPos + MSG_CLASS_VERIFY + PAC_INDEX)- nPos];
	}

	msg_class->nVerify = byteToInt(szTmpVerify);

	//判断标记尾

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
