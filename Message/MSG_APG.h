#ifndef VNOC_MSG_APG
#define VNOC_MSG_APG


#include "BaseMessage.h"

//APG 注册帐号（请求）
class MSG_APG:public CMessage
{

public:

	MSG_APG(){
		//0x13
		BEGIN_PARAM_LIST
		ADD_PARAM_LIST("RetTag")
		INIT_PARAM_OBJEDT(19)
		END_PARAM_LIST
	}

	virtual ~MSG_APG(){}

public:
	//返回标记
	uint GetRetTag() const
		GetParam_t_byte_r_int("RetTag",4);

	void SetRetTag(uint in_int)
		SetParam_t("RetTag",in_int);
};


#endif