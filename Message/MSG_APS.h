


#ifndef VNOC_MSG_APS
#define VNOC_MSG_APS

#include "BaseMessage.h"
//APS(������Ϣͬ��֪ͨȷ��)

class MSG_APS:public CMessage
{

public:

	MSG_APS(){
		//0x19
		BEGIN_PARAM_LIST
		ADD_PARAM_LIST("MessageSynchro")
		INIT_PARAM_OBJEDT(25)
		END_PARAM_LIST
	}

	virtual ~MSG_APS(){}

public:
	//������Ϣͬ��֪ͨ
	const byte* GetMessageSynchro() const
	GetParam_t_r("MessageSynchro");

	void SetMessageSynchro(const byte* in_byte_ptr,size_t len)
	SetParam_t_ptr("MessageSynchro",in_byte_ptr,len);

	int GetMessageSynchroLen() const
	GetParamLen_t_r("MessageSynchro");

};


#endif