
#pragma once


/*
#define  MSG_ERROR_BDMISS  1
#define  MSG_ERROR_COMMAND 2
#define  MSG_ERROR_PARAM   3
#define  MSG_ERROR_LEN     4*/

#include <iostream>


#define MSG_BEGIN    0x55   // 'V' 标记消息的开始
#define MSG_END		 0x43	// 'C' 标记消息的结束

#define  MSG_CLASS_BEGIN  1
#define  MSG_CLASS_END    1
#define  MSG_CLASS_LEN    4
#define	 MSG_CLASS_VER    1
#define  MSG_CLASS_SERIAL 1
#define  MSG_CLASS_GUID   16
#define  MSG_CLASS_COMMAND 1
#define  MSG_CLASS_OBL    4
#define  MSG_CLASS_PARAMCONST 1
#define  MSG_CLASS_VERIFY 2   //效验码 未定默认为2个字节
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


	bool  m_Begin;				  //标记消息的开始  统一字符'V"的ASCⅡ码 0x55
	bool  m_End;				  //标记消息的结束  固定字符'C‘的ASCⅡ码 0x43

	byte   m_Command;			  //指令			具体的指令，用来标注此数据包的功能
	byte*  m_CmlListLen;		  //参数列表    4字节，对应参数N的长度
	byte   m_Serial;			  //序号       指令的编号
	byte   m_GUID[17];            //GUID       用来提供用户验证
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
class MSG_AVC: public CMessage
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
class MSG_RLI:public CMessage
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
class MSG_ALI:public CMessage
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
class MSG_RPS:public CMessage
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
class MSG_APS:public CMessage
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