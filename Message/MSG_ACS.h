#ifndef VNOC_MSG_ACS
#define VNOC_MSG_ACS

#include "BaseMessage.h"
//ACS(班级信息同步通知确认)

class MSG_ACS:public CMessage
{

public:

	MSG_ACS(){
		//0x1A
		BEGIN_PARAM_LIST
			ADD_PARAM_LIST("MessageSynchro")
			INIT_PARAM_OBJEDT(MSG_ACS_COM)
			END_PARAM_LIST
	}

	virtual ~MSG_ACS(){}

public:
	//班级信息同步通知
	const byte* GetMessageSynchro() const
		GetParam_t_r("MessageSynchro");

	void SetMessageSynchro(const byte* in_byte_ptr,size_t len)
		SetParam_t_ptr("MessageSynchro",in_byte_ptr,len);

	int GetMessageSynchroLen() const
		GetParamLen_t_r("MessageSynchro");

};


#endif