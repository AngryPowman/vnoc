#pragma once
// ������ȫ�ֵĴ�����

#define ResultSucceeded(result)	((result&0xffffff00)? FALSE: TRUE)
#define ResultFailed(result)	((result&0xffffff00)? TRUE: FALSE)

// ÿһ�������Ԥ��1024��������
enum ResultCode
{
	Result_Success = 0,
	Result_Fault = 1,	// ok,but not good
	//////////////////////////////////////////////////////////////////////////
	// �������
	Result_Unknown = 256,
	Result_NotSet,
	Result_FatalError,
	Result_Fail,
	Result_InvalidParam,
	Result_NotFound,
	Result_NotImpl,
	//////////////////////////////////////////////////////////////////////////
	// �˺����
	Result_UserName_LengthError=1024,
	Result_UserName_InvalidChar,

	Result_Pwd_LengthError = 1536,
	Result_Pwd_InvalidChar,

	//////////////////////////////////////////////////////////////////////////
	// ��̬������
	Result_DynamicError = 0x80000000,

	Result_MaxID = 0xffffffff,
};
