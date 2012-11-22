

#ifndef VNOC_ACL
#define VNOC_ACL

#include "BaseMessage.h"

class MSG_ACL:public CMessage
{
public:
	MSG_ACL(){
		//0x1F
		BEGIN_PARAM_LIST
		//ADD_PARAM_LIST("MachineAddress")
		INIT_PARAM_OBJEDT(31)
		END_PARAM_LIST
	}
	virtual ~MSG_ACL(){}

public:

};

#endif