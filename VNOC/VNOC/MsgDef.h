
#pragma once


/*
#define  MSG_ERROR_BDMISS  1
#define  MSG_ERROR_COMMAND 2
#define  MSG_ERROR_PARAM   3
#define  MSG_ERROR_LEN     4*/

#include <iostream>


#define MSG_BEGIN    0x55   // 'V' �����Ϣ�Ŀ�ʼ
#define MSG_END		 0x43	// 'C' �����Ϣ�Ľ���

#define  MSG_CLASS_BEGIN  1
#define  MSG_CLASS_END    1
#define  MSG_CLASS_LEN    4
#define	 MSG_CLASS_VER    1
#define  MSG_CLASS_SERIAL 1
#define  MSG_CLASS_GUID   16
#define  MSG_CLASS_COMMAND 1
#define  MSG_CLASS_OBL    4
#define  MSG_CLASS_PARAMCONST 1
#define  MSG_CLASS_VERIFY 2   //Ч���� δ��Ĭ��Ϊ2���ֽ�
#define  MSG_CLASS_PARAM  4

#define  VER_INDEX    MSG_CLASS_LEN + MSG_CLASS_VER
#define	 SER_INDEX	  MSG_CLASS_LEN + MSG_CLASS_VER + MSG_CLASS_SERIAL
#define  GUID_INDEX   MSG_CLASS_LEN + MSG_CLASS_VER + MSG_CLASS_SERIAL + MSG_CLASS_GUID
#define  COM_INDEX	  MSG_CLASS_LEN + MSG_CLASS_VER + MSG_CLASS_SERIAL + MSG_CLASS_COMMAND + MSG_CLASS_GUID
#define	 OBL_INDEX	  MSG_CLASS_LEN + MSG_CLASS_VER + MSG_CLASS_SERIAL + MSG_CLASS_GUID + MSG_CLASS_COMMAND + MSG_CLASS_OBL
#define  PAC_INDEX    MSG_CLASS_LEN + MSG_CLASS_VER + MSG_CLASS_SERIAL + MSG_CLASS_GUID + MSG_CLASS_COMMAND + MSG_CLASS_OBL + MSG_CLASS_PARAMCONST



typedef  unsigned char byte;	
typedef  unsigned int  uint;


uint byteToInt(byte* in_byte);

class CMessage
{
public:

	friend class CMessageParser;

	CMessage(){
		_Initialization();
	}

	~CMessage(){
		_Close();
	}

protected:

	bool returnBeginTab();

	bool returnEndTab();

	byte returnCommand();

	byte* returnCmlListLen();

	byte returnSerial();

	byte* returnGUID();

	byte** returnCmlCommandList();

	uint returnVerify();

	uint returnObligate();

	uint returnVersion();

	uint returnCmlCount();

	uint returnDataLen();

private:


	bool  m_Begin;				  //�����Ϣ�Ŀ�ʼ  ͳһ�ַ�'V"��ASC���� 0x55
	bool  m_End;				  //�����Ϣ�Ľ���  �̶��ַ�'C����ASC���� 0x43

	byte   m_Command;			  //ָ��			�����ָ�������ע�����ݰ��Ĺ���
	byte*  m_CmlListLen;		  //�����б�    4�ֽڣ���Ӧ����N�ĳ���
	byte   m_Serial;			  //���       ָ��ı��
	byte   m_GUID[17];            //GUID       �����ṩ�û���֤
	byte** m_CmlCommandList;      //�����б�    �����Ĳ������������͸��ݾ���ָ�����
	//DWORD dwType;				  //��Ϣ����		


	uint  m_Verify;               //Ч���� Ԥ��

	uint  m_Obligate;              //Ԥ���ռ�
	uint  m_Ver;				   //�汾��     ��־��VNOCЭ��İ汾
	uint  m_CmlCount;			   //��������    �����Ĳ������������͸��ݾ���ָ�����
	uint  m_Len;                   //���峤��	   4���ֽڣ�����ָ������ĳ��ȣ����Ը������ݽ�����

private:

	void   _Initialization();
	void   _Close();

};


//RVC(��ȡ��֤������)
class MSG_RVC: public CMessage
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
class MSG_AVC: public CMessage
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
class MSG_RLI:public CMessage
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
class MSG_ALI:public CMessage
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
class MSG_RPS:public CMessage
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
class MSG_APS:public CMessage
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