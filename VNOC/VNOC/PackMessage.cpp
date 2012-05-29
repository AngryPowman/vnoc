#pragma once

#include "PackMessage.h"

int PackMessage::_Head(CMessage* msg_clss,byte* buf, size_t len)
{
	//head---------------------
	int index = 0;
	int tmpInt = 0;
	byte tmpByte[4] = {0};
	byte tmpByteTwo[2] = {0};
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
	index++;

	CHECKUP_DATALEN(index,len);
	buf[index] = msg_clss->GetVersion();
	index++;
	CHECKUP_DATALEN(index,len);
	// = msg_clss->GetSerial();
	memset(tmpByte,0,4);
	IntTobyte(msg_clss->GetSerial(),tmpByte);
	//因为效验码预订2位 所以只取低位
	CHECKUP_DATALEN(index,len);
	for (int i = 2; i < 4; index++ , i++)
	{
		buf[index] = tmpByte[ i ];
	}

	memset(tmpByte,0,4);
	IntTobyte(msg_clss->GetDataLen(),tmpByte);
	CHECKUP_DATALEN(index,len);
	for (int record = 0; record < 4; index++,record++ )
	{
		buf[index] = tmpByte[record];
	}
	//GUID
	CHECKUP_DATALEN(index,len);
	for (int i = 0; i < 16; index++, i++)
	{
		buf[index] = msg_clss->GetGUID()[i];
	}
	//指令
	CHECKUP_DATALEN(index,len);
	buf[index] = msg_clss->GetCommand();
	index++;
	//保留位置
	CHECKUP_DATALEN(index,len);
	memset(tmpByte,0,4);
	IntTobyte(msg_clss->GetObligate(),tmpByte);
	for (int i = 0; i < 4; index++ , i++)
	{
		buf[index] = tmpByte[ i ];
	}
	//参数数量
	CHECKUP_DATALEN(index,len);
	buf[index]  = msg_clss->GetCmlCount();
	index++;

	return index;
}

int PackMessage::_Tail(CMessage* msg_clss,byte* buf,int index,size_t len)
{
	byte tmpByte[4] = {0};
	//Tail
	//效验码
	CHECKUP_DATALEN(index,len);
	if ((msg_clss == NULL) || (buf == NULL))
	{
		return -1;
	}
	memset(tmpByte,0,4);
	IntTobyte(msg_clss->GetVerify(),tmpByte);
	//因为效验码预订2位 所以只取低位
	CHECKUP_DATALEN(index,len);
	for (int i = 2; i < 4; index++ , i++)
	{
		buf[index] = tmpByte[ i ];
	}
	//结尾符
	CHECKUP_DATALEN(index,len);
	buf[index] = MSG_END;
	return 0;
}

int PackMessage::Pack(MSG_RVC* rvc,byte* buf, size_t len)
{

	byte tmpByte[4] = {0};

	int index = _Head(rvc,buf,len);
	//body
	//参数长度
	CHECKUP_DATALEN(index,len);
	for (int i = 0; i < (int)(rvc->GetCmlCount() * 4); i++, index++)
	{
		buf[index] = rvc->GetCmlListLen()[i];
	}
	//参数
	//1
	CHECKUP_DATALEN(index,len);
	for (int i = 0; i < rvc->GetMachineAddressLen(); i++, index++)
	{
		buf[index] = rvc->GetCmlCommandList()[0][i];
	}

	//Tail
	if (_Tail(rvc,buf,index,len) != 0)
	{
		return -1;
	}
	return 0;
}


int PackMessage::Pack(MSG_AVC* avc, byte* buf, size_t len)
{
	byte tmpByte[4] = {0};

	int index = _Head(avc,buf,len);
	//body
	//参数长度
	CHECKUP_DATALEN(index,len);
	for (int i = 0; i < (int)(avc->GetCmlCount() * 4); i++, index++)
	{
		buf[index] = avc->GetCmlListLen()[i];
	}
	//参数部分
	//1
	CHECKUP_DATALEN(index,len);
	buf[index] = avc->GetCmlCommandList()[2][0];
	index++;
	//2
	CHECKUP_DATALEN(index,len);
	buf[index] = avc->GetCmlCommandList()[1][0];
	index++;
	//3
	CHECKUP_DATALEN(index,len);
	for (int i = 0; i < avc->GetCaptchaLen(); i++, index++)
	{
		buf[index] = avc->GetCmlCommandList()[0][i];
	}
	//Tail
	if (_Tail(avc,buf,index,len) != 0)
	{
		return -1;
	}

	return 0;
}


int PackMessage::Pack(MSG_RLI* rli,byte* buf, size_t len)
{
	byte tmpByte[4] = {0};

	int index = _Head(rli,buf,len);

	//body
	//参数长度
	CHECKUP_DATALEN(index,len);
	for (int i = 0; i < (int)(rli->GetCmlCount() * 4); i++, index++)
	{
		buf[index] = rli->GetCmlListLen()[i];
	}

	//参数部分
	//1
	CHECKUP_DATALEN(index,len);
	for (int i = 0; i < rli->GetVerificationCodeLen(); i++, index++)
	{
		buf[index] = rli->GetCmlCommandList()[2][i];
	}
	//2
	CHECKUP_DATALEN(index,len);
	for (int i = 0; i < rli->GetAccountNumberLen(); i++, index++)
	{
		buf[index] = rli->GetCmlCommandList()[1][i];
	}
	//3
	CHECKUP_DATALEN(index,len);
	for (int i = 0; i < rli->GetPasswordLen(); i++, index++)
	{
		buf[index] = rli->GetCmlCommandList()[0][i];
	}

	//Tail
	if (_Tail(rli,buf,index,len) != 0)
	{
		return -1;
	}

	return 0;
}

int PackMessage::Pack(MSG_ALI* ali,byte* buf, size_t len)
{
	byte tmpByte[4] = {0};

	int index = _Head(ali,buf,len);

	//body
	//参数长度
	CHECKUP_DATALEN(index,len);
	for (int i = 0; i < (int)(ali->GetCmlCount() * 4); i++, index++)
	{
		buf[index] = ali->GetCmlListLen()[i];
	}
	//参数部分
	//1
	CHECKUP_DATALEN(index,len);
	buf[index] = ali->GetCmlCommandList()[2][0];
	index++;
	//2
	CHECKUP_DATALEN(index,len);
	for (int i = 0; i < ali->GetTokenLen(); i++, index++)
	{
		buf[index] = ali->GetCmlCommandList()[1][i];
	}
	//3
	CHECKUP_DATALEN(index,len);
	for (int i = 0; i < ali->GetTokenLen(); i++, index++)
	{
		buf[index] = ali->GetCmlCommandList()[0][i];
	}

	//Tail
	if (_Tail(ali,buf,index,len) != 0)
	{
		return -1;
	}

	return 0;

}


int PackMessage::Pack(MSG_RPS* rps,byte* buf, size_t len)
{
	byte tmpByte[4] = {0};

	int index = _Head(rps,buf,len);


	//body
	//参数长度
	CHECKUP_DATALEN(index,len);
	for (int i = 0; i < (int)(rps->GetCmlCount() * 4); i++, index++)
	{
		buf[index] = rps->GetCmlListLen()[i];
	}
	//参数部分
	//1
	CHECKUP_DATALEN(index,len);
	buf[index] = rps->GetCmlCommandList()[4][0];
	index++;
	//2
	CHECKUP_DATALEN(index,len);
	for (int i = 0; i < rps->GetNicknameLen(); i++, index++)
	{
		buf[index] = rps->GetCmlCommandList()[3][i];
	}
	//3
	CHECKUP_DATALEN(index,len);
	for (int i = 0; i < rps->GetAutographLen(); i++, index++)
	{
		buf[index] = rps->GetCmlCommandList()[2][i];
	}
	//4
	CHECKUP_DATALEN(index,len);
	buf[index] = rps->GetCmlCommandList()[1][0];
	index++;
	//5
	CHECKUP_DATALEN(index,len);
	for (int i = 0; i < rps->GetHeadPortraitLen(); i++, index++)
	{
		buf[index] = rps->GetCmlCommandList()[0][i];
	}

	//Tail
	if (_Tail(rps,buf,index,len) != 0)
	{
		return -1;
	}

	return 0;

}


int PackMessage::Pack(MSG_APS* aps,byte* buf, size_t len)
{
	byte tmpByte[4] = {0};

	int index = _Head(aps,buf,len);
	//body
	//参数长度
	CHECKUP_DATALEN(index,len);
	for (int i = 0; i < (int)(aps->GetCmlCount() * 4); i++, index++)
	{
		buf[index] = aps->GetCmlListLen()[i];
	}
	//参数部分
	//1
	CHECKUP_DATALEN(index,len);
	for (int i = 0; i < aps->GetMessageSynchroLen(); i++, index++)
	{
		buf[index] = aps->GetCmlCommandList()[0][i];
	}

	//Tail
	if (_Tail(aps,buf,index,len) != 0)
	{
		return -1;
	}

	return 0;
}