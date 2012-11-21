

#ifndef VNOC_MSG_ALI
#define VNOC_MSG_ALI

#include "BaseMessage.h"

//ALI(登录应答)
class MSG_ALI:public CMessage
{

public:

	MSG_ALI(){
		//0x17
		BEGIN_PARAM_LIST
		ADD_PARAM_LIST("LoginResult")
		ADD_PARAM_LIST("Token")
		ADD_PARAM_LIST("ATLGUID")
		INIT_PARAM_OBJEDT(23)
		END_PARAM_LIST
	}

	virtual ~MSG_ALI(){}

public:
	//登录结果：1字节，用来标志此次登录的结果
	uint GetLoginResult() const
	GetParam_t_byte_r_int("LoginResult",1);
	//令牌长度：4字节，0x00 00 00 10用户帐号编码后的长度，单位为字节
	uint GetToken() const
	GetParam_t_byte_r_int("Token",4);
	//令牌：16字节，一个GUID ，帐号验证通过后生成，通过此数据包发给客户端，之后的指令在包头部均需带上此令牌
	const byte* GetATLGUID() const
	GetParam_t_r("ATLGUID");

	void SetLoginResult(byte in_byte)
	SetParam_t("LoginResult",in_byte);

	void SetToken(const byte* in_byte_ptr,size_t len = 4)
	SetParam_t_ptr("Token",in_byte_ptr,len);

	void SetATLGUID(const byte* in_byte_ptr, size_t len = 16)
	SetParam_t_ptr("ATLGUID",in_byte_ptr,len);


	int GetATLGUIDLen() const
	GetParamLen_t_r("ATLGUID");

	int GetTokenLen() const
	GetParamLen_t_r("Token");
};

#endif