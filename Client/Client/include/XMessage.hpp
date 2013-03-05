#pragma once
#include "../FrameWork/FrameBase.hpp"
#include "../../../NMessage/MessageDef.h"
using namespace VNOC::Message::Define;

//////////////////////////////////////////////////////////////////////////
// Messages

// 特殊消息，获取模块所监听的消息列表
struct XMessage_GetListenList : public XMessage
{
	ImTheMessageOf(XMessage_GetListenList);

	std::list<CString>	msgIDList;
};

//////////////////////////////////////////////////////////////////////////
// 登陆相关

struct XMessage_Login : public XMessage
{
	ImTheMessageOf(XMessage_Login);

	CString username;
	CString pwd;
};

struct XMessage_LoginError : public XMessage
{
	ImTheMessageOf(XMessage_LoginError);

	ResultCode result;
};

struct XMessage_Login_Result : public XMessage
{
	ImTheMessageOf(XMessage_Login_Result);

	BOOL success;
	uint32 userToken;
	std::string guid;
};

struct XMessage_ShowLogin : public XMessage
{
	ImTheMessageOf(XMessage_ShowLogin);
};

//////////////////////////////////////////////////////////////////////////
// 房间相关

struct XMessage_ShowRoomListWnd : public XMessage
{
	ImTheMessageOf(XMessage_ShowRoomListWnd);
};


struct XMessage_ShowRoomListWnd_Result : public XMessage
{
	ImTheMessageOf(XMessage_ShowRoomListWnd_Result);
};

struct XMessage_GetRoomList : public XMessage
{
	ImTheMessageOf(XMessage_GetRoomList);
};

struct XMessage_GetRoomList_Result : public XMessage
{
	ImTheMessageOf(XMessage_GetRoomList_Result);

	std::vector<uint32> roomIdList;
    std::vector<std::string> roomNameList;
    std::vector<uint32> roomStateList;
};

struct XMessage_ShowClassroom : public XMessage
{
	ImTheMessageOf(XMessage_ShowClassroom);
};

struct XMessage_EnterRoom : public XMessage
{
    ImTheMessageOf(XMessage_EnterRoom);

    uint32 roomId;
    std::string password;
    std::string verificationCode;
};

struct XMessage_EnterRoom_Result : public XMessage
{
    ImTheMessageOf(XMessage_EnterRoom_Result);

    uint32 retTag;
};

//////////////////////////////////////////////////////////////////////////
// Framework

struct XMessage_FrameStart : public XMessage
{
	ImTheMessageOf(XMessage_FrameStart);
};

struct XMessage_StartTest : public XMessage
{
	ImTheMessageOf(XMessage_StartTest);
};