
#pragma once

#include "MessageParser.h"


//PackMessage
int PackMessage(MSG_RVC* rvc,byte* buf, size_t len)
{
	
	//head---------------------
	int index = 0;
	int tmpInt = 0;
	byte tmpByte[4] = {0};
	if (buf == NULL)
	{
		return -2;
	}

// 	if (len < rvc->GetDataLen())
// 	{
// 		return -1;
// 	}
	//忽视头尾标记  自动填充
	// 	if ((avc->GetBeginTab() != true ) && (avc->GetEndTab() != true))
	// 	{
	// 		return -1;
	// 	}

	buf[0] = MSG_BEGIN;
	tmpInt = rvc->GetDataLen();
	IntTobyte(rvc->GetDataLen(),tmpByte);
	for (index = 1; index < 5; index++ )
	{
		buf[index] = tmpByte[index - 1];
	}
	buf[index] = rvc->GetVersion();
	index++;
	buf[index] = rvc->GetSerial();
	index++;
	//GUID
	for (int i = 0; i < 16; index++, i++)
	{
		buf[index] = rvc->GetGUID()[i];
	}
	//指令
	buf[index] = rvc->GetCommand();
	index++;
	//保留位置
	memset(tmpByte,0,4);
	IntTobyte(rvc->GetObligate(),tmpByte);
	for (int i = 0; i < 4; index++ , i++)
	{
		buf[index] = tmpByte[ i ];
	}
	//参数数量
	buf[index]  = rvc->GetCmlCount();
	index++;

	//body
	//参数长度
	for (int i = 0; i < (int)(rvc->GetCmlCount() * 4); i++, index++)
	{
		buf[index] = rvc->GetCmlListLen()[i];
	}
	//参数
	//1
	for (int i = 0; i < rvc->GetMachineAddressLen(); i++, index++)
	{
		buf[index] = rvc->GetCmlCommandList()[0][i];
	}

	//Tail
	//效验码
	memset(tmpByte,0,4);
	IntTobyte(rvc->GetVerify(),tmpByte);
	//因为效验码预订2位 所以只取低位
	for (int i = 2; i < 4; index++ , i++)
	{
		buf[index] = tmpByte[ i ];
	}
	//结尾符
	buf[index] = MSG_END;


	return 0;
}


int PackMessage(MSG_AVC* avc, byte* buf, size_t len)
{
	//head---------------------
	int index = 0;
	int tmpInt = 0;
	byte tmpByte[4] = {0};
	if (buf == NULL)
	{
		return -2;
	}

// 	if (len < avc->GetDataLen())
// 	{
// 		return -1;
// 	}
	//忽视头尾标记  自动填充
// 	if ((avc->GetBeginTab() != true ) && (avc->GetEndTab() != true))
// 	{
// 		return -1;
// 	}

	buf[0] = MSG_BEGIN;
	tmpInt = avc->GetDataLen();
	IntTobyte(avc->GetDataLen(),tmpByte);
	for (index = 1; index < 5; index++ )
	{
		buf[index] = tmpByte[index - 1];
	}
	buf[index] = avc->GetVersion();
	index++;
	buf[index] = avc->GetSerial();
	index++;
	//GUID
	for (int i = 0; i < 16; index++, i++)
	{
		buf[index] = avc->GetGUID()[i];
	}
	//指令
	buf[index] = avc->GetCommand();
	index++;
	//保留位置
	memset(tmpByte,0,4);
	IntTobyte(avc->GetObligate(),tmpByte);
	for (int i = 0; i < 4; index++ , i++)
	{
		buf[index] = tmpByte[ i ];
	}
	//参数数量
	buf[index]  = avc->GetCmlCount();
	index++;

	//body
	//参数长度
	for (int i = 0; i < (int)(avc->GetCmlCount() * 4); i++, index++)
	{
		buf[index] = avc->GetCmlListLen()[i];
	}
	//参数部分
	//1
	buf[index] = avc->GetCmlCommandList()[2][0];
	index++;
	//2
	buf[index] = avc->GetCmlCommandList()[1][0];
	index++;
	//3
	for (int i = 0; i < avc->GetCaptchaLen(); i++, index++)
	{
		buf[index] = avc->GetCmlCommandList()[0][i];
	}
	//Tail
	//效验码
	memset(tmpByte,0,4);
	IntTobyte(avc->GetVerify(),tmpByte);
	//因为效验码预订2位 所以只取低位
	for (int i = 2; i < 4; index++ , i++)
	{
		buf[index] = tmpByte[ i ];
	}
	//结尾符
	buf[index] = MSG_END;

	return 0;
}


int PackMessage(MSG_RLI* rli,byte* buf, size_t len)
{
	//head---------------------
	int index = 0;
	int tmpInt = 0;
	byte tmpByte[4] = {0};
	if (buf == NULL)
	{
		return -2;
	}

	// 	if (len < avc->GetDataLen())
	// 	{
	// 		return -1;
	// 	}
	//忽视头尾标记  自动填充
	// 	if ((avc->GetBeginTab() != true ) && (avc->GetEndTab() != true))
	// 	{
	// 		return -1;
	// 	}

	buf[0] = MSG_BEGIN;
	tmpInt = rli->GetDataLen();
	IntTobyte(rli->GetDataLen(),tmpByte);
	for (index = 1; index < 5; index++ )
	{
		buf[index] = tmpByte[index - 1];
	}
	buf[index] = rli->GetVersion();
	index++;
	buf[index] = rli->GetSerial();
	index++;
	//GUID
	for (int i = 0; i < 16; index++, i++)
	{
		buf[index] = rli->GetGUID()[i];
	}
	//指令
	buf[index] = rli->GetCommand();
	index++;
	//保留位置
	memset(tmpByte,0,4);
	IntTobyte(rli->GetObligate(),tmpByte);
	for (int i = 0; i < 4; index++ , i++)
	{
		buf[index] = tmpByte[ i ];
	}
	//参数数量
	buf[index]  = rli->GetCmlCount();
	index++;

	//参数部分
	//1
	for (int i = 0; i < rli->GetVerificationCodeLen(); i++, index++)
	{
		buf[index] = rli->GetCmlCommandList()[2][i];
	}
	//2
	for (int i = 0; i < rli->GetAccountNumberLen(); i++, index++)
	{
		buf[index] = rli->GetCmlCommandList()[1][i];
	}
	//3
	for (int i = 0; i < rli->GetPasswordLen(); i++, index++)
	{
		buf[index] = rli->GetCmlCommandList()[0][i];
	}

	//Tail
	//效验码
	memset(tmpByte,0,4);
	IntTobyte(rli->GetVerify(),tmpByte);
	//因为效验码预订2位 所以只取低位
	for (int i = 2; i < 4; index++ , i++)
	{
		buf[index] = tmpByte[ i ];
	}
	//结尾符
	buf[index] = MSG_END;

	return 0;
}

int PackMessage(MSG_ALI* ali,byte* buf, size_t len)
{
	//head---------------------
	int index = 0;
	int tmpInt = 0;
	byte tmpByte[4] = {0};
	if (buf == NULL)
	{
		return -2;
	}

	// 	if (len < avc->GetDataLen())
	// 	{
	// 		return -1;
	// 	}
	//忽视头尾标记  自动填充
	// 	if ((avc->GetBeginTab() != true ) && (avc->GetEndTab() != true))
	// 	{
	// 		return -1;
	// 	}

	buf[0] = MSG_BEGIN;
	tmpInt = ali->GetDataLen();
	IntTobyte(ali->GetDataLen(),tmpByte);
	for (index = 1; index < 5; index++ )
	{
		buf[index] = tmpByte[index - 1];
	}
	buf[index] = ali->GetVersion();
	index++;
	buf[index] = ali->GetSerial();
	index++;
	//GUID
	for (int i = 0; i < 16; index++, i++)
	{
		buf[index] = ali->GetGUID()[i];
	}
	//指令
	buf[index] = ali->GetCommand();
	index++;
	//保留位置
	memset(tmpByte,0,4);
	IntTobyte(ali->GetObligate(),tmpByte);
	for (int i = 0; i < 4; index++ , i++)
	{
		buf[index] = tmpByte[ i ];
	}
	//参数数量
	buf[index]  = ali->GetCmlCount();
	index++;

	//参数部分
	//1
	buf[index] = ali->GetCmlCommandList()[2][0];
	index++;
	//2
	for (int i = 0; i < ali->GetTokenLen(); i++, index++)
	{
		buf[index] = ali->GetCmlCommandList()[1][i];
	}
	//3
	for (int i = 0; i < ali->GetTokenLen(); i++, index++)
	{
		buf[index] = ali->GetCmlCommandList()[0][i];
	}

	//Tail
	//效验码
	memset(tmpByte,0,4);
	IntTobyte(ali->GetVerify(),tmpByte);
	//因为效验码预订2位 所以只取低位
	for (int i = 2; i < 4; index++ , i++)
	{
		buf[index] = tmpByte[ i ];
	}
	//结尾符
	buf[index] = MSG_END;

	return 0;

}


int PackMessage(MSG_RPS* rps,byte* buf, size_t len)
{
	//head---------------------
	int index = 0;
	int tmpInt = 0;
	byte tmpByte[4] = {0};
	if (buf == NULL)
	{
		return -2;
	}

	// 	if (len < avc->GetDataLen())
	// 	{
	// 		return -1;
	// 	}
	//忽视头尾标记  自动填充
	// 	if ((avc->GetBeginTab() != true ) && (avc->GetEndTab() != true))
	// 	{
	// 		return -1;
	// 	}

	buf[0] = MSG_BEGIN;
	tmpInt = rps->GetDataLen();
	IntTobyte(rps->GetDataLen(),tmpByte);
	for (index = 1; index < 5; index++ )
	{
		buf[index] = tmpByte[index - 1];
	}
	buf[index] = rps->GetVersion();
	index++;
	buf[index] = rps->GetSerial();
	index++;
	//GUID
	for (int i = 0; i < 16; index++, i++)
	{
		buf[index] = rps->GetGUID()[i];
	}
	//指令
	buf[index] = rps->GetCommand();
	index++;
	//保留位置
	memset(tmpByte,0,4);
	IntTobyte(rps->GetObligate(),tmpByte);
	for (int i = 0; i < 4; index++ , i++)
	{
		buf[index] = tmpByte[ i ];
	}
	//参数数量
	buf[index]  = rps->GetCmlCount();
	index++;

	//参数部分
	//1
	buf[index] = rps->GetCmlCommandList()[4][0];
	index++;
	//2
	for (int i = 0; i < rps->GetNicknameLen(); i++, index++)
	{
		buf[index] = rps->GetCmlCommandList()[3][i];
	}
	//3
	for (int i = 0; i < rps->GetAutographLen(); i++, index++)
	{
		buf[index] = rps->GetCmlCommandList()[2][i];
	}
	//4
	buf[index] = rps->GetCmlCommandList()[1][0];
	index++;
	//5
	for (int i = 0; i < rps->GetHeadPortraitLen(); i++, index++)
	{
		buf[index] = rps->GetCmlCommandList()[0][i];
	}

	//Tail
	//效验码
	memset(tmpByte,0,4);
	IntTobyte(rps->GetVerify(),tmpByte);
	//因为效验码预订2位 所以只取低位
	for (int i = 2; i < 4; index++ , i++)
	{
		buf[index] = tmpByte[ i ];
	}
	//结尾符
	buf[index] = MSG_END;

	return 0;

}


int PackMessage(MSG_APS* aps,byte* buf, size_t len)
{
	//head---------------------
	int index = 0;
	int tmpInt = 0;
	byte tmpByte[4] = {0};
	if (buf == NULL)
	{
		return -2;
	}

	// 	if (len < avc->GetDataLen())
	// 	{
	// 		return -1;
	// 	}
	//忽视头尾标记  自动填充
	// 	if ((avc->GetBeginTab() != true ) && (avc->GetEndTab() != true))
	// 	{
	// 		return -1;
	// 	}

	buf[0] = MSG_BEGIN;
	tmpInt = aps->GetDataLen();
	IntTobyte(aps->GetDataLen(),tmpByte);
	for (index = 1; index < 5; index++ )
	{
		buf[index] = tmpByte[index - 1];
	}
	buf[index] = aps->GetVersion();
	index++;
	buf[index] = aps->GetSerial();
	index++;
	//GUID
	for (int i = 0; i < 16; index++, i++)
	{
		buf[index] = aps->GetGUID()[i];
	}
	//指令
	buf[index] = aps->GetCommand();
	index++;
	//保留位置
	memset(tmpByte,0,4);
	IntTobyte(aps->GetObligate(),tmpByte);
	for (int i = 0; i < 4; index++ , i++)
	{
		buf[index] = tmpByte[ i ];
	}
	//参数数量
	buf[index]  = aps->GetCmlCount();
	index++;

	//参数部分
	//1
	for (int i = 0; i < aps->GetMessageSynchroLen(); i++, index++)
	{
		buf[index] = aps->GetCmlCommandList()[0][i];
	}

	//Tail
	//效验码
	memset(tmpByte,0,4);
	IntTobyte(aps->GetVerify(),tmpByte);
	//因为效验码预订2位 所以只取低位
	for (int i = 2; i < 4; index++ , i++)
	{
		buf[index] = tmpByte[ i ];
	}
	//结尾符
	buf[index] = MSG_END;

	return 0;
}


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
