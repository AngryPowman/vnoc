#pragma once
#include <string>
//////////////////////////////////////////////////////////////////////////
// cs���������еĴ����붨��

const wchar_t* GetErrorDescription(MsgErrorCode errCode);

enum VNOCMessageErrorEnum
{
	MsgError_None,
	MsgError_General_NetLinkFail,
	MsgError_General_NetLinkTimeout,
};


