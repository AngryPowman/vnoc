
#include "PackMessage.h"

#include <cstring>

using namespace std;

int PackMessage::_Head( CMessage* const msg_clss,byte* buf, size_t len)
{
	//head---------------------
	int index = 0;
	int tmpInt = 0;
	byte tmpByte[4] = {0};
	if (buf == NULL)
	{
		return -2;
	}

	//设置包长度
	msg_clss->SetDataLen(len);

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
	memset(tmpByte,0,4);
	IntTobyte(msg_clss->GetSerial(),tmpByte);
	//因为序号只有2位 所以只取低位
	CHECKUP_DATALEN(index,len);
	for (int i = 2; i < 4; i++,index++)
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

int PackMessage::_Tail( CMessage* const msg_clss,byte* buf,int index,size_t len)
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
	for (int i = 2; i < 4; i++,index++)
	{
		buf[index] = tmpByte[ i ];
	}
	//结尾符
	CHECKUP_DATALEN(index,len);
	buf[index] = MSG_END;
	return index;
}

int PackMessage::_Pack( MSG_RVC* const rvc,byte* buf, size_t len)
{
	byte tmpComLen[4] = {0}; 
	byte tmpByte[4] = {0};

	int index = _Head(rvc,buf,len);
	//body
	byte* ComLenList = new byte[rvc->GetCmlCount() * 4];
	memset(ComLenList,0,rvc->GetCmlCount() * 4);
	//设置参数长度
	//1
	IntTobyte(rvc->GetMachineAddressLen(),tmpComLen);
	for (int i = 0; i < 4; i++)
	{
		ComLenList[i] = tmpComLen[i];
	}
	rvc->SetComListLen(ComLenList,rvc->GetCmlCount());
	delete [] ComLenList;
	ComLenList = NULL;
	//参数长度
	CHECKUP_DATALEN(index,len);
	for (int i = 0; i < (int)(rvc->GetCmlCount() * 4); i++, index++)
	{
		buf[index] = rvc->GetComListLen()[i];
	}
	//参数
	//1
	CHECKUP_DATALEN(index,len);
	for (int i = 0; i < rvc->GetMachineAddressLen(); i++, index++)
	{
		buf[index] = rvc->GetComCommandList()[0][i];
	}

	//Tail
	return _Tail(rvc,buf,index,len);
}


int PackMessage::_Pack( MSG_AVC* const avc, byte* buf, size_t len)
{
	byte tmpComLen[4] = {0}; 
	byte OneComLen[] = {0x00,0x00,0x00,0x01};
	byte tmpByte[4] = {0};

	int index = _Head(avc,buf,len);
	//body
	byte* ComLenList = new byte[avc->GetCmlCount() * 4];
	memset(ComLenList,0,avc->GetCmlCount() * 4);
	//设置参数长度
	//1
	int Pos = 0;
	for (; Pos < 4; Pos++)
	{
		ComLenList[Pos] = OneComLen[Pos];
	}
	//2
	for (int i = 0; Pos < 8; Pos++,i++)
	{
		ComLenList[Pos] = OneComLen[ i ];
	}
	//3
	IntTobyte(avc->GetCaptchaLen(),tmpComLen);
	for (int i = 0; Pos < 12; Pos++, i++)
	{
		ComLenList[Pos] = tmpComLen[ i ];
	}
	avc->SetComListLen(ComLenList,avc->GetCmlCount());
	delete [] ComLenList;
	ComLenList = NULL;
	//参数长度
	CHECKUP_DATALEN(index,len);
	for (int i = 0; i < (int)(avc->GetCmlCount() * 4); i++, index++)
	{
		buf[index] = avc->GetComListLen()[i];
	}
	//参数部分
	//1
	CHECKUP_DATALEN(index,len);
	buf[index] = avc->GetComCommandList()[2][0];
	index++;
	//2
	CHECKUP_DATALEN(index,len);
	buf[index] = avc->GetComCommandList()[1][0];
	index++;
	//3
	CHECKUP_DATALEN(index,len);
	for (int i = 0; i < avc->GetCaptchaLen(); i++, index++)
	{
		buf[index] = avc->GetComCommandList()[0][i];
	}
	//Tail
	return _Tail(avc,buf,index,len);
}


int PackMessage::_Pack( MSG_RLI* const rli,byte* buf, size_t len)
{
	byte tmpComLen[4] = {0}; 
	byte OneComLen[] = {0x00,0x00,0x00,0x01};

	byte tmpByte[4] = {0};

	int index = _Head(rli,buf,len);

	//body
	byte* ComLenList = new byte[rli->GetCmlCount() * 4];
	memset(ComLenList,0,rli->GetCmlCount() * 4);
	//设置参数长度
	//1
	int Pos = 0;
	IntTobyte(rli->GetVerificationCodeLen(),tmpComLen);
	for (;Pos < 4; Pos++)
	{
		ComLenList[Pos] = tmpComLen[Pos];
	}
	memset(tmpComLen,0,4);
	//2
	IntTobyte(rli->GetAccountNumberLen(),tmpComLen);
	for (int i = 0; Pos < 8; Pos++,i++)
	{
		ComLenList[Pos] = tmpComLen[ i ];
	}
	memset(tmpComLen,0,4);
	//3
	IntTobyte(rli->GetPasswordLen(),tmpComLen);
	for (int i = 0; Pos < 12; Pos++,i++)
	{
		ComLenList[Pos] = tmpComLen[ i ];
	}
	memset(tmpComLen,0,4);
	rli->SetComListLen(ComLenList,rli->GetCmlCount());
	delete [] ComLenList;
	ComLenList = NULL;
	//参数长度
	CHECKUP_DATALEN(index,len);
	for (int i = 0; i < (int)(rli->GetCmlCount() * 4); i++, index++)
	{
		buf[index] = rli->GetComListLen()[i];
	}

	//参数部分
	//1
	CHECKUP_DATALEN(index,len);
	for (int i = 0; i < rli->GetVerificationCodeLen(); i++, index++)
	{
		buf[index] = rli->GetComCommandList()[2][i];
	}
	//2
	CHECKUP_DATALEN(index,len);
	for (int i = 0; i < rli->GetAccountNumberLen(); i++, index++)
	{
		buf[index] = rli->GetComCommandList()[1][i];
	}
	//3
	CHECKUP_DATALEN(index,len);
	for (int i = 0; i < rli->GetPasswordLen(); i++, index++)
	{
		buf[index] = rli->GetComCommandList()[0][i];
	}

	//Tail
	return _Tail(rli,buf,index,len);
}

int PackMessage::_Pack( MSG_ALI* const ali,byte* buf, size_t len)
{
	byte tmpComLen[4] = {0}; 
	byte OneComLen[] = {0x00,0x00,0x00,0x01};
	byte tmpByte[4] = {0};

	int index = _Head(ali,buf,len);

	//body
	byte* ComLenList = new byte[ali->GetCmlCount() * 4];
	memset(ComLenList,0,ali->GetCmlCount() * 4);
	//设置参数长度
	//1
	int Pos = 0;
	for (; Pos < 4; Pos++)
	{
		ComLenList[Pos] = OneComLen[Pos];
	}
	//2
	IntTobyte(ali->GetTokenLen(),tmpComLen);
	for (int i = 0; Pos < 8; Pos++,i++)
	{
		ComLenList[Pos] = tmpComLen[ i ];
	}
	memset(tmpComLen,0,4);
	//3
	IntTobyte(ali->GetATLGUIDLen(),tmpComLen);
	for (int i = 0; Pos < 12; Pos++, i++)
	{
		ComLenList[Pos] = tmpComLen[ i ];
	}
	memset(tmpComLen,0,4);
	ali->SetComListLen(ComLenList,ali->GetCmlCount());
	delete [] ComLenList;
	ComLenList = NULL;
	//参数长度
	CHECKUP_DATALEN(index,len);
	for (int i = 0; i < (int)(ali->GetCmlCount() * 4); i++, index++)
	{
		buf[index] = ali->GetComListLen()[i];
	}
	//参数部分
	//1
	CHECKUP_DATALEN(index,len);
	buf[index] = ali->GetComCommandList()[2][0];
	index++;
	//2
	CHECKUP_DATALEN(index,len);
	for (int i = 0; i < ali->GetTokenLen(); i++, index++)
	{
		buf[index] = ali->GetComCommandList()[1][i];
	}
	//3
	CHECKUP_DATALEN(index,len);
	for (int i = 0; i < ali->GetTokenLen(); i++, index++)
	{
		buf[index] = ali->GetComCommandList()[0][i];
	}

	//Tail
	return _Tail(ali,buf,index,len);

}


int PackMessage::_Pack( MSG_RPS* const rps,byte* buf, size_t len)
{
	byte tmpComLen[4] = {0}; 
	byte OneComLen[] = {0x00,0x00,0x00,0x01};
	byte tmpByte[4] = {0};

	int index = _Head(rps,buf,len);


	//body
	byte* ComLenList = new byte[rps->GetCmlCount() * 4];
	memset(ComLenList,0,rps->GetCmlCount() * 4);
	//设置参数长度
	//1
	int Pos = 0;
	for (; Pos < 4; Pos++)
	{
		ComLenList[Pos] = OneComLen[Pos];
	}
	//2
	IntTobyte(rps->GetNicknameLen(),tmpComLen);
	for (int i = 0; Pos < 8; Pos++, i ++)
	{
		ComLenList[Pos] = tmpComLen[ i ];
	}
	memset(tmpComLen,0,4);
	//3
	IntTobyte(rps->GetAutographLen(),tmpComLen);
	for (int i = 0; Pos < 12; Pos++,i++)
	{
		ComLenList[Pos] = tmpComLen[ i ];
	}
	memset(tmpComLen,0,4);
	//4
	for (int i = 0; Pos < 16; Pos++,i++)
	{
		ComLenList[Pos] = OneComLen[ i ];
	}
	//5
	IntTobyte(rps->GetHeadPortraitLen(),tmpComLen);
	for (int i = 0; Pos < 20; Pos++,i++)
	{
		ComLenList[Pos] = tmpComLen[ i ];
	}
	memset(tmpComLen,0,4);
	rps->SetComListLen(ComLenList,rps->GetCmlCount());
	delete [] ComLenList;
	ComLenList = NULL;
	//参数长度
	CHECKUP_DATALEN(index,len);
	for (int i = 0; i < (int)(rps->GetCmlCount() * 4); i++, index++)
	{
		buf[index] = rps->GetComListLen()[i];
	}
	//参数部分
	//1
	CHECKUP_DATALEN(index,len);
	buf[index] = rps->GetComCommandList()[4][0];
	index++;
	//2
	CHECKUP_DATALEN(index,len);
	for (int i = 0; i < rps->GetNicknameLen(); i++, index++)
	{
		buf[index] = rps->GetComCommandList()[3][i];
	}
	//3
	CHECKUP_DATALEN(index,len);
	for (int i = 0; i < rps->GetAutographLen(); i++, index++)
	{
		buf[index] = rps->GetComCommandList()[2][i];
	}
	//4
	CHECKUP_DATALEN(index,len);
	buf[index] = rps->GetComCommandList()[1][0];
	index++;
	//5
	CHECKUP_DATALEN(index,len);
	for (int i = 0; i < rps->GetHeadPortraitLen(); i++, index++)
	{
		buf[index] = rps->GetComCommandList()[0][i];
	}

	//Tail
	return _Tail(rps,buf,index,len);

}


int PackMessage::_Pack( MSG_APS* const aps,byte* buf, size_t len)
{
	byte tmpComLen[4] = {0}; 
	byte OneComLen[] = {0x00,0x00,0x00,0x01};
	byte tmpByte[4] = {0};

	int index = _Head(aps,buf,len);
	//body
	byte* ComLenList = new byte[aps->GetCmlCount() * 4];
	memset(ComLenList,0,aps->GetCmlCount() * 4);
	//设置参数长度
	//1
	IntTobyte(aps->GetMessageSynchroLen(),tmpComLen);
	int Pos = 0;
	for (; Pos < 4; Pos++)
	{
		ComLenList[Pos] = tmpComLen[Pos];
	}
	memset(tmpComLen,0,4);
	aps->SetComListLen(ComLenList,aps->GetCmlCount());
	delete [] ComLenList;
	ComLenList = NULL;
	//参数长度
	CHECKUP_DATALEN(index,len);
	for (int i = 0; i < (int)(aps->GetCmlCount() * 4); i++, index++)
	{
		buf[index] = aps->GetComListLen()[i];
	}
	//参数部分
	//1
	CHECKUP_DATALEN(index,len);
	for (int i = 0; i < aps->GetMessageSynchroLen(); i++, index++)
	{
		buf[index] = aps->GetComCommandList()[0][i];
	}

	//Tail
	return _Tail(aps,buf,index,len);
}

//GetMessageLen

int PackMessage::GetMessageLen(const CMessage* const msg)
{
	int Head = 30;
	int Tail = 3;
	int Param = 0;
	int PLen  = 0;

	MSG_AVC* avc;
	MSG_RVC* rvc;
	MSG_ALI* ali;
	MSG_RLI* rli;
	MSG_RPS* rps;
	MSG_APS* aps;

	if (!msg)
	{
		return 0;
	}


	PLen  =  msg->GetCmlCount() * 4;
	
	switch (msg->GetMessageType())
	{
	case MSG_AVC_TYPE:
		avc = (MSG_AVC*)msg;
		Param = avc->GetCaptchaLen() + 1 + 1;
		break;
	case MSG_RVC_TYPE:
		rvc = (MSG_RVC*)msg;
		Param = rvc->GetMachineAddressLen();
		break;
	case MSG_ALI_TYPE:
		ali = (MSG_ALI*)msg;
		Param = ali->GetATLGUIDLen() + ali->GetTokenLen() + 1;
		break;
	case MSG_RLI_TYPE:
		rli = (MSG_RLI*)msg;
		Param = rli->GetAccountNumberLen() + rli->GetPasswordLen() + rli->GetVerificationCodeLen();
		break;
	case MSG_RPS_TYPE:
		rps = (MSG_RPS*)msg;
		Param = rps->GetNicknameLen() + rps->GetAutographLen() + rps->GetHeadPortraitLen() + 1 + 1;
		break;
	case MSG_APS_TYPE:
		aps = (MSG_APS*)msg;
		Param = aps->GetMessageSynchroLen();
		break;
	}
	return Param + PLen + Head + Tail;
}

int PackMessage::Pack(const CMessage* const msg, byte* buf, size_t len )
{
	int Result = 0;

	switch (msg->GetMessageType())
	{
	case MSG_AVC_TYPE:
		Result = _Pack((MSG_AVC*)msg,buf,len);
		break;
	case MSG_RVC_TYPE:
		Result = _Pack((MSG_RVC*)msg,buf,len);
		break;
	case MSG_ALI_TYPE:
		Result = _Pack((MSG_ALI*)msg,buf,len);
		break;
	case MSG_RLI_TYPE:
		Result = _Pack((MSG_RLI*)msg,buf,len);
		break;
	case MSG_RPS_TYPE:
		Result = _Pack((MSG_RPS*)msg,buf,len);
		break;
	case MSG_APS_TYPE:
		Result = _Pack((MSG_APS*)msg,buf,len);
		break;
	}
	return Result;
}
