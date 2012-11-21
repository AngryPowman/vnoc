

#ifndef VNOC_MSG_ALI
#define VNOC_MSG_ALI

#include "BaseMessage.h"

//ALI(��¼Ӧ��)
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
	//��¼�����1�ֽڣ�������־�˴ε�¼�Ľ��
	uint GetLoginResult() const
	GetParam_t_byte_r_int("LoginResult",1);
	//���Ƴ��ȣ�4�ֽڣ�0x00 00 00 10�û��ʺű����ĳ��ȣ���λΪ�ֽ�
	uint GetToken() const
	GetParam_t_byte_r_int("Token",4);
	//���ƣ�16�ֽڣ�һ��GUID ���ʺ���֤ͨ�������ɣ�ͨ�������ݰ������ͻ��ˣ�֮���ָ���ڰ�ͷ��������ϴ�����
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