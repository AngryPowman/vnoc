
#pragma once

#include "msganalysis.h"

void MsgAnalysis::Close()
{
	if (nCmlCommandList)
	{
		for (int count = 0; (int)nCmlCount < count; count++)
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

	nCmlCount = NULL;
}

int   MsgAnalysis::byteToInt(byte* in_byte)
{
	if (in_byte == NULL)
	{
		return 0;
	}

	if (strcmp((const char*)in_byte,"") == 0)
	{
		return 0;
	}
	int*  lpDataLen = NULL; 
	lpDataLen = (int *)in_byte;
	return (*lpDataLen);
}

int   MsgAnalysis::Analysis(byte* lpszData)
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
	  this->nBegin = false;
	  return -1;
	}
	//匹配标记成功 
	this->nBegin = true;

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
	this->nLen = byteToInt(szTmpDataLen);

	//取版本号
	this->nVer = (int)lpszData[VER_INDEX];

	//取序号
	this->nSerial = (int)lpszData[SER_INDEX];

	//获取GUID           大端存放
	for(nPos = 0;nPos < MSG_CLASS_GUID; nPos++)
	{
		this->nGUID[nPos] = lpszData[(GUID_INDEX)- nPos];
	}
	this->nCommand = (int)lpszData[COM_INDEX];

	//获取保留字           大端存放
	for(nPos = 0;nPos < MSG_CLASS_OBL; nPos++)
	{
		szTmpObligate[nPos] = lpszData[OBL_INDEX - nPos];
	}
	this->nObligate = byteToInt(szTmpObligate);

	//获取参数个数
	this->nCmlCount = (int)lpszData[PAC_INDEX];

	//预设空间
	this->nCmlCommandList = new byte*[this->nCmlCount];

	//处理参数
	this->nCmlListLen = new byte[MSG_CLASS_PARAM * this->nCmlCount + 1];
	memset(this->nCmlListLen,0,sizeof(byte) * (MSG_CLASS_PARAM * this->nCmlCount) + 1);
	for(nPos = 0;nPos < (int)(MSG_CLASS_PARAM * this->nCmlCount); nPos++)
	{
		this->nCmlListLen[nPos] = lpszData[(PAC_INDEX + (MSG_CLASS_PARAM * this->nCmlCount)) - nPos];
	}

	int* tmpCmlListLen = new int[this->nCmlCount];

	byte tmpComlLen[MSG_CLASS_PARAM + 1] = {0};

	//参数处理
	//计算参数所占位数及位置
	int VerifyPos = this->nCmlCount * MSG_CLASS_PARAM;
	int TmpIndex = 0;
	int ParamLen = 0;
	int ParamPos = 0;
	for(int i = 0;i < (int)(this->nCmlCount);i++)
	{
		int j = i * 4;
		for (int index = 0; index < MSG_CLASS_PARAM; j++,index++)
		{
			tmpComlLen[index] = this->nCmlListLen[j];
		}
		ParamLen += byteToInt(tmpComlLen);
		memset(tmpComlLen,0,MSG_CLASS_PARAM + 1);
	}

	for(int i = 0;i < (int)(this->nCmlCount);i++)
	{
		int j = i * 4;
		for (int index = 0; index < MSG_CLASS_PARAM; j++,index++)
		{
			tmpComlLen[index] = this->nCmlListLen[j];
		}

		if (tmpComlLen != NULL)
		{

			tmpCmlListLen[i] = byteToInt(tmpComlLen);
			//取参数

			memset(tmpComlLen,0,MSG_CLASS_PARAM + 1);

			this->nCmlCommandList[i] = new byte[tmpCmlListLen[i] + 1];
			memset(this->nCmlCommandList[i],0,tmpCmlListLen[i] + 1);
			//- (tmpCmlListLen[i] * i)

			if (i == 0)
			{
				TmpIndex = (PAC_INDEX + (MSG_CLASS_PARAM * this->nCmlCount) + ParamLen);
			}
			else
			{
				ParamPos += tmpCmlListLen[i - 1];
				TmpIndex = ((PAC_INDEX + (MSG_CLASS_PARAM * this->nCmlCount) + ParamLen) -  ParamPos);
			}

			for (int index = 0; index < tmpCmlListLen[i]; index++ )
			{
				this->nCmlCommandList[i][index] = lpszData[TmpIndex - index];
				VerifyPos++;
			}
			//std::cout<<nCmlCommandList[i]<<std::endl;
		}
	}

	//取出效验码
	for(nPos = 0;nPos < MSG_CLASS_VERIFY; nPos++)
	{
		szTmpVerify[nPos] = lpszData[(VerifyPos + MSG_CLASS_VERIFY + PAC_INDEX)- nPos];
	}

	this->nVerify = byteToInt(szTmpVerify);

	//判断标记尾

// 	if (this->nLen != (VerifyPos + MSG_CLASS_VERIFY + PAC_INDEX + MSG_CLASS_END))
// 	{
// 		this->nEnd = false;
// 		if (tmpCmlListLen != NULL)
// 		{
// 			delete [] tmpCmlListLen;
// 			tmpCmlListLen = NULL;
// 		}
// 		return -1;
// 	}

	if (lpszData[VerifyPos + MSG_CLASS_VERIFY + PAC_INDEX + MSG_CLASS_END] != MSG_END)
	{
		this->nEnd = false;
	}
	else
	{
		this->nEnd = true;
	}
	

	if (tmpCmlListLen != NULL)
	{
		delete [] tmpCmlListLen;
		tmpCmlListLen = NULL;
	}
	return 0;
}
