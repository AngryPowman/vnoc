

#ifndef VNOC_ACL
#define VNOC_ACL

#include "BaseMessage.h"

class MSG_ACL:public CMessage
{
public:
	MSG_ACL(){
		//0x1F
		BEGIN_PARAM_LIST
		ADD_PARAM_LIST("RoomList")
		INIT_PARAM_OBJEDT(31)
		END_PARAM_LIST
	}
	virtual ~MSG_ACL(){}

public:

	int  GetRoomList(std::vector<int>& RoomList) const
	GetParam_t_tamp_r("RoomList",int,RoomList);

	void SetRoomList(std::vector<int> RoomList)
	SetParam_t_tamp("RoomList",int,RoomList);

	int GetRoomListSize() const{
		return (CMessage::GetParamLen(0) / 4);
	}

};

#endif