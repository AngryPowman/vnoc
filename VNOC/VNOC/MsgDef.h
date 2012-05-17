
#pragma once

#include "MessageParser.h"

/*
#define  MSG_ERROR_BDMISS  1
#define  MSG_ERROR_COMMAND 2
#define  MSG_ERROR_PARAM   3
#define  MSG_ERROR_LEN     4*/


//RVC(��ȡ��֤������)
class MSG_RVC: public MessageParser
{
public:
	
	MSG_RVC(){
		//һ������
		m_CmlCount = 1;
		//0x14
		m_Command  = 20;
		m_Error    = 0;
	}
	//������ַ
	byte* returnMachineAddress();
	
	~MSG_RVC(){}
	
private:

	void Initialize();

	//������ַ
	//byte* m_MachineAddress;
	
	//��������
	int m_CmlCount;

	//ָ��
	int m_Command;

	//������
	int m_Error;

};

//AVC(��ȡ��֤����Ӧ)
class MSG_AVC: public MessageParser
{
public:

	MSG_AVC(){
		//��������
		m_CmlCount = 3;
		//0x15
		m_Command  = 21;
		m_Error    = 0;
	}
	//��ȡ��֤������1�ֽڣ�������־�˴ε�¼�Ľ��
    uint returnLoginTag();
	//��֤�����ͣ���־�������֤�����õĺ������壬��������֤����������Ͷ���һ��ö�٣�
	uint returnType();
	//��֤�����壺����֤�����峤�Ȳ���ָ����С�������������ʽͼ�����������
	byte* returnCarrier();


	~MSG_AVC(){}

private:

	void Initialize();

	//��������
	int m_CmlCount;

	//ָ��
	int m_Command;

	//������
	int m_Error;

};


//RLI(��¼����)
class MSG_RLI:public MessageParser
{

public:

	MSG_RLI(){
		//��������
		m_CmlCount = 3;
		//0x16
		m_Command  = 22;
		m_Error    = 0;
	}
	//��֤�룺δ֪���ȣ�ΪAUT��������ͼƬ�������ַ���ɣ��˲����ڷ�������Ӧ�������ư󶨣���һ����֤��-����ӳ�䣩
	byte* returnVerificationCode();
	//�û��ʺţ��û��ʺű���Ķ���������
	byte* returnAccountNumber();
	//�û����룺���뾭�������Ķ���������
	byte* returnPassword();


	~MSG_RLI(){}

private:

	void Initialize();

	//��������
	int m_CmlCount;

	//ָ��
	int m_Command;

	//������
	int m_Error;

};


//ALI(��¼Ӧ��)
class MSG_ALI:public MessageParser
{

public:

	MSG_ALI(){
		//��������
		m_CmlCount = 3;
		//0x17
		m_Command  = 23;
		m_Error    = 0;
	}
	//��¼�����1�ֽڣ�������־�˴ε�¼�Ľ��
	uint returnResult();
	//���Ƴ��ȣ�4�ֽڣ�0x00 00 00 10�û��ʺű����ĳ��ȣ���λΪ�ֽ�
	byte* returnToken();
	//���ƣ�16�ֽڣ�һ��GUID ���ʺ���֤ͨ�������ɣ�ͨ�������ݰ������ͻ��ˣ�֮���ָ���ڰ�ͷ��������ϴ�����
	byte* returnATLGUID();


	~MSG_ALI(){}

private:

	void Initialize();

	//��������
	int m_CmlCount;

	//ָ��
	int m_Command;

	//������
	int m_Error;
};


//RPS(������Ϣͬ��֪ͨ)
class MSG_RPS:public MessageParser
{

public:

	MSG_RPS(){
		//��������
		m_CmlCount = 5;
		//0x18
		m_Command  = 24;
		m_Error    = 0;
	}
	//�û�Ȩ��
	uint returnRank();
	//�ǳ�
	byte* returnNickname();
	//����ǩ��
	byte* returnAutograph();
	//ͷ���ʽ
	uint returnHeadForm();
	//ͷ��
	byte* returnHeadPortrait();

	~MSG_RPS(){}

private:

	void Initialize();

	//��������
	int m_CmlCount;

	//ָ��
	int m_Command;

	//������
	int m_Error;

};


//APS(������Ϣͬ��֪ͨȷ��)
class MSG_APS:public MessageParser
{

public:

	MSG_APS(){
		//��������
		m_CmlCount = 1;
		//0x19
		m_Command  = 25;
		m_Error    = 0;
	}

	//������Ϣͬ��֪ͨ
	byte* returnMessageSynchro();

	~MSG_APS(){}

private:

	void Initialize();

	//��������
	int m_CmlCount;

	//ָ��
	int m_Command;

	//������
	int m_Error;

};