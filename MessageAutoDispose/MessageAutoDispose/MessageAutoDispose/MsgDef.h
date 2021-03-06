
#pragma once


/*
#define  MSG_ERROR_BDMISS  1
#define  MSG_ERROR_COMMAND 2
#define  MSG_ERROR_PARAM   3
#define  MSG_ERROR_LEN     4*/

#include <iostream>


#define MSG_BEGIN    0x55   // 'V' 标记消息的开始
#define MSG_END		 0x43	// 'C' 标记消息的结束
#define MSG_VER      0		//版本号

#define  MSG_CLASS_BEGIN  1
#define  MSG_CLASS_END    1
#define  MSG_CLASS_LEN    4
#define	 MSG_CLASS_VER    1
#define  MSG_CLASS_SERIAL 2
#define  MSG_CLASS_GUID   16
#define  MSG_CLASS_COMMAND 1
#define  MSG_CLASS_OBL    4
#define  MSG_CLASS_PARAMCONST 1
#define  MSG_CLASS_VERIFY 2   //效验码 未定默认为2个字节
#define  MSG_CLASS_PARAM  4

#define  VER_INDEX    MSG_CLASS_VER
#define  SER_INDEX    MSG_CLASS_VER + MSG_CLASS_SERIAL
#define	 LEN_INDEX	  MSG_CLASS_LEN + MSG_CLASS_VER + MSG_CLASS_SERIAL
#define  GUID_INDEX   MSG_CLASS_LEN + MSG_CLASS_VER + MSG_CLASS_SERIAL + MSG_CLASS_GUID
#define  COM_INDEX	  MSG_CLASS_LEN + MSG_CLASS_VER + MSG_CLASS_SERIAL + MSG_CLASS_COMMAND + MSG_CLASS_GUID
#define	 OBL_INDEX	  MSG_CLASS_LEN + MSG_CLASS_VER + MSG_CLASS_SERIAL + MSG_CLASS_GUID + MSG_CLASS_COMMAND + MSG_CLASS_OBL
#define  PAC_INDEX    MSG_CLASS_LEN + MSG_CLASS_VER + MSG_CLASS_SERIAL + MSG_CLASS_GUID + MSG_CLASS_COMMAND + MSG_CLASS_OBL + MSG_CLASS_PARAMCONST

//$ MSGTYPE
enum  MSGTYPE
{
	MSGTYPE_NULL,
	MSG_AVC_TYPE,
	MSG_RVC_TYPE,
	MSG_ALI_TYPE,
	MSG_RLI_TYPE,
	MSG_RPS_TYPE,
	MSG_APS_TYPE,
};

//$ MSGCONMMAND
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

// 短整型大小端互换
#define BigLittleSwap16(A)        ((((ushort)(A) & 0xff00) >> 8) | \
	(((ushort)(A) & 0x00ff) << 8))

// 长整型大小端互换
#define BigLittleSwap32(A)        ((((uint)(A) & 0xff000000) >> 24) | \
	(((uint)(A) & 0x00ff0000) >> 8) | \
	(((uint)(A) & 0x0000ff00) << 8) | \
	(((uint)(A) & 0x000000ff) << 24))


uint byteToInt(byte* in_byte,size_t len);

void IntTobyte(int in_int,byte* out_byte);

void LittleSwapBigByte(byte* in_byte_ptr,size_t len);


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
	//获取消息类型
	int  GetMessageType();


public:

	bool GetBeginTab();

	bool GetEndTab();

	byte GetCommand();

	byte* GetCmlListLen();

	uint GetSerial();

	byte* GetGUID();

	byte** GetCmlCommandList();

	uint GetVerify();

	uint GetObligate();

	uint GetVersion();

	uint GetCmlCount();

	uint GetDataLen();


// Set

	bool SetCommand(byte in_byte);

	bool SetCmlListLen(byte* in_byte_ptr,int CmlCount);

	bool SetSerial(ushort in_short);

	bool SetGUID(byte* in_byte_ptr);

	bool SetCmlCommandList(int CmlCount);

	bool SetVerify(uint in_Int);

	bool SetObligate(uint in_Int);

	bool SetVersion(uint in_Int);

	bool SetCmlCount(uint in_Int);

	bool SetDataLen(uint in_Int);


private:


	bool  m_Begin;				  //标记消息的开始  统一字符'V"的ASCⅡ码 0x55
	bool  m_End;				  //标记消息的结束  固定字符'C‘的ASCⅡ码 0x43

	byte   m_Command;			  //指令			具体的指令，用来标注此数据包的功能
	byte*  m_CmlListLen;		  //参数列表    4字节，对应参数N的长度
	byte   m_Serial[2];			  //序号       指令的编号
	byte   m_GUID[16];            //GUID       用来提供用户验证
	byte** m_CmlCommandList;      //参数列表    编码后的参数，具体类型根据具体指令决定
	//DWORD dwType;				  //消息类型		


	uint  m_Verify;               //效验码 预订

	uint  m_Obligate;              //预留空间
	uint  m_Ver;				   //版本号     标志本VNOC协议的版本
	uint  m_CmlCount;			   //参数数量    编码后的参数，具体类型根据具体指令决定
	uint  m_Len;                   //包体长度	   4个字节，用于指定包体的长度（可以辅助数据解析）

private:


	void   _Initialization();
	void   _Close();
	int _MessageType();
};


//RVC(获取验证码请求)
class MSG_RVC: public CMessage
{
public:
	
	MSG_RVC(){
		//一个参数
		m_CmlCount = 1;
		//0x14
		m_Command  = 20;
		m_Error    = 0;

		//预设
		SetDataLen(0);
		SetVersion(MSG_VER);
		SetSerial(0x00);
		SetCommand(m_Command);
		SetObligate(0);
		SetCmlCount(m_CmlCount);
		SetVerify(0);
		//留好参数长度空间
		SetCmlListLen(0,m_CmlCount);
		//留好参数空间
		SetCmlCommandList(m_CmlCount);
	}
	//机器地址
	byte* GetMachineAddress();

	void SetMachineAddress(byte* in_byte_ptr, size_t len);

	int  GetMachineAddressLen(){
		return  m_MachineAddressLen;
    }
	
	~MSG_RVC(){}
	
private:

	void Initialize();

	//机器地址
	//byte* m_MachineAddress;
	
	//参数数量
	int m_CmlCount;

	//指令
	int m_Command;

	//错误标记
	int m_Error;

	//存放MachineAddress长度
	int m_MachineAddressLen;

};

//AVC(获取验证码响应)
class MSG_AVC: public CMessage
{
public:

	MSG_AVC(){
		//三个参数
		m_CmlCount = 3;
		//0x15
		m_Command  = 21;
		m_Error    = 0;

		//预设
		SetDataLen(0);
		SetVersion(MSG_VER);
		SetSerial(0x00);
		SetCommand(m_Command);
		SetObligate(0);
		SetCmlCount(m_CmlCount);
		SetVerify(0);
		//留好参数长度空间
		SetCmlListLen(0,m_CmlCount);
		//留好参数空间
		SetCmlCommandList(m_CmlCount);
	}
	//获取验证码结果：1字节，用来标志此次登录的结果
    uint GetLoginTag();
	//验证码类型：标志后面的验证码是用的何种载体，（根据验证码载体的类型定义一个枚举）
	uint GetCaptchaType();
	//验证码载体：由验证码载体长度参数指定大小，可以是任意格式图像或者声音。
	byte* GetCaptcha();
	

	void SetLoginTag(byte in_byte);

	void SetCaptchaType(byte in_byte);

	void SetCaptcha(byte* in_byte_ptr,size_t len);


	//
    int GetCaptchaLen(){
		return m_CaptchaLen;
	}

	~MSG_AVC(){}

private:

	void Initialize();

	//参数数量
	int m_CmlCount;

	//指令
	int m_Command;

	//错误标记
	int m_Error;

	//存放Captcha长度
	int m_CaptchaLen;

};


//RLI(登录请求)
class MSG_RLI:public CMessage
{

public:

	MSG_RLI(){
		//三个参数
		m_CmlCount = 3;
		//0x16
		m_Command  = 22;
		m_Error    = 0;

		//预设
		SetDataLen(0);
		SetVersion(MSG_VER);
		SetSerial(0x00);
		SetCommand(m_Command);
		SetObligate(0);
		SetCmlCount(m_CmlCount);
		SetVerify(0);
		//留好参数长度空间
		SetCmlListLen(0,m_CmlCount);
		//留好参数空间
		SetCmlCommandList(m_CmlCount);
	}
	//验证码：未知长度，为AUT传回来的图片包含的字符组成，此参数在服务器端应该与令牌绑定（有一个验证码-令牌映射）
	byte* GetVerificationCode();
	//用户帐号：用户帐号编码的二进制数据
	byte* GetAccountNumber();
	//用户密码：密码经过处理后的二进制数据
	byte* GetPassword();


	void SetVerificationCode(byte* in_byte_ptr,size_t len);

	void SetAccountNumber(byte* in_byte_ptr,size_t len);

	void SetPassword(byte* in_byte_ptr,size_t len);


	int GetVerificationCodeLen(){
		return m_VerificationCodeLen;
	}

	int GetAccountNumberLen(){
		return m_AccountNumberLen;
	}

	int GetPasswordLen(){
		return m_PasswordLen;
	}

	~MSG_RLI(){}

private:

	void Initialize();

	//参数数量
	int m_CmlCount;

	//指令
	int m_Command;

	//错误标记
	int m_Error;

	//存储长度
	int m_VerificationCodeLen;
	int m_AccountNumberLen;
	int m_PasswordLen;
};


//ALI(登录应答)
class MSG_ALI:public CMessage
{

public:

	MSG_ALI(){
		//三个参数
		m_CmlCount = 3;
		//0x17
		m_Command  = 23;
		m_Error    = 0;

		//预设
		SetDataLen(0);
		SetVersion(MSG_VER);
		SetSerial(0x00);
		SetCommand(m_Command);
		SetObligate(0);
		SetCmlCount(m_CmlCount);
		SetVerify(0);
		//留好参数长度空间
		SetCmlListLen(0,m_CmlCount);
		//留好参数空间
		SetCmlCommandList(m_CmlCount);
	}
	//登录结果：1字节，用来标志此次登录的结果
	uint GetLoginResult();
	//令牌长度：4字节，0x00 00 00 10用户帐号编码后的长度，单位为字节
	uint GetToken();
	//令牌：16字节，一个GUID ，帐号验证通过后生成，通过此数据包发给客户端，之后的指令在包头部均需带上此令牌
	byte* GetATLGUID();

	void SetLoginResult(byte in_byte);

	void SetToken(byte* in_byte_ptr,size_t len = 4);

	void SetATLGUID(byte* in_byte_ptr, size_t len = 16);


	int GetATLGUIDLen(){
		return m_ALTGUIDLen;
	}

	int GetTokenLen(){
		return m_TokenLen;
	}

	~MSG_ALI(){}

private:

	void Initialize();

	//参数数量
	int m_CmlCount;

	//指令
	int m_Command;

	//错误标记
	int m_Error;

	//TokenLen
	int m_TokenLen;
	//ATLGUIDLen
	int m_ALTGUIDLen;
};


//RPS(个人信息同步通知)
class MSG_RPS:public CMessage
{

public:

	MSG_RPS(){
		//三个参数
		m_CmlCount = 5;
		//0x18
		m_Command  = 24;
		m_Error    = 0;

		//预设
		SetDataLen(0);
		SetVersion(MSG_VER);
		SetSerial(0x00);
		SetCommand(m_Command);
		SetObligate(0);
		SetCmlCount(m_CmlCount);
		SetVerify(0);
		//留好参数长度空间
		SetCmlListLen(0,m_CmlCount);
		//留好参数空间
		SetCmlCommandList(m_CmlCount);
	}
	//用户权限
	uint GetRank();
	//昵称
	byte* GetNickname();
	//个人签名
	byte* GetAutograph();
	//头像格式
	uint GetHeadForm();
	//头像
	byte* GetHeadPortrait();

	void SetRank(byte in_byte);

	void SetNickname(byte* in_byte_ptr,size_t len);

	void SetAutograph(byte* in_byte_ptr,size_t len);

	void SetHeadForm(byte in_byte);

	void SetHeadPortrait(byte* in_byte_ptr,size_t len);


	int GetNicknameLen(){
		return m_NicknameLen;
	}

	int GetAutographLen(){
		return m_AutographLen;
	}

	int GetHeadPortraitLen(){
		return m_HeadPortraitLen;
	}

	~MSG_RPS(){}

private:

	void Initialize();

	//参数数量
	int m_CmlCount;

	//指令
	int m_Command;

	//错误标记
	int m_Error;

	//长度
	int m_NicknameLen;
	int m_AutographLen;
	int m_HeadPortraitLen;
};


//APS(个人信息同步通知确认)
class MSG_APS:public CMessage
{

public:

	MSG_APS(){
		//三个参数
		m_CmlCount = 1;
		//0x19
		m_Command  = 25;
		m_Error    = 0;

		//预设
		SetDataLen(0);
		SetVersion(MSG_VER);
		SetSerial(0x00);
		SetCommand(m_Command);
		SetObligate(0);
		SetCmlCount(m_CmlCount);
		SetVerify(0);
		//留好参数长度空间
		SetCmlListLen(0,m_CmlCount);
		//留好参数空间
		SetCmlCommandList(m_CmlCount);
	}

	//个人信息同步通知
	byte* GetMessageSynchro();

	void SetMessageSynchro(byte* in_byte_ptr,size_t len);

	int GetMessageSynchroLen(){
		return m_MessageSynchroLen;
	}

	~MSG_APS(){}

private:

	void Initialize();

	//参数数量
	int m_CmlCount;

	//指令
	int m_Command;

	//错误标记
	int m_Error;

	//长度
	int m_MessageSynchroLen;
};