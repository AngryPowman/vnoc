

#ifndef VNOC_MSG_RLI
#define VNOC_MSG_RLI

#include "BaseMessage.h"

//RLI(登录请求)
class MSG_RLI:public CMessage
{

public:

	MSG_RLI(){
		//0x16
		BEGIN_PARAM_LIST
		ADD_PARAM_LIST("VerificationCode")
		ADD_PARAM_LIST("AccountNumber")
		ADD_PARAM_LIST("Password")
		INIT_PARAM_OBJEDT(22)
		END_PARAM_LIST
	}
	virtual ~MSG_RLI(){}

public:
	//验证码：未知长度，为AUT传回来的图片包含的字符组成，此参数在服务器端应该与令牌绑定（有一个验证码-令牌映射）
	const byte* GetVerificationCode() const
	GetParam_t_r("VerificationCode");
	//用户帐号：用户帐号编码的二进制数据
	const byte* GetAccountNumber() const
	GetParam_t_r("AccountNumber");
	//用户密码：密码经过处理后的二进制数据
	const byte* GetPassword() const
	GetParam_t_r("Password");


	void SetVerificationCode(const byte* in_byte_ptr,size_t len)
	SetParam_t_ptr("VerificationCode",in_byte_ptr,len);

	void SetAccountNumber(const byte* in_byte_ptr,size_t len)
	SetParam_t_ptr("AccountNumber",in_byte_ptr,len);

	void SetPassword(const byte* in_byte_ptr,size_t len)
	SetParam_t_ptr("Password",in_byte_ptr,len);


	int GetVerificationCodeLen() const
	GetParamLen_t_r("VerificationCode");

	int GetAccountNumberLen() const
	GetParamLen_t_r("AccountNumber");

	int GetPasswordLen() const
	GetParamLen_t_r("Password");
};


#endif