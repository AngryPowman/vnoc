
#pragma once

#include "MessageParser.h"

//CMessageParser

CMessage* CMessageParser::_MessageType()
{
	CMessage* msg_str = NULL;
	switch (_GetMessageType())
	{
	case MSG_AVC_TYPE: msg_str = new MSG_AVC; 
		break;
	case MSG_ALI_TYPE: msg_str = new MSG_ALI; 
		break;
	case MSG_RVC_TYPE: msg_str = new MSG_RVC; 
		break;
	case MSG_RLI_TYPE: msg_str = new MSG_RLI;
		break;
	case MSG_RPS_TYPE: msg_str = new MSG_RPS;
	    break;
	case MSG_APS_TYPE: msg_str = new MSG_APS;
		break;
	}
	return msg_str;
}

int  CMessageParser::_GetMessageType()
{
	int  _type = 0;
	switch(m_Instruct)
	{
	case MSG_AVC_COM: _type =  MSG_AVC_TYPE; 
		break;
	case MSG_ALI_COM: _type =  MSG_ALI_TYPE; 
		break;
	case MSG_RVC_COM: _type =  MSG_RVC_TYPE; 
		break;
	case MSG_RLI_COM: _type =  MSG_RLI_TYPE;
		break;
	case MSG_RPS_COM: _type =  MSG_RPS_TYPE;
		break;
	case MSG_APS_COM: _type =  MSG_APS_TYPE;
		break;
	}
	return _type;
}

void CMessageParser::_Head(CMessage* _Messsage,byte* lpszData)
{
	int    nPos      = 0;
	byte  szTmpObligate[4] = {0};
	byte  szTmpDataLen[4] = {0};

	if (_Messsage == NULL)
	{
		return;
	}


	if(lpszData == NULL)
	{
		return;
	}

	if(lpszData[0] != MSG_BEGIN )
	{
	  _Messsage->m_Begin = false;
	  return;
	}
	//匹配标记成功 
	_Messsage->m_Begin = true;

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
	_Messsage->m_Len = byteToInt(szTmpDataLen);

	//取版本号
	_Messsage->m_Ver = (int)lpszData[VER_INDEX];

	//取序号
	_Messsage->m_Serial = (int)lpszData[SER_INDEX];

	//获取GUID           大端存放
	for(nPos = 0;nPos < MSG_CLASS_GUID; nPos++)
	{
		_Messsage->m_GUID[nPos] = lpszData[(GUID_INDEX)- nPos];
	}
	_Messsage->m_Command = (int)lpszData[COM_INDEX];

	//获取保留字           大端存放
	for(nPos = 0;nPos < MSG_CLASS_OBL; nPos++)
	{
		szTmpObligate[nPos] = lpszData[OBL_INDEX - nPos];
	}
	_Messsage->m_Obligate = byteToInt(szTmpObligate);

	//获取参数个数
	_Messsage->m_CmlCount = (int)lpszData[PAC_INDEX];

}

int CMessageParser::_Body(CMessage* _Messsage,byte* lpszData)
{
	int   nPos   = 0;
	if (_Messsage == NULL)
	{
		return 0;
	}


	if(lpszData == NULL)
	{
		return 0;
	}
	
	//预设空间
	_Messsage->m_CmlCommandList = new byte*[_Messsage->m_CmlCount];

	//处理参数
	_Messsage->m_CmlListLen = new byte[MSG_CLASS_PARAM * _Messsage->m_CmlCount + 1];
	memset(_Messsage->m_CmlListLen,0,sizeof(byte) * (MSG_CLASS_PARAM * _Messsage->m_CmlCount) + 1);
	for(nPos = 0;nPos < (int)(MSG_CLASS_PARAM * _Messsage->m_CmlCount); nPos++)
	{
		_Messsage->m_CmlListLen[nPos] = lpszData[(PAC_INDEX + (MSG_CLASS_PARAM * _Messsage->m_CmlCount)) - nPos];
	}

	int* tmpCmlListLen = new int[_Messsage->m_CmlCount];

	byte tmpComlLen[MSG_CLASS_PARAM + 1] = {0};

	//参数处理
	//计算参数所占位数及位置
	int VerifyPos = _Messsage->m_CmlCount * MSG_CLASS_PARAM;
	int TmpIndex = 0;
	int ParamLen = 0;
	int ParamPos = 0;
	for(int i = 0;i < (int)(_Messsage->m_CmlCount);i++)
	{
		int j = i * 4;
		for (int index = 0; index < MSG_CLASS_PARAM; j++,index++)
		{
			tmpComlLen[index] = _Messsage->m_CmlListLen[j];
		}
		ParamLen += byteToInt(tmpComlLen);
		memset(tmpComlLen,0,MSG_CLASS_PARAM + 1);
	}

	for(int i = 0;i < (int)(_Messsage->m_CmlCount);i++)
	{
		int j = i * 4;
		for (int index = 0; index < MSG_CLASS_PARAM; j++,index++)
		{
			tmpComlLen[index] = _Messsage->m_CmlListLen[j];
		}

		if (tmpComlLen != NULL)
		{

			tmpCmlListLen[i] = byteToInt(tmpComlLen);
			//取参数

			memset(tmpComlLen,0,MSG_CLASS_PARAM + 1);

			_Messsage->m_CmlCommandList[i] = new byte[tmpCmlListLen[i] + 1];
			memset(_Messsage->m_CmlCommandList[i],0,tmpCmlListLen[i] + 1);
			//- (tmpCmlListLen[i] * i)

			if (i == 0)
			{
				TmpIndex = (PAC_INDEX + (MSG_CLASS_PARAM * _Messsage->m_CmlCount) + ParamLen);
			}
			else
			{
				ParamPos += tmpCmlListLen[i - 1];
				TmpIndex = ((PAC_INDEX + (MSG_CLASS_PARAM * _Messsage->m_CmlCount) + ParamLen) -  ParamPos);
			}

			for (int index = 0; index < tmpCmlListLen[i]; index++ )
			{
				_Messsage->m_CmlCommandList[i][index] = lpszData[TmpIndex - index];
				VerifyPos++;
			}
		}
	}

	if (tmpCmlListLen != NULL)
	{
		delete [] tmpCmlListLen;
		tmpCmlListLen = NULL;
	}

	return VerifyPos;
}


void CMessageParser::_Tail(CMessage* _Messsage,byte* lpszData,int ParamLen)
{
	int   nPos   = 0;
	byte szTmpVerify[4] = {0};
	if (_Messsage == NULL)
	{
		return;
	}


	if(lpszData == NULL)
	{
		return;
	}

	if(ParamLen == 0)
	{
		return;
	}

	//取出效验码
	for(nPos = 0;nPos < MSG_CLASS_VERIFY; nPos++)
	{
		szTmpVerify[nPos] = lpszData[(ParamLen + MSG_CLASS_VERIFY + PAC_INDEX)- nPos];
	}

	_Messsage->m_Verify = byteToInt(szTmpVerify);


	if (lpszData[ParamLen + MSG_CLASS_VERIFY + PAC_INDEX + MSG_CLASS_END] != MSG_END)
	{
		_Messsage->m_End = false;
	}
	else
	{
		_Messsage->m_End = true;
	}
}

CMessage* CMessageParser::Parse(byte* lpszData)
{

	//开始标记匹配
	if(lpszData[0] != MSG_BEGIN )
	{
		return 0;
	}
	//获取指令
	m_Instruct = (int)lpszData[COM_INDEX];

	m_MessageStr = _MessageType();
	int ParamLen = 0;

	_Head(m_MessageStr,lpszData);
	ParamLen = _Body(m_MessageStr,lpszData);
	_Tail(m_MessageStr,lpszData,ParamLen);

	return m_MessageStr;
}
