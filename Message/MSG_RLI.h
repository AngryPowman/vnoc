

#ifndef VNOC_MSG_RLI
#define VNOC_MSG_RLI

#include "BaseMessage.h"

//RLI(��¼����)
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
	//��֤�룺δ֪���ȣ�ΪAUT��������ͼƬ�������ַ���ɣ��˲����ڷ�������Ӧ�������ư󶨣���һ����֤��-����ӳ�䣩
	const byte* GetVerificationCode() const
	GetParam_t_r("VerificationCode");
	//�û��ʺţ��û��ʺű���Ķ���������
	const byte* GetAccountNumber() const
	GetParam_t_r("AccountNumber");
	//�û����룺���뾭�������Ķ���������
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