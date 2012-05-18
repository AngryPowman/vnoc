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


bool CMessage::returnBeginTab()
{
	return m_Begin;
} 

bool CMessage::returnEndTab()
{
	return m_Begin;
}

byte CMessage::returnCommand()
{
	return m_Command;
}

byte* CMessage::returnCmlListLen()
{
	return  m_CmlListLen;
}

byte CMessage::returnSerial()
{
	return m_Serial;
}

byte* CMessage::returnGUID()
{
	return m_GUID;
}

byte** CMessage::returnCmlCommandList()
{
	return m_CmlCommandList;
}

uint CMessage::returnVerify()
{
	return m_Verify;
}

uint CMessage::returnObligate()
{
	return m_Obligate;
}

uint CMessage::returnVersion()
{
	return m_Ver;
}

uint CMessage::returnCmlCount()
{
	return m_CmlCount;
}

uint CMessage::returnDataLen()
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
	if (returnBeginTab() != true || returnEndTab() != true)
	{
		m_Error++;
	}

	if (returnCommand() != m_Command)
	{
		m_Error++;
	}

	if (returnCmlCount() != m_CmlCount)
	{
		m_Error++;
	}
	//效验码
// 	if (strcmp(returnVerify(),"") != 0)
// 	{
// 		m_Error++;
// 	}

}

byte* MSG_RVC::returnMachineAddress()
{
	Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return returnCmlCommandList()[0];
}

//AVC(获取验证码响应)
void MSG_AVC::Initialize()
{
	if (returnBeginTab() != true || returnEndTab() != true)
	{
		m_Error++;
	}

	if (returnCommand() != m_Command)
	{
		m_Error++;
	}

	if (returnCmlCount() != m_CmlCount)
	{
		m_Error++;
	}
	//效验码
	// 	if (strcmp(returnVerify(),"") != 0)
	// 	{
	// 		m_Error++;
	// 	}

}

uint MSG_AVC::returnLoginTag()
{
	Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return byteToInt(returnCmlCommandList()[2]);
}

uint MSG_AVC::returnType()
{
	Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return byteToInt(returnCmlCommandList()[1]);
}

byte* MSG_AVC::returnCarrier()
{
	Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return returnCmlCommandList()[0];
}


//RLI(登录请求)
void MSG_RLI::Initialize()
{
	if (returnBeginTab() != true || returnEndTab() != true)
	{
		m_Error++;
	}

	if (returnCommand() != m_Command)
	{
		m_Error++;
	}

	if (returnCmlCount() != m_CmlCount)
	{
		m_Error++;
	}
	//效验码
	// 	if (strcmp(returnVerify(),"") != 0)
	// 	{
	// 		m_Error++;
	// 	}

}
byte* MSG_RLI::returnVerificationCode()
{
	Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return returnCmlCommandList()[2];
}


byte* MSG_RLI::returnAccountNumber()
{
	Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return returnCmlCommandList()[1];
}


byte* MSG_RLI::returnPassword()
{
	Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return returnCmlCommandList()[0];
}

//ALI(登录应答)
void MSG_ALI::Initialize()
{
	if (returnBeginTab() != true || returnEndTab() != true)
	{
		m_Error++;
	}

	if (returnCommand() != m_Command)
	{
		m_Error++;
	}

	if (returnCmlCount() != m_CmlCount)
	{
		m_Error++;
	}
	//效验码
	// 	if (strcmp(returnVerify(),"") != 0)
	// 	{
	// 		m_Error++;
	// 	}

}
uint MSG_ALI::returnResult()
{
	Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return byteToInt(returnCmlCommandList()[2]);
}


byte* MSG_ALI::returnToken()
{
	Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return returnCmlCommandList()[1];
}


byte* MSG_ALI::returnATLGUID()
{
	Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return returnCmlCommandList()[0];
}

//RPS(个人信息同步通知)
void MSG_RPS::Initialize()
{
	if (returnBeginTab() != true || returnEndTab() != true)
	{
		m_Error++;
	}

	if (returnCommand() != m_Command)
	{
		m_Error++;
	}

	if (returnCmlCount() != m_CmlCount)
	{
		m_Error++;
	}
	//效验码
	// 	if (strcmp(returnVerify(),"") != 0)
	// 	{
	// 		m_Error++;
	// 	}

}
uint MSG_RPS::returnRank()
{
	Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return byteToInt(returnCmlCommandList()[4]);
}


byte* MSG_RPS::returnNickname()
{
	Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return returnCmlCommandList()[3];
}


byte* MSG_RPS::returnAutograph()
{
	Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return returnCmlCommandList()[2];
}

uint MSG_RPS::returnHeadForm()
{
	Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return byteToInt(returnCmlCommandList()[1]);
}

byte* MSG_RPS::returnHeadPortrait()
{
	Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return returnCmlCommandList()[0];
}

//APS(个人信息同步通知确认)
void MSG_APS::Initialize()
{
	if (returnBeginTab() != true || returnEndTab() != true)
	{
		m_Error++;
	}

	if (returnCommand() != m_Command)
	{
		m_Error++;
	}

	if (returnCmlCount() != m_CmlCount)
	{
		m_Error++;
	}
	//效验码
	// 	if (strcmp(returnVerify(),"") != 0)
	// 	{
	// 		m_Error++;
	// 	}

}
byte* MSG_APS::returnMessageSynchro()
{
	Initialize();
	if (m_Error != 0)
	{
		return 0;
	}
	return returnCmlCommandList()[0];
}