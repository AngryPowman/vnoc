
#ifndef VNOC_MSG_AVC
#define VNOC_MSG_AVC

#include "BaseMessage.h"

//AVC(��ȡ��֤����Ӧ)
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
	//��ȡ��֤������1�ֽڣ�������־�˴ε�¼�Ľ��
	uint GetLoginTag() const
	GetParam_t_byte_r_int("LoginTag",1);
	//��֤�����ͣ���־�������֤�����õĺ������壬��������֤����������Ͷ���һ��ö�٣�
	uint GetCaptchaType() const
	GetParam_t_byte_r_int("CaptchaType",1);
	//��֤�����壺����֤�����峤�Ȳ���ָ����С�������������ʽͼ�����������
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