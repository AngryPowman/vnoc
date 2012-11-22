
#ifndef VNOC_MSG_AVC
#define VNOC_MSG_AVC

#include "BaseMessage.h"

//AVC(获取验证码响应)
class MSG_AVC: public CMessage
{
public:

	MSG_AVC(){
		//0x15
		BEGIN_PARAM_LIST
		ADD_PARAM_LIST("LoginTag")
		ADD_PARAM_LIST("CaptchaType")
		ADD_PARAM_LIST("Captcha")
		INIT_PARAM_OBJEDT(21)
		END_PARAM_LIST
	}

	virtual ~MSG_AVC(){}

public:
	//获取验证码结果：1字节，用来标志此次登录的结果
	uint GetLoginTag() const
	GetParam_t_byte_r_int("LoginTag",1);
	//验证码类型：标志后面的验证码是用的何种载体，（根据验证码载体的类型定义一个枚举）
	uint GetCaptchaType() const
	GetParam_t_byte_r_int("CaptchaType",1);
	//验证码载体：由验证码载体长度参数指定大小，可以是任意格式图像或者声音。
	const byte* GetCaptcha() const
	GetParam_t_r("Captcha");


	void SetLoginTag(byte in_byte)
	SetParam_t("LoginTag",in_byte);

	void SetCaptchaType(byte in_byte)
	SetParam_t("CaptchaType",in_byte);

	void SetCaptcha(const byte* in_byte_ptr,size_t len)
	SetParam_t_ptr("Captcha",in_byte_ptr,len);


	
	int GetCaptchaLen() const 
	GetParamLen_t_r("Captcha");

};


#endif