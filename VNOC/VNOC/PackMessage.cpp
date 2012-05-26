#pragma once

#include "PackMessage.h"

int PackMessage::Pack(MSG_RVC* rvc,byte* buf, size_t len)
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
	//����ͷβ���  �Զ����
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
	//ָ��
	buf[index] = rvc->GetCommand();
	index++;
	//����λ��
	memset(tmpByte,0,4);
	IntTobyte(rvc->GetObligate(),tmpByte);
	for (int i = 0; i < 4; index++ , i++)
	{
		buf[index] = tmpByte[ i ];
	}
	//��������
	buf[index]  = rvc->GetCmlCount();
	index++;

	//body
	//��������
	for (int i = 0; i < (int)(rvc->GetCmlCount() * 4); i++, index++)
	{
		buf[index] = rvc->GetCmlListLen()[i];
	}
	//����
	//1
	for (int i = 0; i < rvc->GetMachineAddressLen(); i++, index++)
	{
		buf[index] = rvc->GetCmlCommandList()[0][i];
	}

	//Tail
	//Ч����
	memset(tmpByte,0,4);
	IntTobyte(rvc->GetVerify(),tmpByte);
	//��ΪЧ����Ԥ��2λ ����ֻȡ��λ
	for (int i = 2; i < 4; index++ , i++)
	{
		buf[index] = tmpByte[ i ];
	}
	//��β��
	buf[index] = MSG_END;


	return 0;
}


int PackMessage::Pack(MSG_AVC* avc, byte* buf, size_t len)
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
	//����ͷβ���  �Զ����
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
	//ָ��
	buf[index] = avc->GetCommand();
	index++;
	//����λ��
	memset(tmpByte,0,4);
	IntTobyte(avc->GetObligate(),tmpByte);
	for (int i = 0; i < 4; index++ , i++)
	{
		buf[index] = tmpByte[ i ];
	}
	//��������
	buf[index]  = avc->GetCmlCount();
	index++;

	//body
	//��������
	for (int i = 0; i < (int)(avc->GetCmlCount() * 4); i++, index++)
	{
		buf[index] = avc->GetCmlListLen()[i];
	}
	//��������
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
	//Ч����
	memset(tmpByte,0,4);
	IntTobyte(avc->GetVerify(),tmpByte);
	//��ΪЧ����Ԥ��2λ ����ֻȡ��λ
	for (int i = 2; i < 4; index++ , i++)
	{
		buf[index] = tmpByte[ i ];
	}
	//��β��
	buf[index] = MSG_END;

	return 0;
}


int PackMessage::Pack(MSG_RLI* rli,byte* buf, size_t len)
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
	//����ͷβ���  �Զ����
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
	//ָ��
	buf[index] = rli->GetCommand();
	index++;
	//����λ��
	memset(tmpByte,0,4);
	IntTobyte(rli->GetObligate(),tmpByte);
	for (int i = 0; i < 4; index++ , i++)
	{
		buf[index] = tmpByte[ i ];
	}
	//��������
	buf[index]  = rli->GetCmlCount();
	index++;

	//��������
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
	//Ч����
	memset(tmpByte,0,4);
	IntTobyte(rli->GetVerify(),tmpByte);
	//��ΪЧ����Ԥ��2λ ����ֻȡ��λ
	for (int i = 2; i < 4; index++ , i++)
	{
		buf[index] = tmpByte[ i ];
	}
	//��β��
	buf[index] = MSG_END;

	return 0;
}

int PackMessage::Pack(MSG_ALI* ali,byte* buf, size_t len)
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
	//����ͷβ���  �Զ����
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
	//ָ��
	buf[index] = ali->GetCommand();
	index++;
	//����λ��
	memset(tmpByte,0,4);
	IntTobyte(ali->GetObligate(),tmpByte);
	for (int i = 0; i < 4; index++ , i++)
	{
		buf[index] = tmpByte[ i ];
	}
	//��������
	buf[index]  = ali->GetCmlCount();
	index++;

	//��������
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
	//Ч����
	memset(tmpByte,0,4);
	IntTobyte(ali->GetVerify(),tmpByte);
	//��ΪЧ����Ԥ��2λ ����ֻȡ��λ
	for (int i = 2; i < 4; index++ , i++)
	{
		buf[index] = tmpByte[ i ];
	}
	//��β��
	buf[index] = MSG_END;

	return 0;

}


int PackMessage::Pack(MSG_RPS* rps,byte* buf, size_t len)
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
	//����ͷβ���  �Զ����
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
	//ָ��
	buf[index] = rps->GetCommand();
	index++;
	//����λ��
	memset(tmpByte,0,4);
	IntTobyte(rps->GetObligate(),tmpByte);
	for (int i = 0; i < 4; index++ , i++)
	{
		buf[index] = tmpByte[ i ];
	}
	//��������
	buf[index]  = rps->GetCmlCount();
	index++;

	//��������
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
	//Ч����
	memset(tmpByte,0,4);
	IntTobyte(rps->GetVerify(),tmpByte);
	//��ΪЧ����Ԥ��2λ ����ֻȡ��λ
	for (int i = 2; i < 4; index++ , i++)
	{
		buf[index] = tmpByte[ i ];
	}
	//��β��
	buf[index] = MSG_END;

	return 0;

}


int PackMessage::Pack(MSG_APS* aps,byte* buf, size_t len)
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
	//����ͷβ���  �Զ����
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
	//ָ��
	buf[index] = aps->GetCommand();
	index++;
	//����λ��
	memset(tmpByte,0,4);
	IntTobyte(aps->GetObligate(),tmpByte);
	for (int i = 0; i < 4; index++ , i++)
	{
		buf[index] = tmpByte[ i ];
	}
	//��������
	buf[index]  = aps->GetCmlCount();
	index++;

	//��������
	//1
	for (int i = 0; i < aps->GetMessageSynchroLen(); i++, index++)
	{
		buf[index] = aps->GetCmlCommandList()[0][i];
	}

	//Tail
	//Ч����
	memset(tmpByte,0,4);
	IntTobyte(aps->GetVerify(),tmpByte);
	//��ΪЧ����Ԥ��2λ ����ֻȡ��λ
	for (int i = 2; i < 4; index++ , i++)
	{
		buf[index] = tmpByte[ i ];
	}
	//��β��
	buf[index] = MSG_END;

	return 0;
}