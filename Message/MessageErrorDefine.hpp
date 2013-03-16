#pragma once
#include <string>
//////////////////////////////////////////////////////////////////////////
// cs交互数据中的错误码定义

const wchar_t* GetErrorDescription(MsgErrorCode errCode);

enum VNOCMessageErrorEnum
{
	MsgError_None,
	MsgError_General_NetLinkFail,
	MsgError_General_NetLinkTimeout,
};


