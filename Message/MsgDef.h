

#pragma once


/*
#define  MSG_ERROR_BDMISS  1
#define  MSG_ERROR_COMMAND 2
#define  MSG_ERROR_PARAM   3
#define  MSG_ERROR_LEN     4*/

#include <iostream>
#include <vector>
#include <string>
#include <algorithm> 

#define MSG_BEGIN    0x56   // 'V' �����Ϣ�Ŀ�ʼ
#define MSG_END		 0x43	// 'C' �����Ϣ�Ľ���
#define MSG_VER      0		//�汾��

#define  MSG_CLASS_BEGIN  1
#define  MSG_CLASS_END    1
#define  MSG_CLASS_LEN    4
#define	 MSG_CLASS_VER    1
#define  MSG_CLASS_SERIAL 2
#define  MSG_CLASS_GUID   16
#define  MSG_CLASS_COMMAND 1
#define  MSG_CLASS_OBL    4
#define  MSG_CLASS_PARAMCONST 1
#define  MSG_CLASS_VERIFY 2   //Ч���� δ��Ĭ��Ϊ2���ֽ�
#define  MSG_CLASS_PARAM  4

#define  VER_INDEX    MSG_CLASS_VER
#define  SER_INDEX    (MSG_CLASS_VER + MSG_CLASS_SERIAL)
#define  LEN_INDEX    (MSG_CLASS_LEN + MSG_CLASS_VER + MSG_CLASS_SERIAL)
#define  GUID_INDEX   (MSG_CLASS_LEN + MSG_CLASS_VER + MSG_CLASS_SERIAL + MSG_CLASS_GUID)
#define  COM_INDEX    (MSG_CLASS_LEN + MSG_CLASS_VER + MSG_CLASS_SERIAL + MSG_CLASS_COMMAND + MSG_CLASS_GUID)
#define  OBL_INDEX    (MSG_CLASS_LEN + MSG_CLASS_VER + MSG_CLASS_SERIAL + MSG_CLASS_GUID + MSG_CLASS_COMMAND + MSG_CLASS_OBL)
#define  PAC_INDEX    (MSG_CLASS_LEN + MSG_CLASS_VER + MSG_CLASS_SERIAL + MSG_CLASS_GUID + MSG_CLASS_COMMAND + MSG_CLASS_OBL + MSG_CLASS_PARAMCONST)


enum  MSGTYPE
{
	MSGTYPE_NULL,

	MSG_AVC_TYPE,
	MSG_RVC_TYPE,

	MSG_ALI_TYPE,
	MSG_RLI_TYPE,

	MSG_RPS_TYPE,
	MSG_APS_TYPE,
    MSGTYPE_END
};

enum  MSGCONMMAND
{
	MSGCONMMAND_NULL,

	MSG_RVC_COM = 0x14,
	MSG_AVC_COM = 0x15,

	MSG_RLI_COM = 0x16,
	MSG_ALI_COM = 0x17,

	MSG_RPS_COM = 0x18,
	MSG_APS_COM = 0x19,
};

typedef  unsigned char byte;	
typedef  unsigned int  uint;
typedef unsigned short ushort;


typedef std::vector<byte> ByteArr;

// �����ʹ�С�˻���
#define BigLittleSwap16(A)        ((((ushort)(A) & 0xff00) >> 8) | \
	(((ushort)(A) & 0x00ff) << 8))

// �����ʹ�С�˻���
#define BigLittleSwap32(A)        ((((uint)(A) & 0xff000000) >> 24) | \
	(((uint)(A) & 0x00ff0000) >> 8) | \
	(((uint)(A) & 0x0000ff00) << 8) | \
	(((uint)(A) & 0x000000ff) << 24))


uint byteToInt(const byte* in_byte,size_t len);

void IntTobyte(int in_int,byte* out_byte);

void LittleSwapBigByte(byte* in_byte_ptr,size_t len);

void LittleSwapBigByte(ByteArr* arr);

class CMessage
{
public:

	friend class CMessageParser;

	CMessage(){
		_Initialization();
	}

	virtual ~CMessage(){
		_Close();
	}
	//��ȡ��Ϣ����
	int  GetMessageType() const;


public:

	bool GetBeginTab() const ;

	bool GetEndTab() const;

	byte GetCommand() const;

	byte* GetComListLen() const;

	uint GetSerial() const;

	const byte* GetGUID() const;

	std::vector<ByteArr> GetComCommandList() const;


	uint GetVerify() const;

	uint GetObligate() const;

	uint GetVersion() const;

	uint GetCmlCount() const;

	uint GetDataLen() const;


// Set

	bool SetCommand(byte in_byte);

	bool SetSerial(ushort in_short);

	bool SetGUID(byte* in_byte_ptr);

	bool SetComCommandList(int ComCount);
	
	bool SetComListLen(byte* in_byte_ptr,int CmlCount);

	bool SetVerify(uint in_Int);

	bool SetObligate(uint in_Int);

	bool SetVersion(uint in_Int);

	bool SetCmlCount(uint in_Int);

	bool SetDataLen(uint in_Int);


protected:


	bool  m_Begin;				  //�����Ϣ�Ŀ�ʼ  ͳһ�ַ�'V"��ASC���� 0x56
	bool  m_End;				  //�����Ϣ�Ľ���  �̶��ַ�'C����ASC���� 0x43
								  //�����б�    4�ֽڣ���Ӧ����N�ĳ���
	std::vector<byte>			 m_ComListLen;  
								 //�����б�    �����Ĳ������������͸��ݾ���ָ�����
	std::vector<ByteArr>         m_ComCommandList;
	byte   m_Command;			  //ָ��			�����ָ�������ע�����ݰ��Ĺ���
	byte   m_Serial[2];			  //���       ָ��ı��
	byte   m_GUID[16];            //GUID       �����ṩ�û���֤
	//DWORD dwType;				  //��Ϣ����		


	uint  m_Verify;               //Ч���� Ԥ��

	uint  m_Obligate;              //Ԥ���ռ�
	uint  m_Ver;				   //�汾��     ��־��VNOCЭ��İ汾
	uint  m_CmlCount;			   //��������    �����Ĳ������������͸��ݾ���ָ�����
	uint  m_Len;                   //���峤��	   4���ֽڣ�����ָ������ĳ��ȣ����Ը������ݽ�����

private:


	void   _Initialization();
	void   _Close();
	int _MessageType();
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
		m_MachineAddressLen = 0;
		//Ԥ��
		SetDataLen(0);
		SetVersion(MSG_VER);
		SetSerial(0x00);
		SetCommand(m_Command);
		SetObligate(0);
		SetCmlCount(m_CmlCount);
		SetVerify(0);
		//���ò������ȿռ�
		SetComListLen( 0, m_CmlCount );
		//���ò����ռ�
		SetComCommandList(m_CmlCount);
	}
	//������ַ
	byte* GetMachineAddress() const;

	void SetMachineAddress(byte* in_byte_ptr, size_t len);

	int  GetMachineAddressLen()const{
		return  m_MachineAddressLen;
    }
	
	~MSG_RVC(){}
	
private:

	void Initialize();
	
	//��������
	int m_CmlCount;

	//ָ��
	int m_Command;

	//������
	int m_Error;

	//���MachineAddress����
	int m_MachineAddressLen;

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
		m_CaptchaLen =0;
		//Ԥ��
		SetDataLen(0);
		SetVersion(MSG_VER);
		SetSerial(0x00);
		SetCommand(m_Command);
		SetObligate(0);
		SetCmlCount(m_CmlCount);
		SetVerify(0);
		//���ò������ȿռ�
		SetComListLen( 0, m_CmlCount );
		//���ò����ռ�
		SetComCommandList(m_CmlCount);
	}
	//��ȡ��֤������1�ֽڣ�������־�˴ε�¼�Ľ��
    uint GetLoginTag() const;
	//��֤�����ͣ���־�������֤�����õĺ������壬��������֤����������Ͷ���һ��ö�٣�
	uint GetCaptchaType() const;
	//��֤�����壺����֤�����峤�Ȳ���ָ����С�������������ʽͼ�����������
	byte* GetCaptcha() const;
	

	void SetLoginTag(byte in_byte);

	void SetCaptchaType(byte in_byte);

	void SetCaptcha(byte* in_byte_ptr,size_t len);


	//
    int GetCaptchaLen() const {
		return m_CaptchaLen;
	}

	~MSG_AVC(){}

private:

	void Initialize();

	//��������
	int m_CmlCount;

	//ָ��
	int m_Command;

	//������
	int m_Error;

	//���Captcha����
	int m_CaptchaLen;

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
		m_VerificationCodeLen = 0;
	    m_AccountNumberLen = 0;
		m_PasswordLen  = 0;
		//Ԥ��
		SetDataLen(0);
		SetVersion(MSG_VER);
		SetSerial(0x00);
		SetCommand(m_Command);
		SetObligate(0);
		SetCmlCount(m_CmlCount);
		SetVerify(0);
		//���ò������ȿռ�
		SetComListLen( 0, m_CmlCount );
		//���ò����ռ�
		SetComCommandList(m_CmlCount);
	}
	//��֤�룺δ֪���ȣ�ΪAUT��������ͼƬ�������ַ���ɣ��˲����ڷ�������Ӧ�������ư󶨣���һ����֤��-����ӳ�䣩
	byte* GetVerificationCode() const;
	//�û��ʺţ��û��ʺű���Ķ���������
	byte* GetAccountNumber() const;
	//�û����룺���뾭�������Ķ���������
	byte* GetPassword() const;


	void SetVerificationCode(byte* in_byte_ptr,size_t len);

	void SetAccountNumber(byte* in_byte_ptr,size_t len);

	void SetPassword(byte* in_byte_ptr,size_t len);


	int GetVerificationCodeLen() const{
		return m_VerificationCodeLen;
	}

	int GetAccountNumberLen() const{
		return m_AccountNumberLen;
	}

	int GetPasswordLen() const{
		return m_PasswordLen;
	}

	~MSG_RLI(){}

private:

	void Initialize();

	//��������
	int m_CmlCount;

	//ָ��
	int m_Command;

	//������
	int m_Error;

	//�洢����
	int m_VerificationCodeLen;
	int m_AccountNumberLen;
	int m_PasswordLen;
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
		m_TokenLen = 0;
		m_ALTGUIDLen = 0;
		//Ԥ��
		SetDataLen(0);
		SetVersion(MSG_VER);
		SetSerial(0x00);
		SetCommand(m_Command);
		SetObligate(0);
		SetCmlCount(m_CmlCount);
		SetVerify(0);
		//���ò������ȿռ�
		SetComListLen( 0, m_CmlCount );
		//���ò����ռ�
		SetComCommandList(m_CmlCount);
	}
	//��¼�����1�ֽڣ�������־�˴ε�¼�Ľ��
	uint GetLoginResult() const;
	//���Ƴ��ȣ�4�ֽڣ�0x00 00 00 10�û��ʺű����ĳ��ȣ���λΪ�ֽ�
	uint GetToken() const;
	//���ƣ�16�ֽڣ�һ��GUID ���ʺ���֤ͨ�������ɣ�ͨ�������ݰ������ͻ��ˣ�֮���ָ���ڰ�ͷ��������ϴ�����
	byte* GetATLGUID() const;

	void SetLoginResult(byte in_byte);

	void SetToken(byte* in_byte_ptr,size_t len = 4);

	void SetATLGUID(byte* in_byte_ptr, size_t len = 16);


	int GetATLGUIDLen() const{
		return m_ALTGUIDLen;
	}

	int GetTokenLen() const{
		return m_TokenLen;
	}

	~MSG_ALI(){}

private:

	void Initialize();

	//��������
	int m_CmlCount;

	//ָ��
	int m_Command;

	//������
	int m_Error;

	//TokenLen
	int m_TokenLen;
	//ATLGUIDLen
	int m_ALTGUIDLen;
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
	    m_NicknameLen = 0;
		m_AutographLen = 0;
		m_HeadPortraitLen = 0;
		//Ԥ��
		SetDataLen(0);
		SetVersion(MSG_VER);
		SetSerial(0x00);
		SetCommand(m_Command);
		SetObligate(0);
		SetCmlCount(m_CmlCount);
		SetVerify(0);
		//���ò������ȿռ�
		SetComListLen( 0, m_CmlCount );
		//���ò����ռ�
		SetComCommandList(m_CmlCount);
	}
	//�û�Ȩ��
	uint GetRank() const;
	//�ǳ�
	byte* GetNickname() const;
	//����ǩ��
	byte* GetAutograph() const;
	//ͷ���ʽ
	uint GetHeadForm() const;
	//ͷ��
	byte* GetHeadPortrait() const;

	void SetRank(byte in_byte);

	void SetNickname(byte* in_byte_ptr,size_t len);

	void SetAutograph(byte* in_byte_ptr,size_t len);

	void SetHeadForm(byte in_byte);

	void SetHeadPortrait(byte* in_byte_ptr,size_t len);


	int GetNicknameLen() const{
		return m_NicknameLen;
	}

	int GetAutographLen() const{
		return m_AutographLen;
	}

	int GetHeadPortraitLen() const{
		return m_HeadPortraitLen;
	}

	~MSG_RPS(){}

private:

	void Initialize();

	//��������
	int m_CmlCount;

	//ָ��
	int m_Command;

	//������
	int m_Error;

	//����
	int m_NicknameLen;
	int m_AutographLen;
	int m_HeadPortraitLen;
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
		m_MessageSynchroLen = 0;
		//Ԥ��
		SetDataLen(0);
		SetVersion(MSG_VER);
		SetSerial(0x00);
		SetCommand(m_Command);
		SetObligate(0);
		SetCmlCount(m_CmlCount);
		SetVerify(0);
		//���ò������ȿռ�
		SetComListLen( 0, m_CmlCount );
		//���ò����ռ�
		SetComCommandList(m_CmlCount);
	}

	//������Ϣͬ��֪ͨ
	byte* GetMessageSynchro() const;

	void SetMessageSynchro(byte* in_byte_ptr,size_t len);

	int GetMessageSynchroLen() const{
		return m_MessageSynchroLen;
	}

	~MSG_APS(){}

private:

	void Initialize();

	//��������
	int m_CmlCount;

	//ָ��
	int m_Command;

	//������
	int m_Error;

	//����
	int m_MessageSynchroLen;
};