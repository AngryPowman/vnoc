


#ifndef VNOC_MSG_ACI
#define VNOC_MSG_ACI

#include "BaseMessage.h"

class MSG_ACI: public CMessage
{
public:

	MSG_ACI(){
		//0x21
		BEGIN_PARAM_LIST
		ADD_PARAM_LIST("RoomID")
		ADD_PARAM_LIST("RoomType")
		ADD_PARAM_LIST("RoomName")
		ADD_PARAM_LIST("RoomRank")
		ADD_PARAM_LIST("RoomState")
		ADD_PARAM_LIST("RoomPassword")
		ADD_PARAM_LIST("RoomPeopleNumMax")
		ADD_PARAM_LIST("RoomPeopleNum")
		ADD_PARAM_LIST("RoomPeopleList")
		ADD_PARAM_LIST("RoomManageID")
		INIT_PARAM_OBJEDT(33)
		END_PARAM_LIST
	}

	virtual ~MSG_ACI(){}

public:

	//获取房间ID：要获取信息的房间的ID
	uint GetRoomID() const
	GetParam_t_int_r("RoomID",4);

	uint GetRoomType() const
	GetParam_t_int_r("RoomType",4);

	const byte* GetRoomName() const
	GetParam_t_r("RoomName");

	uint GetRoomRank() const
	GetParam_t_int_r("RoomRank",4);

	uint GetRoomState() const
	GetParam_t_int_r("RoomState",4);

	const byte* GetRoomPassword() const
	GetParam_t_r("RoomPassword");

	uint GetRoomPeopleNumMax()  const
	GetParam_t_int_r("RoomPeopleNumMax",4);

	uint GetRoomPeopleNum()   const
	GetParam_t_int_r("RoomPeopleNum",4);

	int  GetRoomPeopleList(std::vector<int>& PeopleList) const
	GetParam_t_tamp_r("RoomPeopleList",int,PeopleList);

	uint GetRoomManageID()    const
	GetParam_t_int_r("RoomManageID",4);

	void SetRoomID(uint in_int)
	SetParam_t("RoomID",in_int);

	void SetRoomType(uint in_int)
	SetParam_t("RoomType",in_int);

	void SetRoomName(const byte* in_byte_ptr,size_t len)
	SetParam_t_ptr("RoomName",in_byte_ptr,len);

	void SetRoomRank(uint in_int)
	SetParam_t("RoomRank",in_int);

	void SetRoomState(uint in_int)
	SetParam_t("RoomState",in_int);

	void SetRoomPassword(const byte* in_byte_ptr,size_t len)
	SetParam_t_ptr("RoomPassword",in_byte_ptr,len);

	void SetRoomPeopleNumMax(uint in_int)
	SetParam_t("RoomPeopleNumMax",in_int);

	void SetRoomPeopleNum(uint in_int)
	SetParam_t("RoomPeopleNum",in_int);

	void  SetRoomPeopleList(std::vector<int> PeopleList)
	SetParam_t_tamp("RoomPeopleList",int,PeopleList);

	void SetRoomManageID(uint in_int)
	SetParam_t("RoomManageID",in_int);

	int GetRoomNameLen() const
	GetParamLen_t_r("RoomName");

	int GetRoomPasswordLen() const
	GetParamLen_t_r("RoomPassword");

	int GetPeopleListSize() const{
		return (CMessage::GetParamLen(8) / 4);
	}
};


#endif