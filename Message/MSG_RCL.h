
#ifndef VNOC_MSG_RCL
#define VNOC_MSG_RCL

#include "BaseMessage.h"

class MSG_RCL:public CMessage
{
public:
	MSG_RCL(){
		//0x1E
		BEGIN_PARAM_LIST
		//ADD_PARAM_LIST("MachineAddress")
		INIT_PARAM_OBJEDT(30)
		END_PARAM_LIST
	}

	//±£¡Ù
	//uint GetUserType() const;

	//void SetUserType(uint in_int);

	virtual ~MSG_RCL(){}
};

#endif