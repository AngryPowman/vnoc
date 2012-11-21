#ifndef VNOC_MSG_RPS
#define VNOC_MSG_RPS


#include "BaseMessage.h"

//RPS(个人信息同步通知)
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
	//用户权限
	uint GetRank() const
	GetParam_t_byte_r_int("Rank",1);
	//昵称
	const byte* GetNickname() const
	GetParam_t_r("Nickname");
	//个人签名
	const byte* GetAutograph() const
	GetParam_t_r("Autograph");
	//头像格式
	uint GetHeadForm() const
	GetParam_t_byte_r_int("HeadForm",1);
	//头像
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