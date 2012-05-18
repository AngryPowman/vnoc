#pragma once

#include "MsgDef.h"


uint byteToInt(byte* in_byte)
{
	byte tmpByte[4] = {0};
	if (in_byte == NULL)
	{
		return 0;
	}

	for (int index = 0; index < (int)strlen((const char*)in_byte); index++)
	{
		tmpByte[index] = in_byte[index];
	}

	uint*  lpDataLen = NULL; 
	lpDataLen = (uint *)tmpByte;
	return (*lpDataLen);
}


bool CMessage::GetBeginTab()
{
	return m_Begin;
} 

bool CMessage::GetEndTab()
{
	return m_Begin;
}

byte CMessage::GetCommand()
{
	return m_Command;
}

byte* CMessage::GetCmlListLen()
{
	return  m_CmlListLen;
}

byte CMessage::GetSerial()
{
	return m_Serial;
}

byte* CMessage::GetGUID()
{
	return m_GUID;
}

byte** CMessage::GetCmlCommandList()
{
	return m_CmlCommandList;
}

uint CMessage::GetVerify()
{
	return m_Verify;
}

uint CMessage::GetObligate()
{
	return m_Obligate;
}

uint CMessage::GetVersion()
{
	return m_Ver;
}

uint CMessage::GetCmlCount()
{
	return m_CmlCount;
}

uint CMessage::GetDataLen()
{
	return m_Len;
}


void CMessage::_Initialization()
{
	memset(m_GUID,0,17);
	m_CmlListLen  = NULL;
	m_CmlCommandList = NULL;
	m_CmlCount = 0;
	m_Obligate = 0;
	m_Ver = 0;
	m_Verify  = 0;
	m_Serial  = 0;
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

		delete m_CmlCommandList;
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
	if (GetBeginTab() != true || GetEndTab() != true)
	{
		m_Error++;
	}

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

byte* MSG_RVC::GetMachineAddress()
{
	Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return GetCmlCommandList()[0];
}

//AVC(获取验证码响应)
void MSG_AVC::Initialize()
{
	if (GetBeginTab() != true || GetEndTab() != true)
	{
		m_Error++;
	}

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

uint MSG_AVC::GetLoginTag()
{
	Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return byteToInt(GetCmlCommandList()[2]);
}

uint MSG_AVC::GetType()
{
	Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return byteToInt(GetCmlCommandList()[1]);
}

byte* MSG_AVC::GetCarrier()
{
	Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return GetCmlCommandList()[0];
}


//RLI(登录请求)
void MSG_RLI::Initialize()
{
	if (GetBeginTab() != true || GetEndTab() != true)
	{
		m_Error++;
	}

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
byte* MSG_RLI::GetVerificationCode()
{
	Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return GetCmlCommandList()[2];
}


byte* MSG_RLI::GetAccountNumber()
{
	Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return GetCmlCommandList()[1];
}


byte* MSG_RLI::GetPassword()
{
	Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return GetCmlCommandList()[0];
}

//ALI(登录应答)
void MSG_ALI::Initialize()
{
	if (GetBeginTab() != true || GetEndTab() != true)
	{
		m_Error++;
	}

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
uint MSG_ALI::GetResult()
{
	Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return byteToInt(GetCmlCommandList()[2]);
}


byte* MSG_ALI::GetToken()
{
	Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return GetCmlCommandList()[1];
}


byte* MSG_ALI::GetATLGUID()
{
	Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return GetCmlCommandList()[0];
}

//RPS(个人信息同步通知)
void MSG_RPS::Initialize()
{
	if (GetBeginTab() != true || GetEndTab() != true)
	{
		m_Error++;
	}

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
uint MSG_RPS::GetRank()
{
	Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return byteToInt(GetCmlCommandList()[4]);
}


byte* MSG_RPS::GetNickname()
{
	Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return GetCmlCommandList()[3];
}


byte* MSG_RPS::GetAutograph()
{
	Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return GetCmlCommandList()[2];
}

uint MSG_RPS::GetHeadForm()
{
	Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return byteToInt(GetCmlCommandList()[1]);
}

byte* MSG_RPS::GetHeadPortrait()
{
	Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return GetCmlCommandList()[0];
}

//APS(个人信息同步通知确认)
void MSG_APS::Initialize()
{
	if (GetBeginTab() != true || GetEndTab() != true)
	{
		m_Error++;
	}

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
byte* MSG_APS::GetMessageSynchro()
{
	Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return GetCmlCommandList()[0];
}