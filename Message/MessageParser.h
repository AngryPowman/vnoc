

#pragma once

#include "MSG_UNION.h"

class CMessageParser
{
public:
	
	CMessageParser(){
		m_Instruct = 0;
		m_SubPos = 0;
	}

	~CMessageParser(){

	}

public:
	//����
	CMessage* Parse(byte* lpszData,size_t len);

	static int Check(byte* lpszDate,size_t len); //Ч����ĳ���


private:

	CMessage* m_MessageStr;           //

    int       m_SubPos;

	byte   m_Instruct;			  //ָ��			�����ָ�������ע�����ݰ��Ĺ���

	int _Head(CMessage* _Messsage,byte* lpszData,size_t len);

	int _Body(CMessage* _Messsage,byte* lpszData,size_t len);

	int _Tail(CMessage* _Messsage,byte* lpszData,int ParamLen,size_t len);

	CMessage* _MessageType();
	int  _GetMessageType();

	int _CheckUpLen(int Index,size_t len)
	{
		if (Index > (int)len)
		{
			return -1;
		}
		return 0;
	}

};

