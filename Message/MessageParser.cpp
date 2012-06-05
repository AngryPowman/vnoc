
#include "MessageParser.h"

#include <cstring>

using namespace std;

#define  CHECLUP_LEN(X,Y) if (_CheckUpLen(X,Y) != 0){return -1;}


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

int CMessageParser::_Head(CMessage* _Messsage,byte* lpszData,size_t len)
{
	int    nPos      = 0;
	byte  szTmpObligate[4] = {0};
	byte  szTmpDataLen[4] = {0};

	if (_Messsage == NULL)
	{
		return -1;
	}


	if(lpszData == NULL)
	{
		return -1;
	}

	if(lpszData[0] != MSG_BEGIN )
	{
	  _Messsage->m_Begin = false;
	  return -1;
	}
	//匹配标记成功 
	_Messsage->m_Begin = true;


	//取版本号
	CHECLUP_LEN(VER_INDEX + 1, len );
	_Messsage->m_Ver = (int)lpszData[VER_INDEX];

	//取序号
	CHECLUP_LEN(SER_INDEX + 1, len );
	//_Messsage->m_Serial = (int)lpszData[SER_INDEX];
	for (int index = 0; index < 2; index++)
	{
		_Messsage->m_Serial[index] = (int)lpszData[SER_INDEX - index];
	}

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
		szTmpDataLen[nPos] = lpszData[LEN_INDEX- nPos];
	}

	//转换
	CHECLUP_LEN(MSG_CLASS_LEN + 1, len );
	_Messsage->m_Len = byteToInt(szTmpDataLen,4);


	CHECLUP_LEN(COM_INDEX + 1, len );
	//获取GUID           大端存放
	for(nPos = 0;nPos < MSG_CLASS_GUID; nPos++)
	{
		_Messsage->m_GUID[nPos] = lpszData[(GUID_INDEX)- nPos];
	}
	_Messsage->m_Command = (int)lpszData[COM_INDEX];

	CHECLUP_LEN(OBL_INDEX + 1, len );
	//获取保留字           大端存放
	for(nPos = 0;nPos < MSG_CLASS_OBL; nPos++)
	{
		szTmpObligate[nPos] = lpszData[OBL_INDEX - nPos];
	}
	_Messsage->m_Obligate = byteToInt(szTmpObligate,4);

	CHECLUP_LEN(PAC_INDEX + 1, len );
	//获取参数个数
	_Messsage->m_CmlCount = (int)lpszData[PAC_INDEX];
	return 0;
}

int CMessageParser::_Body(CMessage* _Messsage,byte* lpszData,size_t len)
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
	CHECLUP_LEN((PAC_INDEX + (MSG_CLASS_PARAM * _Messsage->m_CmlCount)) + 1, len );

	_Messsage->m_CmlListLen = new byte[MSG_CLASS_PARAM * _Messsage->m_CmlCount + 1];
	memset(_Messsage->m_CmlListLen,0,sizeof(byte) * (MSG_CLASS_PARAM * _Messsage->m_CmlCount) + 1);
	for(nPos = 0;nPos < (int)(MSG_CLASS_PARAM * _Messsage->m_CmlCount); nPos++)
	{
		_Messsage->m_CmlListLen[nPos] = lpszData[(PAC_INDEX + (MSG_CLASS_PARAM * _Messsage->m_CmlCount)) - nPos];
	}

	int* tmpCmlListLen = new int[_Messsage->m_CmlCount];

	byte tmpComlLen[MSG_CLASS_PARAM] = {0};

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
		ParamLen += byteToInt(tmpComlLen,4);
		memset(tmpComlLen,0,MSG_CLASS_PARAM);
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

			tmpCmlListLen[i] = byteToInt(tmpComlLen,4);
			//取参数
			CHECLUP_LEN((PAC_INDEX + (MSG_CLASS_PARAM * _Messsage->m_CmlCount) + ParamLen) + 1, len );

			memset(tmpComlLen,0,MSG_CLASS_PARAM);

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


int CMessageParser::_Tail(CMessage* _Messsage,byte* lpszData,int ParamLen,size_t len)
{
	int   nPos   = 0;
	byte szTmpVerify[4] = {0};
	if (_Messsage == NULL)
	{
		return -1;
	}


	if(lpszData == NULL)
	{
		return -1;
	}

	if(ParamLen == 0)
	{
		return -1;
	}

	//取出效验码
	CHECLUP_LEN((ParamLen + MSG_CLASS_VERIFY + PAC_INDEX) + 1, len );

	for(nPos = 0;nPos < MSG_CLASS_VERIFY; nPos++)
	{
		szTmpVerify[nPos] = lpszData[(ParamLen + MSG_CLASS_VERIFY + PAC_INDEX)- nPos];
	}

	_Messsage->m_Verify = byteToInt(szTmpVerify,2);

	CHECLUP_LEN((ParamLen + MSG_CLASS_VERIFY + PAC_INDEX + MSG_CLASS_END) +1, len );

	if (lpszData[ParamLen + MSG_CLASS_VERIFY + PAC_INDEX + MSG_CLASS_END] != MSG_END)
	{
		_Messsage->m_End = false;
	}
	else
	{
		_Messsage->m_End = true;
	}
	return 0;
}

CMessage* CMessageParser::Parse(byte* lpszData,size_t len)
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

	_Head(m_MessageStr,lpszData,len);
	ParamLen = _Body(m_MessageStr,lpszData,len);
	_Tail(m_MessageStr,lpszData,ParamLen,len);

	return m_MessageStr;
}
