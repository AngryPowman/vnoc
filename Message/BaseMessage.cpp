

#include "BaseMessage.h"


uint byteToInt(const byte* in_byte,size_t len)
{
	byte tmpByte[4] = {0};
	if (in_byte == NULL)
	{
		return 0;
	}

	for (int index = 0; index < (int)len; index++)
	{
		tmpByte[index] = in_byte[index];
	}

	uint*  lpDataLen = NULL; 
	lpDataLen = (uint *)tmpByte;
	return (*lpDataLen);
}

void IntTobyte(int in_int,byte* out_byte)
{
	if (out_byte == NULL)
	{
		return;
	}
	for(int i = 0; i < 4; i++)
	{
		out_byte[i]=(byte)(in_int>>(24-i*8));
	}
}


int  CMessage::GetMessageType() const 
{
	return _MessageType();
}

bool CMessage::GetBeginTab() const
{
	return m_Begin;
} 

bool CMessage::GetEndTab() const
{
	return m_End;
}

byte CMessage::GetCommand() const
{
	return m_Command;
}

byte CMessage::GetComListLen(uint in_index) const
{
	return  m_ComListLen[in_index];
}

uint CMessage::GetSerial() const
{
	return BigLittleSwap16(byteToInt(m_Serial,2));
}

const byte* CMessage::GetGUID() const
{
	return m_GUID;
}

const byte* CMessage::GetComCommandList(uint in_index)const
{
	return (byte*)(m_ComCommandList[in_index].second.data());
}

uint CMessage::GetVerify() const
{
	return m_Verify;
}

uint CMessage::GetObligate() const
{
	return m_Obligate;
}

uint CMessage::GetVersion() const
{
	return m_Ver;
}

uint CMessage::GetCmlCount() const
{
	return m_CmlCount;
}

uint CMessage::GetDataLen() const
{
	return m_Len;
}


// Set

bool CMessage::SetCommand(byte in_byte)
{
	m_Command = in_byte;
	return true;
}


bool CMessage::SetComListLen(byte* in_byte_ptr,int CmlCount)
{
	if (in_byte_ptr != NULL)
	{
		if (CmlCount != 0)
		{
			m_ComListLen.clear();
			for (int index = 0; index < (CmlCount * 4); index++)
			{
				m_ComListLen.push_back(in_byte_ptr[index]);
			}
		}
		else
		{
			m_ComListLen.clear();
		}
	}
	else
	{
		m_ComListLen.resize(CmlCount * 4);
	}
	return true;
}

bool CMessage::SetSerial(ushort in_short)
{
	byte tmpbyte[4] = {0};
	if (m_Serial != NULL)
	{
		IntTobyte(in_short,tmpbyte);
		//取低2位
		for(int i = 2,Pos = 0; i < 4;Pos++,i++)
		{
			m_Serial[Pos] = tmpbyte[i];
		}
		return true;
	}
	return false;
}


bool CMessage::SetGUID(byte* in_byte_ptr)
{
	if (in_byte_ptr  != NULL)
	{
		memset(m_GUID,0,16);
		memcpy(m_GUID,in_byte_ptr,16);
		return true;
	}
	return false;
}


bool CMessage::SetComCommandList(int ComCount)
{
	if (ComCount == 0)
	{
		m_ComCommandList.clear();
		return true;
	}

	m_ComCommandList.clear();
	m_ComCommandList.resize(ComCount);
	return true;
}


bool CMessage::SetVerify(uint in_Int) 
{
	m_Verify = in_Int;
	return true;
}

bool CMessage::SetObligate(uint in_Int)
{
	m_Obligate = in_Int;
	return true;
}

bool CMessage::SetVersion(uint in_Int)
{
	m_Ver = in_Int;
	return true;
}

bool CMessage::SetCmlCount(uint in_Int)
{
	m_CmlCount = in_Int;
	return true;
}

bool CMessage::SetDataLen(uint in_Int)
{
	m_Len = in_Int;
	return true;
}



int  CMessage::_MessageType() const
{
	int  _type = 0;
	switch(m_Command)
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

	case MSG_RCL_COM: _type =  MSG_RCL_TYPE;
		break;
	case MSG_ACL_COM: _type =  MSG_ACL_TYPE;
		break;
	}
	return _type;
}

void CMessage::_Initialization() 
{
	memset(m_GUID,0,16);
	memset(m_Serial,0,2);
	m_Begin = false;
	m_End   = false;
	m_CmlCount = 0;
	m_Obligate = 0;
	m_Ver = 0;
	m_Verify  = 0;
	//m_Serial  = 0;
	m_Len     = 0;
	m_Command = 0;
	m_Error = 0;
}

void CMessage::_Close()
{
	//保留
}


int CMessage::MateParam( std::string in_str ) const
{
	auto iter = m_ParamMate.find(in_str);
	if (iter != m_ParamMate.end())
	{
		return iter->second;
	}
	return -1;
}

void CMessage::SetParam( int Pos,byte in_Param_byte )
{
	if (Pos == -1)
	{
		return;
	}
	//将参数长度存放进表中
	CMessage::SetParamLen(Pos,1);
	m_ComCommandList[Pos].second.clear();
	m_ComCommandList[Pos].second.push_back(in_Param_byte);
}

void CMessage::SetParam( int Pos,const byte* in_Param_byte_Ptr,uint len )
{
	if (Pos == -1)
	{
		return;
	}
	SetParamLen(Pos,len);
	if (in_Param_byte_Ptr == NULL)
	{
		return;
	}
	m_ComCommandList[Pos].second.clear();
	for (int index = 0; index < (int)len; index++)
	{
		m_ComCommandList[Pos].second.push_back(in_Param_byte_Ptr[index]);
	}
}

void CMessage::SetParam( int Pos,uint in_Param_int )
{
	if (Pos == -1)
	{
		return;
	}
	SetParamLen(Pos,4);
	byte tmpByte[4] = {0};
	IntTobyte(in_Param_int,tmpByte);

	m_ComCommandList[Pos].second.clear();

	for (int index = 0; index < 4; index++)
	{
		m_ComCommandList[Pos].second.push_back(tmpByte[index]);
	}
}

uint CMessage::GetParamLen( uint int_index ) const
{
	if (int_index > m_ComCommandList.size())
	{
		return -1;
	}

	return m_ComCommandList[int_index].first;
}



void CMessage::SetParamLen( uint int_index, int int_Value )
{
	if (int_index > m_ComCommandList.size())
	{
		return;
	}

	m_ComCommandList[int_index].first = int_Value;

}

const byte* CMessage::GetParam( int int_index )  const
{
	if (int_index == -1)
	{
		return 0;
	}
	if (int_index > (int)m_ComCommandList.size())
	{
		return 0;
	}

	return ((const byte*)m_ComCommandList[int_index].second.data());
}



void CMessage::InitiaPack( int in_ComType, int in_ComNum )
{
	m_CmlCount = in_ComNum;
	m_Command  = in_ComType;
	SetDataLen(0);			 
	SetVersion(MSG_VER);	 
	SetSerial(0x00);		 
	SetCommand(m_Command);	 
	SetObligate(0);          
	SetCmlCount(m_CmlCount); 
	SetVerify(0);			 
	//留好参数长度空间
	SetComListLen( 0, m_CmlCount );
	//留好参数空间
	SetComCommandList(m_CmlCount);
}


