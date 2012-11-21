

#ifndef VNOC_MSG_RVC
#define VNOC_MSG_RVC

#include "BaseMessage.h"

//RVC(��ȡ��֤������)
class MSG_RVC: public CMessage
{
public:

	MSG_RVC(){
		//0x14
		BEGIN_PARAM_LIST
		ADD_PARAM_LIST("MachineAddress")
		INIT_PARAM_OBJEDT(20)
		END_PARAM_LIST
	}
	virtual ~MSG_RVC(){}

public:
	//������ַ
	const byte* GetMachineAddress() const 
	GetParam_t_r("MachineAddress");

	void SetMachineAddress(const byte* in_byte_ptr, size_t len) 
	SetParam_t_ptr("MachineAddress",in_byte_ptr,len);

	int  GetMachineAddressLen()const
	GetParamLen_t_r("MachineAddress");
};


#endif