#pragma once
#include <string>
//////////////////////////////////////////////////////////////////////////
// cs交互数据中的错误码定义

typedef unsigned int MsgErrorCode;



struct MessageErrorInfo
{
	MsgErrorCode	errorCode;
	std::wstring	errorMsg;
};
/*
#define DefineMsgError(_id,_msg)	{_id,_T(_msg)}

MessageErrorInfo ErrorInfoList[]
{
	DefineMsgError(MsgError_None,"没有错误"),
};
*/