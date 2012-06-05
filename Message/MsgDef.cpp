
#include "MsgDef.h"

#include <cstring>

//CMessageParser


using namespace std;

void LittleSwapBigByte(byte* in_byte_ptr,size_t len)
{
	byte* tmpByte_ptr = new byte[len];
	if (in_byte_ptr != NULL)
	{
		for (int index = 0,Pos = 1; index < (int)len; index++,Pos++)
		{
			tmpByte_ptr[index] = in_byte_ptr[len - Pos];
		}
		memcpy(in_byte_ptr,tmpByte_ptr,len);
	}
	delete [] tmpByte_ptr;
}

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
// 	if (GetBeginTab() != true || GetEndTab() != true)
// 	{
// 		return -1;
	// 	}

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
	}
	return _type;
}

bool CMessage::GetBeginTab() const
{
	return m_Begin;
} 

bool CMessage::GetEndTab() const
{
	return m_Begin;
}

byte CMessage::GetCommand() const
{
	return m_Command;
}

byte* CMessage::GetCmlListLen() const
{
	return  m_CmlListLen;
}

uint CMessage::GetSerial() const
{
	return BigLittleSwap16(byteToInt(m_Serial,2));
}

const byte* CMessage::GetGUID() const
{
	return m_GUID;
}

byte** CMessage::GetCmlCommandList() const
{
	return m_CmlCommandList;
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

bool CMessage::SetCmlListLen(byte* in_byte_ptr,int CmlCount)
{
	if (CmlCount  !=  0)
	{
		delete [] m_CmlListLen;
		m_CmlListLen = new byte[CmlCount * 4];
		memset(m_CmlListLen,0,CmlCount * 4);
		if (in_byte_ptr != NULL)
		{
			memcpy(m_CmlListLen,in_byte_ptr,CmlCount * 4);
		}
		return true;
	}
	return false;
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

bool CMessage::SetCmlCommandList(int CmlCount)
{
	if (CmlCount  !=  0)
	{
		for (int count = 0; (int)m_CmlCount < count; count++)
		{
			if (m_CmlCommandList[count])
			{
				delete [] m_CmlCommandList[count];
				m_CmlCommandList[count] = NULL;
			}
		}

		delete [] m_CmlCommandList;
		m_CmlCommandList = new byte*[CmlCount];
		return true;
	}
	return false;
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



int  CMessage::_MessageType()
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
	}
	return _type;
}

void CMessage::_Initialization() 
{
	memset(m_GUID,0,16);
	memset(m_Serial,0,2);
	m_Begin = false;
	m_End   = false;
	m_CmlListLen  = NULL;
	m_CmlCommandList = NULL;
	m_CmlCount = 0;
	m_Obligate = 0;
	m_Ver = 0;
	m_Verify  = 0;
	//m_Serial  = 0;
	m_Len     = 0;
	m_Command = 0;
}

void CMessage::_Close()
{
	if (m_CmlCommandList)
	{
		for (int count = 0; (int)m_CmlCount < count; count++)
		{
			if (m_CmlCommandList[count])
			{
				delete [] m_CmlCommandList[count];
				m_CmlCommandList[count] = NULL;
			}
		}

		delete [] m_CmlCommandList;
		m_CmlCommandList  = NULL;
	}

	if (m_CmlListLen != NULL)
	{
		delete[] m_CmlListLen;
		m_CmlListLen = NULL;
	}

	m_CmlCount = NULL;
}


//RVC(获取验证码请求)
void MSG_RVC::Initialize() 
{
// 	if (GetBeginTab() != true || GetEndTab() != true)
// 	{
// 		m_Error++;
// 	}

	if (GetCommand() != m_Command)
	{
		m_Error++;
	}

	if (GetCmlCount() != m_CmlCount)
	{
		m_Error++;
	}
	//效验码
// 	if (strcmp(returnVerify(),"") != 0)
// 	{
// 		m_Error++;
// 	}

}

byte* MSG_RVC::GetMachineAddress() const
{
	// Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return GetCmlCommandList()[0];
}

void MSG_RVC::SetMachineAddress( byte* in_byte_ptr, size_t len )
{
	if (in_byte_ptr == NULL)
	{
		return;
	}
	m_MachineAddressLen = len;
	if (GetCmlCommandList()[0] != NULL)
	{
		GetCmlCommandList()[0] = new byte[len];
	}
	memset(GetCmlCommandList()[0],0,len);
	memcpy(GetCmlCommandList()[0],in_byte_ptr,16);
}

//AVC(获取验证码响应)
void MSG_AVC::Initialize()
{
// 	if (GetBeginTab() != true || GetEndTab() != true)
// 	{
// 		m_Error++;
// 	}

	if (GetCommand() != m_Command)
	{
		m_Error++;
	}

	if (GetCmlCount() != m_CmlCount)
	{
		m_Error++;
	}
	//效验码
	// 	if (strcmp(returnVerify(),"") != 0)
	// 	{
	// 		m_Error++;
	// 	}

}

uint MSG_AVC::GetLoginTag() const
{
	// Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return byteToInt(GetCmlCommandList()[2],1);
}

uint MSG_AVC::GetCaptchaType() const
{
	// Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return byteToInt(GetCmlCommandList()[1],1);
}

byte* MSG_AVC::GetCaptcha() const
{
	// Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return GetCmlCommandList()[0];
}


void MSG_AVC::SetLoginTag(byte in_byte)
{
	//只占一个字节
	if (GetCmlCommandList()[2] != NULL)
	{
		GetCmlCommandList()[2] = new byte[1];
	}
	GetCmlCommandList()[2][0] = in_byte;
}

void MSG_AVC::SetCaptchaType(byte in_byte)
{
	//只占一个字节
	if (GetCmlCommandList()[1] != NULL)
	{
		GetCmlCommandList()[1] = new byte[1];
	}
	GetCmlCommandList()[1][0] = in_byte;
}

void MSG_AVC::SetCaptcha(byte* in_byte_ptr,size_t len)
{
	if (in_byte_ptr == NULL)
	{
		return;
	}
	m_CaptchaLen = len;
	if (GetCmlCommandList()[0] != NULL)
	{
		GetCmlCommandList()[0] = new byte[len];
	}
	memset(GetCmlCommandList()[0],0,len);
	memcpy(GetCmlCommandList()[0],in_byte_ptr,len);
}

//RLI(登录请求)
void MSG_RLI::Initialize()
{
// 	if (GetBeginTab() != true || GetEndTab() != true)
// 	{
// 		m_Error++;
// 	}

	if (GetCommand() != m_Command)
	{
		m_Error++;
	}

	if (GetCmlCount() != m_CmlCount)
	{
		m_Error++;
	}
	//效验码
	// 	if (strcmp(returnVerify(),"") != 0)
	// 	{
	// 		m_Error++;
	// 	}

}
byte* MSG_RLI::GetVerificationCode() const
{
	// Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return GetCmlCommandList()[2];
}


byte* MSG_RLI::GetAccountNumber() const
{
	// Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return GetCmlCommandList()[1];
}


byte* MSG_RLI::GetPassword() const
{
	// Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return GetCmlCommandList()[0];
}

void MSG_RLI::SetVerificationCode( byte* in_byte_ptr,size_t len )
{
	if (in_byte_ptr == NULL)
	{
		return;
	}
	m_VerificationCodeLen = len;
	if (GetCmlCommandList()[2] != NULL)
	{
		GetCmlCommandList()[2] = new byte[len];
	}
	memset(GetCmlCommandList()[2],0,len);
	memcpy(GetCmlCommandList()[2],in_byte_ptr,len);
}

void MSG_RLI::SetAccountNumber( byte* in_byte_ptr,size_t len )
{
	if (in_byte_ptr == NULL)
	{
		return;
	}
	m_AccountNumberLen = len;
	if (GetCmlCommandList()[1] != NULL)
	{
		GetCmlCommandList()[1] = new byte[len];
	}
	memset(GetCmlCommandList()[1],0,len);
	memcpy(GetCmlCommandList()[1],in_byte_ptr,len);
}

void MSG_RLI::SetPassword( byte* in_byte_ptr,size_t len )
{
	if (in_byte_ptr == NULL)
	{
		return;
	}
	m_PasswordLen = len;
	if (GetCmlCommandList()[0] != NULL)
	{
		GetCmlCommandList()[0] = new byte[len];
	}
	memset(GetCmlCommandList()[0],0,len);
	memcpy(GetCmlCommandList()[0],in_byte_ptr,len);
}

//ALI(登录应答)
void MSG_ALI::Initialize()
{
// 	if (GetBeginTab() != true || GetEndTab() != true)
// 	{
// 		m_Error++;
// 	}

	if (GetCommand() != m_Command)
	{
		m_Error++;
	}

	if (GetCmlCount() != m_CmlCount)
	{
		m_Error++;
	}
	//效验码
	// 	if (strcmp(returnVerify(),"") != 0)
	// 	{
	// 		m_Error++;
	// 	}

}
uint MSG_ALI::GetLoginResult() const
{
	// Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return byteToInt(GetCmlCommandList()[2],1);
}


uint MSG_ALI::GetToken() const
{
	// Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return byteToInt(GetCmlCommandList()[1],4);
}


byte* MSG_ALI::GetATLGUID() const
{
	// Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return GetCmlCommandList()[0];
}

void MSG_ALI::SetLoginResult( byte in_byte )
{
	if (GetCmlCommandList()[2] != NULL)
	{
		GetCmlCommandList()[2] = new byte[1];
	}
	GetCmlCommandList()[2][0] = in_byte;
}

void MSG_ALI::SetToken( byte* in_byte_ptr, size_t len /*= 4*/)
{
	if (in_byte_ptr == NULL)
	{
		return;
	}
	m_TokenLen = len;
	if (GetCmlCommandList()[1] != NULL)
	{
		GetCmlCommandList()[1] = new byte[len];
	}
	memset(GetCmlCommandList()[1],0,len);
	memcpy(GetCmlCommandList()[1],in_byte_ptr,len);
}

void MSG_ALI::SetATLGUID( byte* in_byte_ptr, size_t len /*= 16*/ )
{
	if (in_byte_ptr == NULL)
	{
		return;
	}
	m_ALTGUIDLen = len;
	if (GetCmlCommandList()[0] != NULL)
	{
		GetCmlCommandList()[0] = new byte[len];
	}
	memset(GetCmlCommandList()[0],0,len);
	memcpy(GetCmlCommandList()[0],in_byte_ptr,len);
}



//RPS(个人信息同步通知)
void MSG_RPS::Initialize()
{
// 	if (GetBeginTab() != true || GetEndTab() != true)
// 	{
// 		m_Error++;
// 	}

	if (GetCommand() != m_Command)
	{
		m_Error++;
	}

	if (GetCmlCount() != m_CmlCount)
	{
		m_Error++;
	}
	//效验码
	// 	if (strcmp(returnVerify(),"") != 0)
	// 	{
	// 		m_Error++;
	// 	}

}
uint MSG_RPS::GetRank() const
{
	// Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return byteToInt(GetCmlCommandList()[4],1);
}


byte* MSG_RPS::GetNickname() const
{ 
	// Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return GetCmlCommandList()[3];
}


byte* MSG_RPS::GetAutograph() const
{
	// Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return GetCmlCommandList()[2];
}

uint MSG_RPS::GetHeadForm() const
{
	// Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return byteToInt(GetCmlCommandList()[1],1);
}

byte* MSG_RPS::GetHeadPortrait() const
{
	// Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return GetCmlCommandList()[0];
}

void MSG_RPS::SetRank( byte in_byte )
{
	if (GetCmlCommandList()[4] != NULL)
	{
		GetCmlCommandList()[4] = new byte[1];
	}
	GetCmlCommandList()[4][0] = in_byte;
}


void MSG_RPS::SetNickname( byte* in_byte_ptr,size_t len )
{
	if (in_byte_ptr == NULL)
	{
		return;
	}
	m_NicknameLen = len;
	if (GetCmlCommandList()[3] != NULL)
	{
		GetCmlCommandList()[3] = new byte[len];
	}
	memset(GetCmlCommandList()[3],0,len);
	memcpy(GetCmlCommandList()[3],in_byte_ptr,len);
}


void MSG_RPS::SetAutograph( byte* in_byte_ptr,size_t len )
{
	if (in_byte_ptr == NULL)
	{
		return;
	}
	m_AutographLen = len;
	if (GetCmlCommandList()[2] != NULL)
	{
		GetCmlCommandList()[2] = new byte[len];
	}
	memset(GetCmlCommandList()[2],0,len);
	memcpy(GetCmlCommandList()[2],in_byte_ptr,len);
}

void MSG_RPS::SetHeadForm( byte in_byte )
{
	if (GetCmlCommandList()[1] != NULL)
	{
		GetCmlCommandList()[1] = new byte[1];
	}
	GetCmlCommandList()[1][0] = in_byte;
}

void MSG_RPS::SetHeadPortrait( byte* in_byte_ptr,size_t len )
{
	if (in_byte_ptr == NULL)
	{
		return;
	}
	m_HeadPortraitLen = len;
	if (GetCmlCommandList()[0] != NULL)
	{
		GetCmlCommandList()[0] = new byte[len];
	}
	memset(GetCmlCommandList()[0],0,len);
	memcpy(GetCmlCommandList()[0],in_byte_ptr,len);
}




//APS(个人信息同步通知确认)
void MSG_APS::Initialize()
{
// 	if (GetBeginTab() != true || GetEndTab() != true)
// 	{
// 		m_Error++;
// 	}

	if (GetCommand() != m_Command)
	{
		m_Error++;
	}

	if (GetCmlCount() != m_CmlCount)
	{
		m_Error++;
	}
	//效验码
	// 	if (strcmp(returnVerify(),"") != 0)
	// 	{
	// 		m_Error++;
	// 	}

}
byte* MSG_APS::GetMessageSynchro() const
{
	// Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return GetCmlCommandList()[0];
}

void MSG_APS::SetMessageSynchro( byte* in_byte_ptr,size_t len )
{
	if (in_byte_ptr == NULL)
	{
		return;
	}
	m_MessageSynchroLen = len;
	if (GetCmlCommandList()[0] != NULL)
	{
		GetCmlCommandList()[0] = new byte[len];
	}
	memset(GetCmlCommandList()[0],0,len);
	memcpy(GetCmlCommandList()[0],in_byte_ptr,len);
}
