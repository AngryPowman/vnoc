
#pragma once

#include "MessageParser.h"

/*
#define  MSG_ERROR_BDMISS  1
#define  MSG_ERROR_COMMAND 2
#define  MSG_ERROR_PARAM   3
#define  MSG_ERROR_LEN     4*/


//RVC(获取验证码请求)
class MSG_RVC: public MessageParser
{
public:
	
	MSG_RVC(){
		//一个参数
		m_CmlCount = 1;
		//0x14
		m_Command  = 20;
		m_Error    = 0;
	}
	//机器地址
	byte* returnMachineAddress();
	
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

};

//AVC(获取验证码响应)
class MSG_AVC: public MessageParser
{
public:

	MSG_AVC(){
		//三个参数
		m_CmlCount = 3;
		//0x15
		m_Command  = 21;
		m_Error    = 0;
	}
	//获取验证码结果：1字节，用来标志此次登录的结果
    uint returnLoginTag();
	//验证码类型：标志后面的验证码是用的何种载体，（根据验证码载体的类型定义一个枚举）
	uint returnType();
	//验证码载体：由验证码载体长度参数指定大小，可以是任意格式图像或者声音。
	byte* returnCarrier();


	~MSG_AVC(){}

private:

	void Initialize();

	//参数数量
	int m_CmlCount;

	//指令
	int m_Command;

	//错误标记
	int m_Error;

};


//RLI(登录请求)
class MSG_RLI:public MessageParser
{

public:

	MSG_RLI(){
		//三个参数
		m_CmlCount = 3;
		//0x16
		m_Command  = 22;
		m_Error    = 0;
	}
	//验证码：未知长度，为AUT传回来的图片包含的字符组成，此参数在服务器端应该与令牌绑定（有一个验证码-令牌映射）
	byte* returnVerificationCode();
	//用户帐号：用户帐号编码的二进制数据
	byte* returnAccountNumber();
	//用户密码：密码经过处理后的二进制数据
	byte* returnPassword();


	~MSG_RLI(){}

private:

	void Initialize();

	//参数数量
	int m_CmlCount;

	//指令
	int m_Command;

	//错误标记
	int m_Error;

};


//ALI(登录应答)
class MSG_ALI:public MessageParser
{

public:

	MSG_ALI(){
		//三个参数
		m_CmlCount = 3;
		//0x17
		m_Command  = 23;
		m_Error    = 0;
	}
	//登录结果：1字节，用来标志此次登录的结果
	uint returnResult();
	//令牌长度：4字节，0x00 00 00 10用户帐号编码后的长度，单位为字节
	byte* returnToken();
	//令牌：16字节，一个GUID ，帐号验证通过后生成，通过此数据包发给客户端，之后的指令在包头部均需带上此令牌
	byte* returnATLGUID();


	~MSG_ALI(){}

private:

	void Initialize();

	//参数数量
	int m_CmlCount;

	//指令
	int m_Command;

	//错误标记
	int m_Error;
};


//RPS(个人信息同步通知)
class MSG_RPS:public MessageParser
{

public:

	MSG_RPS(){
		//三个参数
		m_CmlCount = 5;
		//0x18
		m_Command  = 24;
		m_Error    = 0;
	}
	//用户权限
	uint returnRank();
	//昵称
	byte* returnNickname();
	//个人签名
	byte* returnAutograph();
	//头像格式
	uint returnHeadForm();
	//头像
	byte* returnHeadPortrait();

	~MSG_RPS(){}

private:

	void Initialize();

	//参数数量
	int m_CmlCount;

	//指令
	int m_Command;

	//错误标记
	int m_Error;

};


//APS(个人信息同步通知确认)
class MSG_APS:public MessageParser
{

public:

	MSG_APS(){
		//三个参数
		m_CmlCount = 1;
		//0x19
		m_Command  = 25;
		m_Error    = 0;
	}

	//个人信息同步通知
	byte* returnMessageSynchro();

	~MSG_APS(){}

private:

	void Initialize();

	//参数数量
	int m_CmlCount;

	//指令
	int m_Command;

	//错误标记
	int m_Error;

};