
#include "PackMessage.h"

#include <cstring>

int PackMessage::_Head(const CMessage* const msg_clss,byte* buf, size_t len)
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
	//msg_clss->m_Len = len;

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
	IntTobyte(len,tmpByte);
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

int PackMessage::_Tail(const CMessage* const msg_clss,byte* buf,int index,size_t len)
{
	byte tmpByte[4] = {0};
	//Tail
	//效验码
	CHECKUP_DATALEN(index,len);
	if ((msg_clss == NULL) || (buf == NULL))
	{
		return -1;
	}
	//memset(tmpByte,0,4);
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



int PackMessage::_CreateCommandLenList( byte* buf,int in_index)
{
	byte* ComLenList = new byte[m_CommandLenList.size() * 4];
	memset(ComLenList,0,m_CommandLenList.size() * 4);
	for (int i = 0,index = 0; i < (int)m_CommandLenList.size();i++)
	{
		byte tmpComLen[4] = {0};
		IntTobyte(m_CommandLenList[i],tmpComLen);
		for (int j = 0; j < 4; j++,index++)
		{
			ComLenList[index] = tmpComLen[j];
		}
	}


	//打包参数长度
	for (int i = 0; i < (int)(m_CommandLenList.size() * 4);in_index++, i++)
	{
		buf[in_index] = ComLenList[i];
	}
	if (ComLenList != NULL)
	{
		delete [] ComLenList;
		ComLenList = NULL;
	}
	return in_index;
}


void PackMessage::_PushCommandLenList( int len )
{
	m_CommandLenList.push_back(len);
}


void PackMessage::_CloseCommandLenList()
{
	m_CommandLenList.clear();
}


int PackMessage::_PushCommandList(const byte* const in_Command, int len )
{
	if (in_Command == NULL)
	{
		return -1;
	}
	int index = 0;
	for (int i = 0; i < len; i++, index++)
	{
		m_CommandList[CommandListPos].push_back(in_Command[i]);
	}
	//记录当前列表位置
	CommandListPos++;
	return index;
}

int PackMessage::_CreateCommandList( byte* buf,int in_index)
{
	for (int i = 0; i < (int)m_CommandList.size();i++)
	{
		for (int j = 0; j < (int)m_CommandList[i].size(); in_index++,j++)
		{
			buf[in_index] = m_CommandList[i][j];
		}
	}
	return in_index;
}

void PackMessage::_CloseCommandList()
{
	CommandListPos = 0;
	m_CommandList.clear();
}

//GetMessageLen

int PackMessage::GetMessageLen(const CMessage* const msg)
{
	int Head = 30;
	int Tail = 3;
	int Param = 0;
	int PLen  = 0;

	if (!msg)
	{
		return 0;
	}


	PLen  =  msg->GetCmlCount() * 4;

	for (int index = 0; index < (int)msg->m_ComCommandList.size(); index++)
	{
		Param += msg->m_ComCommandList[index].first;
	}

	return Param + PLen + Head + Tail;
}

int PackMessage::Pack(const CMessage* const msg, byte* buf, size_t len )
{
	int index = _Head(msg,buf,len);
	for (int i = 0; i < (int)msg->m_ComCommandList.size(); i++)
	{
		_PushCommandLenList(msg->m_ComCommandList[i].first);
	}

	//创建参数长度列表
	CHECKUP_DATALEN(index,len);
	index = _CreateCommandLenList(buf,index);

	m_CommandList.resize(msg->m_ComCommandList.size());

	for (int i = 0; i < (int)msg->m_ComCommandList.size(); i++)
	{
		_PushCommandList(msg->GetComCommandList(i),msg->m_ComCommandList[i].second.size());
	}

	//创建参数列表
	CHECKUP_DATALEN(index,len);
	index = _CreateCommandList(buf,index);

	//Tail
	return _Tail(msg,buf,index,len);
}