#ifndef VNOC_MSG_RPS
#define VNOC_MSG_RPS


#include "BaseMessage.h"

//RPS(������Ϣͬ��֪ͨ)
class MSG_RPS:public CMessage
{

public:

	MSG_RPS(){
		//0x18
		BEGIN_PARAM_LIST
		ADD_PARAM_LIST("Rank")
		ADD_PARAM_LIST("Nickname")
		ADD_PARAM_LIST("Autograph")
		ADD_PARAM_LIST("HeadForm")
		ADD_PARAM_LIST("HeadPortrait")
		INIT_PARAM_OBJEDT(24)
		END_PARAM_LIST
	}

	virtual ~MSG_RPS(){}

public:
	//�û�Ȩ��
	uint GetRank() const
	GetParam_t_byte_r_int("Rank",1);
	//�ǳ�
	const byte* GetNickname() const
	GetParam_t_r("Nickname");
	//����ǩ��
	const byte* GetAutograph() const
	GetParam_t_r("Autograph");
	//ͷ���ʽ
	uint GetHeadForm() const
	GetParam_t_byte_r_int("HeadForm",1);
	//ͷ��
	const byte* GetHeadPortrait() const
	GetParam_t_r("HeadPortrait");


	void SetRank(byte in_byte)
	SetParam_t("Rank",in_byte);

	void SetNickname(const byte* in_byte_ptr,size_t len)
	SetParam_t_ptr("Nickname",in_byte_ptr,len);

	void SetAutograph(const byte* in_byte_ptr,size_t len)
	SetParam_t_ptr("Autograph",in_byte_ptr,len);

	void SetHeadForm(byte in_byte)
	SetParam_t("HeadForm",in_byte);

	void SetHeadPortrait(const byte* in_byte_ptr,size_t len)
	SetParam_t_ptr("HeadPortrait",in_byte_ptr,len);


	int GetNicknameLen() const
	GetParamLen_t_r("Nickname");

	int GetAutographLen() const
	GetParamLen_t_r("Autograph");

	int GetHeadPortraitLen() const
	GetParamLen_t_r("HeadPortrait");
};


#endif