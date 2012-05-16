#pragma once

#include "MsgDef.h"

uint Convert(byte* _Str)
{
	if (_Str == NULL)
	{
		return -1;
	}
	if (strcmp((const char*)_Str,"") == 0)
	{
		return -1;
	}
	//ȥ��ĩβ��0
    //int* tmpInt = new int[strlen((const char*)_Str)];
	int * tmpInt = (int *)_Str;
// 	byte * tmpData = new byte[strlen((const char*)_Str)];
// 	memset(tmpData,0,strlen((const char*)_Str));
// 	for (int index = 0; index < (int)strlen((const char*)_Str); index++)
// 	{
// 		tmpData[index] = _Str[index];
// 	}
	return (*tmpInt);
}


//RVC(��ȡ��֤������)
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
	//Ч����
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

//AVC(��ȡ��֤����Ӧ)
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
	//Ч����
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


//RLI(��¼����)
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
	//Ч����
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

//ALI(��¼Ӧ��)
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
	//Ч����
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

//RPS(������Ϣͬ��֪ͨ)
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
	//Ч����
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

//APS(������Ϣͬ��֪ͨȷ��)
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
	//Ч����
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