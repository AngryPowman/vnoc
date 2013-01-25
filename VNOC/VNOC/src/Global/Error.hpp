#pragma once
// 定义了全局的错误码

#define ResultSucceeded(result)	((result&0xffffff00)? FALSE: TRUE)
#define ResultFailed(result)	((result&0xffffff00)? TRUE: FALSE)

// 每一类错误请预留1024个错误码
enum ResultCode
{
	Result_Success = 0,
	//////////////////////////////////////////////////////////////////////////
	// 常规错误
	Result_FatalError = 256,
	Result_Fail,
	Result_InvalidParam,
	//////////////////////////////////////////////////////////////////////////
	// 账号相关
	Result_UserName_LengthError=1024,
	Result_UserName_InvalidChar,

	Result_Pwd_LengthError = 1536,
	Result_Pwd_InvalidChar,

	//////////////////////////////////////////////////////////////////////////
	// 动态错误码
	Result_DynamicError = 0x80000000,

	Result_MaxID = 0xffffffff,
};
