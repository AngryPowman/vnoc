#pragma once
#include <string>
//////////////////////////////////////////////////////////////////////////
// cs���������еĴ����붨��

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
	DefineMsgError(MsgError_None,"û�д���"),
};
*/