

#ifndef VNOC_MSG_RCI
#define VNOC_MSG_RCI

#include "BaseMessage.h"

class MSG_RCI: public CMessage
{
public:

	MSG_RCI(){
		//0x20
		BEGIN_PARAM_LIST
		ADD_PARAM_LIST("RoomID")
		INIT_PARAM_OBJEDT(32)
		END_PARAM_LIST
	}

	virtual ~MSG_RCI(){}

public:
	//获取房间ID：要获取信息的房间的ID
	uint GetRoomID() const
	GetParam_t_byte_r_int("RoomID",4);

	void SetRoomID(uint in_int)
	SetParam_t("RoomID",in_int);
};

#endif